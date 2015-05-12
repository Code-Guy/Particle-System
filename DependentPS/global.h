#ifndef _GLOBAL_H
#define  _GLOBAL_H

#include "camera.h"
#include "light.h"
#include "ork/render/FrameBuffer.h"

#define PI 3.1415926	//PI
#define ANGLE_TO_RADIAN_RATIO PI / 180	//角度到弧度的转化系数
#define FLOAT_EQUAL_EPSILON 0.0001	//判断浮点数相等的阈值
#define INFINITY_NUM 1e8	//无穷大数

/** 所有的全局变量 **/
extern Camera *camera;//摄像机
extern Light light1;//一个全局灯光
extern Light light2;//灯光2

//屏幕变量
extern ork::vec2f ScreenSize;
extern float ratioX;
extern float ratioY;
extern float ratio;
extern ork::vec2f ratioXY;

#endif //_GLOBAL_H