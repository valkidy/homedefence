#ifndef __TMX_SCENE_H__
#define __TMX_SCENE_H__

#include "Config.h"

class IKeyboardDelegate
{
public:
    virtual void KeyPress(int iKeyCode) = 0;
    virtual void KeyRelease(int iKeyCode) = 0;
};

class CTMXScene
    : public cocos2d::CCLayer,
      public IKeyboardDelegate
{
public:
    CTMXScene() { memset(m_TileFlag,0,1024);}
    virtual ~CTMXScene() {}

    // override
    virtual bool init();
    
    static cocos2d::CCScene* scene();
    CREATE_FUNC(CTMXScene);
    
    // touch event
    virtual void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent) { return true;}
    virtual void ccTouchEnded(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent) {}
    virtual void ccTouchCancelled(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent) {}
    virtual void ccTouchMoved(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent);
    // keyboard event
    virtual void KeyPress(int iKeyCode);
    virtual void KeyRelease(int iKeyCode);

    // update
    void repositionSprite(float dt);
    void updateController(float dt);    
    void updateSpritePosition(cocos2d::CCSprite* pSprite, float x, float y);
    void updateSpritePosition(cocos2d::CCSprite* pSprite, const cocos2d::CCPoint& point) {updateSpritePosition(pSprite, point.x, point.y);}
    void offsetSpritePosition(cocos2d::CCSprite* pSprite, float x, float y);    
    
    void updateCameraPosition(cocos2d::CCSprite* pSprite);
    void updateCameraScale();
    
    void enemyThinking();

protected:
    bool checkTileCollidable(cocos2d::CCTMXLayer* _pLayer, const cocos2d::CCPoint& ccCoordIndex);

private:
    typedef std::vector<cocos2d::CCSprite*> SpriteVEC;
    SpriteVEC m_vEnemyContainer;
    
    char m_TileFlag[1024];

    cocos2d::CCSprite* m_pMainCharacter; 
    //cocos2d::CCSprite* m_pEnemy;     
    cocos2d::CCTMXTiledMap* m_pMap;
}; // End of CTMXScene
    
#endif  // __TMX_SCENE_H__