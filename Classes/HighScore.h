#ifndef __HighScore__
#define __HighScore__

#include "cocos2d.h"

class HighScore
{
public:
    static HighScore* getInstance();
    static void destroyInstance();

    HighScore();
    
    int getHighScore();
    int getScore();
    void setScore(int score);
 
private:
    bool init();
    int m_score;
    int m_highScore;
};

#endif