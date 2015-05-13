#ifndef _BOID_H
#define  _BOID_H

#include "geometry.h"

struct RenderParam
{
	ork::vec3f pos;//λ��
	float size;//��С
	int tileIndex;//��ͼ���
};

struct Boid
{
	RenderParam renderParam;//���Ի��ƵĲ���
	ork::vec3f vec;//�ٶ�
	float timer;//��ʱ��

	bool isPerching;//�Ƿ�ͣ��
	float perchTimer;//ͣ����ʱ��
	float perchTime;//ͣ��ʱ��

	bool isAvoidPerching;//�Ƿ��ֹͣ��
	float avoidPerchTimer;//ͣ��һ��֮����avoidPerchTimeʱ���ڲ���ͣ��
	float avoidPerchTime;

	AABB location;//��������

	int index;//�ڼ���boid ����
};

#endif //_BOID_H