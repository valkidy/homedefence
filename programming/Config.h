#ifndef __CONFIG_H__
#define __CONFIG_H__

/****************************************************************
  lib
 ****************************************************************/
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "libcocos2d.lib")
#pragma comment(lib, "libCocosDenshion.lib")


/****************************************************************
  header file
 ****************************************************************/
// std 
#include "CCStdC.h"
#include <assert.h>
#include <windows.h>
#include <tchar.h>

#pragma warning (push)
// disable warning for cocos2d-x
#pragma warning(disable:4251)
#pragma warning(disable:4996)
#pragma warning(disable:4819)

// cocos2d-x header
#include "cocos2d.h"
#include "CCEGLView.h"
//#include "Box2D/Box2D.h"
#include "SimpleAudioEngine.h"
#pragma warning (pop)

/****************************************************************
  define
 ****************************************************************/
// app window size
#define WINDOW_WIDTH 960
#define WINDOW_HEIGHT 640
// exclude rarely-used stuff from Windows headers
#define WIN32_LEAN_AND_MEAN


// stats ,fps
#ifdef _DEBUG
	#define CC_DISPLAY_STATS 1
	#define CC_CHECK_MEMORYLEAK 1
#endif
// leak
#ifdef CC_CHECK_MEMORYLEAK 
	#define _CRTDBG_MAP_ALLOC
	#include <crtdbg.h>
#endif

#endif    // __CONFIG_H__
