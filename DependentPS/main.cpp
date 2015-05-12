#include "mainwindow.h"

using namespace ork;

int main(int argc, char* argv[])
{
	//main执行结束后调用的函数
	atexit(Object::exit);
	MainWindow::app = new MainWindow();
	MainWindow::app->start();
	return 0;
}