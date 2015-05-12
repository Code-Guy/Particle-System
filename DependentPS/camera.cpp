#include "camera.h"
#include "global.h"

using namespace ork;

Camera::Camera(vec3f pos, vec3f view, vec3f up,
	float fov, float aspectRatio, float nearDist, float farDist,
	float mouseSensitivity, float moveSpeed)
	: pos(pos), view(view.normalize()), up(up.normalize()),
	fov(fov), aspectRatio(aspectRatio), nearDist(nearDist), farDist(farDist),
	mouseSensitivity(mouseSensitivity), moveSpeed(moveSpeed)
{
	originPos = pos;
	originUp = up;
	originView = view;

	CalcUVN();

	originV = mat4f(u.x, u.y, u.z, -pos.dotproduct(u),
		v.x, v.y, v.z, -pos.dotproduct(v),
		n.x, n.y, n.z, -pos.dotproduct(n), 
		0, 0, 0, 1);//初始视图矩阵
	
	originN = originV.inverse().transpose();//初始法向矩阵
}

Camera::~Camera()
{
}

void Camera::Yaw(float angle)
{
	float radian = angle * ANGLE_TO_RADIAN_RATIO;
	float cosVal = cos(radian);
	float sinVal = sin(radian);

	vec3f t = n;
	vec3f s = u;

	n = t * cosVal - s * sinVal;
	u = t * sinVal + s * cosVal;
}

void Camera::Pitch(float angle)
{
	float radian = angle * ANGLE_TO_RADIAN_RATIO;
	float cosVal = cos(radian);
	float sinVal = sin(radian);

	vec3f t = v;
	vec3f s = n;

	v = t * cosVal - s * sinVal;
	n = t * sinVal + s * cosVal;
}

void Camera::Roll(float angle)
{
	float radian = angle * ANGLE_TO_RADIAN_RATIO;
	float cosVal = cos(radian);
	float sinVal = sin(radian);

	vec3f t = u;
	vec3f s = v;

	u = t * cosVal - s * sinVal;
	v = t * sinVal + s * cosVal;
}

void Camera::Slide(float du, float dv, float dn)
{
	du *= moveSpeed;
	dv *= moveSpeed;
	dn *= moveSpeed;

	pos.x += du * u.x + dv * v.x + dn * n.x;
	pos.y += du * u.y + dv * v.y + dn * n.y;
	pos.z += du * u.z + dv * v.z + dn * n.z;

	view.x += du * u.x + dv * v.x + dn * n.x;
	view.y += du * u.y + dv * v.y + dn * n.y;
	view.z += du * u.z + dv * v.z + dn * n.z;
}

void Camera::RotateX(float angle)
{
	float d = CalcDisToOrigin();
	int cnt = 100;
	float theta = angle / cnt;
	float slide_d = (float)(-2 * d * sin(theta * PI / 360.0));
	Yaw(theta / 2);
	for (; cnt != 0; --cnt)
	{
		Slide(slide_d, 0, 0);
		Yaw(theta);
	}
	Yaw(-theta / 2);
}

void Camera::RotateY(float angle)
{
	float d = CalcDisToOrigin();
	int cnt = 100;
	float theta = angle / cnt;
	float slide_d = (float)(2 * d * sin(theta * PI / 360.0));
	Pitch(theta / 2);
	for (; cnt != 0; --cnt)
	{
		Slide(0, slide_d, 0);
		Pitch(theta);
	}
	Pitch(-theta / 2);
}

//更新相关参数（矩阵）
void Camera::Update(int w, int h)
{
	V = mat4f(u.x, u.y, u.z, -pos.dotproduct(u),
		v.x, v.y, v.z, -pos.dotproduct(v),
		n.x, n.y, n.z, -pos.dotproduct(n),
		0, 0, 0, 1);

	axesV = mat4f(V[0][0], V[0][1],  V[0][2], -0.9f, 
		V[1][0], V[1][1],  V[1][2], 0.85f,
		V[2][0], V[2][1],  V[2][2], -5.0f,
		0, 0, 0, 1.0f);

	aspectRatio = (float)w / (float)h;
	P = mat4f::perspectiveProjection(fov, aspectRatio, nearDist, farDist);
	O = mat4f::orthoProjection(1, -1, 1, -1, nearDist, farDist);

	VP = P * V;

	N = V.inverse().transpose();
}

//重置摄像机参数
void Camera::Reset()
{
	pos = originPos;
	up = originUp;
	view = originView;

	CalcUVN();
}

//计算摄像机的u v n向量
void Camera::CalcUVN()
{
	vec3f upVector = up - view;

	n = pos - view;
	u = upVector.crossProduct(n);
	v = n.crossProduct(u);

	n = n.normalize();
	u = u.normalize();
	v = v.normalize();
}

//计算摄像机到原点的距离
float Camera::CalcDisToOrigin()
{
	return sqrt(pos.x * pos.x + pos.y * pos.y + pos.z * pos.z);
}