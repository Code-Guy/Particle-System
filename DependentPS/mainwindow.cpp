#include "mainwindow.h"
#include "fileloader.h"
#include <time.h>

using namespace ork;

static_ptr<Window> MainWindow::app;

/** 所有的全局变量 **/
Camera *camera = NULL;//摄像机
Light light1;//一个全局灯光
Light light2;//灯光2

//屏幕变量
vec2f ScreenSize = vec2f(WIDTH, HEIGHT);
float ratioX;
float ratioY;
float ratio;
vec2f ratioXY;

MainWindow::MainWindow() : GlutWindow(Window::Parameters().name("Particle System").size(WIDTH, HEIGHT))
{
	//1.初始化摄像机
	camera = new Camera(vec3f(0, 1, 20), vec3f(0, -1, -1), vec3f(0, 1, 0), 
		60.0f, 9.0/16.0, 0.1f, 1000.0f, 
		0.1f, 0.1f);
	//2.初始化灯光
	InitLight();
	//3.初始化FBO
	fb = FrameBuffer::getDefault();
	//4.初始化天空盒
	skybox = new Skybox("skybox/FishermansBastion");
	//5.初始化模型
	InitModel();
	//6.初始化粒子系统
	InitParticleSystem();
	//7.初始化Boid
	InitBoid();

	srand((unsigned)time(NULL));
}

MainWindow::~MainWindow()
{
 	delete camera;
	delete psMng;
 	delete axes;
}

//重绘函数 t:重绘时刻 dt:上一次重绘时间间隔
void MainWindow::redisplay(double t, double dt)
{
	dt *= TIME_RATIO;

	fb->setClearColor(vec4f(0, 0, 0, 1.0));
	fb->clear(true, false, true);
	fb->setDepthTest(true);

	Logic(dt);
	Render(dt);

	GlutWindow::redisplay(t, dt);
}

//窗口大小变化调用函数
void MainWindow::reshape(int x, int y)
{
	FrameBuffer::getDefault()->setViewport(vec4<GLint>(0, 0, x, y));
	halfW = x / 2;
	halfH = y / 2;
	GlutWindow::reshape(x, y);
	idle(false);
}

//键盘响应
bool MainWindow::keyTyped(unsigned char c, EventHandler::modifier m, int x, int y)
{
	if (c == 27) {
		::exit(0);
	}
	if (c == 'f' || c == 'F'){
	}
	return true;
}

//更新键盘事件
void MainWindow::UpdateKey()
{
	if (GetAsyncKeyState('W')){
		camera->Slide(0, 0, -1);
	}
	if (GetAsyncKeyState('S')){
		camera->Slide(0, 0, 1);
	}
	if (GetAsyncKeyState('A')){
		camera->Slide(-1, 0, 0);
	}
	if (GetAsyncKeyState('D')){
		camera->Slide(1, 0, 0);
	}
	if (GetAsyncKeyState('R')){
		camera->Reset();
	}
}

bool MainWindow::mouseMotion(int x, int y)
{
	float dx, dy;

	x -= halfW;
	y -= halfH;

	dx = (float)(x - oldX) * camera->mouseSensitivity;
	dy = (float)(y - oldY) * camera->mouseSensitivity;

	oldX = x;
	oldY = y;

	camera->RotateX(dx);
	camera->RotateY(dy);

	return true;
}

bool MainWindow::mousePassiveMotion(int x, int y)
{
	return true;
}

bool MainWindow::mouseClick(EventHandler::button b, EventHandler::state s, EventHandler::modifier m, int x, int y)
{
	oldX = x - halfW;
	oldY = y - halfH;
	idle(false);
	return true;
}

void MainWindow::Logic(float dt)//逻辑
{
	//1.更新键盘事件
	UpdateKey();

	//2.更新摄像机参数
	vec4i viewport = fb->getViewport();
	camera->Update(viewport.z, viewport.w);

	boidMng->Logic(dt);
}

void MainWindow::Render(float dt)//绘制
{
	//1.画天空盒
	skybox->Render(fb);

	//2.画粒子系统
	//psMng->Render(fb, dt);
	boidMng->Render(fb);
	//3.画坐标轴
	axes->Render(fb);
}

//初始化灯光
void MainWindow::InitLight()
{
	light1.pos = vec3f(0.0f, 40.0f, 0.0f);
	light1.color = vec3f(1.0f, 1.0f, 1.0f);
	light1.dir = vec3f(1.0f, 1.0f, 1.0f);
	light1.power = 1.0f;
	light1.lightType = PointLight;//其实是PointLight，只是SpotLight比较好做ShadowMap而已

	light2.pos = vec3f(0.0f, 40.0f, -30.0f);
	light2.color = vec3f(1.0f, 1.0f, 1.0f);
	light2.dir = vec3f(0.5f, 2, 2);
	light2.power = 1.5f;
	light2.lightType = PointLight;//其实是PointLight，只是SpotLight比较好做ShadowMap而已
}

//初始化模型
void MainWindow::InitModel()
{
	axes = new Model("axes.obj", false, true);
}

//初始化粒子系统
void MainWindow::InitParticleSystem()
{
	//得到静态粒子系统集合
	psMng = PSManager::GetInstance();

	ParticleSystem *ps1 = new ParticleSystem();
	FileLoader::LoadParticleSystem("LeavesA.para", ps1);

	ParticleSystem *ps2 = new ParticleSystem(3);
	FileLoader::LoadParticleSystem("FX_Snow.para", ps2);

	ps1->SetTriggerPS(INVALID_ID, ps2->ID);

	psMng->Add(ps1);
	psMng->AddUnique(ps2);
}

//初始化Boid模型
void MainWindow::InitBoid()
{
	boidMng = new BoidManager(100, 
		vec3f(5, 5, 5), 1,
		0.5, 1,
		vec3f(-0.5, -0.5, -0.5), vec3f(0.5, 0.5, 0.5),
		2, 4,
		20, 4);

	boidMng->SetTexture("bird.png", vec2i(4, 2), 10);
	boidMng->SetParams(0.01, 0.01, 0.005);
	boidMng->SetLimit(AABB(-10, 10, -10, 10, -10, 10), 3);
	boidMng->Init();
}