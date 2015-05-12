#include "skybox.h"
#include "fileloader.h"

using namespace std;
using namespace ork;

Skybox::Skybox(string texCubePath)
{
	mesh = new Mesh<vec4f, unsigned int>(TRIANGLE_STRIP, GPU_STATIC);
	mesh->addAttributeType(0, 4, A32F, false);

	mesh->addVertex(vec4f(-1, -1, 0, 0));
	mesh->addVertex(vec4f(1, -1, 1, 0));
	mesh->addVertex(vec4f(-1, 1, 0, 1));
	mesh->addVertex(vec4f(1, 1, 1, 1));

	texture = FileLoader::LoadTextureCube(GetTexturePath("skybox/FishermansBastion"));

	program = FileLoader::LoadProgram(GetShaderPath("SkyBox.glsl"));
	program->getUniformSampler("skyboxTex")->set(texture);
}

Skybox::~Skybox()
{

}

void Skybox::Render(ptr<FrameBuffer> fb)
{
	fb->setDepthMask(false);

	program->getUniformMatrix4f("screenToModelMat")->setMatrix(camera->VP.inverse());
	program->getUniform3f("cameraPos")->set(camera->pos);
	fb->draw(program, *mesh);

	fb->setDepthMask(true);
}