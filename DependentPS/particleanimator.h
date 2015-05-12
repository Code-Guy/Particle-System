#ifndef _PARTICLEANIMATOR_H
#define  _PARTICLEANIMATOR_H

#include "particleparameter.h"

class ParticleAnimator
{
public:
	ParticleAnimator();
	~ParticleAnimator();

	//�����Ӷ���������̬�ı����ӵĸ�������
	void AnimateParticles(float dt, std::list<ParticleParameter>& particles, ork::mat4f ITM, ork::vec3f position);
	//���ô���������ϵͳ
	void SetDeathPS(int deathPS);
	int GetDeathPS();

	bool doesAnimateColor;
	ork::vec4f animationColor[5];
	ork::vec3f worldRotationAxis;  
	ork::vec3f localRotationAxis;
	float sizeGrow;
	ork::vec3f rndForce;
	ork::vec3f force;
	float damping;
	bool autoDestruct;
	ork::vec4f tintColor;

private:
	int deathPS;
};

#endif //_PARTICLEANIMATOR_H