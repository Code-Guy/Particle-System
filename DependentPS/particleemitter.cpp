#include "particleemitter.h"

#include "math3d.h"

using namespace std;
using namespace ork;

ParticleEmitter::ParticleEmitter()
{
	emittedParticleCount = 0;
	particleCount = 0;
	oneShotTimer = 0;
	birthPS = -1;

	//粒子系统发射器本身的速度默认为0
	velocity = vec3f::ZERO;
}

ParticleEmitter::~ParticleEmitter()
{

}

//发射粒子
void ParticleEmitter::EmitParticles(float dt, list<ParticleParameter>& particles)
{
	//计算模型矩阵
	M = Math3D::CalcModelMatrix(position, rotation, scale);
	ITM = mat4f::rotatez(rotation.z) * mat4f::rotatez(rotation.y) * mat4f::rotatez(rotation.x);

	if (!isEmit)//如果发射属性为false，直接退出
	{
		return;
	}

	if (isOneShot)
	{
		Reset();//重置正常发射计数器
		oneShotTimer -= dt;
		if (oneShotTimer < 0)
		{
			particleCount = Math3D::RandFloatToFloat(minEmission, maxEmission);
			oneShotTimer = GenerateParticles(particleCount, particles);
		}
	}
	else
	{
		emittedParticleCount += Math3D::RandFloatToFloat(minEmission, maxEmission) * dt;//发射粒子总数（浮点数）

		int IEmittedParticleCount = (int)emittedParticleCount;//发射粒子总数（整数）
		if (IEmittedParticleCount > 0)
		{
			particleCount = IEmittedParticleCount;
			emittedParticleCount -= IEmittedParticleCount;
			GenerateParticles(particleCount, particles);
		}
	}
}

void ParticleEmitter::SetBirthPS(int birthPS)//设置触发的粒子系统
{
	this->birthPS = birthPS;
}

int ParticleEmitter::GetBirthPS()
{
	return birthPS;
}

void ParticleEmitter::Reset()//重置发射器参数
{
	emittedParticleCount = 0;
	particleCount = 0;
}