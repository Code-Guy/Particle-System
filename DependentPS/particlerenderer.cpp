#include "particlerenderer.h"
#include "fileloader.h"

using namespace std;
using namespace ork;

ParticleRenderer::ParticleRenderer()
{
}

ParticleRenderer::~ParticleRenderer()
{
	
}

//��Ⱦ����
void ParticleRenderer::RenderParticles(list<ParticleParameter>& particles, mat4f M, bool isBondWithCamera)
{
	particleProgram->getUniform3f("UVAniamtion")->set(uvAniamtion);

	switch (billboardStyle)
	{
	case Billboard:
		particleProgram->getUniformMatrix4f("MV")->setMatrix(isBondWithCamera ? camera->originV : camera->V);//����ģ����ͼ����
		particleProgram->getUniformMatrix4f("N")->setMatrix(isBondWithCamera ? camera->originN : camera->N);//����ͶӰ����
		particleProgram->getUniformMatrix4f("P")->setMatrix(camera->P);//����ͶӰ����
		particleProgram->getUniform2f("TexSize")->set(texSize);
		break;
	case SortedBillboard:
		particleProgram->getUniformMatrix4f("MV")->setMatrix(camera->V);//����ģ����ͼ����
		particleProgram->getUniformMatrix4f("P")->setMatrix(camera->P);//����ͶӰ����
		particleProgram->getUniform2f("TexSize")->set(texSize);
		//�ȼ���ÿ�����ӵ����
		for (list<ParticleParameter>::iterator iter = particles.begin(); iter != particles.end(); iter++)
		{
			iter->depth = (camera->V * M * iter->renderPara.pos).z;
		}
		particles.sort();//STL��list��vector�����򷽷���ͬ
		break;
	case Stretched:
		particleProgram->getUniformMatrix4f("MVP")->setMatrix(camera->VP);//����ģ����ͼͶӰ����
		particleProgram->getUniform3f("CameraPos")->set(camera->pos);//���������λ��

		//�������ٶȷ����ϵ��������
		for (list<ParticleParameter>::iterator iter = particles.begin(); iter != particles.end(); iter++)
		{
			iter->renderPara.stretchScale = 1 + lengthScale_velocityScale.x + iter->curLinearVelocity.length() * lengthScale_velocityScale.y;
		}
		break;
	case HorizontalBillboard:
		particleProgram->getUniformMatrix4f("MVP")->setMatrix(camera->VP);//����ģ����ͼͶӰ����
		break;
	case VerticalBillboard:
		particleProgram->getUniformMatrix4f("MVP")->setMatrix(camera->VP);//����ģ����ͼͶӰ����
		break;
	default:
		break;
	}
}

void ParticleRenderer::GetBillboardStyle(string billboardStyleName)
{
	if (billboardStyleName == "Billboard")
	{
		this->particleProgram = new Program(new Module(400,
			(char*)FileLoader::LoadFile(GetShaderPath("PSBillboardVS.glsl")),
			(char*)FileLoader::LoadFile(GetShaderPath("PSBillboardGS.glsl")),
			(char*)FileLoader::LoadFile(GetShaderPath("PSBillboardFS.glsl"))));
		billboardStyle = Billboard;
	}
	else if (billboardStyleName == "Sorted Billboard")
	{
		this->particleProgram = new Program(new Module(400,
			(char*)FileLoader::LoadFile(GetShaderPath("PSBillboardVS.glsl")),
			(char*)FileLoader::LoadFile(GetShaderPath("PSBillboardGS.glsl")),
			(char*)FileLoader::LoadFile(GetShaderPath("PSBillboardFS.glsl"))));
		billboardStyle = SortedBillboard;
	}
	else if (billboardStyleName == "Stretch")
	{
		this->particleProgram = new Program(new Module(400,
			(char*)FileLoader::LoadFile(GetShaderPath("PSStretchedVS.glsl")),
			(char*)FileLoader::LoadFile(GetShaderPath("PSStretchedGS.glsl")),
			(char*)FileLoader::LoadFile(GetShaderPath("PSStretchedFS.glsl"))));
		billboardStyle = Stretched;
	}
	else if (billboardStyleName == "HorizontalBillboard")
	{
		this->particleProgram = new Program(new Module(400,
			(char*)FileLoader::LoadFile(GetShaderPath("PSHorizontalBillboardVS.glsl")),
			(char*)FileLoader::LoadFile(GetShaderPath("PSHorizontalBillboardGS.glsl")),
			(char*)FileLoader::LoadFile(GetShaderPath("PSHorizontalBillboardFS.glsl"))));
		billboardStyle = HorizontalBillboard;
	}
	else if (billboardStyleName == "VerticalBillboard")
	{
		this->particleProgram = new Program(new Module(400,
			(char*)FileLoader::LoadFile(GetShaderPath("PSVerticalBillboardVS.glsl")),
			(char*)FileLoader::LoadFile(GetShaderPath("PSVerticalBillboardGS.glsl")),
			(char*)FileLoader::LoadFile(GetShaderPath("PSVerticalBillboardFS.glsl"))));
		billboardStyle = VerticalBillboard;
	}
	else
	{
		this->particleProgram = new Program(new Module(400,
			(char*)FileLoader::LoadFile(GetShaderPath("PSBillboardVS.glsl")),
			(char*)FileLoader::LoadFile(GetShaderPath("PSBillboardGS.glsl")),
			(char*)FileLoader::LoadFile(GetShaderPath("PSBillboardFS.glsl"))));
		billboardStyle = Billboard;
	}
}

//ͨ���ַ�����ȡParticle Material������
void ParticleRenderer::GetParticleMaterialStyle(string particleMatName)
{
	printf("%s\n", particleMatName.c_str());
	if (particleMatName == "Particles/Additive" || particleMatName == "Mobile/Particles/Additive")
	{
		particleMatStyle = PM_Add;
	}
	else if (particleMatName == "Particles/Additive (Soft)" || particleMatName == "Mobile/Particles/Additive (Soft)")
	{
		particleMatStyle = PM_AddSmooth;
	}
	else if (particleMatName == "Particles/Alpha Blended" || particleMatName == "Mobile/Particles/Alpha Blended")
	{
		particleMatStyle = PM_AlphaBlend;
	}
	else if (particleMatName == "Particles/Alpha Blended Premultiply" || particleMatName == "Mobile/Particles/Alpha Blended Premultiply")
	{
		particleMatStyle = PM_PremultiplyBlend;
	}
	else if (particleMatName == "Particles/Multiply" || particleMatName == "Mobile/Particles/Multiply")
	{
		particleMatStyle = PM_Multiply;
	}
	else if (particleMatName == "Particles/Multiply (Double)" || particleMatName == "Mobile/Particles/Multiply (Double)")
	{
		particleMatStyle = PM_MultiplyDouble;
	}
	else if (particleMatName == "Particles/VertexLit Blended" || particleMatName == "Mobile/Particles/VertexLit Blended")
	{
		particleMatStyle = PM_VertexLitBlended;
	}
	else if (particleMatName == "Particles/~Additive-Multiply" || particleMatName == "Mobile/Particles/~Additive-Multiply")
	{
		particleMatStyle = PM_AddMultiply;
	}
}