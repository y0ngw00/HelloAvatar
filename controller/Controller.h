#ifndef __ENVIRONMENT_H__
#define __ENVIRONMENT_H__
#include <iostream>
#include <tuple>
#include <vector>
#include <Eigen/Core>

class BVH;
class Motion;
class Character;
class Controller
{
public:
	Controller();

	void reset();
	void step();
	void LoadBVH(const std::string& _path);
	void LoadFBX(const std::string& _path);
	void FollowBVH(int idx);

	int GetCurrentFrame(){return mFrame;}

	BVH* getBVH() {return m_bvh;}
	Character* getCharacter() {return m_Character;}
private:

	int mControlHz, mSimulationHz;
	int mElapsedFrame, mFrame;
	int mMaxFrame;

	BVH *m_bvh;
	Character* m_Character;
	std::vector<Motion*> m_Motions;

	double mFramesPerMotion;

	int mNumMotions, mNumFeatureAdds,mDimLabel;



};
#endif