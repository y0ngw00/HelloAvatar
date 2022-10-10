#ifndef __CHARACTER_H__
#define __CHARACTER_H__
#include "BVHLoader.h"

class Motion;
class Character
{
public:
	Character(BVH* _bvh);

	Eigen::Isometry3d getRootTransform();
	void setRootTransform(const Eigen::Isometry3d& T);

	Eigen::Isometry3d getReferenceTransform();
	void setReferenceTransform(const Eigen::Isometry3d& T_ref);

	void recordHierarchy(BVH* bvh);

	void SetOffset(const std::vector<Eigen::Vector3d>& _offset);
	const std::vector<Eigen::Vector3d>& GetOffset(){return m_offset;}

	std::vector<std::string> getHierarchy() {return hierarchy_format;}
	std::vector<std::string> getBVHMap() {return mBVHMap;}

	void buildBVHIndices(const std::vector<std::string>& bvh_names);

	Motion* getMotion(){return mMotion;}

	int getBVHIndex(int idx){return mBVHIndices[idx];}
	const std::vector<int>& getBVHIndices(){return mBVHIndices;}
	const std::vector<int>& getParentsIndices(){return m_ParentsIndices;}

private:

	Motion* mMotion;
	std::vector<std::string> mBVHMap;
	std::vector<std::string> hierarchy_format;
	std::vector<int> mBVHIndices;
	std::vector<int> m_ParentsIndices;
	std::vector<Eigen::Vector3d> m_offset;

};

#endif