#ifndef _MAINWINDOW_H
#define  _MAINWINDOW_H

#define WIDTH 1024
#define HEIGHT 768

#define TIME_RATIO 1e-6

#include <windows.h>

#include "model.h"
#include "skybox.h"
#include "psmanager.h"
#include "boidmanager.h"

#include "ork/ui/GlutWindow.h"

class MainWindow : public ork::GlutWindow
{
public:
	MainWindow();
	~MainWindow();

	//�ػ溯�� t:�ػ�ʱ�� dt:��һ���ػ�ʱ����
	virtual void redisplay(double t, double dt);
	//���ڴ�С�仯���ú���
	virtual void reshape(int x, int y);

	//������Ӧ
	bool keyTyped(unsigned char c, EventHandler::modifier m, int x, int y);
	bool mouseMotion(int x, int y);
	bool mousePassiveMotion(int x, int y);
	bool mouseClick(EventHandler::button b, EventHandler::state s, EventHandler::modifier m, int x, int y);

	static ork::static_ptr<Window> app;//��̬��window����

protected:
	void Logic(float dt);//�߼�
	void Render(float dt);//����

private:
	//FBO
	ork::ptr<ork::FrameBuffer> fb;

	//��պ�
	Skybox *skybox;
	//������
	Model *axes;
	//����ϵͳ������
	PSManager *psMng;
	//Boid������
	BoidManager *boidMng;

	//���λ�ò���
	int oldX, oldY;
	int halfW, halfH;

	//��ʼ���ƹ�
	void InitLight();
	//��ʼ��ģ��
	void InitModel();
	//��ʼ������ϵͳ
	void InitParticleSystem();
	//��ʼ��Boidģ��
	void InitBoid();

	//���¼����¼�
	void UpdateKey();
};

#endif //_MAINWINDOW_H
