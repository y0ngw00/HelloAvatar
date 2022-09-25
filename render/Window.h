#ifndef __WINDOW_H__
#define __WINDOW_H__
#include "GLUTWindow3D.h"
#include <utility>
#include <chrono>
#include <iomanip>
#include <Eigen/Core>

class Window : public GLUTWindow3D
{
public:
	Window();

	void render() override;
	void ImGuiDisplay();

	void step();
	void reset(int frame=-1);

protected:
	void keyboard(unsigned char key, int x, int y) override;
	void special(int key, int x, int y) override;
	void mouse(int button, int state, int x, int y) override;
	void motion(int x, int y) override;
	void reshape(int w, int h) override;
	void timer(int tic) override;

	std::vector<unsigned char> mScreenshotTemp;
	std::vector<unsigned char> mScreenshotTemp2;
	bool mCapture;
	void capture_screen();

	bool mPlay;
	bool mDrawKinPose;

	std::vector<int> mMotionTypes;

	std::chrono::system_clock::time_point mTimePoint;
	double mComputedTime;

    int mMotionType, mPrevMotionType;
    std::vector<std::string> motion_lists;
};

#endif