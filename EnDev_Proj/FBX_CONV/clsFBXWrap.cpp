#include "stdafx.h"
#include "clsFBXWrap.h"


namespace bFBX_CONVERT
{
	clsFBXWrapper::clsFBXWrapper()
	{
	}


	clsFBXWrapper::~clsFBXWrapper()
	{
	}


	/**
	* Print the required number of tabs.
	*/
	int numTabs = 0;
	FbxScene* lScene;
	void PrintTabs()
	{
		for (int i = 0; i < numTabs; i++)
			printf("\t");
	}
	Mesh LoadMesh(FbxMesh* thisMesh) {
		Mesh m;


		//==This is the logic for verts===================================
		//how many verts?
		int vertCnt = thisMesh->GetControlPointsCount();
		std::vector<vertex> temporaryVerts;
		for (int v = 0; v < vertCnt; ++v)
		{

			FbxVector4 vert = thisMesh->GetControlPointAt(v);
			vertex myVert;
			for (int i = 0; i < 3; ++i)
				myVert.pos[i] = (float)vert.mData[i];
			temporaryVerts.push_back(myVert);

		}
		//================================================================

		//==For indices===================================================
		int indCnt = thisMesh->GetPolygonVertexCount();
		int polyCnt = thisMesh->GetPolygonCount();
		//m.indices = new int[indCnt];
		//printf("%d", sizeof(m.indices));
		//==============================================somany============

		//get vert uv data
		FbxStringList uvsetlist;
		thisMesh->GetUVSetNames(uvsetlist);
		for (int p = 0; p < polyCnt; ++p)
		{
			for (int i = 0; i < 3; i++)
			{
				FbxVector2 uv;
				bool unmapped;
				FbxVector4 normal;
				thisMesh->GetPolygonVertexUV(p, i, uvsetlist.GetStringAt(0), uv, unmapped);
				int indx = thisMesh->GetPolygonVertex(p, i);
				thisMesh->GetPolygonVertexNormal(p, i, normal);
				m.indices.push_back(m.verts.size());
				vertex vert;
				vert.x = temporaryVerts[indx].x;
				vert.y = temporaryVerts[indx].y;
				vert.z = temporaryVerts[indx].z;
				vert.w = 1;
				vert.r = uv[0];
				vert.g = uv[1];
				vert.b = 0;
				vert.a = 1;
				m.verts.push_back(vert);
			}
		}

		//number of bones
		unsigned int deformerCnt = thisMesh->GetDeformerCount();
		FbxSkin * skin = (FbxSkin*)thisMesh->GetDeformer(0, FbxDeformer::eSkin);
		//check to make sure we have skin
		if (skin) {
			//vertex groups
			int boneCnt = skin->GetClusterCount();
			//Loop vertex groups
			for (unsigned int bIndex = 0; bIndex < boneCnt; ++bIndex) {
				//individual bone group
				FbxCluster * cluster = skin->GetCluster(bIndex);
				//Bone this cluster is for.
				FbxNode* bone = cluster->GetLink();
				Bone thisB;
				//Get the bind pos mat for this bone cluster
				FbxAMatrix bindPos;

				cluster->GetTransformLinkMatrix(bindPos);
				for (USHORT i = 0; i < 4; ++i)
					for (USHORT j = 0; j < 4; ++j)
					{

						thisB.matrix[i + j * 4] = bindPos.Get(j, i);
					}

				//get collections of the stuff.
				int* boneVertIndx = cluster->GetControlPointIndices();
				double* boneVertWeight = cluster->GetControlPointWeights();

				//Let bones know who they affect
				unsigned int clusterVertCnt = cluster->GetControlPointIndicesCount();
				for (unsigned int vIndex = 0; vIndex < clusterVertCnt; ++vIndex) {
					thisB.vertIDs.push_back(boneVertIndx[vIndex]);
					thisB.boneWeight.push_back((float)boneVertWeight[vIndex]);
				}
				m.bones.push_back(thisB);
			}

		}

		//m.verts.push_back(myVert);
		return m;
	}
	/**
	* Return a string-based representation based on the attribute type.
	*/
	FbxString GetAttributeTypeName(FbxNodeAttribute::EType type)
	{
		switch (type)
		{
		case FbxNodeAttribute::eUnknown: return "unidentified";
		case FbxNodeAttribute::eNull: return "null";
		case FbxNodeAttribute::eMarker: return "marker";
		case FbxNodeAttribute::eSkeleton: return "skeleton";
		case FbxNodeAttribute::eMesh: return "mesh";
		case FbxNodeAttribute::eNurbs: return "nurbs";
		case FbxNodeAttribute::ePatch: return "patch";
		case FbxNodeAttribute::eCamera: return "camera";
		case FbxNodeAttribute::eCameraStereo: return "stereo";
		case FbxNodeAttribute::eCameraSwitcher: return "camera switcher";
		case FbxNodeAttribute::eLight: return "light";
		case FbxNodeAttribute::eOpticalReference: return "optical reference";
		case FbxNodeAttribute::eOpticalMarker: return "marker";
		case FbxNodeAttribute::eNurbsCurve: return "nurbs curve";
		case FbxNodeAttribute::eTrimNurbsSurface: return "trim nurbs surface";
		case FbxNodeAttribute::eBoundary: return "boundary";
		case FbxNodeAttribute::eNurbsSurface: return "nurbs surface";
		case FbxNodeAttribute::eShape: return "shape";
		case FbxNodeAttribute::eLODGroup: return "lodgroup";
		case FbxNodeAttribute::eSubDiv: return "subdiv";
		default: return "unknown";
		}
	}

	/**
	* Print an attribute.
	*/
	void PrintAttribute(FbxNodeAttribute* pAttribute)
	{
		if (!pAttribute) return;

		FbxString typeName = GetAttributeTypeName(pAttribute->GetAttributeType());
		FbxString attrName = pAttribute->GetName();
		PrintTabs();
		// Note: to retrieve the character array of a FbxString, use its Buffer() method.
		printf("<attribute type='%s' name='%s'/>\n", typeName.Buffer(), attrName.Buffer());
	}

	/**
	* Print a node, its attributes, and all its children recursively.
	*/
	void PrintNode(FbxNode* pNode)
	{
		PrintTabs();
		const char* nodeName = pNode->GetName();
		FbxDouble3 translation = pNode->LclTranslation.Get();
		FbxDouble3 rotation = pNode->LclRotation.Get();
		FbxDouble3 scaling = pNode->LclScaling.Get();

		// Print the contents of the node.
		printf("<node name='%s' translation='(%f, %f, %f)' rotation='(%f, %f, %f)' scaling='(%f, %f, %f)'>\n",
			nodeName,
			translation[0], translation[1], translation[2],
			rotation[0], rotation[1], rotation[2],
			scaling[0], scaling[1], scaling[2]
		);
		numTabs++;

		// Print the node's attributes.
		for (int i = 0; i < pNode->GetNodeAttributeCount(); i++)
			PrintAttribute(pNode->GetNodeAttributeByIndex(i));

		// Recursively print the children.
		for (int j = 0; j < pNode->GetChildCount(); j++)
			PrintNode(pNode->GetChild(j));

		numTabs--;
		PrintTabs();
		printf("</node>\n");
	}


	void GetNodeKeyFrameData(FbxNode* node, Bone& b) {
		// Iterate all animations (for example, walking, running, falling and etc.)
		FbxCriteria crit = FbxCriteria::ObjectTypeStrict(FbxAnimStack::ClassId);
		int numAnimations = lScene->GetSrcObjectCount(crit);
		for (int animationIndex = 0; animationIndex < numAnimations; animationIndex++)
		{
			Animation ani;

			FbxAnimStack *animStack = (FbxAnimStack*)lScene->GetSrcObject(crit, animationIndex);
			FbxAnimEvaluator *animEvaluator = lScene->GetAnimationEvaluator();
			animStack->GetName(); // Get the name of the animation if needed

								  // Iterate all the transformation layers of the animation. You can have several layers, for example one for translation, one for rotation, one for scaling and each can have keys at different frame numbers.
			int numLayers = animStack->GetMemberCount();
			for (int layerIndex = 0; layerIndex < numLayers; layerIndex++)
			{
				AnimKey key;
				FbxAnimLayer *animLayer = (FbxAnimLayer*)animStack->GetMember(layerIndex);
				//animLayer->GetName(); // Get the layer's name if needed
				//animStack->getm
				
				//only using this to get key timings, because idk wtf else to use
				FbxAnimCurve *translationCurve = node->LclTranslation.GetCurve(animLayer);
				//FbxAnimCurve *rotationCurve = node->LclRotation.GetCurve(animLayer);
				//FbxAnimCurve *scalingCurve = node->LclScaling.GetCurve(animLayer);
				FbxMatrix mat;
				if (translationCurve != 0)
				{
					//get key frame count
					int numKeys = translationCurve->KeyGetCount();
					//cycle key frames
					for (int keyIndex = 0; keyIndex < numKeys; keyIndex++)
					{
						//Doing this way is probably slow, I could probably optimize this.
						//Lets get it working first, finish milestone then speed up
						//HACK

						FbxTime frameTime = translationCurve->KeyGetTime(keyIndex);

						//float frameSeconds = (float)frameTime.GetSecondDouble(); // If needed, get the time of the scaling keyframe, in seconds

						//Keyframe matrix at this time.
						mat = node->EvaluateGlobalTransform(frameTime);
						key.KeyTime = frameTime.GetMilliSeconds()*1e-3;
						for (USHORT i = 0; i < 4; ++i)
							for (USHORT j = 0; j < 4; ++j)
							{

								key.data[i + j * 4] = mat.Get(j, i);

							}

						ani.keys.push_back(key);


					}
				}
				

					// Analogically, process rotationa and translation 

			}
			b.Anims.push_back(ani);
		}

	}

	void AddBonePost(Bone &b, FbxNode* node)
	{
		unsigned int childrenCnt = node->GetChildCount();
		for (unsigned int i = 0; i < childrenCnt; i++)
		{
			FbxNode *child = node->GetChild(i);
			Bone nb;
			/*FbxDouble3 trans = child->LclTranslation.Get();
			nb.v.x = trans[0];
			nb.v.y = trans[1];
			nb.v.z = trans[2];*/

			auto mat = child->EvaluateGlobalTransform(0);

			for (USHORT i = 0; i < 4; ++i)
				for (USHORT j = 0; j < 4; ++j)
				{

					nb.matrix[i + j * 4] = mat.Get(j, i);

				}

			nb.v.x = mat.GetT().mData[0];
			nb.v.y = mat.GetT().mData[1];
			nb.v.z = mat.GetT().mData[2];

			nb.v.w = 1;
			nb.v.r = 1;
			nb.v.g = 1;
			nb.v.b = 1;
			nb.v.a = 1;

			AddBonePost(nb, child);
			GetNodeKeyFrameData(child, nb);

			b.children.push_back(nb);
		}
	}

	DLLHANDLER std::vector<Mesh> LoadRoot(FbxNode * root)
	{
		std::vector<Mesh> res;
		const int numChilds = root->GetChildCount();
		FbxNode* child = nullptr;
		Mesh m;

		for (int i = 0; i < numChilds; ++i)
		{
			child = root->GetChild(i);

			//Get the fbxmesh of the node
			FbxMesh * thisMesh = child->GetMesh();

			//If it actually has a mesh, lets get its stuff
			if (thisMesh)
			{
				printf("FOUND A MESH\n");
				printf(child->GetName());
				printf("\n");
				//Make one of my mesh objects to store the data
				Mesh mt = LoadMesh(thisMesh);
				m.verts = mt.verts;
				m.indices = mt.indices;
				//res.push_back(m);
			}
			else //IF node isn't a mesh
			{
				int attribs = child->GetNodeAttributeCount();
				for (int attrI = 0; attrI < attribs; ++attrI)
				{
					FbxNodeAttribute * attrb = child->GetNodeAttributeByIndex(attrI);
					if (attrb->GetAttributeType() == FbxNodeAttribute::eSkeleton)
					{
						//I'm a bone.
						FbxDouble3 trans = child->LclTranslation.Get();

						m.root.v.w = 1;
						//m.root.v.color = 0xFFFFFFFF;
						m.root.v.r = 1;
						m.root.v.b = 1;
						m.root.v.g = 1;
						m.root.v.a = 1;

						auto mat = child->EvaluateGlobalTransform(0);
						for (USHORT i = 0; i < 4; ++i)
							for (USHORT j = 0; j < 4; ++j)
							{
								//m.root.matrix[j + i * 4] = mat.Get(j, i);

								m.root.matrix[i + j * 4] = mat.Get(j, i);

							}
						GetNodeKeyFrameData(child, m.root);
						m.root.v.x = mat.GetT().mData[0];
						m.root.v.y = mat.GetT().mData[1];
						m.root.v.z = mat.GetT().mData[2];
						//GetNodeKeyFrameData(child, m.root);
						AddBonePost(m.root, child);
						break;
					}
				}
			}
		}
		//HACK
		res.push_back(m);

		return res;
	}

	DLLHANDLER std::vector<Mesh> clsFBXWrapper::LoadFBXFile(const char * filename)
	{

		// Change the following filename to a suitable filename value.
		//const char* lFilename = "file.fbx";

		// Initialize the SDK manager. This object handles all our memory management.
		FbxManager* lSdkManager = FbxManager::Create();

		// Create the IO settings object.
		FbxIOSettings *ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
		lSdkManager->SetIOSettings(ios);

		// Create an importer using the SDK manager.
		FbxImporter* lImporter = FbxImporter::Create(lSdkManager, "");

		// Use the first argument as the filename for the importer.
		if (!lImporter->Initialize(filename, -1, lSdkManager->GetIOSettings()))
		{
			printf("Call to FbxImporter::Initialize() failed.\n");
			printf("Error returned: %s\n\n", lImporter->GetStatus().GetErrorString());
			exit(-1);
		}

		// Create a new scene so that it can be populated by the imported file.
		lScene = FbxScene::Create(lSdkManager, "myScene");

		// Import the contents of the file into the scene.
		lImporter->Import(lScene);

		// The file is imported; so get rid of the importer.
		lImporter->Destroy();

		//Used to triangulate scene
		//the scene to triangulate, and if to override the existing scene
		FbxGeometryConverter conv(lSdkManager);

		if (conv.Triangulate(lScene, true) == false)
		{
			//if failed, return;
			return std::vector<Mesh>();
		}
		// Print the nodes of the scene and their attributes recursively.
		// Note that we are not printing the root node because it should
		// not contain any attributes.

		//This node is how to tell where objects are.
		FbxNode* lRootNode = lScene->GetRootNode();

		std::vector<Mesh> msh = LoadRoot(lRootNode);
#ifdef _DEBUG
		if (lRootNode)
		{
			for (int i = 0; i < lRootNode->GetChildCount(); i++)
				PrintNode(lRootNode->GetChild(i));
		}
#endif
		// Destroy the SDK manager and all the other objects it was handling.
		lSdkManager->Destroy();
		return msh;
	}

}