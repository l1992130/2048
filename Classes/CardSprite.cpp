#include "CardSprite.h"

USING_NS_CC;

CardSprite* CardSprite::createCardSprite(int numbers,int width,int height,float CardSpriteX,float CardSpriteY)
{
	CardSprite* enemy = new CardSprite();
	if (enemy && enemy->init())
	{
		enemy->autorelease();
		enemy->enemyInit(numbers,width,height,CardSpriteX,CardSpriteY);
		return enemy;
	}
	CC_SAFE_DELETE(enemy);
	return NULL;
}

bool CardSprite::init()
{
	if (!Sprite::init())
		return false;
	return true;
}

void CardSprite::enemyInit(int numbers,int width,int height,float CardSpriteX,float CardSpriteY)
{
	number = numbers;
	layerColorBG = cocos2d::LayerColor::create(Color4B(200,190,180,255),width - 15,height - 15);
	layerColorBG->setPosition(Point(CardSpriteX,CardSpriteY));

	if (number > 0)
		labelTTFCardNumber = Label::createWithSystemFont(String::createWithFormat("%i",number)->getCString(),"HiraKakuProN-W6",100);
	else
		labelTTFCardNumber = Label::createWithSystemFont("","HiraKakuProN-W6",100);

	labelTTFCardNumber->setPosition(Point(layerColorBG->getContentSize().width/2,layerColorBG->getContentSize().height/2)); 
	labelTTFCardNumber->setTag(8);
	layerColorBG->addChild(labelTTFCardNumber);
	this->addChild(layerColorBG);
}

void CardSprite::runNewNumberAction()
{
    auto action = ScaleBy::create(0.05,0.99);
    runAction(Sequence::create(action, action->reverse(), nullptr));
}

int CardSprite::getNumber()
{
	return number;
}

void CardSprite::setNumber(int num)
{
	number = num;
	if (number > 0)
		labelTTFCardNumber->setString(String::createWithFormat("%i",number)->getCString()); 
	else
		labelTTFCardNumber->setString(""); 

	if (number >= 0)
	{
		labelTTFCardNumber->setSystemFontSize(100);
	}
	if (number >= 16)
	{
		labelTTFCardNumber->setSystemFontSize(90);
	} 
	if (number >= 128)
	{
		labelTTFCardNumber->setSystemFontSize(60);
	}
	if (number >= 1024)
	{
		labelTTFCardNumber->setSystemFontSize(40);
	}

	//判断数字的大小来调整颜色
    if(number == 0){
        layerColorBG->setColor(cocos2d::Color3B(200,190,180));
    }
    if (number == 2) {
        layerColorBG->setColor(cocos2d::Color3B(240,230,220));
    }
    if (number == 4) {
        layerColorBG->setColor(cocos2d::Color3B(240,220,200));
    }
    if (number == 8) {
        layerColorBG->setColor(cocos2d::Color3B(240,180,120));
    }
    if (number == 16) {
        layerColorBG->setColor(cocos2d::Color3B(240,140,90));
    }
    if (number == 32) {
        layerColorBG->setColor(cocos2d::Color3B(240,120,90));
    }
    if (number == 64) {
        layerColorBG->setColor(cocos2d::Color3B(240,90,60));
    }
    if (number == 128) {
        layerColorBG->setColor(cocos2d::Color3B(240,90,60));
    }
    if (number == 256) {
        layerColorBG->setColor(cocos2d::Color3B(240,200,70));
    }
    if (number == 512) {
        layerColorBG->setColor(cocos2d::Color3B(240,200,70));
    }
    if (number == 1024) {
        layerColorBG->setColor(cocos2d::Color3B(0,130,0));
    }
    if (number == 2048) {
        layerColorBG->setColor(cocos2d::Color3B(0,130,0));
    }

}