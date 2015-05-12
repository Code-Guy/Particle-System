#ifndef _ELLIPSOIDPARTICALSYSTEM_H
#define  _ELLIPSOIDPARTICALSYSTEM_H

#include "ellipsoidparticleemitter.h"
#include "meshparticleemitter.h"
#include "particleanimator.h"
#include "particlerenderer.h"

#define FOREVER -1
#define INVALID_ID -1

class ParticleSystem
{
public:
	ParticleSystem(float lifeTime = FOREVER);//-1��������ϵͳ����������
	ParticleSystem(ParticleSystem *ps);
	~ParticleSystem();

	bool operator==(const ParticleSystem &rhs);

	void Render(ork::ptr<ork::FrameBuffer> fb, float dt);//��Ⱦ
	void SetTriggerPS(int birthPS, int deathPS);//���ô���������ϵͳ
	void SetTrail(bool flag);//������βЧ��
	void SetBondWithCamera(bool flag);
	void SetBlur(bool flag);//����ģ��Ч��

	std::string name;//����ϵͳ������
	int ID;//��ʶ�� ��Ϊoperator==�ıȽ��� ��ʱ�������ʾ

	ParticleEmitter *emitter;//������
	ParticleAnimator *animator;//������
	ParticleRenderer *renderer;//��Ⱦ��

	EmitterType emitterType;//0 Ellipsoid 1 Mesh

private:
	ork::ptr< ork::Mesh<RenderParameter, unsigned int> > particleMesh;
	std::list<ParticleParameter> particles;

	float lifeTime;//���� s
	float curTime;//��ǰʱ��

	bool isBondWithCamera;//�Ƿ���������

	void SetBlendFunction(ork::ptr<ork::FrameBuffer> fb);//���û�Ϸ���
};

#endif //_ELLIPSOIDPARTICALSYSTEM_H