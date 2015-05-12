#ifndef _PARTICLEPARAMETER_H
#define  _PARTICLEPARAMETER_H

#include "ork/math/vec3.h"
#include "ork/math/vec4.h"

#include "ork/render/FrameBuffer.h"

#include <list>
#include <string>

//����billboard����
enum BillboardStyle
{
	Billboard = 0,
	SortedBillboard,
	Stretched,
	HorizontalBillboard,
	VerticalBillboard
};

//�������Ӳ�������
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

//���ӵ���Ⱦ���ԣ���ʱ��ı仯���仯��
struct RenderParameter
{
	ork::vec3f pos;//λ��
	ork::vec4f color;//��ɫ
	ork::vec3f size_angle_timeRatio;//��С + ��ת�Ƕ� + ��ǰ��ȥ��ʱ��ռ���������ڵı���
	ork::vec3f velocityDir;//�ٶȷ���
	float stretchScale;//���ٶȷ����ϵ��������
};

//���ӵĸ��ֲ���
struct ParticleParameter
{
	//�������������ӵ����������ڲ���
	ork::vec3f bornPos;//����λ��
	float bornSize;//������С
	float bornAngle;//������ת�Ƕ�

	float lifeTime;//��������

	float curTime;//��ǰʱ��
	float curAngleVelocity;//��ǰ���ٶ�
	ork::vec3f curLinearVelocity;//��ǰ���ٶ�

	float depth;//������������

	RenderParameter renderPara;//��Ҫ����shader�������������

	//���Խ�������Խ�Ȼ�
	bool operator<(const ParticleParameter& rhs) const 
	{
		return this->depth > rhs.depth;
	}
};

#endif //_PARTICLEPARAMETER_H