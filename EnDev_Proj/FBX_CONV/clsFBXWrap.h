#pragma once
#include <vector>
#ifdef FBX_CONV_EXPORTS  
#define DLLHANDLER __declspec(dllexport)   
#else  
#define DLLHANDLER __declspec(dllimport)   
#endif  



namespace bFBX_CONVERT
{
	// This class is exported from the MathLibrary.dll  
	class clsFBXWrapper
	{
		//int numTabs = 0;
	public:
		DLLHANDLER clsFBXWrapper();
		DLLHANDLER ~clsFBXWrapper();

		// Returns a + b  
		DLLHANDLER char test() { return 'a'; };
		DLLHANDLER std::vector<Mesh> LoadFBXFile(const char * filename);

		/*DLLHANDLER void PrintTabs();
		DLLHANDLER void PrintNode(FbxNode* pNode);
		DLLHANDLER void PrintAttribute(FbxNodeAttribute* pAttribute);

		DLLHANDLER FbxString GetAttributeTypeName(FbxNodeAttribute::EType type);*/
	};
}