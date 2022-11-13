#include "FBXLoader.h"
#include <fstream>
#include <iostream>
#include <vector>

#include <Eigen/Core>
#include <Eigen/Geometry>

using namespace fbxsdk;

FBXLoader::
FBXLoader()
{
    initialize();
}

FBXLoader::
~FBXLoader()
{
    clearSDK();
}

void
FBXLoader::
initialize()
{
    m_fbxScene = NULL;
}

void
FBXLoader::
clearSDK()
{	
	if (m_fbxScene)
	{
		m_fbxScene->Destroy();
		m_fbxScene = NULL;
	}


}

bool
FBXLoader::
LoadFBX(const std::string& _filePath)
{

    FbxManager* sdkManager;

    std::cout<<"Load FBX file from : "<<_filePath<<"\n";

    bool bLoadResult = loadScene(sdkManager, _filePath);
    if(bLoadResult == false)
    {
        std::cout<<"Some errors occurred while loading the scene.."<<"\n";
        std::cout<<"Failed to load FBX file from : "<<_filePath<<"\n";
        return bLoadResult;
    }
    //Check Fbx Nodes normality

    //Scale

    //load mesh
    FbxNode* rootNode = m_fbxScene->GetRootNode();
    FbxAMatrix globalPosition = globalPosition = rootNode->EvaluateGlobalTransform();
    if(rootNode->GetNodeAttribute()) 
	{
		//geometry offset 값은 자식 노드에 상속되지 않는다.
		FbxAMatrix geometryOffset = GetGeometry(rootNode);
		FbxAMatrix globalOffPosition = globalPosition * geometryOffset;

	}
    //load texture 

    //load joint

    //load skinning


    // // Display the scene.
    // DisplayMetaData(m_fbxScene);

    // FBXSDK_printf("\n\n---------------------\nGlobal Light Settings\n---------------------\n\n");

    // if( gVerbose ) DisplayGlobalLightSettings(&m_fbxScene->GetGlobalSettings());

    // FBXSDK_printf("\n\n----------------------\nGlobal Camera Settings\n----------------------\n\n");

    // if( gVerbose ) DisplayGlobalCameraSettings(&m_fbxScene->GetGlobalSettings());

    // FBXSDK_printf("\n\n--------------------\nGlobal Time Settings\n--------------------\n\n");

    // if( gVerbose ) DisplayGlobalTimeSettings(&m_fbxScene->GetGlobalSettings());

    // FBXSDK_printf("\n\n---------\nHierarchy\n---------\n\n");

    // if( gVerbose ) DisplayHierarchy(&m_fbxScenene);

    // FBXSDK_printf("\n\n------------\nNode Content\n------------\n\n");

    // if( gVerbose ) DisplayContent(m_fbxScene);

    // FBXSDK_printf("\n\n----\nPose\n----\n\n");

    // if( gVerbose ) DisplayPose(m_fbxScene);

    // FBXSDK_printf("\n\n---------\nAnimation\n---------\n\n");

    // if( gVerbose ) DisplayAnimation(m_fbxScene);

    // //now display generic information

    // FBXSDK_printf("\n\n---------\nGeneric Information\n---------\n\n");
    // if( gVerbose ) DisplayGenericInfo(m_fbxScene);

    //Delete the FBX Manager. All the objects that have been allocated using the FBX Manager and that haven't been explicitly destroyed are also automatically destroyed.
    if( sdkManager ) sdkManager->Destroy();
	if( bLoadResult ) FBXSDK_printf("Program Success!\n");

    return true;

}

bool
FBXLoader::
loadMesh(FbxNode* _pNode)
{
    FbxMesh* lMesh = (FbxMesh*) _pNode->GetNodeAttribute ();

}

// Check the fbx sdk version and load fbx Scene.
bool
FBXLoader::
loadScene(FbxManager* _pManager, const std::string& _pFileName)
{
    int sdkMajor,  sdkMinor,  sdkRevision;
    int fileMajor, fileMinor, fileRevision;
    bool bResult = true;

    // create an importer.

    FbxImporter* fbxImporter = nullptr;
    fbxImporter = FbxImporter::Create( _pManager, "" );

    // Initialize the importer by providing a filename.
    const bool importStatus = fbxImporter->Initialize(_pFileName.c_str(), -1, _pManager->GetIOSettings());

	if(!importStatus)
	{
		std::cout<<"Failed to initialize FBX Importer" <<"\n";
		std::cout << fbxImporter->GetStatus().GetErrorString();
        return false;
	}
    // Get the file version number generate by the FBX SDK.
	FbxManager::GetFileFormatVersion(sdkMajor, sdkMinor, sdkRevision);

    fbxImporter->GetFileVersion(fileMajor, fileMinor, fileRevision);

    std::cout<<"FBX SDK version is "<< sdkMajor<<" "<<sdkMinor<<" "<< sdkRevision<<"\n";
    std::cout<<"FBX file format version SDK is "<< fileMajor<<" "<<fileMinor<<" "<< fileRevision<<"\n";

    ///major, minor, revision 까지 체크 
    unsigned int fileVersion = (fileMajor << 16 | fileMinor << 8 | fileRevision);
    unsigned int sdkVersion = (sdkMajor << 16 | sdkMinor << 8 | sdkRevision);

    if( fileVersion != sdkVersion )
    {			
        std::cout<<"The fbx file version is different from fbx sdk version."<<"\n";
        bResult = true; ///sdk version이 틀려도 loading이 가능할 수 있음.  단 경고창은 보여줘야함.
    }

    if (fbxImporter->IsFBX())
    {
        // Set the import states. By default, the import states are always set to 
        // true. The code below shows how to change these states.
        _pManager->GetIOSettings()->SetBoolProp(IMP_FBX_MATERIAL,        true);
        _pManager->GetIOSettings()->SetBoolProp(IMP_FBX_TEXTURE,         true);
        _pManager->GetIOSettings()->SetBoolProp(IMP_FBX_LINK,            true);
        _pManager->GetIOSettings()->SetBoolProp(IMP_FBX_SHAPE,           true);
        _pManager->GetIOSettings()->SetBoolProp(IMP_FBX_GOBO,            true);
        _pManager->GetIOSettings()->SetBoolProp(IMP_FBX_ANIMATION,       true);
        _pManager->GetIOSettings()->SetBoolProp(IMP_FBX_GLOBAL_SETTINGS, true);
    }
	
    //Import the scene.
    bResult = fbxImporter->Import(m_fbxScene);
    if(bResult==false && fbxImporter->GetStatus() == FbxStatus::ePasswordError)
    {
        std::cout<<"This file requires password."<<"\n";
        return false;
    }

    if(bResult==false || fbxImporter->GetStatus() != FbxStatus::eSuccess)
    {
        if(bResult)
        {
            std::cout<<"The importer was able to read the file but some error occurs."<<'\n';
            std::cout<<"Loaded scene may be incomplete."<<'\n';
        }
        else
        {
            std::cout<<"Importer failed to load the FBX file."<<'\n';
        }

        FbxArray<FbxString*> history;
		fbxImporter->GetStatus().GetErrorStringHistory(history);
		if (history.GetCount() > 1)
		{
			std::cout<<"   Error history stack: "<<"\n";
			for (int i = 0; i < history.GetCount(); i++)
			{
				std::cout<<history[i]->Buffer()<<"\n";
			}
		}
		FbxArrayDelete<FbxString*>(history);
    }

    fbxImporter->Destroy();

	return bResult;
}

void
FBXLoader::
GetPolygons(FbxMesh* _pMesh)
{
    int lPolygonCount = _pMesh->GetPolygonCount();
    FbxVector4* lControlPoints = _pMesh->GetControlPoints(); 
    char header[100];

    int vertexId = 0;
    for (int i = 0; i < lPolygonCount; i++)
    {
        for (int l = 0; l < _pMesh->GetElementPolygonGroupCount(); l++)
        {
            FbxGeometryElementPolygonGroup* lePolgrp = _pMesh->GetElementPolygonGroup(l);
			switch (lePolgrp->GetMappingMode())
			{
			case FbxGeometryElement::eByPolygon:
				if (lePolgrp->GetReferenceMode() == FbxGeometryElement::eIndex)
				{
					int polyGroupId = lePolgrp->GetIndexArray().GetAt(i);
					break;
				}
			default:
				// any other mapping modes don't make sense
				std::cout<<("        \"unsupported group assignment\"")<<"\n";
				break;
			}
        }

        int lPolygonSize = _pMesh->GetPolygonSize(i);

	// 	for (int j = 0; j < lPolygonSize; j++)
	// 	{
	// 		int lControlPointIndex = _pMesh->GetPolygonVertex(i, j);
	// 		if (lControlPointIndex < 0)
	// 		{
	// 			std::cout<<"         Coordinates: Invalid index found!"<<"\n";
	// 			continue;
	// 		}

	// 		for (int l = 0; l < _pMesh->GetElementVertexColorCount(); l++)
	// 		{
	// 			FbxGeometryElementVertexColor* leVtxc = _pMesh->GetElementVertexColor( l);
	// 			FBXSDK_sprintf(header, 100, "            Color vertex: "); 

	// 			switch (leVtxc->GetMappingMode())
	// 			{
	// 			default:
	// 			    break;
	// 			case FbxGeometryElement::eByControlPoint:
	// 				switch (leVtxc->GetReferenceMode())
	// 				{
	// 				case FbxGeometryElement::eDirect:
	// 					DisplayColor(header, leVtxc->GetDirectArray().GetAt(lControlPointIndex));
	// 					break;
	// 				case FbxGeometryElement::eIndexToDirect:
	// 					{
	// 						int id = leVtxc->GetIndexArray().GetAt(lControlPointIndex);
	// 						DisplayColor(header, leVtxc->GetDirectArray().GetAt(id));
	// 					}
	// 					break;
	// 				default:
	// 					break; // other reference modes not shown here!
	// 				}
	// 				break;

	// 			case FbxGeometryElement::eByPolygonVertex:
	// 				{
	// 					switch (leVtxc->GetReferenceMode())
	// 					{
	// 					case FbxGeometryElement::eDirect:
	// 						DisplayColor(header, leVtxc->GetDirectArray().GetAt(vertexId));
	// 						break;
	// 					case FbxGeometryElement::eIndexToDirect:
	// 						{
	// 							int id = leVtxc->GetIndexArray().GetAt(vertexId);
	// 							DisplayColor(header, leVtxc->GetDirectArray().GetAt(id));
	// 						}
	// 						break;
	// 					default:
	// 						break; // other reference modes not shown here!
	// 					}
	// 				}
	// 				break;

	// 			case FbxGeometryElement::eByPolygon: // doesn't make much sense for UVs
	// 			case FbxGeometryElement::eAllSame:   // doesn't make much sense for UVs
	// 			case FbxGeometryElement::eNone:       // doesn't make much sense for UVs
	// 				break;
	// 			}
	// 		}
	// 		for (int l = 0; l < _pMesh->GetElementUVCount(); ++l)
	// 		{
	// 			FbxGeometryElementUV* leUV = _pMesh->GetElementUV( l);
	// 			FBXSDK_sprintf(header, 100, "            Texture UV: "); 

	// 			switch (leUV->GetMappingMode())
	// 			{
	// 			default:
	// 			    break;
	// 			case FbxGeometryElement::eByControlPoint:
	// 				switch (leUV->GetReferenceMode())
	// 				{
	// 				case FbxGeometryElement::eDirect:
	// 					Display2DVector(header, leUV->GetDirectArray().GetAt(lControlPointIndex));
	// 					break;
	// 				case FbxGeometryElement::eIndexToDirect:
	// 					{
	// 						int id = leUV->GetIndexArray().GetAt(lControlPointIndex);
	// 						Display2DVector(header, leUV->GetDirectArray().GetAt(id));
	// 					}
	// 					break;
	// 				default:
	// 					break; // other reference modes not shown here!
	// 				}
	// 				break;

	// 			case FbxGeometryElement::eByPolygonVertex:
	// 				{
	// 					int lTextureUVIndex = _pMesh->GetTextureUVIndex(i, j);
	// 					switch (leUV->GetReferenceMode())
	// 					{
	// 					case FbxGeometryElement::eDirect:
	// 					case FbxGeometryElement::eIndexToDirect:
	// 						{
	// 							Display2DVector(header, leUV->GetDirectArray().GetAt(lTextureUVIndex));
	// 						}
	// 						break;
	// 					default:
	// 						break; // other reference modes not shown here!
	// 					}
	// 				}
	// 				break;

	// 			case FbxGeometryElement::eByPolygon: // doesn't make much sense for UVs
	// 			case FbxGeometryElement::eAllSame:   // doesn't make much sense for UVs
	// 			case FbxGeometryElement::eNone:       // doesn't make much sense for UVs
	// 				break;
	// 			}
	// 		}
	// 		for(int l = 0; l < _pMesh->GetElementNormalCount(); ++l)
	// 		{
	// 			FbxGeometryElementNormal* leNormal = _pMesh->GetElementNormal( l);
	// 			FBXSDK_sprintf(header, 100, "            Normal: "); 

	// 			if(leNormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	// 			{
	// 				switch (leNormal->GetReferenceMode())
	// 				{
	// 				case FbxGeometryElement::eDirect:
	// 					Display3DVector(header, leNormal->GetDirectArray().GetAt(vertexId));
	// 					break;
	// 				case FbxGeometryElement::eIndexToDirect:
	// 					{
	// 						int id = leNormal->GetIndexArray().GetAt(vertexId);
	// 						Display3DVector(header, leNormal->GetDirectArray().GetAt(id));
	// 					}
	// 					break;
	// 				default:
	// 					break; // other reference modes not shown here!
	// 				}
	// 			}

	// 		}
	// 		for(int l  = 0; l < _pMesh->GetElementTangentCount(); ++l)
	// 		{
	// 			FbxGeometryElementTangent* leTangent = _pMesh->GetElementTangent( l);
	// 			FBXSDK_sprintf(header, 100, "            Tangent: "); 

	// 			if(leTangent->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	// 			{
	// 				switch (leTangent->GetReferenceMode())
	// 				{
	// 				case FbxGeometryElement::eDirect:
	// 					Display3DVector(header, leTangent->GetDirectArray().GetAt(vertexId));
	// 					break;
	// 				case FbxGeometryElement::eIndexToDirect:
	// 					{
	// 						int id = leTangent->GetIndexArray().GetAt(vertexId);
	// 						Display3DVector(header, leTangent->GetDirectArray().GetAt(id));
	// 					}
	// 					break;
	// 				default:
	// 					break; // other reference modes not shown here!
	// 				}
	// 			}

	// 		}
	// 		for(int l = 0; l < _pMesh->GetElementBinormalCount(); ++l)
	// 		{

	// 			FbxGeometryElementBinormal* leBinormal = _pMesh->GetElementBinormal( l);

	// 			FBXSDK_sprintf(header, 100, "            Binormal: "); 
	// 			if(leBinormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	// 			{
	// 				switch (leBinormal->GetReferenceMode())
	// 				{
	// 				case FbxGeometryElement::eDirect:
	// 					Display3DVector(header, leBinormal->GetDirectArray().GetAt(vertexId));
	// 					break;
	// 				case FbxGeometryElement::eIndexToDirect:
	// 					{
	// 						int id = leBinormal->GetIndexArray().GetAt(vertexId);
	// 						Display3DVector(header, leBinormal->GetDirectArray().GetAt(id));
	// 					}
	// 					break;
	// 				default:
	// 					break; // other reference modes not shown here!
	// 				}
	// 			}
	// 		}
			vertexId++;
		// } // for polygonSize
    } // for polygonCount
}


FbxAMatrix
FBXLoader::
GetGeometry(FbxNode* _pNode)
{
    const FbxVector4 translation = _pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
    const FbxVector4 rotation = _pNode->GetGeometricRotation(FbxNode::eSourcePivot);
    const FbxVector4 scaling = _pNode->GetGeometricScaling(FbxNode::eSourcePivot);

    return FbxAMatrix(translation, rotation, scaling);
}

FbxAMatrix
FBXLoader::
GetNodeTransform(FbxNode* _pNode)
{
	FbxAMatrix translationM, scalingM, scalingPivotM, scalingOffsetM, rotationOffsetM, rotationPivotM,
                preRotationM, rotationM, postRotationM, transformM;

	FbxVector4 translation = _pNode->LclTranslation.Get();
    FbxVector4 preRotation = _pNode->PreRotation.Get();
	FbxVector4 postRotation = _pNode->PostRotation.Get();
	FbxVector4 rotation = _pNode->LclRotation.Get();
	FbxVector4 scaling = _pNode->LclScaling.Get();
	FbxVector4 scalingOffset = _pNode->ScalingOffset.Get();
    FbxVector4 scalingPivot = _pNode->ScalingPivot.Get();
    FbxVector4 rotationOffset = _pNode->RotationOffset.Get();
    FbxVector4 rotationPivot = _pNode->RotationPivot.Get();

	translationM.SetIdentity();
	translationM.SetT(translation);

	scalingM.SetIdentity();
	scalingM.SetS(scaling);

	scalingPivotM.SetIdentity();
	scalingPivotM.SetT(scalingPivot);

	scalingOffsetM.SetIdentity();
	scalingOffsetM.SetT(scalingOffset);

	rotationOffsetM.SetIdentity();
	rotationOffsetM.SetT(rotationOffset);

	rotationPivotM.SetIdentity();
	rotationPivotM.SetT(rotationPivot);

	preRotationM.SetIdentity();
	preRotationM.SetR(preRotation);

	rotationM.SetIdentity();
	rotationM.SetR(rotation);

	postRotationM.SetIdentity();
	postRotationM.SetR(postRotation);

	transformM = translationM * rotationOffsetM * rotationPivotM * preRotationM * rotationM * postRotationM * rotationPivotM.Inverse() *
		scalingOffsetM * scalingPivotM * scalingM * scalingPivotM.Inverse();

	return transformM;
}

FbxAMatrix 
FBXLoader::
EigenToFbxMatrix(const Eigen::MatrixXd& _mat, float _scale)
{
    FbxAMatrix fbxMatrix;
	fbxMatrix.mData[0][0] = _mat(0,0);			fbxMatrix.mData[1][0] = _mat(1,0);			fbxMatrix.mData[2][0] = _mat(2,0);			fbxMatrix.mData[3][0] = _mat(3,0) * _scale;
	fbxMatrix.mData[0][1] = _mat(0,1);			fbxMatrix.mData[1][1] = _mat(1,1);			fbxMatrix.mData[2][1] = _mat(2,1);			fbxMatrix.mData[3][1] = _mat(3,1) * _scale;
	fbxMatrix.mData[0][2] = _mat(0,2);			fbxMatrix.mData[1][2] = _mat(1,2);			fbxMatrix.mData[2][2] = _mat(2,2);			fbxMatrix.mData[3][2] = _mat(3,2) * _scale;
	fbxMatrix.mData[0][3] = _mat(0,3);			fbxMatrix.mData[1][3] = _mat(1,3);			fbxMatrix.mData[2][3] = _mat(2,3);			fbxMatrix.mData[3][3] = _mat(3,3);
	return fbxMatrix;
}

Eigen::MatrixXd
FBXLoader::
FbxToEigenMatrix(const FbxAMatrix& _fbxMatrix, float _scale)
{
    Eigen::MatrixXd mat;
	mat(0,0) = _fbxMatrix.Get(0, 0); mat(1,0) = _fbxMatrix.Get(1, 0); mat(2,0) = _fbxMatrix.Get(2, 0); mat(3,0) = _fbxMatrix.Get(3, 0) * _scale;
	mat(0,1) = _fbxMatrix.Get(0, 1); mat(1,1) = _fbxMatrix.Get(1, 1); mat(2,1) = _fbxMatrix.Get(2, 1); mat(3,1) = _fbxMatrix.Get(3, 1) * _scale;
	mat(0,2) = _fbxMatrix.Get(0, 2); mat(1,2) = _fbxMatrix.Get(1, 2); mat(2,2) = _fbxMatrix.Get(2, 2); mat(3,2) = _fbxMatrix.Get(3, 2) * _scale;
	mat(0,3) = _fbxMatrix.Get(0, 3); mat(1,3) = _fbxMatrix.Get(1, 3); mat(2,3) = _fbxMatrix.Get(2, 3); mat(3,3) = _fbxMatrix.Get(3, 3);
	return mat;
}

Eigen::Quaterniond
FBXLoader::
FbxToEigenQuaternion(const FbxAMatrix& _fbxMatrix)
{
    FbxQuaternion fbxQuat = _fbxMatrix.GetQ();
	return Eigen::Quaterniond(fbxQuat.GetAt(0),fbxQuat.GetAt(1),fbxQuat.GetAt(2),fbxQuat.GetAt(3));
}