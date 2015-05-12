#include "meshparticleemitter.h"
#include "psmanager.h"

#include "math3d.h"

using namespace std;
using namespace ork;

MeshParticleEmitter::MeshParticleEmitter()
{
	faceCount = 0;//�������������������һ����ֵ̬
}

MeshParticleEmitter::~MeshParticleEmitter()
{

}

vec3f MeshParticleEmitter::UniformMeshSample(vec3f &normalVelocity)
{
	float rndNormalSpeed = Math3D::RandFloatToFloat(minNormalVelocity, maxNormalVelocity);

	if (!isSystematic)//������������������
	{
		Face rndFace = mesh.faces[Math3D::RandIntToInt(0, mesh.faces.size())];
		if (!isInterpolateTris)//�������ֻ�ڶ���������
		{
			Refer rndRefer = rndFace.refers[Math3D::RandIntToInt(0, 3)];

			normalVelocity = VBO::normals[rndRefer.n] * rndNormalSpeed;
			return VBO::vertices[rndRefer.v];
		}
		else
		{
			float a = Math3D::RandZeroToOne();
			float b = Math3D::RandFloatToFloat(0, 1 - a);
			float c = 1 - a - b;

			normalVelocity = (VBO::vertices[rndFace.refers[0].n] * a +
				VBO::vertices[rndFace.refers[1].n] * b +
				VBO::vertices[rndFace.refers[2].n] * c) * rndNormalSpeed;

			return VBO::vertices[rndFace.refers[0].v] * a +
				VBO::vertices[rndFace.refers[1].v] * b +
				VBO::vertices[rndFace.refers[2].v] * c;
		}
	}
	else
	{
		Face face = mesh.faces[faceCount];

		if (faceCount >= mesh.faces.size() - 1)
		{
			faceCount = 0;
		}
		else
		{
			faceCount++;
		}
		if (!isInterpolateTris)//�������ֻ�ڶ���������
		{
			Refer rndRefer = face.refers[faceCount % 3];

			normalVelocity = VBO::normals[rndRefer.n] * rndNormalSpeed;
			return VBO::vertices[rndRefer.v];
		}
		else
		{
			float a = Math3D::RandZeroToOne();
			float b = Math3D::RandFloatToFloat(0, 1 - a);
			float c = 1 - a - b;

			normalVelocity = (VBO::vertices[face.refers[0].n] * a +
				VBO::vertices[face.refers[1].n] * b +
				VBO::vertices[face.refers[2].n] * c) * rndNormalSpeed;

			return VBO::vertices[face.refers[0].v] * a +
				VBO::vertices[face.refers[1].v] * b +
				VBO::vertices[face.refers[2].v] * c;
		}
	}
}

float MeshParticleEmitter::GenerateParticles(int particleCount, list<ParticleParameter>& particles)//����particleCount������,���������ӵ������������
{
	float maxLifeTime = -INFINITY_NUM;//���������������

	for (int i = 0; i < particleCount; i++)
	{
		ParticleParameter particle;
		vec3f normalVelocity;//�����ٶ�
		particle.bornPos = position + UniformMeshSample(normalVelocity);
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

		//���ٶ�����������ɣ�
		particle.curLinearVelocity = (isSimulateInWorldspace ? velocity*emitterVelocityScale : vec3f::ZERO) + //1.������������ٶȣ���isSimulateInWorldspace�йأ�
			M * localVelocity + //2.�ֲ���������ٶ�
			Math3D::CalcTangentVelocity(particle.bornPos - position, vec3f(1, 1, 1), tangentVelocity) + //3.�����ٶ�
			worldVelocity + //4.�����ٶ�
			Math3D::RandVec3ToVec3(-rndVelocity, rndVelocity) + //5.����ٶ�
			normalVelocity;//6.�����ٶ�

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