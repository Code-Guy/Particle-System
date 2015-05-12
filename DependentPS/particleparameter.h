#ifndef _PARTICLEPARAMETER_H
#define  _PARTICLEPARAMETER_H

#include "ork/math/vec3.h"
#include "ork/math/vec4.h"

#include "ork/render/FrameBuffer.h"

#include <list>
#include <string>

//各种billboard类型
enum BillboardStyle
{
	Billboard = 0,
	SortedBillboard,
	Stretched,
	HorizontalBillboard,
	VerticalBillboard
};

//各种粒子材质类型
enum ParticleMaterialStyle
{
	PM_Add = 0,
	PM_AddMultiply,
	PM_AddSmooth,
	PM_AlphaBlend,
	PM_Blend,
	PM_Multiply,
	PM_MultiplyDouble,
	PM_PremultiplyBlend,
	PM_VertexLitBlended
};

//粒子的渲染属性（随时间的变化而变化）
struct RenderParameter
{
	ork::vec3f pos;//位置
	ork::vec4f color;//颜色
	ork::vec3f size_angle_timeRatio;//大小 + 旋转角度 + 当前过去的时间占总生命周期的比例
	ork::vec3f velocityDir;//速度方向
	float stretchScale;//沿速度方向上的拉伸比例
};

//粒子的各种参数
struct ParticleParameter
{
	//以下属性在粒子的生命周期内不变
	ork::vec3f bornPos;//出生位置
	float bornSize;//出生大小
	float bornAngle;//出生旋转角度

	float lifeTime;//生命周期

	float curTime;//当前时刻
	float curAngleVelocity;//当前角速度
	ork::vec3f curLinearVelocity;//当前线速度

	float depth;//到摄像机的深度

	RenderParameter renderPara;//需要传到shader里面的粒子数据

	//深度越大的粒子越先画
	bool operator<(const ParticleParameter& rhs) const 
	{
		return this->depth > rhs.depth;
	}
};

#endif //_PARTICLEPARAMETER_H