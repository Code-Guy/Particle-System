#ifndef _PSMANAGER_H
#define  _PSMANAGER_H

#include "particlesystem.h"

class PSManager	//����ϵͳ������
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
	std::list<ParticleSystem *> pss;//��������ϵͳ����
	std::list<ParticleSystem *> waitForRemovePSS;//�ȴ���ɾ��������ϵͳ����
	std::list<ParticleSystem *> uniquePSS;//��������ϵͳ����

	static PSManager *instance;
	
	ParticleSystem *FindByID(int ID);
};

#endif //_PSMANAGER_H