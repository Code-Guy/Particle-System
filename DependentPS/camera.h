#ifndef _CAMERA_H
#define  _CAMERA_H

#include "ork/math/mat4.h"

class Camera
{
public:
	Camera(ork::vec3f pos, ork::vec3f view, ork::vec3f up,
		float fov, float aspectRatio, float nearDist, float farDist,
		float mouseSensitivity, float moveSpeed);

	~Camera();

	void Yaw(float angle);
	void Pitch(float angle);
	void Roll(float angle);

	void Slide(float du, float dv, float dn);

	void RotateX(float angle);
	void RotateY(float angle);

	//更新相关参数（矩阵）
	void Update(int w, int h);
	//重置摄像机参数
	void Reset();

	ork::vec3f pos;//摄像机的位置
	ork::vec3f view;//摄像机的观看目标点
	ork::vec3f up;//摄像机的向上的方向

	float mouseSensitivity;//鼠标的灵敏度

	ork::mat4f V;//视图矩阵
	ork::mat4f originV;//初始视图矩阵
	ork::mat4f axesV;//坐标轴专用视图矩阵
	ork::mat4f P;//投影矩阵
	ork::mat4f O;//正交投影矩阵
	ork::mat4f VP;//视图投影矩阵
	ork::mat4f N;//normalmatrix
	ork::mat4f originN;//original normalmatrix

private:
	ork::vec3f originPos;//摄像机的位置
	ork::vec3f originView;//摄像机的观看目标点
	ork::vec3f originUp;//摄像机的向上的方向

	ork::vec3f u, v, n;

	float fov;//视角
	float aspectRatio;//宽高比
	float nearDist;//视域棱台的最近距离
	float farDist;//视域棱台的最远距离
	float moveSpeed;//按下按键摄像机的平移速度

	//计算摄像机的u v n向量
	void CalcUVN();
	//计算摄像机到原点的距离
	float CalcDisToOrigin();
};

#endif //_CAMERA_H