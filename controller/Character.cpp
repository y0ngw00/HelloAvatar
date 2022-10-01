#include "Character.h"
#include "MathUtils.h"
#include "Motion.h"
#include <algorithm>
#include <iostream>
#include <sstream>

Character::
Character(const std::vector<std::string>& bvh_map,
			const Eigen::VectorXd& w_joint)
	:mBVHMap(bvh_map),
	mJointWeights(w_joint)
{

}

Eigen::Isometry3d
Character::
getRootTransform()
{
}
	
void
Character::
setRootTransform(const Eigen::Isometry3d& T)
{
	
}

Eigen::Isometry3d
Character::
getReferenceTransform()
{
	Eigen::Isometry3d T = this->getRootTransform();
	Eigen::Matrix3d R = T.linear();
	Eigen::Vector3d p = T.translation();
	Eigen::Vector3d z = R.col(2);
	Eigen::Vector3d y = Eigen::Vector3d::UnitY();
	z -= MathUtils::projectOnVector(z, y);
	p -= MathUtils::projectOnVector(p, y);

	z.normalize();
	Eigen::Vector3d x = y.cross(z);

	R.col(0) = x;
	R.col(1) = y;
	R.col(2) = z;

	T.linear() = R;
	T.translation() = p;

	return T;
}

void
Character::
setReferenceTransform(const Eigen::Isometry3d& T_ref)
{
	Eigen::Isometry3d T_ref_cur = this->getReferenceTransform();
	Eigen::Isometry3d T_cur = this->getRootTransform();
	Eigen::Isometry3d T_diff = T_ref_cur.inverse()*T_cur;
	Eigen::Isometry3d T_next = T_ref*T_diff;

	this->setRootTransform(T_next);
}

void
Character::
buildBVHIndices(const std::vector<std::string>& bvh_names)
{
	mBVHIndices.reserve(mBVHMap.size());
	mBVHNames = bvh_names;

	for(int i=0;i<mBVHMap.size();i++)
	{
		int index = std::distance(bvh_names.begin(),std::find(bvh_names.begin(),bvh_names.end(), mBVHMap[i]));
		if(index>=bvh_names.size())
			mBVHIndices.emplace_back(-1);
		else
			mBVHIndices.emplace_back(index);
	}

}

void 
Character::
recordHierarchy(BVH* bvh){
	hierarchy_format = bvh->getHierarchy();
	return;
}