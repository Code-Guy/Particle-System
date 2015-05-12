#ifndef _ELLIPSOIDPARTICLEEMITTER_H
#define  _ELLIPSOIDPARTICLEEMITTER_H

#include "particleemitter.h"

class EllipsoidParticleEmitter : public ParticleEmitter
{
public:
	EllipsoidParticleEmitter();
	~EllipsoidParticleEmitter();

	ork::vec3f ellipoid;
	float minEmitterRange;

private:
	ork::vec3f UniformEllipsoidSample(const ork::vec3f & ellipsoid, float minEmitterRange);
	virtual float GenerateParticles(int particleCount, std::list<ParticleParameter>& particles);//����particleCount������,���������ӵ������������
};

#endif //_ELLIPSOIDPARTICLEEMITTER_H