#include <functional>
#include <vector>
#include <fstream>
#include <sstream>
#include "Controller.h"
#include "Character.h"
#include "MathUtils.h"
#include "BVHLoader.h"
// #include "FBXLoader.h"
#include "Motion.h"


Controller::
Controller()
	:mControlHz(30),
	mElapsedFrame(0)
{
	mElapsedFrame = 0;
	mFrame = 0;
	mMaxFrame = 0;
	m_Character = nullptr;

	this->reset();
}

void
Controller::
reset()
{
	mFrame = 0;
	mElapsedFrame = 0;
}

void
Controller::
step()
{
	mFrame++;
	if(mFrame >= mMaxFrame) reset();
}

void 
Controller::
LoadBVH(const std::string& _path)
{
	std::string path = std::string(ROOT_DIR)+"/data/bvh/" + _path;
	
	m_bvh = new BVH(path);
	m_Character = new Character(m_bvh);
	
	Motion* motion = new Motion(m_bvh);
	for(int j=0;j<m_bvh->getNumFrames();j++){
		motion->append(m_bvh->getPosition(j), m_bvh->getRotation(j),false);
	}

	motion->computeVelocity();
	
	
	mMaxFrame = m_bvh->getNumFrames();

	m_Motions.push_back(motion);


	// delete bvh;
	delete motion;
}

void 
Controller::
LoadFBX(const std::string& _path)
{
	std::string path = std::string(ROOT_DIR)+"/data/fbx/" + _path;
	FBXLoader* fbxLoader = new FBXLoader();
	fbxLoader->LoadFBX(path);

	delete fbxLoader;
}

void
Controller::
FollowBVH(int idx)
{

	// auto& motion = mMotions[idx];
	// if(mFrame > (motion->getNumFrames()-3))
	// 	mFrame = 0;
	
	// Eigen::Vector3d position = motion->getPosition(mFrame);
	// Eigen::MatrixXd rotation = motion->getRotation(mFrame);
	// Eigen::Vector3d linear_velocity = motion->getLinearVelocity(mFrame);
	// Eigen::MatrixXd angular_velocity = motion->getAngularVelocity(mFrame);

	return;
}
