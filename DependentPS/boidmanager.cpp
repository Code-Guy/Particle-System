#include "boidmanager.h"
#include "math3d.h"
#include "fileloader.h"

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
	
	mesh = new Mesh<RenderParam, unsigned int>(POINTS, GPU_DYNAMIC);
	mesh->addAttributeType(0, 3, AttributeType::A32F, false);
	mesh->addAttributeType(1, 1, AttributeType::A32F, false);
	mesh->addAttributeType(2, 1, AttributeType::A32F, false);

	latticeProgram = new Program(new Module(400, (char*)FileLoader::LoadFile(GetShaderPath("LineVS.glsl")),
		(char*)FileLoader::LoadFile(GetShaderPath("LineFS.glsl"))));

	latticeProgram->getUniform3f("LineColor")->set(LatticeColor);

	latticeMesh = new Mesh<vec3f, unsigned int>(LINES, GPU_STATIC);
	latticeMesh->addAttributeType(0, 3, A32F, false);
}

BoidManager::~BoidManager()
{

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
		boid.vec = vec3f(0, 0, 0);

		boid.timer = Math3D::RandIntToInt(0, uvTile.x * uvTile.y) * uvAnimSpeed;

		boid.isPerching = false;
		boid.perchTime = Math3D::RandFloatToFloat(minPerchTime, maxPerchTime);
		boid.perchTimer = 0;
		boid.index = i;

		boids.push_back(boid);
	}
	//2.初始化网格
	gridNum = latticeSize / gridSize;

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
	//1.遍历每个boid，更新速度和位置
	for (size_t i=0; i<boids.size(); i++)
	{
		Boid &boid = boids[i];

		if (boid.isPerching)//判断boid是否处于静止状态
		{
			if (boid.perchTimer >= boid.perchTime)
			{
				boid.isPerching = false;
			}
			else
			{
				boid.perchTimer += dt;
				continue;
			}
		}

		vec3f c = vec3f::ZERO;
		vec3f s = vec3f::ZERO;
		vec3f a = vec3f::ZERO;
		vec3f b = BoundPos(boid);
		
		if (boid.location != INFINITY_AABB)//判断boid有没有超出网格
		{
			vector<int> neighbor = lattice[boid.location];//找到邻居

			if (neighbor.size() != 1)
			{
				c = Cohesion(boid, neighbor);
				s = Separation(boid, neighbor);
				a = Alignment(boid, neighbor);
			}
		}

		LimitVec(boid);
		vec3f newVec = boid.vec + c + s + a + b;
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

	program->getUniformMatrix4f("MV")->setMatrix(camera->V);
	program->getUniformMatrix4f("P")->setMatrix(camera->P);

	fb->setBlend(true, ADD, SRC_ALPHA, ONE_MINUS_SRC_ALPHA);
	fb->setDepthMask(false);
	fb->draw(program, *mesh);
	fb->setDepthMask(true);
	fb->setBlend(false);

	DrawLattice(fb);
}

void BoidManager::SetTexture(string fileName, ork::vec2i uvTile, float uvAnimSpeed)//设置贴图
{
	program->getUniformSampler("ParticleTex")->set(FileLoader::LoadTexture(GetTexturePath(fileName)));
	program->getUniform2i("UVTile")->set(uvTile);
	this->uvTile = uvTile;
	this->uvAnimSpeed = 1.0f / uvAnimSpeed;//一次变化需几秒
}

void BoidManager::SetParams(float cohesionParam, float separationParam, float alignmentParam)//设置参数
{
	this->cohesionParam = cohesionParam;
	this->separationParam = separationParam;
	this->alignmentParam = alignmentParam;
}

void BoidManager::SetLimit(AABB boundBox, float maxSpeed)
{
	this->boundBox = boundBox;
	this->maxSpeed = maxSpeed;
}

vec3f BoidManager::Cohesion(const Boid &boid, const std::vector<int> &neighbor)//聚集
{
	vec3f c = vec3f::ZERO;
	for (size_t i=0; i<neighbor.size(); i++)
	{
		if (boids[neighbor[i]].index != boid.index)
		{
			c = c + boids[neighbor[i]].renderParam.pos;
		}
	}

	c = c / (neighbor.size() - 1);
	return (c - boid.renderParam.pos) * cohesionParam;
}

vec3f BoidManager::Separation(const Boid &boid, const std::vector<int> &neighbor)//分离
{
	vec3f s = vec3f::ZERO;
	for (size_t i=0; i<neighbor.size(); i++)
	{
		if (boids[neighbor[i]].index != boid.index)
		{
			s = s + boid.renderParam.pos - boids[neighbor[i]].renderParam.pos;
		}
	}

	return s * separationParam;
}

vec3f BoidManager::Alignment(const Boid &boid, const std::vector<int> &neighbor)//对齐
{
	vec3f a = vec3f::ZERO;
	for (size_t i=0; i<neighbor.size(); i++)
	{
		if (boids[neighbor[i]].index != boid.index)
		{
			a = a + boids[neighbor[i]].vec;
		}
	}

	a = a / (neighbor.size() - 1);
	return (a - boid.vec) * alignmentParam;
}

vec3f BoidManager::BoundPos(const Boid &boid)//限制位置
{
	vec3f v = vec3f::ZERO;
	vec3f pos = boid.renderParam.pos;

	if (pos.x < boundBox.xMin)
	{
		v.x = 0.2;
	}
	else if (pos.x > boundBox.xMax)
	{
		v.x = -0.2;
	}

	if (pos.y < boundBox.yMin)
	{
		v.y = 0.2;
	}
	else if (pos.y > boundBox.yMax)
	{
		v.y = -0.2;
	}

	if (pos.z < boundBox.zMin)
	{
		v.z = 0.2;
	}
	else if (pos.z > boundBox.zMax)
	{
		v.z = -0.2;
	}

	return v;
}

void BoidManager::LimitVec(Boid &boid)//限制速度
{
	if (boid.vec.length() > maxSpeed)
	{
		boid.vec = boid.vec.normalize() * maxSpeed;
	}
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