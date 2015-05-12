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
	ParticleSystem(float lifeTime = FOREVER);//-1代表粒子系统的寿命无限
	ParticleSystem(ParticleSystem *ps);
	~ParticleSystem();

	bool operator==(const ParticleSystem &rhs);

	void Render(ork::ptr<ork::FrameBuffer> fb, float dt);//渲染
	void SetTriggerPS(int birthPS, int deathPS);//设置触发的粒子系统
	void SetTrail(bool flag);//开启拖尾效果
	void SetBondWithCamera(bool flag);
	void SetBlur(bool flag);//开启模糊效果

	std::string name;//粒子系统的名称
	int ID;//标识符 作为operator==的比较项 用时间戳来表示

	ParticleEmitter *emitter;//发射器
	ParticleAnimator *animator;//动画器
	ParticleRenderer *renderer;//渲染器

	EmitterType emitterType;//0 Ellipsoid 1 Mesh

private:
	ork::ptr< ork::Mesh<RenderParameter, unsigned int> > particleMesh;
	std::list<ParticleParameter> particles;

	float lifeTime;//寿命 s
	float curTime;//当前时间

	bool isBondWithCamera;//是否和摄像机绑定

	void SetBlendFunction(ork::ptr<ork::FrameBuffer> fb);//设置混合方程
};

#endif //_ELLIPSOIDPARTICALSYSTEM_H