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

	//重绘函数 t:重绘时刻 dt:上一次重绘时间间隔
	virtual void redisplay(double t, double dt);
	//窗口大小变化调用函数
	virtual void reshape(int x, int y);

	//键盘响应
	bool keyTyped(unsigned char c, EventHandler::modifier m, int x, int y);
	bool mouseMotion(int x, int y);
	bool mousePassiveMotion(int x, int y);
	bool mouseClick(EventHandler::button b, EventHandler::state s, EventHandler::modifier m, int x, int y);

	static ork::static_ptr<Window> app;//静态的window变量

protected:
	void Logic(float dt);//逻辑
	void Render(float dt);//绘制

private:
	//FBO
	ork::ptr<ork::FrameBuffer> fb;

	//天空盒
	Skybox *skybox;
	//坐标轴
	Model *axes;
	//粒子系统管理器
	PSManager *psMng;
	//Boid管理器
	BoidManager *boidMng;

	//鼠标位置参数
	int oldX, oldY;
	int halfW, halfH;

	//初始化灯光
	void InitLight();
	//初始化模型
	void InitModel();
	//初始化粒子系统
	void InitParticleSystem();
	//初始化Boid模型
	void InitBoid();

	//更新键盘事件
	void UpdateKey();
};

#endif //_MAINWINDOW_H
