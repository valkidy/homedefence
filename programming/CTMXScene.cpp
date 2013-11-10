#include "CTMXScene.h"

USING_NS_CC;

#include <iostream>
#include <map>

namespace std
{
    class CKeyboard
    {
    protected:
        typedef map<int, bool> KeyboardMAP;
        typedef KeyboardMAP::iterator KeyboardMAP_ITER;
        typedef KeyboardMAP::const_iterator KeyboardMAP_CITER;

    public:
        CKeyboard() : m_pDelegateLayer(NULL) {}
        virtual ~CKeyboard() {}

        void RegisterKey(const int keycode) {m_KeyBoard[keycode] = false;}
        void RegisterWithKeyDispatcher(IKeyboardDelegate* _pDelegateLayer) {m_pDelegateLayer = _pDelegateLayer;}
        
        void Capture() 
        {
            for (KeyboardMAP_ITER iter=m_KeyBoard.begin();iter!=m_KeyBoard.end();++iter)
            {
                if ((GetAsyncKeyState(iter->first)<0) && (!iter->second))
                {
                    if (m_pDelegateLayer)
                        m_pDelegateLayer->KeyPress(iter->first);
                    iter->second = true;
                }
                
                if ((GetAsyncKeyState(iter->first)>=0) && (iter->second))
                {
                    if (m_pDelegateLayer)
                        m_pDelegateLayer->KeyRelease(iter->first);
                    iter->second = false;
                }
            } // End for
        }
    protected:
        KeyboardMAP m_KeyBoard;

        IKeyboardDelegate* m_pDelegateLayer;
    };
}; // End namespace

std::CKeyboard g_Keyboard;


/* ******************************************************
* CTMXScene 
****************************************************** */
#define set_empty(t, x ,y ,row)  {t[(y) * (row) + (x) -1] = 0;}
#define set_occupy(t, x ,y ,row) {t[(y) * (row) + (x) -1] = 1;}
#define get_occupy(t, x ,y ,row) (t[(y) * (row) + (x) -1] & 1)

enum 
{
    kTagTileMap = 1,
};

CCScene* 
CTMXScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    CTMXScene *layer = CTMXScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
} // End for scene of CTMXScene
    
bool 
CTMXScene::init()
{
    if (!CCLayer::init())
        return false;

    setTouchEnabled(true);
         
    // add map
    m_pMap = CCTMXTiledMap::create("../resource/orthogonal-test-zorder.tmx");
    addChild(m_pMap, 0, kTagTileMap);
    
    // add main character
    m_pMainCharacter = CCSprite::create("../resource/ogredance_0.png", CCRect(0,0,32,32));
    m_pMap->addChild(m_pMainCharacter, m_pMap->getChildren()->count());
    m_pMainCharacter->setAnchorPoint(ccp(0.5f,0.0f));    
    m_pMainCharacter->setScale(5.0f);
    
    cocos2d::CCAnimation *_pAnimation = CCAnimation::create();
    _pAnimation->setDelayPerUnit(1.0f/8.0f);
    
    // not pack texture yet
    char buf[256];
    for (int i=0;i<8;++i)
    {
        sprintf_s(buf, 256, "../resource/ogredance_%d.png", i);
        _pAnimation->addSpriteFrameWithFileName(buf);
        //CCTexture2D* texture = CCTextureCache::sharedTextureCache()->textureForKey(buf);
        //_pAnimation->addSpriteFrameWithTexture(texture, CCRect(0,0,32,32));
    }
    cocos2d::CCAnimate *_pAnimate = CCAnimate::create(_pAnimation);
    CCRepeatForever* _ccRepeat = CCRepeatForever::create(_pAnimate);
	m_pMainCharacter->runAction(_ccRepeat);

    updateSpritePosition(m_pMainCharacter, 1, 1);

    // add enemy
    
    
    CCTMXLayer* _pLayer = m_pMap->layerNamed("grass");
    const CCSize& ccLayerSize = _pLayer->getLayerSize();
    
    
    CCTMXLayer* _pObjectLayer = m_pMap->layerNamed("enemy");
    int iNumOfEnemy = 5; //_pObjectLayer->getChildrenCount();   
    cocos2d::CCArray* _objectIndex = _pObjectLayer->getChildren();
    for (int i=0;i<iNumOfEnemy;++i)
    {
        CCSprite* _pEnemy = CCSprite::create("../resource/Icon.png");
        m_vEnemyContainer.push_back(_pEnemy);

        m_pMap->addChild(_pEnemy, m_pMap->getChildren()->count());
        
        _pEnemy->setAnchorPoint(ccp(0.5f,0.0f));
        _pEnemy->setScale(1.5f);

        updateSpritePosition(_pEnemy, rand() % (int)(ccLayerSize.width-2.0f) + 1, rand() % (int)(ccLayerSize.height-2.0f) + 1);
    } // End for
    
    

    // update function
    schedule( schedule_selector(CTMXScene::repositionSprite));
    schedule( schedule_selector(CTMXScene::updateController), 1.0f/60.0f);

    // register keyboard
    g_Keyboard.RegisterWithKeyDispatcher(this);
    g_Keyboard.RegisterKey('W');
    g_Keyboard.RegisterKey('A');
    g_Keyboard.RegisterKey('S');
    g_Keyboard.RegisterKey('D');
    g_Keyboard.RegisterKey('Q');
    g_Keyboard.RegisterKey('Z');
    g_Keyboard.RegisterKey(VK_ESCAPE);

    return true;
}

void 
CTMXScene::repositionSprite(float dt)
{
    CCPoint p = m_pMainCharacter->getPosition();
    p = CC_POINT_POINTS_TO_PIXELS(p);
    CCNode* map = getChildByTag(kTagTileMap);
    
    // there are only 4 layers. (grass and 3 trees layers)
    // if tamara < 81, z=4
    // if tamara < 162, z=3
    // if tamara < 243,z=2

    // -10: customization for this particular sample
    int newZ = 4 - ( (p.y-10) / 81);
    newZ = max(newZ,0);

    map->reorderChild(m_pMainCharacter, newZ);
    //map->reorderChild(m_pEnemy, newZ);
}

void 
CTMXScene::updateController(float dt)
{
    g_Keyboard.Capture();

    updateCameraPosition(m_pMainCharacter);
}


void 
CTMXScene::updateSpritePosition(cocos2d::CCSprite* pSprite, float x, float y)
{
    if (pSprite && pSprite->getParent())
    {
        const CCSize& size = m_pMap->getTileSize();

        CCTMXLayer* _pLayer = m_pMap->layerNamed("grass");
        if (_pLayer)
        {
            const CCSize& ccLayerSize = _pLayer->getLayerSize();
            pSprite->setPosition(ccp((x + 0.5f) * size.width, ((ccLayerSize.height - y - 1.0f) + 0.5f)* size.height));            
        } // End if
    } //End if
}

void 
CTMXScene::updateCameraPosition(cocos2d::CCSprite* pSprite)
{
    if (pSprite && pSprite->getParent())
    {
        CCNode *node = getChildByTag(kTagTileMap);
        if (node)
        {
            const CCSize& size = m_pMap->getTileSize();
            const CCPoint& ccPosition = pSprite->getPosition();
            float fcoord_x = floor(ccPosition.x/size.width-0.5f);
            float fcoord_y = floor(ccPosition.y/size.height-0.5f);

            CCPoint ccTargetPos = ccp( (3.0f-fcoord_x) * size.width, (3.0f-fcoord_y) * size.height);
            const CCPoint& currentPos = node->getPosition();
            if (ccpDistance(ccTargetPos, currentPos) > 300.0f)
            {
                CCActionInterval* move = CCMoveBy::create(0.3f, ccpSub(ccTargetPos, currentPos));

                if (node->numberOfRunningActions() <=0)
                    node->runAction(move);
            } // End if
        } // End if 
    } // End if
}

void 
CTMXScene::updateCameraScale()
{
    CCNode *node = getChildByTag(kTagTileMap);
    if (node)
    {        
        float fElapsedTime = 1.5f;

        CCScaleBy* scale = CCScaleBy::create(fElapsedTime, node->getScale() * 0.5f);
        CCScaleBy* scaleBack = (CCScaleBy*) (scale->reverse());
        CCFiniteTimeAction* seq1 = CCSequence::create(scale, scaleBack, NULL);
        
        CCMoveBy* move = CCMoveBy::create(fElapsedTime, ccpSub(CCPointZero, node->getPosition()));
        CCMoveBy* moveBack = (CCMoveBy*) (move->reverse());
        CCFiniteTimeAction* seq2 = CCSequence::create(move, moveBack, NULL);

        if (node->numberOfRunningActions() <=0)
        {
            node->runAction(seq1);
            node->runAction(seq2);
        } // End if
    } // End if 
}

void 
CTMXScene::offsetSpritePosition(cocos2d::CCSprite* pSprite, float dx, float dy)
{
    if (pSprite && pSprite->getParent())
    {
        CCTMXLayer* _pLayer = m_pMap->layerNamed("grass");
        if (!_pLayer)
            return;

        const CCSize& ccLayerSize = _pLayer->getLayerSize();
        const CCSize& ccTileSize = m_pMap->getTileSize();
        const CCPoint& ccPosition = pSprite->getPosition();

        CCPoint ccCurrCoord = ccp( floor(ccPosition.x/ccTileSize.width-0.5f), floor(ccPosition.y/ccTileSize.height-0.5f));
        CCPoint ccNextCoord = ccpAdd(ccCurrCoord, ccp(dx, dy));
        // flip coord y by tile.num_height
        ccNextCoord.y = ccLayerSize.height - ccNextCoord.y - 1.0f;

        if (ccNextCoord.x >= 0.0f && ccNextCoord.x < ccLayerSize.width 
         && ccNextCoord.y >= 0.0f && ccNextCoord.y < ccLayerSize.height)
        {
            if (true == checkTileCollidable(_pLayer, ccNextCoord))
                return;
            //if (true == get_occupy(m_TileFlag, (int)ccNextCoord.x, (int)ccNextCoord.y, (int)ccLayerSize.width))
            //    return;
            //set_empty(m_TileFlag, (int)ccCurrCoord.x, (int)(ccLayerSize.height - ccCurrCoord.y - 1.0f), (int)ccLayerSize.width);
            //set_occupy(m_TileFlag, (int)ccNextCoord.x, (int)ccNextCoord.y, (int)ccLayerSize.width);

            CCActionInterval* move = CCMoveBy::create(0.03f, ccp(ccTileSize.width * dx, ccTileSize.height * dy));
            pSprite->runAction(move);
//==========================================================
// add redirect tile
            CCTMXLayer* _pMetaLayer = m_pMap->layerNamed("meta");
            if (_pMetaLayer)
            {
                CCDictionary* _dict = m_pMap->propertiesForGID(_pMetaLayer->tileGIDAt(ccNextCoord) );
                if (_dict)
                {
                    const CCString* _ccValue = _dict->valueForKey("redirect");
                    if (_ccValue && _ccValue->compare("true") == 0)
                    {
                        CCDelayTime *delay = CCDelayTime::create(0.03f);
                        
                        //if (true == get_occupy(m_TileFlag, (int)ccNextCoord.x, (int)ccNextCoord.y-1, (int)ccLayerSize.width))
                        //    return;
                        //set_empty(m_TileFlag, (int)ccNextCoord.x, (int)ccNextCoord.y, (int)ccLayerSize.width);
                        //set_occupy(m_TileFlag, (int)ccNextCoord.x, (int)ccNextCoord.y-1, (int)ccLayerSize.width);

                        CCActionInterval* moveDown = CCMoveBy::create(0.03f, ccp(ccTileSize.width * 0.0f, -ccTileSize.height * 1.0f));
                        CCFiniteTimeAction* seq = CCSequence::create(move, delay, moveDown, NULL);                        
                        pSprite->runAction(seq);
                    }
                } 
            } // End if
//==========================================================
        } 
        else
        {
            ccNextCoord.x = fmod(ccNextCoord.x + ccLayerSize.width, ccLayerSize.width);
            ccNextCoord.y = fmod(ccNextCoord.y + ccLayerSize.height, ccLayerSize.height);

            if (true == checkTileCollidable(_pLayer, ccNextCoord))
                return;
            //if (true == get_occupy(m_TileFlag, (int)ccNextCoord.x, (int)ccNextCoord.y, (int)ccLayerSize.width))
            //    return;
            //set_empty(m_TileFlag, (int)ccCurrCoord.x, (int)(ccLayerSize.height - ccCurrCoord.y - 1.0f), (int)ccLayerSize.width);
            //set_occupy(m_TileFlag, (int)ccNextCoord.x, (int)ccNextCoord.y, (int)ccLayerSize.width);

            updateSpritePosition(pSprite, ccNextCoord);
        } // End if


        // simple check win-lost         
        //if (pSprite != m_pMainCharacter) 
        //{
        //    const CCPoint& ccCharPosition = m_pMainCharacter->getPosition();
        //    CCPoint ccCharCoord = ccp( floor(ccCharPosition.x/ccTileSize.width-0.5f), floor(ccCharPosition.y/ccTileSize.height-0.5f));
        //    ccCharCoord.y = ccLayerSize.height - ccCharCoord.y - 1.0f;
        //    if (ccNextCoord.equals(ccCharCoord))
        //    {
        //        ::MessageBox(
        //            NULL,
        //            (LPCWSTR)L"You've been caught by enemy",
        //            (LPCWSTR)L"Game over",
        //            MB_ICONWARNING | MB_CANCELTRYCONTINUE | MB_DEFBUTTON2
        //        );
        //    } // End if
        //}
    } // End if
}

bool 
CTMXScene::checkTileCollidable(cocos2d::CCTMXLayer* _pLayer, const cocos2d::CCPoint& ccCoordIndex)
{
    if (!_pLayer)
        return false;

    CCDictionary* _dict = m_pMap->propertiesForGID(_pLayer->tileGIDAt(ccCoordIndex) );
    if (_dict)
    {
        const CCString* _ccValue = _dict->valueForKey("collidable");
        if (_ccValue && _ccValue->compare("true") == 0)
            return true;
    } // End if

    return false;
}

void
CTMXScene::ccTouchMoved(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent)
{
    CCPoint diff = pTouch->getDelta();
    CCNode *node = getChildByTag(kTagTileMap);
    if (node)
    {
        CCPoint currentPos = node->getPosition();
        node->setPosition( ccpAdd(currentPos, diff) );
    } 
}

void 
CTMXScene::registerWithTouchDispatcher()
{
    cocos2d::CCDirector* pDirector = cocos2d::CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

void 
CTMXScene::KeyPress(int iKeyCode)
{
    CCLog("KeyPress, code=%d", iKeyCode);
    
    switch (iKeyCode)
    {
    case 'W':
        offsetSpritePosition(m_pMainCharacter, 0.0f, 1.0f);
        break;
    case 'A':
        offsetSpritePosition(m_pMainCharacter, -1.0f, 0.0f);
        break;
    case 'S':
        offsetSpritePosition(m_pMainCharacter, 0.0f, -1.0f);
        break;
    case 'D':
        offsetSpritePosition(m_pMainCharacter, 1.0f, 0.0f);
        break;
    case 'Q':
        updateCameraScale();
        //enemyThinking();
        break;
    case 'Z':
        break;
    case VK_ESCAPE:
        CCDirector::sharedDirector()->end();
        break;
    }    

    enemyThinking();
}

void 
CTMXScene::KeyRelease(int iKeyCode)
{
    CCLog("KeyRelease, code=%d", iKeyCode);
}


void
CTMXScene::enemyThinking()
{
    for (std::vector<cocos2d::CCSprite*>::iterator iter=m_vEnemyContainer.begin();iter!=m_vEnemyContainer.end();++iter)
    {
        CCSprite* _pEnemy = (*iter);
        CCPoint offset = ccpSub(_pEnemy->getPosition(), m_pMainCharacter->getPosition());

        if (abs(offset.x)>0)
            offsetSpritePosition(_pEnemy, -abs(offset.x)/offset.x, 0.0f);        
        else if (abs(offset.y)>0)
            offsetSpritePosition(_pEnemy, 0.0f, -abs(offset.y)/offset.y);
    } // End for
}
