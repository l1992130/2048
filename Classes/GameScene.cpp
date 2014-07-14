#include "GameScene.h"
#include "CardSprite.h"
#include "PopLayer.h"
#include "HighScore.h"
#include "GameOverLayer.h"

USING_NS_CC;

Scene* GameScene::createScene()
{
	auto scene = Scene::create();
	auto layer = GameScene::create();
	scene->addChild(layer);
	return scene;
}

bool GameScene::init()
{
	if (!Layer::init())
		return false;
	Size visibleSize = Director::getInstance()->getVisibleSize();

	//加入游戏背景
	//1
	auto layerColorBG = LayerColor::create(Color4B(180,170,160,255));
	this->addChild(layerColorBG);

	int unitSize = (visibleSize.height - 28)/4;

	//pause
	//2
	MenuItemFont::setFontName("Consolas");
	MenuItemFont::setFontSize(80);
	auto menuItemPause = MenuItemFont::create("PAUSE",CC_CALLBACK_0(GameScene::onPause,this));

	auto menu = Menu::create(menuItemPause,NULL);
	addChild(menu);
	menu->setPosition(Point((visibleSize.width - unitSize * 4 - 40) / 2 + unitSize * 4 + 40,visibleSize.height / 2 + 250));

	//创建分数
	//3
	auto cardNumberTitle = Label::createWithSystemFont("SCORE","Consolas",80);
	cardNumberTitle->setPosition(Point((visibleSize.width - unitSize * 4 - 40) / 2 + unitSize * 4 + 40,visibleSize.height / 2 + 50));
	addChild(cardNumberTitle);


	score = 0;
	cardNumberTTF = Label::createWithSystemFont("0","Consolas",80);
	cardNumberTTF->setPosition(Point((visibleSize.width - unitSize * 4 - 40) / 2 + unitSize * 4 + 40,visibleSize.height / 2 - 50));
	addChild(cardNumberTTF);

	//添加卡片
	createCardSprite(visibleSize);
	if (UserDefault::getInstance()->getBoolForKey("history"))
	{
		resumeStatus();
	}

	/*autoCreateCardNumber();
	autoCreateCardNumber();*/
	//监听事件
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan,this);
	touchListener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded,this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener,this);

	return true;
}
void GameScene::onPause()
{
	log("i am pause button");
	PopLayer *poplayer = PopLayer::create(Color4B(0,0,0,180));
	getParent()->addChild(poplayer);
	Director::getInstance()->pause();
}

void GameScene::createCardSprite(cocos2d::Size size)
{
	int unitSize = (size.height - 28)/4;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			CardSprite* card = CardSprite::createCardSprite(2,unitSize,unitSize,unitSize * i + 40,unitSize * j + 20);
			cardArr[i][j] = card;
			addChild(card);
		}
	}
}

void GameScene::autoCreateCardNumber(bool animation)
{
	while (1)
	{
		int i = CCRANDOM_0_1() * 4;
		int j = CCRANDOM_0_1() * 4;

		if (cardArr[i][j]->getNumber() == 0)
		{
			cardArr[i][j]->setNumber(CCRANDOM_0_1() * 10 < 1 ? 4 : 2);
			if (animation)
			{
				cardArr[i][j]->runNewNumberAction();
			}
			break;
		}
		if (!shouldCreateCardNumber())
		{
			break;
		}
	}
}

void GameScene::doCheckGameOver()
{
	bool isGameOver = true;

	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			if (cardArr[x][y]->getNumber() == 0 ||
				(x > 0 && (cardArr[x][y]->getNumber() == cardArr[x-1][y]->getNumber())) ||
				(x < 3 && (cardArr[x][y]->getNumber() == cardArr[x+1][y]->getNumber())) ||
				(y > 0 && (cardArr[x][y]->getNumber() == cardArr[x][y-1]->getNumber())) ||
				(y < 3 && (cardArr[x][y]->getNumber() == cardArr[x][y+1]->getNumber()))
				)
			{
				isGameOver = false;
			}
		}
	}
	if (isWin())
	{
		successLayer = LayerColor::create(Color4B(0, 0, 0, 180));
		Size winSize = Director::getInstance()->getWinSize();
		Point centerPos = Point(winSize.width / 2, winSize.height / 2);
		auto gameOverTitle = Label::createWithSystemFont("YOU WIN","Consolas",80);
		gameOverTitle->setPosition(centerPos);
		successLayer->addChild(gameOverTitle);

		getParent()->addChild(successLayer,1);

		scheduleOnce(SEL_SCHEDULE(&GameScene::removeSuccessLayer), 2);
		return;
	}
	if (isGameOver)
	{
		log("游戏结束！");
		HighScore::getInstance()->setScore(score);
		//Director::getInstance()->replaceScene(TransitionFade::create(1,GameScene::createScene()));
		GameOverLayer* gameoverlayer = GameOverLayer::create(Color4B(0,0,0,180));
		getParent()->addChild(gameoverlayer,1);
		Director::getInstance()->pause();
	}
	else
	{
		if(shouldCreateCardNumber())
		{
			autoCreateCardNumber();
		}
		saveStatus();
	}
}

void GameScene::removeSuccessLayer()
{
	successLayer->removeFromParent();
}

bool GameScene::isWin()
{
	bool win = false;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if( 2048 == cardArr[i][j]->getNumber() )
			{
				win = true;
				break;
			}
		}
	}

	return win;
}

void GameScene::saveStatus()
{
	char temp[10];
	for (int x = 0; x < 4; x++)
	{
		for (int y = 0; y < 4; y++)
		{
			sprintf(temp,"%d%d",x,y);
			UserDefault::getInstance()->setIntegerForKey(temp,cardArr[x][y]->getNumber());
		}
	}

	UserDefault::getInstance()->setIntegerForKey("score",score);
	UserDefault::getInstance()->setBoolForKey("History",true);
}

void GameScene::setScore(int score)
{
	cardNumberTTF->setString(__String::createWithFormat("%i",score)->getCString());
}

void GameScene::resumeStatus()
{
	char temp[10];
	//4*4
	for (int i = 0; i<4; i++) {
		for(int j = 0; j<4; j++)
		{
			sprintf(temp,"%d%d",i,j);
			int number = UserDefault::getInstance()->getIntegerForKey(temp);
			cardArr[i][j]->setNumber(number);
		}
	}

	score = UserDefault::getInstance()->getIntegerForKey("score");
	setScore(score);

	UserDefault::getInstance()->setBoolForKey("history", false);
}

bool GameScene::shouldCreateCardNumber()
{
	bool isCreate = false;
	for (int i = 0; i <	4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (0 == cardArr[i][j]->getNumber())
			{
				isCreate = true;
				break;
			}
		}
	}
	return isCreate;
}

bool GameScene::onTouchBegan(cocos2d::Touch *touch,cocos2d::Event *unused_event)
{
	Point touchPoint = touch->getLocation();
	touch->getLocationInView();
	firstX = touchPoint.x;
	firstY = touchPoint.y;
	return true;
}

void GameScene::onTouchEnded(cocos2d::Touch *touch,cocos2d::Event *unused_event)
{
	Point touchPoint = touch->getLocation();
	endX = firstX - touchPoint.x;
	endY = firstY - touchPoint.y;
	if (abs(endX) > abs(endY))
	{
		if (endX > 0)
		{
			doLeft();
			doCheckGameOver();
		}
		if (endX < 0)
		{
			doRight();
			doCheckGameOver();
		}

	}
	else
	{
		if (endY > 0)
		{
			doDown();
			doCheckGameOver();
		}
		if (endY < 0)
		{
			doUp();
			doCheckGameOver();
		}
	}
}

bool GameScene::doUp()
{ 
	bool isdo = false;
	for (int x = 0; x < 4; x++)
	{
		for (int y = 3; y >= 0; y--)
		{
			for (int y1 = y-1; y1 >= 0; y1--)
			{
				if (cardArr[x][y1]->getNumber() > 0)
				{
					if(cardArr[x][y]->getNumber() <= 0)
					{
						log("ooo:%i,%i",y,y1);
						cardArr[x][y]->setNumber(cardArr[x][y1]->getNumber());
						cardArr[x][y1]->setNumber(0);
						y++;
						isdo = true;
					}
					else if (cardArr[x][y]->getNumber() == cardArr[x][y1]->getNumber())
					{
						log("lll:%i,%i",y,y1);
						cardArr[x][y]->setNumber(cardArr[x][y]->getNumber() * 2);
						cardArr[x][y1]->setNumber(0);

						score += cardArr[x][y]->getNumber();
						setScore(score);

						isdo = true;
					}
					break;
				}
			}
		}
	}
	return isdo;
} 
bool GameScene::doDown()
{ 
	bool isdo = false; 
	for (int x = 0; x < 4; x++) { 
		for (int y = 0; y < 4; y++) { 
			for (int y1 = y + 1; y1 < 4; y1++) { 
				if (cardArr[x][y1]->getNumber() > 0) { 
					if (cardArr[x][y]->getNumber() <= 0) { 
						cardArr[x][y]->setNumber(cardArr[x][y1]->getNumber()); 
						cardArr[x][y1]->setNumber(0); 
						y--; 
						isdo = true; 
					}else if(cardArr[x][y]->getNumber() == cardArr[x][y1]->getNumber()){ 
						cardArr[x][y]->setNumber(cardArr[x][y1]->getNumber()*2); 
						cardArr[x][y1]->setNumber(0); 

						score += cardArr[x][y]->getNumber();
						setScore(score);

						isdo = true; 
					} 
					break; 
				} 
			} 
		} 
	} 
	return isdo; 
} 
bool GameScene::doLeft()
{ 
	bool isdo = false;
	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			for (int x1 = x + 1; x1 < 4; x1++)
			{
				if (cardArr[x1][y]->getNumber() > 0)
				{
					if (cardArr[x][y]->getNumber() <= 0)
					{
						cardArr[x][y]->setNumber(cardArr[x1][y]->getNumber());
						cardArr[x1][y]->setNumber(0);
						x--;
						isdo = true;
					}
					else if (cardArr[x][y]->getNumber() == cardArr[x1][y]->getNumber())
					{
						cardArr[x][y]->setNumber(cardArr[x1][y]->getNumber() * 2);
						cardArr[x1][y]->setNumber(0);

						score += cardArr[x][y]->getNumber();
						setScore(score);

						isdo = true;
					}
					break;
				}
			}
		}
	}
	return isdo;
} 
bool GameScene::doRight()
{ 
	bool isdo = false; 
	for (int y = 0; y < 4; y++) { 
		for (int x = 3; x >= 0; x--) { 
			for (int x1 = x - 1; x1 >= 0; x1--) { 
				if (cardArr[x1][y]->getNumber() > 0) { 
					if (cardArr[x][y]->getNumber() <= 0) { 
						cardArr[x][y]->setNumber(cardArr[x1][y]->getNumber()); 
						cardArr[x1][y]->setNumber(0); 
						x++; 
						isdo = true; 
					}else if(cardArr[x][y]->getNumber() == cardArr[x1][y]->getNumber()){ 
						cardArr[x][y]->setNumber(cardArr[x1][y]->getNumber()*2); 
						cardArr[x1][y]->setNumber(0); 

						score += cardArr[x][y]->getNumber();
						setScore(score);

						isdo = true; 
					} 
					break; 
				} 
			} 
		} 
	} 
	return isdo; 
}