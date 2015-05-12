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

	void SetTexture(string fileName, ork::vec2i uvTile, float uvAnimSpeed);//������ͼ
	void SetParams(float cohesionParam, float separationParam, float alignmentParam);//���ò���
	void SetLimit(AABB boundBox, float maxSpeed);

private:
	int num;//boid������
	ork::vec3f ellipsoid;//boid������Χ
	float emitterRange;//0-1
	float minSize;
	float maxSize;
	ork::vec3f minVec;
	ork::vec3f maxVec;
	float minPerchTime;
	float maxPerchTime;
	std::vector<Boid> boids;//boids
	//boidģ�����������ϵ��
	float cohesionParam;
	float separationParam;
	float alignmentParam;

	std::map< AABB, std::vector<int> > lattice;//��ά�������ĵ�Ϊ��0, 0, 0�� ������Ϊ���� ����洢boid�ı��
	float latticeSize;//����Χ ֱ��
	float gridSize;//���������С
	int gridNum;//һ����������������Ŀ

	ork::vec2i uvTile;//��ͼƽ��ϵ��
	float uvAnimSpeed;//��ͼ�任�ٶȣ�ÿ��仯���� һ�α仯�輸��
	ork::vec2f curOffsetUV;//��ǰ

	AABB boundBox;//��Χ����
	float maxSpeed;//�ٶ�����

	ork::ptr< ork::Mesh<RenderParam, unsigned int> > mesh;
	ork::ptr<ork::Program> program;

	ork::ptr<ork::Program> latticeProgram;
	ork::ptr< ork::Mesh<ork::vec3f, unsigned int> > latticeMesh;

	ork::vec3f Cohesion(const Boid &boid, const std::vector<int> &neighbor);//�ۼ�
	ork::vec3f Separation(const Boid &boid, const std::vector<int> &neighbor);//����
	ork::vec3f Alignment(const Boid &boid, const std::vector<int> &neighbor);//����
	ork::vec3f BoundPos(const Boid &boid);//����λ��
	void LimitVec(Boid &boid);//�����ٶ�

	void ClearLattice();//���lattice
	void FillLattice();//���lattice
	void DrawLattice(ork::ptr<ork::FrameBuffer> fb);//����lattice
};

#endif //_BOIDMANAGER_H