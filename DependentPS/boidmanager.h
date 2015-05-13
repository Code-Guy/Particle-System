#ifndef _BOIDMANAGER_H
#define  _BOIDMANAGER_H

#include "boid.h"
#include "model.h"
#include <vector>
#include <map>

const ork::vec3f LatticeColor(0, 1, 0);
const Material TendTowardMtl(ork::vec3f(1, 1, 0), ork::vec3f(0.5, 0.5, 0.5), ork::vec3f(0.5, 0.5, 0.5));
const Material TendAwayMtl(ork::vec3f(1, 0, 0), ork::vec3f(0.5, 0.5, 0.5), ork::vec3f(0.5, 0.5, 0.5));
const Material CageMtl(ork::vec3f(0, 0, 1), ork::vec3f(0.5, 0.5, 0.5), ork::vec3f(0.5, 0.5, 0.5));
const ork::vec3f BallSize(0.3, 0.3, 0.3);

class BoidManager
{
public:
	BoidManager(int num, 
		ork::vec3f ellipsoid, float emitterRange,
		float minSize, float maxSize,
		ork::vec3f minVec, ork::vec3f maxVec, 
		float minPerchTime, float maxPerchTime,
		float latticeSize, float gridSize);
	~BoidManager();

	void Init();
	void Logic(float dt);
	void Render(ork::ptr<ork::FrameBuffer> fb);

	void SetTexture(string fileName, ork::vec2i uvTile, float uvAnimSpeed);//设置贴图
	void SetParams(float cohesionParam, float separationParam, float alignmentParam, 
		float tendTowardParam, float tendAwayParam);//设置参数
	void SetVecLimit(float maxSpeed);
	void SetNeighborNum(int cohesionNeighborNum, int separationNeighborNum, int alignmentNeighborNum);
	void SetTendDistance(float tendTowardDis, float tendAwayDis);
	void SetScatterTime(float scatterTime);
	void SetAvoidPerchTime(float avoidPerchTime);

	void UpdateKey();//处理键盘事件

private:
	int num;//boid的数量
	ork::vec3f ellipsoid;//boid出生范围
	float emitterRange;//0-1
	float minSize;
	float maxSize;
	ork::vec3f minVec;
	ork::vec3f maxVec;
	float minPerchTime;
	float maxPerchTime;
	std::vector<Boid> boids;//boids
	//boid模型三个规则的系数
	float cohesionParam;
	float originCohesionParam;//backup
	float separationParam;
	float alignmentParam;

	float tendTowardParam;//趋向参数
	float tendAwayParam;//远离参数
	float tendTowardDis;//可以吸引多远的boid
	float tendAwayDis;//可以驱散多远的boid

	std::map< AABB, std::vector<int> > lattice;//三维网格，中心点为（0, 0, 0） 网格作为索引 里面存储boid的编号
	float latticeSize;//网格范围 直径
	float gridSize;//单个网格大小
	int gridNum;//一个方向上网格点的数目

	int cohesionNeighborNum;//一个boid所要考虑的邻居数目 聚集 
	int separationNeighborNum;//一个boid所要考虑的邻居数目 分离
	int alignmentNeighborNum;//一个boid所要考虑的邻居数目 同向

	//惊吓行为
	float scatterTimer;
	float scatterTime;
	bool isScattering;

	//在这段时间内避免boid停留（防止陷入一直暂停的状态）
	float avoidPerchTime;

	ork::vec2i uvTile;//贴图平铺系数
	float uvAnimSpeed;//贴图变换速度，每秒变化几次 一次变化需几秒
	ork::vec2f curOffsetUV;//当前

	AABB boundBox;//范围限制
	float maxSpeed;//速度限制

	Model *ball;//趋向球和打散球
	Model *cage;//外围玻璃框

	ork::vec3f tendTowardBallPos;
	ork::vec3f tendAwayBallPos;

	ork::ptr< ork::Mesh<RenderParam, unsigned int> > mesh;
	ork::ptr<ork::Program> program;

	ork::ptr<ork::Program> latticeProgram;
	ork::ptr< ork::Mesh<ork::vec3f, unsigned int> > latticeMesh;

	ork::vec3f Cohesion(const Boid &boid);//聚集
	ork::vec3f Separation(const Boid &boid);//分离
	ork::vec3f Alignment(const Boid &boid);//对齐

	void BoundPos(Boid &boid);//限制位置
	void LimitVec(Boid &boid);//限制速度
	void Perch(Boid &boid);//栖息 停留
	void TendToward(Boid &boid);//趋向某处
	void TendAway(Boid &boid);//远离某处

	void Scatter();//惊吓

	std::vector<int> GetNeighbor(const Boid &boid, int neighborNum);//获取邻居

	void ClearLattice();//清空lattice
	void FillLattice();//填充lattice
	void DrawLattice(ork::ptr<ork::FrameBuffer> fb);//绘制lattice
	AABB GetLatticeOffsetGrid(const AABB &grid, ork::vec3i offset);//计算偏移网格点
	bool IsGridValid(const AABB &grid);
};

#endif //_BOIDMANAGER_H