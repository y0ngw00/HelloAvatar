#ifndef __ENVIRONMENT_H__
#define __ENVIRONMENT_H__
#include <iostream>
#include <tuple>
#include <Eigen/Core>

class BVH;
class Motion;
class Character;
class Environment
{
public:
	Environment();

	void reset(int motion_idx = -1, bool RSI = true);
	void FollowBVH(int idx);

	const Eigen::VectorXd& getNumFrames();
private:

	int mControlHz, mSimulationHz;
	int mElapsedFrame, mFrame;
	int mMaxElapsedFrame;

	// Character *mKinCharacter;
	std::vector<Motion*> mMotions;

	double mFramesPerMotion;

	int mNumMotions, mNumFeatureAdds,mDimLabel;



};
#endif