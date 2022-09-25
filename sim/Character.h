#ifndef __CHARACTER_H__
#define __CHARACTER_H__
#include "BVH.h"

class Motion;
class Character
{
public:
	Character(
			const std::vector<std::string>& bvh_map,
			const Eigen::VectorXd& w_joint);

	Eigen::Isometry3d getRootTransform();
	void setRootTransform(const Eigen::Isometry3d& T);

	Eigen::Isometry3d getReferenceTransform();
	void setReferenceTransform(const Eigen::Isometry3d& T_ref);

	void recordHierarchy(BVH* bvh);
	std::vector<std::string> getHierarchy() {return hierarchy_format;}
	std::vector<std::string> getBVHNames() {return mBVHNames;}

	void buildBVHIndices(const std::vector<std::string>& bvh_names);

	Motion* getMotion(){return mMotion;}

	int getBVHIndex(int idx){return mBVHIndices[idx];}
	const std::vector<int>& getBVHIndices(){return mBVHIndices;}
	const Eigen::VectorXd& getJointWeights(){return mJointWeights;}

private:

	Motion* mMotion;
	std::vector<std::string> mBVHNames;
	std::vector<std::string> mBVHMap;
	std::vector<std::string> hierarchy_format;
	std::vector<int> mBVHIndices;

	Eigen::VectorXd mJointWeights;

};

#endif