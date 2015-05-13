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

	void SetTexture(string fileName, ork::vec2i uvTile, float uvAnimSpeed);//������ͼ
	void SetParams(float cohesionParam, float separationParam, float alignmentParam, 
		float tendTowardParam, float tendAwayParam);//���ò���
	void SetVecLimit(float maxSpeed);
	void SetNeighborNum(int cohesionNeighborNum, int separationNeighborNum, int alignmentNeighborNum);
	void SetTendDistance(float tendTowardDis, float tendAwayDis);
	void SetScatterTime(float scatterTime);
	void SetAvoidPerchTime(float avoidPerchTime);

	void UpdateKey();//��������¼�

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
	float originCohesionParam;//backup
	float separationParam;
	float alignmentParam;

	float tendTowardParam;//�������
	float tendAwayParam;//Զ�����
	float tendTowardDis;//����������Զ��boid
	float tendAwayDis;//������ɢ��Զ��boid

	std::map< AABB, std::vector<int> > lattice;//��ά�������ĵ�Ϊ��0, 0, 0�� ������Ϊ���� ����洢boid�ı��
	float latticeSize;//����Χ ֱ��
	float gridSize;//���������С
	int gridNum;//һ����������������Ŀ

	int cohesionNeighborNum;//һ��boid��Ҫ���ǵ��ھ���Ŀ �ۼ� 
	int separationNeighborNum;//һ��boid��Ҫ���ǵ��ھ���Ŀ ����
	int alignmentNeighborNum;//һ��boid��Ҫ���ǵ��ھ���Ŀ ͬ��

	//������Ϊ
	float scatterTimer;
	float scatterTime;
	bool isScattering;

	//�����ʱ���ڱ���boidͣ������ֹ����һֱ��ͣ��״̬��
	float avoidPerchTime;

	ork::vec2i uvTile;//��ͼƽ��ϵ��
	float uvAnimSpeed;//��ͼ�任�ٶȣ�ÿ��仯���� һ�α仯�輸��
	ork::vec2f curOffsetUV;//��ǰ

	AABB boundBox;//��Χ����
	float maxSpeed;//�ٶ�����

	Model *ball;//������ʹ�ɢ��
	Model *cage;//��Χ������

	ork::vec3f tendTowardBallPos;
	ork::vec3f tendAwayBallPos;

	ork::ptr< ork::Mesh<RenderParam, unsigned int> > mesh;
	ork::ptr<ork::Program> program;

	ork::ptr<ork::Program> latticeProgram;
	ork::ptr< ork::Mesh<ork::vec3f, unsigned int> > latticeMesh;

	ork::vec3f Cohesion(const Boid &boid);//�ۼ�
	ork::vec3f Separation(const Boid &boid);//����
	ork::vec3f Alignment(const Boid &boid);//����

	void BoundPos(Boid &boid);//����λ��
	void LimitVec(Boid &boid);//�����ٶ�
	void Perch(Boid &boid);//��Ϣ ͣ��
	void TendToward(Boid &boid);//����ĳ��
	void TendAway(Boid &boid);//Զ��ĳ��

	void Scatter();//����

	std::vector<int> GetNeighbor(const Boid &boid, int neighborNum);//��ȡ�ھ�

	void ClearLattice();//���lattice
	void FillLattice();//���lattice
	void DrawLattice(ork::ptr<ork::FrameBuffer> fb);//����lattice
	AABB GetLatticeOffsetGrid(const AABB &grid, ork::vec3i offset);//����ƫ�������
	bool IsGridValid(const AABB &grid);
};

#endif //_BOIDMANAGER_H