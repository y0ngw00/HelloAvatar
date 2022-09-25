#include <iostream>
#include "Window.h"

#include <GL/glew.h>
#include <GL/glut.h>

int main(int argc,char** argv)
{
	Window* window;
	window = new Window();
	glutInit(&argc,argv);
	
	window->initWindow(1920,1080,"kinematics");
	glutMainLoop();

	  // Cleanup
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplGLUT_Shutdown();
	ImGui::DestroyContext();

	return 0;
}