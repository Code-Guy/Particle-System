#ifndef _PSMANAGER_H
#define  _PSMANAGER_H

#include "particlesystem.h"

class PSManager	//粒子系统管理器
{
public:
	PSManager();
	~PSManager();

	void Render(ork::ptr<ork::FrameBuffer> fb, float dt);
	void Add(ParticleSystem *ps);
	void Add(int ID, ork::vec3f position);
	void AddUnique(ParticleSystem *ps);

	void Remove(ParticleSystem *ps);

	static PSManager *GetInstance();

private:
	std::list<ParticleSystem *> pss;//绘制粒子系统集合
	std::list<ParticleSystem *> waitForRemovePSS;//等待被删除的粒子系统集合
	std::list<ParticleSystem *> uniquePSS;//所有粒子系统集合

	static PSManager *instance;
	
	ParticleSystem *FindByID(int ID);
};

#endif //_PSMANAGER_H