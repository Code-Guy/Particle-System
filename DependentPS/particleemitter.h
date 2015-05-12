#ifndef _PARTICLEEMITTER_H
#define  _PARTICLEEMITTER_H

#include "particleparameter.h"

enum EmitterType //����������
{
	EllipsoidType = 0, MeshType = 1
};

class ParticleEmitter
{
public:
	ParticleEmitter();
	~ParticleEmitter();

	//��������
	void EmitParticles(float dt, std::list<ParticleParameter>& particles);
	//���ô���������ϵͳ
	void SetBirthPS(int birthPS);
	int GetBirthPS();

	//����ϵͳ��������transformation��Ϣ
	ork::vec3f position;
	ork::vec3f rotation;
	ork::vec3f scale;

	//����ϵͳ������������ٶ�
	ork::vec3f velocity;

	//���ַ���������
	bool isEmit;
	float minSize, maxSize;
	float minEnergy, maxEnergy;
	float minEmission, maxEmission;
	ork::vec3f worldVelocity;
	ork::vec3f localVelocity;
	ork::vec3f rndVelocity;
	ork::vec3f tangentVelocity;
	float angularVelocity;
	float rndAngularVelocity;
	bool rndRotation;
	float emitterVelocityScale;
	bool isSimulateInWorldspace;
	bool isOneShot;

	ork::mat4f M;//����ϵͳ��ģ�;���
	ork::mat4f ITM;//inverse transpose M ������������localת����global�ľ���

protected:
	int birthPS;//������ʱ�򴥷�������ϵͳ

private:
	float emittedParticleCount;//�Ѿ��������������
	unsigned int particleCount;//��ǰ֡�����������

	float oneShotTimer;//oneshot������

	virtual float GenerateParticles(int particleCount, std::list<ParticleParameter>& particles) = 0;//����particleCount������,���������ӵ������������

	void Reset();//���÷���������
};

#endif //_PARTICLEEMITTER_H