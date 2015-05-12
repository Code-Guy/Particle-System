#include "ellipsoidparticleemitter.h"
#include "psmanager.h"

#include "math3d.h"

using namespace std;
using namespace ork;

EllipsoidParticleEmitter::EllipsoidParticleEmitter()
{

}

EllipsoidParticleEmitter::~EllipsoidParticleEmitter()
{
}

//��׼����ֲ�(minEmitterRangeΪ0���������� minEmitterRangeΪ1�������)
vec3f EllipsoidParticleEmitter::UniformEllipsoidSample(const vec3f & ellipsoid, float minEmitterRange)
{
	//�ȼ����׼����ֲ�
	float a = Math3D::RandFloatToFloat(-180, 180) * ANGLE_TO_RADIAN_RATIO;
	float b = acos(2 * Math3D::RandZeroToOne() - 1);
	float r = pow(Math3D::RandZeroToOne() * (1 - pow(minEmitterRange, 3) + pow(minEmitterRange, 3)), 1 / 3);

	vec3f sampleP;
	sampleP.x = r * sin(b) * cos(a);
	sampleP.y = r * sin(b) * sin(a);
	sampleP.z = r * cos(b);

	return sampleP * ellipsoid;
}

float EllipsoidParticleEmitter::GenerateParticles(int particleCount, list<ParticleParameter>& particles)//����particleCount������,���������ӵ������������
{
	float maxLifeTime = -INFINITY_NUM;//���������������

	for (int i = 0; i < particleCount; i++)
	{
		ParticleParameter particle;
		particle.bornPos = position + UniformEllipsoidSample(ellipoid, minEmitterRange);
		particle.bornSize = Math3D::RandFloatToFloat(minSize, maxSize);
		particle.bornAngle = rndRotation ? Math3D::RandFloatToFloat(0, 360) : 0;
		particle.lifeTime = Math3D::RandFloatToFloat(minEnergy, maxEnergy);

		if (particle.lifeTime > maxLifeTime)
		{
			maxLifeTime = particle.lifeTime;
		}
		//���ٶ�����������ɣ�
		particle.curAngleVelocity = angularVelocity + //1.���ٶ�
			Math3D::RandFloatToFloat(-rndAngularVelocity, rndAngularVelocity);//2.������ٶ�

		//���ٶ����岿����ɣ�
		particle.curLinearVelocity = (isSimulateInWorldspace ? velocity*emitterVelocityScale : vec3f::ZERO) + //1.������������ٶȣ���isSimulateInWorldspace�йأ�
			ITM * localVelocity + //2.�ֲ���������ٶ�
			Math3D::CalcTangentVelocity(particle.bornPos - position, ellipoid, tangentVelocity) + //3.�����ٶ�
			worldVelocity + //4.�����ٶ�
			Math3D::RandVec3ToVec3(-rndVelocity, rndVelocity); //5.����ٶ�

		particle.curTime = 0;

		particle.renderPara.pos = particle.bornPos;

		particle.renderPara.size_angle_timeRatio.x = particle.bornSize;
		particle.renderPara.size_angle_timeRatio.y = particle.bornAngle;
		particle.renderPara.size_angle_timeRatio.z = 0;

		particles.push_back(particle);

		PSManager::GetInstance()->Add(birthPS, particle.renderPara.pos);
	}

	return maxLifeTime;
}