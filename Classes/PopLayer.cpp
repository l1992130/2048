#include "PopLayer.h"
#include "GameScene.h"

PopLayer* PopLayer::create(const Color4B& color)
{
	PopLayer *pRet = new PopLayer();
    if(pRet && pRet->initWithColor(color))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

bool PopLayer::initWithColor(const Color4B& color)
{
	if (!LayerColor::initWithColor(color))
		return false;

	Size winSize = Director::getInstance()->getWinSize();
	Point centerPos = Point(winSize.width / 2,winSize.height / 2);

	MenuItemFont::setFontName("Consolas");
	MenuItemFont::setFontSize(80);

	auto menuItemContinue = MenuItemFont::create("CONTINUE",CC_CALLBACK_1(PopLayer::onContinue,this));
	auto menuItemRestart = MenuItemFont::create("RESTART",CC_CALLBACK_1(PopLayer::onRestart,this));
	auto menuItemExit = MenuItemFont::create("EXIT",CC_CALLBACK_1(PopLayer::onExit,this));

	auto menu = Menu::create(menuItemContinue,menuItemRestart,menuItemExit,NULL);
	menu->alignItemsVertically();
	addChild(menu);
	menu->setPosition(centerPos);

	return true;
}

void PopLayer::onRestart(Ref* pSender)
{
    //UserDefault::getInstance()->setBoolForKey("history", false);
    Director::getInstance()->replaceScene(GameScene::createScene());
    Director::getInstance()->resume();
}

void PopLayer::onExit(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif
    
    Director::getInstance()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void PopLayer::onContinue(Ref* pSender)
{
	//Director::getInstance()->resume();
	log("continue");
	removeFromParent();
	
}

