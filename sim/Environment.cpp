#include <functional>
#include <vector>
#include <fstream>
#include <sstream>
#include "Environment.h"
#include "MathUtils.h"
#include "BVHLoader.h"
#include "Motion.h"


Environment::
Environment()
	:mControlHz(30),
	mElapsedFrame(0),
	mMaxElapsedFrame(300)
{
	char buffer[100];
	std::ifstream txtread;
	std::vector<std::string> motion_lists;
	std::string txt_path = "/data/bvh/motionlist.txt";
	txtread.open(std::string(ROOT_DIR)+txt_path);
	if(!txtread.is_open())
	{
		std::cout<<"Text file does not exist from : "<< txt_path << std::endl;
		return;
	}
	while(txtread>>buffer) motion_lists.push_back(std::string(ROOT_DIR)+"/data/bvh/"+ std::string(buffer));
	txtread.close();


	mNumMotions = motion_lists.size();

	bool load_tree =false;
	for(auto bvh_path : motion_lists)
	{
		BVH* bvh = new BVH(bvh_path);
		Motion* motion = new Motion(bvh);
		for(int j=0;j<bvh->getNumFrames();j++)
		{
			motion->append(bvh->getPosition(j), bvh->getRotation(j),false);
		}
		motion->computeVelocity();
		mMotions.push_back(motion);

		if(!load_tree)
		{
			// mKinCharacter->buildBVHIndices(motion->getBVH()->getNodeNames());
			load_tree = true;			
		}
	}

	mElapsedFrame = 0;
	mFrame = 0;

	this->reset();
}

void
Environment::
reset(int motion_idx, bool RSI)
{
	mFrame = 0;
	mElapsedFrame = 0;
}

void
Environment::
FollowBVH(int idx){

	// auto& motion = mMotions[idx];
	// if(mFrame > (motion->getNumFrames()-3))
	// 	mFrame = 0;
	
	// Eigen::Vector3d position = motion->getPosition(mFrame);
	// Eigen::MatrixXd rotation = motion->getRotation(mFrame);
	// Eigen::Vector3d linear_velocity = motion->getLinearVelocity(mFrame);
	// Eigen::MatrixXd angular_velocity = motion->getAngularVelocity(mFrame);

	return;
}
