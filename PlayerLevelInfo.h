#ifndef __PLAYERLEVELINFO_H__
#define __PLAYERLEVELINFO_H__
#include "cocos2d.h"
#ifndef levelInfoFileName
#define levelInfoFileName "levelInfoFileName.data"
#endif
USING_NS_CC;
class PlayerLevelInfo:public Ref{
public:
	//level index
	CC_SYNTHESIZE(int,level,Level);
	CC_SYNTHESIZE(int,score,Score);
	CC_SYNTHESIZE(int,star,Star);
	CC_SYNTHESIZE(float,time,Time);
	PlayerLevelInfo* get(int levelIndex);
	bool setLevelInfo(PlayerLevelInfo *info);
	bool IsExistsLevelInfo(int levelIndex);
	static PlayerLevelInfo* getInstance();
protected:
	bool init();
	void saveData();
	void readData();
};
#endif