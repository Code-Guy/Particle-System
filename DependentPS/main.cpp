#include "mainwindow.h"

using namespace ork;

int main(int argc, char* argv[])
{
	//mainִ�н�������õĺ���
	atexit(Object::exit);
	MainWindow::app = new MainWindow();
	MainWindow::app->start();
	return 0;
}