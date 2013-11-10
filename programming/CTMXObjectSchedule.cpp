#include "CTMXObjectSchedule.h"

USING_NS_CC;

CCScene* 
CPrimitiveScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    CPrimitiveScene *layer = CPrimitiveScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
} // End for scene of CTMXScene

static CCGLProgram* s_pShader = NULL;
static int s_nColorLocation = -1;
static ccColor4F s_tColor = {1.0f,1.0f,1.0f,1.0f};
static int s_nPointSizeLocation = -1;

bool
CPrimitiveScene::init()
{
    s_pShader = CCShaderCache::sharedShaderCache()->programForKey(kCCShader_Position_uColor);

    s_nColorLocation = glGetUniformLocation( s_pShader->getProgram(), "u_color");
    
    s_nPointSizeLocation = glGetUniformLocation( s_pShader->getProgram(), "u_pointSize");

    return true;
}

void 
CPrimitiveScene::draw()
{    
    CCCamera* pCamera = CCDirector::sharedDirector()->getRunningScene()->getCamera();
    pCamera->setEyeXYZ(0,0,10);
    pCamera->setCenterXYZ(-50.0f, -50.0f, 0);

    // open yellow poly
    ccDrawColor4B(255, 255, 0, 255);
    glLineWidth(10);
    ccVertex3F vertices[] = { vertex3(0,0,0), vertex3(50,50,0), vertex3(100,50,0), vertex3(100,100,0), vertex3(50,100,0.1) };
        
    s_pShader->use();
    s_pShader->setUniformForModelViewProjectionMatrix();
    s_pShader->setUniformLocationWith4fv(s_nColorLocation, (GLfloat*) &s_tColor.r, 1);

    ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position );


    int numberOfPoints = 5;
    // XXX: Mac OpenGL error. arrays can't go out of scope before draw is executed
    ccVertex3F* newPoli = new ccVertex3F[numberOfPoints];

    // iPhone and 32-bit machines optimization
    if( sizeof(CCPoint) == sizeof(ccVertex3F) )
    {
        glVertexAttribPointer(kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, 0, vertices);
    }
    else
    {
        // Mac on 64-bit
        for( unsigned int i=0; i<numberOfPoints;i++)
        {
            newPoli[i] = vertex3( vertices[i].x, vertices[i].y, vertices[i].z );
        }
        glVertexAttribPointer(kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, 0, newPoli);
    }    

    glDrawArrays(GL_TRIANGLE_FAN, 0, (GLsizei) numberOfPoints);

    CC_SAFE_DELETE_ARRAY(newPoli);
    CC_INCREMENT_GL_DRAWS(1);
}