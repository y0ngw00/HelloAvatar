#include "Window.h"
#include "Camera.h"
#include "UIManager.h"

// #include "sim/BVH.h"
#include <fstream>
#include <time.h>
#include <iostream>
#include "DrawUtils.h"
#include "Controller.h"

#include "Character.h"
#include "BVHLoader.h"

static bool show_demo_window = true;
static bool show_another_window = true;

Window::
Window()
	:GLUTWindow3D(),
	mPlay(false),
	mDrawKinPose(true),
	mCapture(false)
{
	mCamera->setLookAt(Eigen::Vector3d(0.0,0.8,-3.0));
	mCamera->setEye( Eigen::Vector3d(0.0,1.0,2.0));

	char buffer[100];
	std::ifstream txtread;
	std::string txt_path = "/data/bvh/motionlist.txt";
	txtread.open(std::string(ROOT_DIR)+ txt_path);
	if(!txtread.is_open()){
		std::cout<<"Text file does not exist from : "<< txt_path << std::endl;
		return;
	}
	while(txtread>>buffer){
        motion_lists.push_back(std::string(buffer));	
	}
	txtread.close();

	m_UIManager = nullptr;
	m_UIManager->initialize();

	m_Controller = new Controller();
	m_Controller->loadBVH(motion_lists[0]);

	this->reset();
}

Window::
~Window()
{
	m_UIManager = nullptr;
}

void
Window::
render()
{
	if(DrawUtils::initialized == false){
		DrawUtils::buildMeshes();
	}
	initLights();
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_MULTISAMPLE);
	
	DrawUtils::drawGround(0,100.0);
	DrawUtils::drawSkeleton(m_Controller->getCharacter(),m_Controller->getBVH(), m_Controller->GetCurrentFrame());
	m_UIManager->render();

	if(m_UIManager->GetFilePath() != "")
	{
		std::cout<<m_UIManager->GetFilePath()<<"\n";
	}
}

void
Window::
reset(int frame)
{
	m_Controller->reset();
}

void
Window::
step()
{
	m_Controller->step();
}

void
Window::
keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
		case '1':mDrawKinPose = !mDrawKinPose;break;
		case 'r':this->reset(mMotionType);break;
		case 'R':this->reset(0);break;
		case 'C':mCapture=true;break;
		case ' ':mPlay = !mPlay; break;

		default:
		{
			GLUTWindow3D::keyboard(key,x,y);
			m_UIManager->keyboardEvent(key,x,y);
			break;
		}
	}
}
void
Window::
special(int key, int x, int y)
{
	switch(key)
	{
		case 100: break;//Left
		case 101: break;//Up
		case 102: break;//Right
		case 103: break;//bottom
		default:
		{
			GLUTWindow3D::special(key,x,y);
			m_UIManager->specialEvent(key,x,y);
			break;
		}
	}

}
void
Window::
mouse(int button, int state, int x, int y)
{
	GLUTWindow3D::mouse(button,state,x,y);
	m_UIManager->mouseEvent(button,state,x,y);

	if(mMouse == 2) // Right
	{
		if(state==0) // Down
		{
			auto ray = mCamera->getRay(x,y);
		}
		else{
		}
	}
}
void
Window::
motion(int x, int y)
{
	GLUTWindow3D::motion(x,y);
	m_UIManager->motionEvent(x,y);

	if(mMouse == 2 && mDrag)
	{
		auto ray = mCamera->getRay(x,y);
	}
	else
	{

	}
	
}
void
Window::
reshape(int w, int h)
{
	mScreenshotTemp.resize(4*w*h);
	mScreenshotTemp2.resize(4*w*h);
	GLUTWindow3D::reshape(w,h);
	m_UIManager->reshapeEvent(w,h);
}
void
Window::
timer(int tic)
{
	auto next_time_point = std::chrono::system_clock::now();
	std::chrono::microseconds micro = std::chrono::duration_cast<std::chrono::microseconds>(next_time_point - mTimePoint);
	mComputedTime = micro.count();

	mTimePoint = next_time_point;

	if(mPlay)
		this->step();
	GLUTWindow3D::timer(tic);
}
#include <chrono>
#include <ctime>
#include "lodepng.h"

std::string timepoint_to_string(const std::chrono::system_clock::time_point& p_tpTime,
                                           const std::string& p_sFormat)
{
    auto converted_timep = std::chrono::system_clock::to_time_t(p_tpTime);
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&converted_timep), p_sFormat.c_str());
 
    return oss.str();
}

void
Window::
capture_screen()
{
	static int count = 0;
	static std::string path = timepoint_to_string(std::chrono::system_clock::now(), "../data/png/%Y_%m_%d:%H_%M_%S");
	if(count == 0){
		std::string command = "mkdir " + path;
		system(command.c_str());	
	}
	
	
	char file_name[256];
	std::string file_base = "Capture";

	std::snprintf(file_name, sizeof(file_name), "%s%s%s%.4d.png",
				path.c_str(), "/", file_base.c_str(), count++);

	int tw = glutGet(GLUT_WINDOW_WIDTH);
	int th = glutGet(GLUT_WINDOW_HEIGHT);

	glReadPixels(0, 0,  tw, th, GL_RGBA, GL_UNSIGNED_BYTE, &mScreenshotTemp[0]);

	// reverse temp2 temp1
	for (int row = 0; row < th; row++) {
	memcpy(&mScreenshotTemp2[row * tw * 4],
		   &mScreenshotTemp[(th - row - 1) * tw * 4], tw * 4);
	}
	
	unsigned result = lodepng::encode(file_name, mScreenshotTemp2, tw, th);

	// if there's an error, display it
	if (result) {
	std::cout << "lodepng error " << result << ": "
			<< lodepng_error_text(result) << std::endl;
	} else {
		std::cout << "wrote screenshot " << file_name << "\n";
	}
}
