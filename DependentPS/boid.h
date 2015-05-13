#ifndef _BOID_H
#define  _BOID_H

#include "geometry.h"

struct RenderParam
{
	ork::vec3f pos;//位置
	float size;//大小
	int tileIndex;//贴图编号
};

struct Boid
{
	RenderParam renderParam;//用以绘制的参数
	ork::vec3f vec;//速度
	float timer;//计时器

	bool isPerching;//是否停留
	float perchTimer;//停留计时器
	float perchTime;//停留时间

	bool isAvoidPerching;//是否禁止停留
	float avoidPerchTimer;//停留一下之后，在avoidPerchTime时间内不再停留
	float avoidPerchTime;

	AABB location;//所处格子

	int index;//第几个boid 索引
};

#endif //_BOID_H