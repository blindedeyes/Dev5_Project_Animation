// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>



// C RunTime Header Files
#include <ctime>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <iostream>
#include <d3d11.h>
#include <DirectXMath.h>
#include <atlcomcli.h>
#include <DirectXPackedVector.h>
#include <DXGIDebug.h>
#include <vector>

// TODO: reference additional headers your program requires here
#include "Vertex.h"
#include "Resource.h"
#include "pipelineState.h"
#include "pipelineManager.h"

//DLL INCLUDE
#include "..\FBX_CONV\clsFBXWrap.h"
