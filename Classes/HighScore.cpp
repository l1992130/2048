#include "HighScore.h"

USING_NS_CC;

#define KHIGHSCORE "HighScore"

static HighScore* s_ShareHighScore = NULL;

HighScore* HighScore::getInstance()
{
	if (!s_ShareHighScore)
	{
		s_ShareHighScore = new HighScore();
		s_ShareHighScore->init();
	}

	return s_ShareHighScore;
}

void HighScore::destroyInstance()
{
    CC_SAFE_DELETE(s_ShareHighScore);
}

HighScore::HighScore()
{
    m_score = 0;
    m_highScore = 0;
}

bool HighScore::init()
{
    m_highScore = UserDefault::getInstance()->getIntegerForKey(KHIGHSCORE);
    
    return true;
}

int HighScore::getHighScore()
{
    return m_highScore;
}

int HighScore::getScore()
{
    return m_score;
}

void HighScore::setScore(int score)
{
    m_score = score;
    if (score > m_highScore) {
        m_highScore = score;
        UserDefault::getInstance()->setIntegerForKey(KHIGHSCORE, score);
    }
}