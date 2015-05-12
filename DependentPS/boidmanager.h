#ifndef _BOIDMANAGER_H
#define  _BOIDMANAGER_H

#include "boid.h"
#include <vector>
#include <map>

const ork::vec3f LatticeColor(0, 1, 0);

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
	void SetParams(float cohesionParam, float separationParam, float alignmentParam);//设置参数
	void SetLimit(AABB boundBox, float maxSpeed);

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
	float separationParam;
	float alignmentParam;

	std::map< AABB, std::vector<int> > lattice;//三维网格，中心点为（0, 0, 0） 网格作为索引 里面存储boid的编号
	float latticeSize;//网格范围 直径
	float gridSize;//单个网格大小
	int gridNum;//一个方向上网格点的数目

	ork::vec2i uvTile;//贴图平铺系数
	float uvAnimSpeed;//贴图变换速度，每秒变化几次 一次变化需几秒
	ork::vec2f curOffsetUV;//当前

	AABB boundBox;//范围限制
	float maxSpeed;//速度限制

	ork::ptr< ork::Mesh<RenderParam, unsigned int> > mesh;
	ork::ptr<ork::Program> program;

	ork::ptr<ork::Program> latticeProgram;
	ork::ptr< ork::Mesh<ork::vec3f, unsigned int> > latticeMesh;

	ork::vec3f Cohesion(const Boid &boid, const std::vector<int> &neighbor);//聚集
	ork::vec3f Separation(const Boid &boid, const std::vector<int> &neighbor);//分离
	ork::vec3f Alignment(const Boid &boid, const std::vector<int> &neighbor);//对齐
	ork::vec3f BoundPos(const Boid &boid);//限制位置
	void LimitVec(Boid &boid);//限制速度

	void ClearLattice();//清空lattice
	void FillLattice();//填充lattice
	void DrawLattice(ork::ptr<ork::FrameBuffer> fb);//绘制lattice
};

#endif //_BOIDMANAGER_H