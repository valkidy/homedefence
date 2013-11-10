#ifndef __MAIN_H__
#define __MAIN_H__

// Exclude rarely-used stuff from Windows headers
#define WIN32_LEAN_AND_MEAN             

// Windows Header Files:
#include <windows.h>
#include <tchar.h>

// C RunTime Header Files
#include "CCStdC.h"

// general application setting
// std lib
#include <assert.h>

#pragma warning (push)
// disable warning for cocos2d-x
#pragma warning(disable:4251)
#pragma warning(disable:4996)
#pragma warning(disable:4819)

// cocos2d-x header
#include "cocos2d.h"
#include "CCEGLView.h"
//#include "Box2D/Box2D.h"
#pragma warning (pop)

#endif    // __MAIN_H__
