#ifndef _SKYBOX_H
#define  _SKYBOX_H

#include "ork/render/FrameBuffer.h"
#include <string>

class Skybox
{
public:
	Skybox(std::string texCubePath);
	~Skybox();

	void Render(ork::ptr<ork::FrameBuffer> fb);

private:
	ork::ptr<ork::Program> program;
	ork::ptr< ork::Mesh<ork::vec4f, unsigned int> > mesh;
	ork::ptr<ork::TextureCube> texture;
};

#endif //_SKYBOX_H