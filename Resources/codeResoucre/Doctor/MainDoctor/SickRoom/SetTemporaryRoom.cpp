//
//  SetTemporaryRoom.cpp
//  Chuangzheng-mobile
//
//  Created by 工作 on 2018/1/3.
//

#include "SetTemporaryRoom.hpp"
#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"
#include <iostream>
#include "NetWrokMangerData.hpp"
using namespace cocos2d::ui;
using namespace std;
USING_NS_CC;
Scene *SetTemporaryRoom::createScene(){
    return SetTemporaryRoom::create();
}
bool SetTemporaryRoom::init(){
    if (!Scene::init()) {
        return false;
    }
    Size visibleSize=Director::getInstance()->getVisibleSize();
    auto bkView=Sprite::create("bk_set_temporary_room.png");
    bkView->setPosition(Vec2(0, 0));
    bkView->setAnchorPoint(Vec2(0, 0));
    bkView->setContentSize(visibleSize);
    this->addChild(bkView);
    
    auto backBtn=Button::create();
    backBtn->loadTextures("btn_register_return.png", "btn_register_return.png");
    backBtn->setPosition(Vec2(80, visibleSize.height-85));
    backBtn->addTouchEventListener([&](Ref* sender, cocos2d::ui::Widget::TouchEventType type){ switch (type){
        case ui::Widget::TouchEventType::BEGAN: break;
        case ui::Widget::TouchEventType::ENDED:
            Director::getInstance()->popScene();
        default:
            break;
    }
    });
    this->addChild(backBtn);
    
    auto addBtn=Button::create();
    addBtn->loadTextures("btn_addCase_add.png", "btn_addCase_add.png");
    addBtn->setPosition(Vec2(visibleSize.width-100, visibleSize.height-85));
    addBtn->addTouchEventListener([&](Ref* sender, cocos2d::ui::Widget::TouchEventType type){ switch (type){
        case ui::Widget::TouchEventType::BEGAN: break;
        case ui::Widget::TouchEventType::ENDED:{
            if (strcmp(textfieldHospital->getText(), "")==0||strcmp(textfieldBuilding->getText(), "")==0||strcmp(textfieldFloor->getText(), "")==0||strcmp(textfieldBed->getText(), "")==0) {
                importCompleteBtn->setVisible(true);
                 importCompleteBtn->runAction(Sequence::create(DelayTime::create(0.5),FadeOut::create(0.5), NULL));
            }else{
                pushDataToNetWork();
                log("新建成功");
            }
           
        }
        default:
            break;
    }
    });
    this->addChild(addBtn);
     textfieldHospital=createBasicData(bkView, Vec2(59, 900), "医院：", "请输入医院");
     textfieldBuilding=createBasicData(bkView, Vec2(59, 810), "楼：", "请输入医院楼名称");;
     textfieldFloor=createBasicData(bkView, Vec2(59, 720), "楼层：", "请输入楼层");;
    textfieldFloor->setInputMode(cocos2d::ui::EditBox::InputMode::NUMERIC);
     textfieldBed=createBasicData(bkView, Vec2(59, 630), "床号：", "请输入病床号");
    textfieldBed->setInputMode(cocos2d::ui::EditBox::InputMode::NUMERIC);
    
    importCompleteBtn=Button::create();
    importCompleteBtn->loadTextures("import_complete_info.png", "import_complete_info.png");
    importCompleteBtn->setPosition(Vec2(visibleSize.width/2, 500));
    importCompleteBtn->setScale(0.87);
    importCompleteBtn->addTouchEventListener([&](Ref* sender, cocos2d::ui::Widget::TouchEventType type){ switch (type){
        case ui::Widget::TouchEventType::BEGAN: break;
        case ui::Widget::TouchEventType::ENDED:{
            //跳转住院病历页面
        }
        default:
            break;
    }
    });
    importCompleteBtn->setVisible(false);
    this->addChild(importCompleteBtn);
    
    return true;
}
EditBox*  SetTemporaryRoom::createBasicData(Sprite* bkView,Vec2 point,string name1,string name2){
    auto visibleSize=Director::getInstance()->getVisibleSize();
    Vec2 origin=Director::getInstance()->getVisibleOrigin();
    auto userName = Label::createWithSystemFont(name1,"Arial",35,Size(200,50),TextHAlignment::LEFT,TextVAlignment::BOTTOM);
    userName->setPosition(point);
    userName->setTextColor(Color4B(91, 144, 229, 255));
    userName->setAnchorPoint(Vec2(0, 0));
    bkView->addChild(userName);

    auto   textFieldUser = EditBox::create(Size(400,50), Scale9Sprite::create("alpha.png"));
    textFieldUser->setTextHorizontalAlignment(TextHAlignment::LEFT);
    textFieldUser->setAnchorPoint(Vec2(0, 0));
    textFieldUser->setPosition(Vec2(180,point.y));
    textFieldUser->setFontName("Arial");
    textFieldUser->setFontSize(35);
    textFieldUser->setPlaceholderFontSize(35);
    textFieldUser->setFontColor(Color3B::BLACK);
    string placeHolder=name2;
    textFieldUser->setPlaceHolder(placeHolder.c_str());
    textFieldUser->setPlaceholderFontColor(Color3B(240, 240, 240));
    textFieldUser->setReturnType(EditBox::KeyboardReturnType::DONE);
    textFieldUser->setInputMode(cocos2d::ui::EditBox::InputMode::SINGLE_LINE);//可以输入任何，但是不包括换行
    bkView->addChild(textFieldUser);
    
    auto lineV=Sprite::create("userInfo_line.png");
    lineV->setPosition(Vec2(51, point.y-11));
    lineV->setAnchorPoint(Vec2(0, 0));
    lineV->setScaleX(0.85);
    bkView->addChild(lineV);
    
    return textFieldUser;
}

void SetTemporaryRoom::eventCallBack(Ref* pSender,cocos2d::ui::TextField::EventType type)
{
    switch (type){
            
        case cocos2d::ui::TextField::EventType::INSERT_TEXT:
            CCLOG("INSERT_TEXT");
            
            break;
        case cocos2d::ui::TextField::EventType::DELETE_BACKWARD:
            
            CCLOG("DELETE_BACKWARD");
        case cocos2d::ui::TextField::EventType::DETACH_WITH_IME:
            
            CCLOG("DETACH_WITH_IME");
            
            break;
            
    }
}

#pragma -新建临时床位
void SetTemporaryRoom::pushDataToNetWork(){
    NetWorkManger* netManeger =NetWorkManger::sharedWorkManger();
    char memberUrl[1000]={0};
    sprintf(memberUrl,"doctorId=%s&bedNo=%s&hospitalName=%s&buildingName=%s&floorName=%s",UserDefault::getInstance()->getStringForKey("id").c_str(),textfieldBed->getText(),textfieldHospital->getText(),textfieldBuilding->getText(),textfieldFloor->getText());
    char* url=memberUrl;
    string memberURL="http://czapi.looper.pro/web/createBed";
    netManeger->postHttpRequest(memberURL,CC_CALLBACK_2(SetTemporaryRoom::onHttpRequestCompleted, this),url);
}

void SetTemporaryRoom::onHttpRequestCompleted(HttpClient* sender, HttpResponse* response)
{
    auto visibleSize=Director::getInstance()->getVisibleSize();
    if (!response)
    {
        return;
    }
    std::vector<char> *data = response->getResponseData();
    std::string recieveData;
    recieveData.assign(data->begin(), data->end());
    
     rapidjson::Document Jsondata;
    
    Jsondata.Parse<rapidjson::kParseDefaultFlags>(recieveData.c_str());
    
    if (Jsondata.HasParseError()) {
        
        return;
    }
    if(Jsondata.HasMember("status")){
        if (Jsondata["status"].GetInt()==0) {
          Director::getInstance()->popScene();
        }
        
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        Jsondata.Accept(writer);
        CCLOG("%s", buffer.GetString());
    }
}
