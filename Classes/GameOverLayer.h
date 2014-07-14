#ifndef __GameOverLayer__
#define __GameOverLayer__

#include "cocos2d.h"

USING_NS_CC;

class GameOverLayer : public LayerColor
{
public:
    static GameOverLayer* create(const Color4B& color);
    virtual bool initWithColor(const Color4B& color);
    
private:
    void onRestart(Ref* pSender);
};

#endif