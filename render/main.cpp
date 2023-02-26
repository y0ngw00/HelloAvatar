#include <iostream>
#include "Window.h"
#ifdef __APPLE__
	#include <OpenGL/gl.h>
	#include <GLUT/glut.h>
#else
	#include <GL/gl.h>
	#include <GL/glut.h>
	#include <GL/glew.h>
#endif

int main(int argc,char** argv)
{
	Window* window;
	window = new Window();
	glutInit(&argc,argv);
	
	window->initWindow(1920,1080,"kinematics");
	glutMainLoop();

	return 0;
}