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
		iter->curTime += dt;//ʱ������
		//���ٴ��ʱ����������������ڵ�����
		if (iter->curTime > iter->lifeTime)
		{
			PSManager::GetInstance()->Add(deathPS, iter->renderPara.pos);
			iter = particles.erase(iter);
			continue;
		}

		//����ʱ�����
		iter->renderPara.size_angle_timeRatio.z = iter->curTime / iter->lifeTime;

		//����λ��
		vec3f curForce = Math3D::RandVec3ToVec3(-rndForce, rndForce) + force;
		//�µ����ٶ�
		vec3f newLinearVelocity = iter->curLinearVelocity + //1.ԭʼ�ٶ�
			curForce * dt; //2.�����ٶȼӳ�
		
		//��ת�ٶ�Rotate Axis
		vec3f rotateAxis = worldRotationAxis + ITM * localRotationAxis;
		if (rotateAxis.length() > FLOAT_EQUAL_EPSILON)
		{
			vec3f rotateAxisDir = rotateAxis.normalize();
			newLinearVelocity = newLinearVelocity.rotateVec(rotateAxis.length() * PI / 180, rotateAxisDir.x, rotateAxisDir.y, rotateAxisDir.z);
		}

		newLinearVelocity += newLinearVelocity * (damping - 1) * dt; //����

		iter->renderPara.pos += (iter->curLinearVelocity + //1.ԭ�ٶ�
			newLinearVelocity //2.�µ��ٶ�
			) * 0.5 * dt;

		iter->curLinearVelocity = newLinearVelocity;

		//������ת�Ƕ�(ע�⣬�ǻ�����)
		iter->renderPara.size_angle_timeRatio.y += iter->curAngleVelocity * dt * ANGLE_TO_RADIAN_RATIO;
		//���ô�С
		iter->renderPara.size_angle_timeRatio.x = iter->bornSize * powf((1 + sizeGrow * iter->curTime), 1.2);
		//�����ٶȷ���
		iter->renderPara.velocityDir = iter->curLinearVelocity.normalize();
		
 		//������ɫ
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

void ParticleAnimator::SetDeathPS(int deathPS)//���ô���������ϵͳ
{
	this->deathPS = deathPS;
}

int ParticleAnimator::GetDeathPS()
{
	return deathPS;
}