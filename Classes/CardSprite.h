#ifndef __CARDSPRITE_SCENE_H_
#define __CARDSPRITE_SCENE_H_

#include "cocos2d.h"

class CardSprite : public cocos2d::Sprite
{
public:
	static CardSprite* createCardSprite(int numbers,int width,int height,float CardSpriteX,float CardSpriteY);
	virtual bool init();
	CREATE_FUNC(CardSprite);
	int getNumber();
	void setNumber(int num);
	void runNewNumberAction();
private:
	int number;
	void enemyInit(int number,int width,int height,float CardSpriteX,float CardSpriteY);
	cocos2d::Label* labelTTFCardNumber;
	cocos2d::LayerColor* layerColorBG;
};
#endif