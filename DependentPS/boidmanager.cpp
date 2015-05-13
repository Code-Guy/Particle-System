#include "boidmanager.h"
#include "math3d.h"
#include "fileloader.h"
#include <Windows.h>

using namespace std;
using namespace ork;

BoidManager::BoidManager(int num, 
	ork::vec3f ellipsoid, float emitterRange, 
	float minSize, float maxSize,
	ork::vec3f minVec, ork::vec3f maxVec, 
	float minPerchTime, float maxPerchTime,
	float latticeSize, float gridSize) 
	: num(num), 
	ellipsoid(ellipsoid), emitterRange(emitterRange),
	minSize(minSize), maxSize(maxSize),
	minVec(minVec), maxVec(maxVec), 
	minPerchTime(minPerchTime), maxPerchTime(maxPerchTime),
	latticeSize(latticeSize), gridSize(gridSize)
{
	program = new Program(new Module(400,
		(char*)FileLoader::LoadFile(GetShaderPath("BoidVS.glsl")),
		(char*)FileLoader::LoadFile(GetShaderPath("BoidGS.glsl")),
		(char*)FileLoader::LoadFile(GetShaderPath("BoidFS.glsl"))));

	mesh = new Mesh<RenderParam, unsigned int>(MeshMode::POINTS, GPU_DYNAMIC);
	mesh->addAttributeType(0, 3, AttributeType::A32F, false);
	mesh->addAttributeType(1, 1, AttributeType::A32F, false);
	mesh->addAttributeType(2, 1, AttributeType::A32F, false);

	latticeProgram = new Program(new Module(400, (char*)FileLoader::LoadFile(GetShaderPath("LineVS.glsl")),
		(char*)FileLoader::LoadFile(GetShaderPath("LineFS.glsl"))));

	latticeProgram->getUniform3f("LineColor")->set(LatticeColor);

	latticeMesh = new Mesh<vec3f, unsigned int>(LINES, GPU_STATIC);
	latticeMesh->addAttributeType(0, 3, A32F, false);

	ball = new Model("ball.obj");
	cage = new Model("cage.obj");
	cage->SetMaterial(CageMtl);

	//初始化趋向球和打散球的位置
	tendTowardBallPos = vec3f(0.3*latticeSize, 0.2*latticeSize, 0.4*latticeSize);
	tendAwayBallPos = vec3f::ZERO;

	scatterTimer = 0;
	isScattering = false;
}

BoidManager::~BoidManager()
{
	delete ball;
	delete cage;
}

void BoidManager::Init()
{
	//1.随机生成num个boid，初始化其参数
	for (int i=0; i<num; i++)
	{
		Boid boid;
		boid.renderParam.pos = Math3D::UniformEllipsoidSample(ellipsoid, emitterRange);
		boid.renderParam.size = Math3D::RandFloatToFloat(minSize, maxSize);
		boid.renderParam.tileIndex = 0;

		boid.vec = Math3D::RandVec3ToVec3(minVec, maxVec);
		//boid.vec = vec3f(0, 0, 0);

		boid.timer = Math3D::RandIntToInt(0, uvTile.x * uvTile.y) * uvAnimSpeed;

		boid.isPerching = false;
		boid.perchTime = Math3D::RandFloatToFloat(minPerchTime, maxPerchTime);
		boid.perchTimer = 0;

		boid.isAvoidPerching = false;
		boid.avoidPerchTime = avoidPerchTime;
		boid.avoidPerchTimer = 0;

		boid.index = i;

		boids.push_back(boid);
	}
	//2.初始化网格
	gridNum = latticeSize / gridSize;
	boundBox = AABB(-0.5*latticeSize, 0.5*latticeSize, 
		-0.5*latticeSize, 0.5*latticeSize, 
		-0.5*latticeSize, 0.5*latticeSize);

	int index = 0;//AABB的索引
	for(int i=0; i<gridNum; i++)
	{
		for(int j=0; j<gridNum; j++)
		{
			for(int k=0; k<gridNum; k++)
			{
				AABB grid(gridSize * i - 0.5 * latticeSize, gridSize * (i + 1) - 0.5 * latticeSize,
					gridSize * j - 0.5 * latticeSize, gridSize * (j + 1) - 0.5 * latticeSize,
					gridSize * k - 0.5 * latticeSize, gridSize * (k + 1) - 0.5 * latticeSize, index++);
				vector<int> gridBoids;

				lattice[grid] = gridBoids;
			}
		}
	}
	//3.将这些boid塞进网格中
	FillLattice();
}

void BoidManager::Logic(float dt)
{
	UpdateKey();//处理键盘事件

	//1.遍历每个boid，更新速度和位置
	for (size_t i=0; i<boids.size(); i++)
	{
		Boid &boid = boids[i];

		if (isScattering)
		{
			if (scatterTimer >= scatterTime)
			{
				isScattering = false;
				cohesionParam = originCohesionParam;
			}
			else
			{
				scatterTimer += dt;
				cohesionParam = 0;
			}
		}

		if (boid.isPerching)//判断boid是否处于静止状态
		{
			if (boid.perchTimer >= boid.perchTime)
			{
				boid.isPerching = false;
				boid.isAvoidPerching = true;
				boid.avoidPerchTimer = 0;
			}
			else
			{
				boid.perchTimer += dt;
				continue;
			}
		}

		if (boid.isAvoidPerching)
		{
			if (boid.avoidPerchTimer >= boid.avoidPerchTime)
			{
				boid.isAvoidPerching = false;
			}
			else
			{
				boid.avoidPerchTimer += dt;
			}
		}

		vec3f c = vec3f::ZERO;
		vec3f s = vec3f::ZERO;
		vec3f a = vec3f::ZERO;

		if (boid.location != INFINITY_AABB)//判断boid有没有超出网格
		{
			c = Cohesion(boid);
			s = Separation(boid);
			a = Alignment(boid);
		}

		BoundPos(boid);
		LimitVec(boid);
		Perch(boid);
		TendToward(boid);
		TendAway(boid);

		vec3f newVec = boid.vec + c + s + a;
		boid.renderParam.pos = boid.renderParam.pos + (boid.vec + newVec) * dt * 0.5;
		boid.vec = newVec;

		//printf("%.4lf %.4lf %.4lf %.4lf\n", boid.vec.length(), c.length(), s.length(), a.length());

		//计算贴图编号
		boid.timer += dt;//计时器递增
		int n = boid.timer / uvAnimSpeed;
		int tileNum = uvTile.x * uvTile.y;
		if (n > tileNum - 1)
		{
			n %= tileNum;
		}
		boid.renderParam.tileIndex = n;
	}
	//2.更新lattice
	ClearLattice();
	FillLattice();
}

void BoidManager::Render(ptr<FrameBuffer> fb)
{
	//更新mesh数据
	mesh->clear();
	for (size_t i=0; i<boids.size(); i++)
	{
		mesh->addVertex(boids[i].renderParam);
	}

	//1.画粒子
	program->getUniformMatrix4f("MV")->setMatrix(camera->V);
	program->getUniformMatrix4f("P")->setMatrix(camera->P);

	fb->setBlend(true, ADD, SRC_ALPHA, ONE_MINUS_SRC_ALPHA);
	fb->setDepthMask(false);
	fb->draw(program, *mesh);
	fb->setDepthMask(true);
	fb->setBlend(false);

	//2.画球
	ball->SetMaterial(TendTowardMtl);
	ball->Render(fb, tendTowardBallPos, vec3f::ZERO, BallSize);
	ball->SetMaterial(TendAwayMtl);
	ball->Render(fb, tendAwayBallPos, vec3f::ZERO, BallSize);

	//3.画网格
	DrawLattice(fb);

	//4.画玻璃框
	cage->Render(fb, vec3f::ZERO, vec3f::ZERO, vec3f(latticeSize * 1.2, latticeSize, latticeSize * 1.2));
}

void BoidManager::SetTexture(string fileName, ork::vec2i uvTile, float uvAnimSpeed)//设置贴图
{
	program->getUniformSampler("ParticleTex")->set(FileLoader::LoadTexture(GetTexturePath(fileName)));
	program->getUniform2i("UVTile")->set(uvTile);
	this->uvTile = uvTile;
	this->uvAnimSpeed = 1.0f / uvAnimSpeed;//一次变化需几秒
}

void BoidManager::SetParams(float cohesionParam, float separationParam, float alignmentParam,
	float tendTowardParam, float tendAwayParam)//设置参数
{
	this->cohesionParam = cohesionParam;
	this->separationParam = separationParam;
	this->alignmentParam = alignmentParam;
	this->tendTowardParam = tendTowardParam;
	this->tendAwayParam = tendAwayParam;

	originCohesionParam = cohesionParam;
}

void BoidManager::SetVecLimit(float maxSpeed)
{
	this->maxSpeed = maxSpeed;
}

void BoidManager::SetNeighborNum(int cohesionNeighborNum, int separationNeighborNum, int alignmentNeighborNum)
{
	this->cohesionNeighborNum = cohesionNeighborNum;
	this->separationNeighborNum = separationNeighborNum;
	this->alignmentNeighborNum = alignmentNeighborNum;
}

void BoidManager::SetTendDistance(float tendTowardDis, float tendAwayDis)
{
	this->tendTowardDis = tendTowardDis;
	this->tendAwayDis = tendAwayDis;
}

void BoidManager::SetScatterTime(float scatterTime)
{
	this->scatterTime = scatterTime;
}

void BoidManager::SetAvoidPerchTime(float avoidPerchTime)
{
	this->avoidPerchTime = avoidPerchTime;
}

void BoidManager::UpdateKey()//处理键盘事件
{
	if (GetAsyncKeyState(VK_UP))
	{
		tendTowardBallPos.y += 0.1;
	}
	else if (GetAsyncKeyState(VK_DOWN))
	{
		tendTowardBallPos.y -= 0.1;
	}
	else if (GetAsyncKeyState(VK_LEFT))
	{
		tendTowardBallPos.x -= 0.1;
	}
	else if (GetAsyncKeyState(VK_RIGHT))
	{
		tendTowardBallPos.x += 0.1;
	}
	else if (GetAsyncKeyState('I'))
	{
		tendAwayBallPos.y += 0.1;
	}
	else if (GetAsyncKeyState('K'))
	{
		tendAwayBallPos.y -= 0.1;
	}
	else if (GetAsyncKeyState('J'))
	{
		tendAwayBallPos.x -= 0.1;
	}
	else if (GetAsyncKeyState('L'))
	{
		tendAwayBallPos.x += 0.1;
	}
	else if (GetAsyncKeyState(VK_SPACE))
	{
		Scatter();
	}
}

vec3f BoidManager::Cohesion(const Boid &boid)//聚集
{
	vector<int> neighbor = GetNeighbor(boid, cohesionNeighborNum);//找到邻居

	vec3f c = vec3f::ZERO;
	for (size_t i=0; i<neighbor.size(); i++)
	{
		if (boids[neighbor[i]].index != boid.index)
		{
			c = c + boids[neighbor[i]].renderParam.pos;
		}
	}

	c = c / neighbor.size();
	return (c - boid.renderParam.pos) * cohesionParam;
}

vec3f BoidManager::Separation(const Boid &boid)//分离
{
	vector<int> neighbor = GetNeighbor(boid, separationNeighborNum);//找到邻居

	vec3f s = vec3f::ZERO;
	for (size_t i=0; i<neighbor.size(); i++)
	{
		if (boids[neighbor[i]].index != boid.index)
		{
			s = s + boid.renderParam.pos - boids[neighbor[i]].renderParam.pos;
		}
	}

	s = s / neighbor.size();

	return s * separationParam;
}

vec3f BoidManager::Alignment(const Boid &boid)//对齐
{
	vector<int> neighbor = GetNeighbor(boid, alignmentNeighborNum);//找到邻居

	vec3f a = vec3f::ZERO;
	for (size_t i=0; i<neighbor.size(); i++)
	{
		if (boids[neighbor[i]].index != boid.index)
		{
			a = a + boids[neighbor[i]].vec;
		}
	}

	a = a / neighbor.size();
	return (a - boid.vec) * alignmentParam;
}

void BoidManager::BoundPos(Boid &boid)//限制位置
{
	vec3f pos = boid.renderParam.pos;

	if (pos.x < boundBox.xMin)
	{
		boid.vec.x += 0.2;
	}
	else if (pos.x > boundBox.xMax)
	{
		boid.vec.x -= 0.2;
	}

	if (pos.y < boundBox.yMin)
	{
		boid.vec.y += 0.2;
	}
	else if (pos.y > boundBox.yMax)
	{
		boid.vec.y -= 0.2;
	}

	if (pos.z < boundBox.zMin)
	{
		boid.vec.z += 0.2;
	}
	else if (pos.z > boundBox.zMax)
	{
		boid.vec.z -= 0.2;
	}
}

void BoidManager::LimitVec(Boid &boid)//限制速度
{
	if (boid.vec.length() > maxSpeed)
	{
		boid.vec = boid.vec.normalize() * maxSpeed;
	}
}

void BoidManager::Perch(Boid &boid)//栖息 停留
{
	if (!boid.isAvoidPerching && boid.renderParam.pos.y <= boundBox.yMin)
	{
		boid.perchTimer = 0;
		boid.isPerching = true;
	}
}

void BoidManager::TendToward(Boid &boid)//趋向某处
{
	if ((boid.renderParam.pos - tendTowardBallPos).length() < tendTowardDis)
	{
		boid.vec += (tendTowardBallPos - boid.renderParam.pos) * tendTowardParam;
	}
}

void BoidManager::TendAway(Boid &boid)//远离某处
{
	if ((boid.renderParam.pos - tendAwayBallPos).length() < tendAwayDis)
	{
		boid.vec += (tendAwayBallPos - boid.renderParam.pos) * tendAwayParam;
	}
}

void BoidManager::Scatter()//惊吓
{
	scatterTimer = 0;
	isScattering = true;
}

vector<int> BoidManager::GetNeighbor(const Boid &boid, int neighborNum)//获取邻居
{
	AABB location = boid.location;//所在的位置

	vector<int> neighbor;
	for(int i=0; ;i++)
	{
		neighbor.clear();
		for (int a=-i; a<=i; a++)
		{
			for (int b=-i; b<=i; b++)
			{
				for (int c=-i; c<=i; c++)
				{
					AABB offsetGrid = GetLatticeOffsetGrid(location, vec3i(a, b, c));
					if (IsGridValid(offsetGrid))
					{
						vector<int> tmpNeighbor = lattice[offsetGrid];
						
						for (size_t j=0; j<tmpNeighbor.size(); j++)
						{
							if (boid.index != tmpNeighbor[j] && !boids[tmpNeighbor[j]].isPerching)
							{
								neighbor.push_back(tmpNeighbor[j]);
								if (neighbor.size() >= neighborNum)
								{
									return neighbor;
								}
							}
						}
					}
				}
			}
		}
	}

	return neighbor;
}

void BoidManager::ClearLattice()//清空lattice
{
	//遍历lattice
	for (map< AABB, std::vector<int> >::iterator iter = lattice.begin(); iter != lattice.end(); iter++)
	{
		iter->second.clear();
	}
}

void BoidManager::FillLattice()//填充lattice
{
	for (size_t i=0; i<boids.size(); i++)
	{
		boids[i].location = INFINITY_AABB;
		//遍历lattice
		for (map< AABB, std::vector<int> >::iterator iter = lattice.begin(); iter != lattice.end(); iter++)
		{
			if (iter->first.IsVertexIn(boids[i].renderParam.pos))//如果该boid在此grid中，将其加入此grid
			{
				//将该boid的location设为此grid
				boids[i].location = iter->first;
				iter->second.push_back(i);
			}
		}
	}
}

void BoidManager::DrawLattice(ptr<FrameBuffer> fb)//绘制lattice
{
	latticeMesh->clear();
	//遍历lattice
	for (map< AABB, std::vector<int> >::const_iterator iter = lattice.begin(); iter != lattice.end(); iter++)
	{
		iter->first.Draw(latticeMesh);
	}

	latticeProgram->getUniformMatrix4f("MVP")->setMatrix(camera->VP);
	fb->draw(latticeProgram, *latticeMesh);
}

AABB BoidManager::GetLatticeOffsetGrid(const AABB &grid, ork::vec3i offset)//计算偏移网格点
{
	return AABB(grid.xMin + offset.x * gridSize, grid.xMax + offset.x * gridSize,
		grid.yMin + offset.y * gridSize, grid.yMax + offset.y * gridSize,
		grid.zMin + offset.z * gridSize, grid.zMax + offset.z * gridSize,
		grid.index + offset.x * gridNum * gridNum + offset.y * gridNum + offset.z);
}

bool BoidManager::IsGridValid(const AABB &grid)
{
	return grid.index >= 0 && grid.index < gridNum * gridNum * gridNum;
}