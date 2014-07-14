#ifndef __POPLAYER_SCENE_H__
#define __POPLAYER_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

class PopLayer : public LayerColor
{
public:
	static PopLayer* create(const Color4B& color);
	virtual bool initWithColor(const Color4B& color);
private:
	void onRestart(Ref* pSender);
    void onExit(Ref* pSender);
    void onContinue(Ref* pSender);
};
#endif