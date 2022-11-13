#ifndef __FBXLOADER_H__
#define __FBXLOADER_H__

#include "fbxsdk.h"
#include <string>

#include <Eigen/Core>
#include <Eigen/Geometry>

class FBXLoader
{
public:
    FBXLoader();
    bool LoadFBX(const std::string& _filePath);
    void CreateScene();
    
    virtual ~FBXLoader();

// Load functions
private:

    void initialize();
    void clearSDK();
    bool loadScene(FbxManager* _pManager, const std::string& _pFileName);
    bool loadMesh(FbxNode* _pNode);

// Utility functions
private:
    void GetPolygons(FbxMesh* _pMesh);
    FbxAMatrix GetGeometry(FbxNode* _pNode);
    FbxAMatrix GetNodeTransform(FbxNode* _pNode);
    FbxAMatrix EigenToFbxMatrix(const Eigen::MatrixXd& _mat, float _scale=1.0f);
    Eigen::MatrixXd FbxToEigenMatrix(const FbxAMatrix& _mat, float _scale=1.0f);
    Eigen::Quaterniond FbxToEigenQuaternion(const FbxAMatrix& _fbxMatrix);

private:
    FbxScene* m_fbxScene;
};

#endif

