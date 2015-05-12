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

//标准椭球分布(minEmitterRange为0：整个椭球 minEmitterRange为1：椭球壳)
vec3f EllipsoidParticleEmitter::UniformEllipsoidSample(const vec3f & ellipsoid, float minEmitterRange)
{
	//先计算标准正球分布
	float a = Math3D::RandFloatToFloat(-180, 180) * ANGLE_TO_RADIAN_RATIO;
	float b = acos(2 * Math3D::RandZeroToOne() - 1);
	float r = pow(Math3D::RandZeroToOne() * (1 - pow(minEmitterRange, 3) + pow(minEmitterRange, 3)), 1 / 3);

	vec3f sampleP;
	sampleP.x = r * sin(b) * cos(a);
	sampleP.y = r * sin(b) * sin(a);
	sampleP.z = r * cos(b);

	return sampleP * ellipsoid;
}

float EllipsoidParticleEmitter::GenerateParticles(int particleCount, list<ParticleParameter>& particles)//产生particleCount个粒子,所产生粒子的最大生命周期
{
	float maxLifeTime = -INFINITY_NUM;//最大粒子生命周期

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
		//角速度有两部分组成：
		particle.curAngleVelocity = angularVelocity + //1.角速度
			Math3D::RandFloatToFloat(-rndAngularVelocity, rndAngularVelocity);//2.随机角速度

		//线速度由五部分组成：
		particle.curLinearVelocity = (isSimulateInWorldspace ? velocity*emitterVelocityScale : vec3f::ZERO) + //1.发射器本身的速度（和isSimulateInWorldspace有关）
			ITM * localVelocity + //2.局部到世界的速度
			Math3D::CalcTangentVelocity(particle.bornPos - position, ellipoid, tangentVelocity) + //3.切线速度
			worldVelocity + //4.世界速度
			Math3D::RandVec3ToVec3(-rndVelocity, rndVelocity); //5.随机速度

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