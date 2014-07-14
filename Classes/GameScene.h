#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "CardSprite.h"

class GameScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	void createCardSprite(cocos2d::Size size);
	void onPause();
	CREATE_FUNC(GameScene);
	virtual bool onTouchBegan(cocos2d::Touch *touch,cocos2d::Event *unused_event);
	virtual void onTouchEnded(cocos2d::Touch *touch,cocos2d::Event *unused_event);
	void autoCreateCardNumber(bool animation = true);
	void doCheckGameOver();
	void saveStatus();
	void resumeStatus();
	void setScore(int score);
	void removeSuccessLayer();
	bool shouldCreateCardNumber();
	bool isWin();
	bool doUp();
	bool doDown();
	bool doLeft();
	bool doRight();
private:
	int score;
	CardSprite* cardArr[4][4];
	int firstX,firstY,endX,endY;
	cocos2d::Label* cardNumberTTF;
	cocos2d::LayerColor *successLayer;
};
#endif