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
	virtual float GenerateParticles(int particleCount, std::list<ParticleParameter>& particles);//产生particleCount个粒子,所产生粒子的最大生命周期
};

#endif //_ELLIPSOIDPARTICLEEMITTER_H