#include "particleanimator.h"
#include "psmanager.h"

#include "math3d.h"

using namespace std;
using namespace ork;

ParticleAnimator::ParticleAnimator()
{
	deathPS = INVALID_ID;
}

ParticleAnimator::~ParticleAnimator()
{

}

void ParticleAnimator::AnimateParticles(float dt, list<ParticleParameter>& particles, mat4f ITM, vec3f position)
{
	for (list<ParticleParameter>::iterator iter = particles.begin(); iter != particles.end();)
	{
		iter->curTime += dt;//时间累增
		//销毁存活时间大于自身生命周期的粒子
		if (iter->curTime > iter->lifeTime)
		{
			PSManager::GetInstance()->Add(deathPS, iter->renderPara.pos);
			iter = particles.erase(iter);
			continue;
		}

		//计算时间比例
		iter->renderPara.size_angle_timeRatio.z = iter->curTime / iter->lifeTime;

		//设置位置
		vec3f curForce = Math3D::RandVec3ToVec3(-rndForce, rndForce) + force;
		//新的线速度
		vec3f newLinearVelocity = iter->curLinearVelocity + //1.原始速度
			curForce * dt; //2.力的速度加成
		
		//旋转速度Rotate Axis
		vec3f rotateAxis = worldRotationAxis + ITM * localRotationAxis;
		if (rotateAxis.length() > FLOAT_EQUAL_EPSILON)
		{
			vec3f rotateAxisDir = rotateAxis.normalize();
			newLinearVelocity = newLinearVelocity.rotateVec(rotateAxis.length() * PI / 180, rotateAxisDir.x, rotateAxisDir.y, rotateAxisDir.z);
		}

		newLinearVelocity += newLinearVelocity * (damping - 1) * dt; //阻尼

		iter->renderPara.pos += (iter->curLinearVelocity + //1.原速度
			newLinearVelocity //2.新的速度
			) * 0.5 * dt;

		iter->curLinearVelocity = newLinearVelocity;

		//设置自转角度(注意，是弧度制)
		iter->renderPara.size_angle_timeRatio.y += iter->curAngleVelocity * dt * ANGLE_TO_RADIAN_RATIO;
		//设置大小
		iter->renderPara.size_angle_timeRatio.x = iter->bornSize * powf((1 + sizeGrow * iter->curTime), 1.2);
		//设置速度方向
		iter->renderPara.velocityDir = iter->curLinearVelocity.normalize();
		
 		//设置颜色
  		if (doesAnimateColor)
  		{
  			float indexTime = iter->renderPara.size_angle_timeRatio.z * 4; 
			int indexTimeInt = (int)indexTime;
  			if (indexTimeInt == 4)
  			{
  				iter->renderPara.color = animationColor[4];
  			}
  			else
  			{
  				float timelerp = indexTime - indexTimeInt;
  				iter->renderPara.color = Math3D::Vec4Lerp(animationColor[indexTimeInt], animationColor[indexTimeInt + 1], timelerp);
  			}
  		}
  		else
  		{
 			iter->renderPara.color = vec4f(1.0, 1.0, 1.0, 1.0);
 		}

		iter->renderPara.color = iter->renderPara.color * tintColor;

		iter++;
	}
}

void ParticleAnimator::SetDeathPS(int deathPS)//设置触发的粒子系统
{
	this->deathPS = deathPS;
}

int ParticleAnimator::GetDeathPS()
{
	return deathPS;
}