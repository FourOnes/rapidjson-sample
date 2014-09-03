#include "PlayerLevelInfo.h"
#include <stdio.h>  
#include <string>  
#include <iostream>  
#include "json/document.h"  
#include "json/prettywriter.h"  
#include "json/filestream.h"  
#include "json/stringbuffer.h" 
USING_NS_CC;
using namespace  rapidjson;
static PlayerLevelInfo *instance;
static Vector<PlayerLevelInfo*> levelInfos;
PlayerLevelInfo* PlayerLevelInfo::get(int levelIndex){
	int count=levelInfos.size();
	if(levelIndex<=0) levelIndex=1;
	if(levelIndex>count) levelIndex=count;
	return levelInfos.at(levelIndex-1);
}
bool PlayerLevelInfo::IsExistsLevelInfo(int levelIndex){
	int count=levelInfos.size();
	if(count==0) return false;
	if(levelIndex<0) return false;
	if(levelIndex>count) return false;
	return true;
}
bool PlayerLevelInfo::setLevelInfo(PlayerLevelInfo *info){
	if(!info) return false;
	bool isExists=this->IsExistsLevelInfo(info->getLevel());
	if(isExists){
		levelInfos.replace(info->getLevel()-1,info);
	}
	else{
		if(levelInfos.size()==0){
			levelInfos.pushBack(info);
		}
		else{
			levelInfos.insert(levelInfos.size()-1,info);
		}
	}
	this->saveData();
	return true;
}
void PlayerLevelInfo::saveData()
{
	rapidjson::Document document;
	rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
	rapidjson::Value datas(rapidjson::kArrayType);
	for (int i = 0; i < levelInfos.size(); i++)
	{
		rapidjson::Value object(rapidjson::kObjectType);
		auto item=levelInfos.at(i);
		object.AddMember("level",item->getLevel(),allocator);
		object.AddMember("score",item->getScore(),allocator);
		object.AddMember("star",item->getStar(),allocator);
		object.AddMember("time",item->getTime(),allocator);
		datas.PushBack(object,allocator);
	}

	StringBuffer buffer;  
	Writer<StringBuffer> writer(buffer);  
	datas.Accept(writer);  
	const char *result = buffer.GetString();  
	auto path=FileUtils::getInstance()->getWritablePath()+levelInfoFileName;
	FILE *file=fopen(path.c_str(),"wb");
	if(file){
		fwrite(result,sizeof(char),strlen(result),file);
		fclose(file);
	}
}
void PlayerLevelInfo::readData()
{
	levelInfos.clear();
	auto path=FileUtils::getInstance()->getWritablePath()+levelInfoFileName;
	CCLog("path=%s",path.c_str());
	auto data=FileUtils::getInstance()->getStringFromFile(path.c_str());
	if(data.size()==0) return;
	rapidjson::Document d;
	d.Parse<0>(data.c_str());
	if(d.HasParseError()){
		CCLog("GetParseError %s \n",d.GetParseError());
		return;
	}	
	if(d.IsArray()){
		for (int i = 0; i < d.Size(); i++)
		{
			auto item=new PlayerLevelInfo();
			const rapidjson::Value &val=d[i];
			item->setLevel(val["level"].GetInt());
			item->setScore(val["score"].GetInt());
			item->setStar(val["star"].GetInt());
			item->setTime(val["time"].GetDouble());
			levelInfos.pushBack(item);
			CC_SAFE_RELEASE(item);
		}
	}
}
PlayerLevelInfo* PlayerLevelInfo::getInstance()
{
	if(instance){
		return instance;
	}
	instance=new PlayerLevelInfo();
	instance->init();
	return instance;
}
bool PlayerLevelInfo::init(){	
	this->readData();
	return true;
}