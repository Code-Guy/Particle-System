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

	//������ز���������
	void Update(int w, int h);
	//�������������
	void Reset();

	ork::vec3f pos;//�������λ��
	ork::vec3f view;//������Ĺۿ�Ŀ���
	ork::vec3f up;//����������ϵķ���

	float mouseSensitivity;//����������

	ork::mat4f V;//��ͼ����
	ork::mat4f originV;//��ʼ��ͼ����
	ork::mat4f axesV;//������ר����ͼ����
	ork::mat4f P;//ͶӰ����
	ork::mat4f O;//����ͶӰ����
	ork::mat4f VP;//��ͼͶӰ����
	ork::mat4f N;//normalmatrix
	ork::mat4f originN;//original normalmatrix

private:
	ork::vec3f originPos;//�������λ��
	ork::vec3f originView;//������Ĺۿ�Ŀ���
	ork::vec3f originUp;//����������ϵķ���

	ork::vec3f u, v, n;

	float fov;//�ӽ�
	float aspectRatio;//��߱�
	float nearDist;//������̨���������
	float farDist;//������̨����Զ����
	float moveSpeed;//���°����������ƽ���ٶ�

	//�����������u v n����
	void CalcUVN();
	//�����������ԭ��ľ���
	float CalcDisToOrigin();
};

#endif //_CAMERA_H