#include "meshparticleemitter.h"
#include "psmanager.h"

#include "math3d.h"

using namespace std;
using namespace ork;

MeshParticleEmitter::MeshParticleEmitter()
{
	faceCount = 0;//遍历过的面的数量，是一个静态值
}

MeshParticleEmitter::~MeshParticleEmitter()
{

}

vec3f MeshParticleEmitter::UniformMeshSample(vec3f &normalVelocity)
{
	float rndNormalSpeed = Math3D::RandFloatToFloat(minNormalVelocity, maxNormalVelocity);

	if (!isSystematic)//如果粒子生成是无序的
	{
		Face rndFace = mesh.faces[Math3D::RandIntToInt(0, mesh.faces.size())];
		if (!isInterpolateTris)//如果粒子只在顶点上生成
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
		if (!isInterpolateTris)//如果粒子只在顶点上生成
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

float MeshParticleEmitter::GenerateParticles(int particleCount, list<ParticleParameter>& particles)//产生particleCount个粒子,所产生粒子的最大生命周期
{
	float maxLifeTime = -INFINITY_NUM;//最大粒子生命周期

	for (int i = 0; i < particleCount; i++)
	{
		ParticleParameter particle;
		vec3f normalVelocity;//法向速度
		particle.bornPos = position + UniformMeshSample(normalVelocity);
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

		//线速度由六部分组成：
		particle.curLinearVelocity = (isSimulateInWorldspace ? velocity*emitterVelocityScale : vec3f::ZERO) + //1.发射器本身的速度（和isSimulateInWorldspace有关）
			M * localVelocity + //2.局部到世界的速度
			Math3D::CalcTangentVelocity(particle.bornPos - position, vec3f(1, 1, 1), tangentVelocity) + //3.切线速度
			worldVelocity + //4.世界速度
			Math3D::RandVec3ToVec3(-rndVelocity, rndVelocity) + //5.随机速度
			normalVelocity;//6.法向速度

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