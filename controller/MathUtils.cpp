#include "MathUtils.h"

#define EPSILON_EXPMAP_THETA 1.0e-3

Eigen::Vector3d
MathUtils::
projectOnVector(const Eigen::Vector3d& u, const Eigen::Vector3d& v)
{
	Eigen::Vector3d projection;
	projection = v.dot(u)/v.dot(v)*v;
	return projection;
}
Eigen::Isometry3d
MathUtils::
orthonormalize(const Eigen::Isometry3d& T_old)
{
	Eigen::Isometry3d T;
	T.translation() = T_old.translation();
	Eigen::Vector3d v0,v1,v2;
	Eigen::Vector3d u0,u1,u2;
	v0 = T_old.linear().col(0);
	v1 = T_old.linear().col(1);
	v2 = T_old.linear().col(2);

	u0 = v0;
	u1 = v1 - projectOnVector(u0,v1);
	u2 = v2 - projectOnVector(u0,v2) - projectOnVector(u1,v2);

	u0.normalize();
	u1.normalize();
	u2.normalize();

	T.linear().col(0) = u0;
	T.linear().col(1) = u1;
	T.linear().col(2) = u2;
	return T;
}

Eigen::VectorXd
MathUtils::
ravel(const std::vector<Eigen::Vector3d>& vv)
{
	Eigen::VectorXd ret(vv.size()*3);
	for(int i = 0;i<vv.size();i++)
		ret.segment<3>(i*3) = vv[i];
	return ret;
}
Eigen::VectorXd
MathUtils::
ravel(const std::vector<Eigen::VectorXd>& vv)
{
	int n=0, o=0;
	for(int i = 0;i<vv.size();i++)
		n += vv[i].rows();
	Eigen::VectorXd ret(n);
	for(int i = 0;i<vv.size();i++){
		int ni = vv[i].rows();
		ret.segment(o,ni) = vv[i];
		o += ni;
	}
	return ret;
}

Eigen::Matrix3d
MathUtils::
ExpMapRot(const Eigen::Vector3d& _q)
{
  double theta = _q.norm();

  Eigen::Matrix3d R = Eigen::Matrix3d::Zero();
  Eigen::Matrix3d qss = MakeSkewSymmetric(_q);
  Eigen::Matrix3d qss2 = qss * qss;

// Using Rodrigues' rotation formula,
  if (theta < EPSILON_EXPMAP_THETA)
    R = Eigen::Matrix3d::Identity() + qss + 0.5 * qss2;
  else
    R = Eigen::Matrix3d::Identity() + (sin(theta) / theta) * qss
        + ((1 - cos(theta)) / (theta * theta)) * qss2;

  return R;
}

Eigen::Matrix3d
MathUtils::
MakeSkewSymmetric(const Eigen::Vector3d& _v)
{
  Eigen::Matrix3d result = Eigen::Matrix3d::Zero();

  result(0, 1) = -_v(2);
  result(1, 0) = _v(2);
  result(0, 2) = _v(1);
  result(2, 0) = -_v(1);
  result(1, 2) = -_v(0);
  result(2, 1) = _v(0);

  return result;
}

Eigen::Matrix3d
MathUtils::
GetRotationMatrixBetweenAxes(const Eigen::Vector3d& _u,const Eigen::Vector3d& _v)
{
	// make sure that we actually have two unique vectors.
    // if(_u == _v) return Eigen::Vector3d::Identity();

	Eigen::Vector3d axis = _u.cross(_v);

	
    Eigen::Matrix3d M(Eigen::Matrix3d::Identity());
	M.row(0) = axis;
	M.row(1) = _v;
	M.row(2) = _u;
	return M;
}

