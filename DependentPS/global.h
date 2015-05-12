#ifndef _GLOBAL_H
#define  _GLOBAL_H

#include "camera.h"
#include "light.h"
#include "ork/render/FrameBuffer.h"

#define PI 3.1415926	//PI
#define ANGLE_TO_RADIAN_RATIO PI / 180	//�Ƕȵ����ȵ�ת��ϵ��
#define FLOAT_EQUAL_EPSILON 0.0001	//�жϸ�������ȵ���ֵ
#define INFINITY_NUM 1e8	//�������

/** ���е�ȫ�ֱ��� **/
extern Camera *camera;//�����
extern Light light1;//һ��ȫ�ֵƹ�
extern Light light2;//�ƹ�2

//��Ļ����
extern ork::vec2f ScreenSize;
extern float ratioX;
extern float ratioY;
extern float ratio;
extern ork::vec2f ratioXY;

#endif //_GLOBAL_H