#include "psmanager.h"

using namespace std;
using namespace ork;

PSManager *PSManager::instance = NULL;

PSManager::PSManager()
{

}

PSManager::~PSManager()
{
	for (list<ParticleSystem *>::iterator iter = uniquePSS.begin(); iter != uniquePSS.end(); iter++)
	{
		delete *iter;
	}
	pss.clear();
	uniquePSS.clear();
}

void PSManager::Render(ptr<FrameBuffer> fb, float dt)
{
	for (list<ParticleSystem *>::iterator iter = pss.begin(); iter != pss.end(); iter++)
	{
		(*iter)->Render(fb, dt);
	}

	if (waitForRemovePSS.size() != 0)
	{
		for (list<ParticleSystem *>::iterator iter = pss.begin(); iter != pss.end(); )
		{
			list<ParticleSystem *>::iterator fIter = find(waitForRemovePSS.begin(), waitForRemovePSS.end(), *iter);
			if (fIter != waitForRemovePSS.end())
			{
				delete *iter;
				iter = pss.erase(iter);
			}
			else
			{
				iter++;
			}
		}
		waitForRemovePSS.clear();
	}
}

void PSManager::Add(ParticleSystem *ps)
{
	pss.push_back(ps);
	AddUnique(ps);
}

void PSManager::Add(int ID, ork::vec3f position)
{
	if (ID != INVALID_ID)
	{
		ParticleSystem *ps = FindByID(ID);
		if (ps == NULL)
		{
			return;
		}
		ps->emitter->position = position;
		pss.push_back(new ParticleSystem(ps));
	}
}

void PSManager::AddUnique(ParticleSystem *ps)
{
	uniquePSS.push_back(ps);
}

void PSManager::Remove(ParticleSystem *ps)
{
	waitForRemovePSS.push_back(ps);
}

ParticleSystem *PSManager::FindByID(int ID)
{
	for (list<ParticleSystem *>::iterator iter = uniquePSS.begin(); iter != uniquePSS.end(); iter++)
	{
		if ((*iter)->ID == ID)
		{
			return *iter;
		}
	}
	return NULL;
}

PSManager *PSManager::GetInstance()
{
	if (instance == NULL)
	{
		instance = new PSManager();
	}
	return instance;
}