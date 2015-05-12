#include "particlesystem.h"
#include "psmanager.h"

#include "global.h"

using namespace ork;

ParticleSystem::ParticleSystem(float lifeTime) : lifeTime(lifeTime)
{
	particleMesh = new Mesh<RenderParameter, unsigned int>(MeshMode::POINTS, MeshUsage::GPU_DYNAMIC);
	particleMesh->addAttributeType(0, 3, AttributeType::A32F, false);
	particleMesh->addAttributeType(1, 4, AttributeType::A32F, false);
	particleMesh->addAttributeType(2, 3, AttributeType::A32F, false);
	particleMesh->addAttributeType(3, 3, AttributeType::A32F, false);
	particleMesh->addAttributeType(4, 1, AttributeType::A32F, false);

	animator = new ParticleAnimator();
	renderer = new ParticleRenderer();

	curTime = 0;

	isBondWithCamera = false;//默认不和摄像机绑定
	particles.clear();
}

ParticleSystem::ParticleSystem(ParticleSystem *ps)
{
	name = ps->name;
	ID = ps->ID;

	EllipsoidParticleEmitter *ellipsoidEmitter;
	MeshParticleEmitter *meshEmitter;

	switch(ps->emitterType)
	{
	case EllipsoidType:
		ellipsoidEmitter = new EllipsoidParticleEmitter();
		*ellipsoidEmitter = *dynamic_cast<EllipsoidParticleEmitter *>(ps->emitter);
		emitter = ellipsoidEmitter;
		break;
	case MeshType:
		meshEmitter = new MeshParticleEmitter();
		*meshEmitter = *dynamic_cast<MeshParticleEmitter *>(ps->emitter);
		emitter = meshEmitter;
		break;
	default:
		ellipsoidEmitter = new EllipsoidParticleEmitter();
		*ellipsoidEmitter = *dynamic_cast<EllipsoidParticleEmitter *>(ps->emitter);
		emitter = ellipsoidEmitter;
		break;
	}

	animator = new ParticleAnimator();
	renderer = new ParticleRenderer();

	*animator = *(ps->animator);
	*renderer = *(ps->renderer);

	emitterType = ps->emitterType;

	particleMesh = ps->particleMesh;
	lifeTime = ps->lifeTime;
	curTime = 0;

	isBondWithCamera = false;

	particles.clear();
}

ParticleSystem::~ParticleSystem()
{
	delete emitter;
	delete animator;
	delete renderer;
}

bool ParticleSystem::operator==(const ParticleSystem &rhs)
{
	return ID == rhs.ID;
}

void ParticleSystem::Render(ptr<FrameBuffer> fb, float dt)//渲染
{
	curTime += dt;

	if (lifeTime != FOREVER && curTime >= lifeTime)//寿命到了
	{
		PSManager::GetInstance()->Remove(this);
		return;
	}

	emitter->EmitParticles(dt, particles);
	animator->AnimateParticles(dt, particles, emitter->ITM, emitter->position);
	renderer->RenderParticles(particles, emitter->M, isBondWithCamera);

	particleMesh->clear();
	for (list<ParticleParameter>::iterator iter = particles.begin(); iter != particles.end(); ++iter)
	{
		particleMesh->addVertex(iter->renderPara);
	}

	SetBlendFunction(fb);
	fb->setDepthMask(false);
	fb->draw(renderer->particleProgram, *particleMesh);
	fb->setDepthMask(true);
	fb->setBlend(false);
}

void ParticleSystem::SetTriggerPS(int birthPS, int deathPS)//设置触发的粒子系统
{
	emitter->SetBirthPS(birthPS);
	animator->SetDeathPS(deathPS);
}

void ParticleSystem::SetTrail(bool flag)//开启拖尾效果
{
	if (renderer->billboardStyle == Billboard)
	{
		renderer->particleProgram->getUniform1i("IsTrail")->set(flag);
	}
}

void ParticleSystem::SetBlur(bool flag)//开启模糊效果
{
	renderer->particleProgram->getUniform1i("IsBlur")->set(flag);
}

void ParticleSystem::SetBondWithCamera(bool flag)
{
	isBondWithCamera = flag;
}

void ParticleSystem::SetBlendFunction(ptr<FrameBuffer> fb)//设置混合方程
{
	switch (renderer->particleMatStyle)
	{
	case PM_Add:
		fb->setBlend(true, ADD, SRC_ALPHA, ONE);
		break;
	case PM_AddMultiply:
		fb->setBlend(true, ADD, ONE, ONE_MINUS_SRC_ALPHA);
		break;
	case PM_AddSmooth:
		//fb->setBlend(true, ADD, ONE, ONE_MINUS_SRC_COLOR);
		fb->setBlend(true, ADD, SRC_ALPHA, ONE);
		break;
	case PM_AlphaBlend:
		fb->setBlend(true, ADD, SRC_ALPHA, ONE_MINUS_SRC_ALPHA);
		break;
	case PM_Multiply:
		fb->setBlend(true, ADD, ZERO, SRC_COLOR);
		break;
	case PM_MultiplyDouble:
		fb->setBlend(true, ADD, DST_COLOR, SRC_COLOR);
		break;
	case PM_PremultiplyBlend:
		fb->setBlend(true, ADD, ONE, ONE_MINUS_SRC_ALPHA);
		break;
	case PM_VertexLitBlended:
		fb->setBlend(true, ADD, SRC_ALPHA, ONE_MINUS_SRC_ALPHA);
		break;
	default:
		fb->setBlend(true, ADD, SRC_ALPHA, ONE_MINUS_SRC_ALPHA);
		break;
	}
}