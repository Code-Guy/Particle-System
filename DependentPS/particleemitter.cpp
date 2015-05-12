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

	//����ϵͳ������������ٶ�Ĭ��Ϊ0
	velocity = vec3f::ZERO;
}

ParticleEmitter::~ParticleEmitter()
{

}

//��������
void ParticleEmitter::EmitParticles(float dt, list<ParticleParameter>& particles)
{
	//����ģ�;���
	M = Math3D::CalcModelMatrix(position, rotation, scale);
	ITM = mat4f::rotatez(rotation.z) * mat4f::rotatez(rotation.y) * mat4f::rotatez(rotation.x);

	if (!isEmit)//�����������Ϊfalse��ֱ���˳�
	{
		return;
	}

	if (isOneShot)
	{
		Reset();//�����������������
		oneShotTimer -= dt;
		if (oneShotTimer < 0)
		{
			particleCount = Math3D::RandFloatToFloat(minEmission, maxEmission);
			oneShotTimer = GenerateParticles(particleCount, particles);
		}
	}
	else
	{
		emittedParticleCount += Math3D::RandFloatToFloat(minEmission, maxEmission) * dt;//����������������������

		int IEmittedParticleCount = (int)emittedParticleCount;//��������������������
		if (IEmittedParticleCount > 0)
		{
			particleCount = IEmittedParticleCount;
			emittedParticleCount -= IEmittedParticleCount;
			GenerateParticles(particleCount, particles);
		}
	}
}

void ParticleEmitter::SetBirthPS(int birthPS)//���ô���������ϵͳ
{
	this->birthPS = birthPS;
}

int ParticleEmitter::GetBirthPS()
{
	return birthPS;
}

void ParticleEmitter::Reset()//���÷���������
{
	emittedParticleCount = 0;
	particleCount = 0;
}