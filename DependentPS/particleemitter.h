#ifndef _PARTICLEEMITTER_H
#define  _PARTICLEEMITTER_H

#include "particleparameter.h"

enum EmitterType //发射器类型
{
	EllipsoidType = 0, MeshType = 1
};

class ParticleEmitter
{
public:
	ParticleEmitter();
	~ParticleEmitter();

	//发射粒子
	void EmitParticles(float dt, std::list<ParticleParameter>& particles);
	//设置触发的粒子系统
	void SetBirthPS(int birthPS);
	int GetBirthPS();

	//粒子系统发射器的transformation信息
	ork::vec3f position;
	ork::vec3f rotation;
	ork::vec3f scale;

	//粒子系统发射器本身的速度
	ork::vec3f velocity;

	//各种发射器属性
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

	ork::mat4f M;//粒子系统的模型矩阵
	ork::mat4f ITM;//inverse transpose M 将方向向量从local转化成global的矩阵

protected:
	int birthPS;//出生的时候触发的粒子系统

private:
	float emittedParticleCount;//已经发射的总粒子数
	unsigned int particleCount;//当前帧发射的离子数

	float oneShotTimer;//oneshot计数器

	virtual float GenerateParticles(int particleCount, std::list<ParticleParameter>& particles) = 0;//产生particleCount个粒子,所产生粒子的最大生命周期

	void Reset();//重置发射器参数
};

#endif //_PARTICLEEMITTER_H