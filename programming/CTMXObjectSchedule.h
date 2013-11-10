#ifndef __TMX_OBJECT_SCHEDULE_H__
#define __TMX_OBJECT_SCHEDULE_H__

#include <iostream>
#include <vector>
    
#include "cocos2d.h"
#include "CTMXScene.h"

class CPrimitiveScene
    : public cocos2d::CCLayer,
      public IKeyboardDelegate
{
public:
    CPrimitiveScene() {}
    virtual ~CPrimitiveScene() {}

    // override
    static cocos2d::CCScene* scene();
    CREATE_FUNC(CPrimitiveScene);
    
    virtual bool init();
    virtual void draw();
    
    // touch event
    virtual void registerWithTouchDispatcher() {}
    virtual bool ccTouchBegan(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent) { return true;}
    virtual void ccTouchEnded(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent) {}
    virtual void ccTouchCancelled(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent) {}
    virtual void ccTouchMoved(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent) {}
    // keyboard event
    virtual void KeyPress(int iKeyCode) {}
    virtual void KeyRelease(int iKeyCode) {}

protected:
}; // End of CPrimitiveScene

#endif  // __TMX_OBJECT_SCHEDULE_H__