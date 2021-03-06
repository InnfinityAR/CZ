//
//  SpineTendernessScene.cpp
//  Chuangzheng-mobile
//
//  Created by 工作 on 2017/12/21.
//

#include "SpineTendernessScene.hpp"
#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"
#include <iostream>
#include "NetWrokMangerData.hpp"
using namespace cocos2d::ui;
using namespace std;
USING_NS_CC;
Scene *SpineTendernessScene::createScene(){
    return SpineTendernessScene::create();
}
bool judgeTouchMoveWith(Vec2 origin,cocos2d::Ref * ref);

//用于储存MenuItemToggle
std::shared_ptr<Vector<MenuItemToggle*>> spintVector;

bool SpineTendernessScene::init(){
    if (!Scene::init()) {
        return false;
    }
    Size visibleSize=Director::getInstance()->getVisibleSize();
    auto bkView=Sprite::create("bk_SpineTenderness.png");
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
    
    auto sureBtn=Button::create();
    sureBtn->loadTextures("btn_step4_sure.png", "btn_step4_sure.png");
    sureBtn->setPosition(Vec2(visibleSize.width-80, visibleSize.height-85));
    sureBtn->addTouchEventListener([&](Ref* sender, cocos2d::ui::Widget::TouchEventType type){ switch (type){
        case ui::Widget::TouchEventType::BEGAN: break;
        case ui::Widget::TouchEventType::ENDED:
            pushDataToNetWork();
            
        default:
            break;
    }
    });
    this->addChild(sureBtn);
    
//注：使用Vector之前需要先申请vector空间
    spintVector=make_shared<Vector<MenuItemToggle*>>();
    
    
    auto cLB=Label::createWithSystemFont("C：","Arial",50,Size(200,60),TextHAlignment::LEFT,TextVAlignment::CENTER);
    cLB->setPosition(Vec2(50, 920));
    cLB->setTextColor(Color4B(91,144,230, 255));
    cLB->setAnchorPoint(Vec2(0, 0));
    this->addChild(cLB);
    for (int i=0; i<7; i++){
    createRectButton(Vec2(50, 830),i,i);
    }
    
    auto tLB=Label::createWithSystemFont("T：","Arial",50,Size(200,60),TextHAlignment::LEFT,TextVAlignment::CENTER);
    tLB->setPosition(Vec2(50, 660));
    tLB->setTextColor(Color4B(91,144,230, 255));
    tLB->setAnchorPoint(Vec2(0, 0));
    this->addChild(tLB);
    for (int i=7; i<19; i++){
        createRectButton(Vec2(50, 570),i-7,i);
    }
    
    auto lLB=Label::createWithSystemFont("L：","Arial",50,Size(200,60),TextHAlignment::LEFT,TextVAlignment::CENTER);
    lLB->setPosition(Vec2(50, 310));
    lLB->setTextColor(Color4B(91,144,230, 255));
    lLB->setAnchorPoint(Vec2(0, 0));
    this->addChild(lLB);
    for (int i=19; i<24; i++){
        createRectButton(Vec2(50, 220),i-19,i);
    }
    
    
    
//添加触控监听
    EventDispatcher* eventDispatcher = Director::getInstance()->getEventDispatcher();
    auto listen = EventListenerTouchAllAtOnce::create();
    listen->onTouchesBegan = CC_CALLBACK_2(SpineTendernessScene::onTouchesBegan,this);
    listen->onTouchesMoved = CC_CALLBACK_2(SpineTendernessScene::onTouchesMoved,this);
    listen->onTouchesEnded = CC_CALLBACK_2(SpineTendernessScene::onTouchesEnded,this);
    listen->onTouchesCancelled = CC_CALLBACK_2(SpineTendernessScene::onTouchesCancelled,this);
    eventDispatcher->addEventListenerWithSceneGraphPriority(listen,this);
    return true;
}

Label*   SpineTendernessScene::createRectButton(Vec2 point,int index,int tag){
    Size visibleSize=Director::getInstance()->getVisibleSize();
    //设置选中和未选中的弹出框button
    auto start = MenuItemImage::create("btn_spine_white.png","");  //显示为on
    auto stop = MenuItemImage::create("btn_spine_blue.png","");  //显示为off
    auto toggle = MenuItemToggle::createWithCallback(
                                                     CC_CALLBACK_1(SpineTendernessScene::menuSelectCallback,this),
                                                     start,
                                                     stop,
                                                     NULL);
    toggle->setPosition(point); //设置坐标在屏幕居中
    toggle->setTag(tag+100);
    toggle->setScale(0.87);
    toggle->setAnchorPoint(Vec2(0, 0));
    auto box = Menu::create(toggle,NULL);
    box->setPosition(Point::ZERO);
    this->addChild(box);
    
    spintVector->pushBack(toggle);
#pragma-按钮的排列和换行
    Size imageSize=toggle->getContentSize()*0.87;
    int width=point.x+(imageSize.width+2)*index+imageSize.width;
    if (width<visibleSize.width) {
        toggle->setPosition(Vec2(point.x+(imageSize.width+2)*index, point.y));
        //                 log("width:%d,height:%f i的值%d,屏幕宽度%f",154+150*(i%3),320-height-(int)(width/(int)visibleSize.width)*150,i,visibleSize.width);
    }else{
        toggle->setPosition(Vec2(point.x+(imageSize.width+2)*(index%4),point.y-(   (int)( (width-point.x)/((imageSize.width+2)*4) )*(imageSize.height+2)   ) ));
        //       log("width:%f,height:%f i的值%d,总长度:%d",50+(imageSize.width+2)*(i%4),((int)((width-50)/((imageSize.width+2)*4))*imageSize.height),i,width);
    }
//同时需要修改button上面的文字位置
    auto endLB=Label::createWithSystemFont(to_string(index+1),"Arial",40,Size(toggle->getContentSize()*0.87),TextHAlignment::CENTER,TextVAlignment::CENTER);
    endLB->setPosition(toggle->getPosition());
    endLB->setTextColor(Color4B(0, 0, 0, 220));
    endLB->setColor(Color3B::WHITE);
    endLB->setTag(toggle->getTag()+100);
    endLB->setAnchorPoint(Vec2(0, 0));
    this->addChild(endLB);
    return endLB;
}

void SpineTendernessScene::menuSelectCallback(Ref* pSender)
{
    Size visibleSize=Director::getInstance()->getVisibleSize();
    MenuItemToggle* item=(MenuItemToggle*)pSender;
    int  index=item->getSelectedIndex();
    int tag= item->getTag();
    log("index:%d",tag);
    Label *num=(Label*)this->getChildByTag(tag+100);
    if (index==1) {
            num->setTextColor(Color4B::WHITE);
        }else{
            num->setTextColor(Color4B::BLACK);
        }
}


//触摸事件开始，手指按下时
void SpineTendernessScene::onTouchesBegan(const std::vector<Touch*>& touches, cocos2d::Event  *event)
{
    for(auto &item : touches)
    {
        auto touch = item;
        auto pos1 = touch->getLocation();
        auto pos2 = touch->getLocationInView();
        auto pos3 = Director::getInstance()->convertToUI(pos2);
        if (pos1.x-pos3.x<0) {
            log("从右往左滑动");
        }
        if (pos1.y-pos3.y<0) {
             log("从上往下滑动");
        }
//        CheckBox* testBox=(CheckBox*)this->getChildByTag(101);
//        if (judgeTouchMoveWith(pos1, testBox)) {
//            testBox->setSelected(true);
//        }
    }
}
//触摸移动事件，也就是手指在屏幕滑动的过程
void SpineTendernessScene::onTouchesMoved(const std::vector<Touch*>& touches, cocos2d::Event  *event)
{
//    log("TouchTest onTouchesMoved");
    for(auto &item : touches)
    {
        auto touch = item;
        auto pos1 = touch->getLocation();
        auto pos2 = touch->getLocationInView();
        auto pos3 = Director::getInstance()->convertToUI(pos2);
//        log("pos1 x: %f, y: %f",pos1.x,pos1.y);
//        log("pos2 x: %f, y: %f",pos2.x,pos2.y);
//        log("pos2 x: %f, y: %f",pos3.x,pos3.y);
        for (int i=0; i<spintVector->size(); i++) {
            MenuItemToggle *toggle=(MenuItemToggle*)spintVector->at(i);
            log("%d",toggle->getTag());
            Label *num=(Label*)this->getChildByTag(toggle->getTag()+100);
            if (judgeTouchMoveWith(pos1, toggle)) {
                toggle->setSelectedIndex(1);
                num->setTextColor(Color4B::WHITE);
            }
        }
    }
}
//触摸事件结束，也就是手指松开时
void SpineTendernessScene::onTouchesEnded(const std::vector<Touch*>& touches, cocos2d::Event  *event)
{
    log("TouchTest onTouchesEnded");
}
//打断触摸事件，一般是系统层级的消息，如来电话，触摸事件就会被打断
void SpineTendernessScene::onTouchesCancelled(const std::vector<Touch*>& touches, cocos2d::Event  *event)
{
    log("TouchTest onTouchesCancelled");
}
#pragma -touchMove路径上是否触摸的sprite等
bool judgeTouchMoveWith(Vec2 origin,cocos2d::Ref * ref){
    MenuItemToggle* item = (MenuItemToggle*)ref;
    if (origin.x>=item->getPosition().x&&origin.x<=item->getContentSize().width+item->getPosition().x&&origin.y>=item->getPosition().y&&origin.y<=item->getContentSize().height+item->getPosition().y) {
        return true;
    }
    return false;
}


std::string SpineTendernessScene::getJsonData(int type)
{
    rapidjson::Document document;
    document.SetObject();
    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
    if (type==0) {
        rapidjson::Value array(rapidjson::kArrayType);
        rapidjson::Value array2(rapidjson::kArrayType);
        rapidjson::Value array3(rapidjson::kArrayType);
       for (int i=0; i<spintVector->size(); i++) {
            MenuItemToggle* item=(MenuItemToggle*)spintVector->at(i);
            int  index=item->getSelectedIndex();
            int tag= item->getTag();
            if (tag>=100&&tag<107) {
                if (index==1) {
                    array.PushBack(rapidjson::Value(to_string(tag-99).c_str(), allocator),allocator);
                }
            }
           if (tag>=107&&tag<119) {
               if (index==1) {
                   array2.PushBack(rapidjson::Value(to_string(tag-106).c_str(), allocator),allocator);
               }
           }
           if (tag>=119&&tag<124) {
               if (index==1) {
                   array3.PushBack(rapidjson::Value(to_string(tag-118).c_str(), allocator),allocator);
               }
           }
        }
        document.AddMember("C", array, allocator);
        document.AddMember("T", array2, allocator);
        document.AddMember("L", array3, allocator);
    }
    
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    document.Accept(writer);
    
    log("buffer:%s",buffer.GetString());
    return buffer.GetString();
}

#pragma-用于加载网络数据
void SpineTendernessScene::pushDataToNetWork(){
    NetWorkManger* netManeger =NetWorkManger::sharedWorkManger();
    char jsonStr[1000]={0};
    sprintf(jsonStr,"%s",getJsonData(0).c_str());
    char*json=jsonStr;
    char memberUrl[1000]={0};
    sprintf(memberUrl,"recordId=%s&keys=%s&answers=%s",UserDefault::getInstance()->getStringForKey("caseId").c_str(),"tz_jtyt",json);
    char* url=memberUrl;
    string memberURL="http://czapi.looper.pro/web/updateMedicalRecords";
    netManeger->postHttpRequest(memberURL,CC_CALLBACK_2(SpineTendernessScene::onHttpRequestCompleted, this),url);
}

void SpineTendernessScene::onHttpRequestCompleted(HttpClient* sender, HttpResponse* response)
{
    auto visibleSize=Director::getInstance()->getVisibleSize();
    if (!response)
    {
        return;
    }
    if(!response -> isSucceed()){
        log("response failed");
        log("error buffer: %s", response -> getErrorBuffer());
        return;
    }
    std::vector<char> *data = response->getResponseData();
    std::string recieveData;
    recieveData.assign(data->begin(), data->end());
    
    rapidjson::Document jsondata;
    
    jsondata.Parse<rapidjson::kParseDefaultFlags>(recieveData.c_str());
    
    if (jsondata.HasParseError()) {
        
        return;
    }
    if(jsondata.HasMember("status")){
        if (jsondata["status"].GetInt()==0) {
            Director::getInstance()->popScene();
        }
        
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        jsondata.Accept(writer);
        CCLOG("%s", buffer.GetString());
    }
}











#pragma-按钮的排列和换行
//        spintVector->pushBack(imageBtn);
//        Size imageSize=imageBtn->getContentSize()*0.87;
//        int width=50+(imageSize.width+2)*i+imageSize.width;
//        if (width<visibleSize.width) {
//            imageBtn->setPosition(Vec2(50+(imageSize.width+2)*i, 850));
//            //                 log("width:%d,height:%f i的值%d,屏幕宽度%f",154+150*(i%3),320-height-(int)(width/(int)visibleSize.width)*150,i,visibleSize.width);
//        }else{
//            imageBtn->setPosition(Vec2(50+(imageSize.width+2)*(i%4),850-((int)((width-50)/((imageSize.width+2)*4))*imageSize.height)));
//           //       log("width:%f,height:%f i的值%d,总长度:%d",50+(imageSize.width+2)*(i%4),((int)((width-50)/((imageSize.width+2)*4))*imageSize.height),i,width);
//        }
////同时需要修改button上面的文字位置
//        auto endLB=Label::createWithSystemFont(to_string(i+1),"Arial",40,Size(imageBtn->getContentSize()*0.87),TextHAlignment::CENTER,TextVAlignment::CENTER);
//        endLB->setPosition(imageBtn->getPosition());
//        endLB->setTextColor(Color4B(0, 0, 0, 255));
//        endLB->setColor(Color3B::WHITE);
//        endLB->setTag(imageBtn->getTag()+100);
//        endLB->setAnchorPoint(Vec2(0, 0));
//        this->addChild(endLB);
