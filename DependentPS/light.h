#ifndef _LIGHT_H
#define  _LIGHT_H

#include "ork/math/vec3.h"

#define LIGHT_ZNEAR 0.2
#define LIGHT_ZFAR 50

//灯光的类型
enum LightType {PointLight, DirectionLight, SpotLight};

class Light
{
public:
	ork::vec3f pos;
	ork::vec3f color;
	ork::vec3f dir;
	float power;

	LightType lightType;
};

#endif //_LIGHT_H