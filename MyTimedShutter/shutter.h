/*
  shutter.h - Library for Controlling Shutters by knowing Time to be closed completly.
  Created by Bernhard Schubel, Mary 5, 2018.
  Released into the public domain.
*/
#ifndef shutter_h
#define shutter_h

#include "Arduino.h"
#include "../MySensors/core/MyMessage.h"
#include "../MySensors/core/MySensorsCore.h"

#if !defined(Number_of_Covers)
#define Number_of_Covers 16
#endif
/*#ifndef MY_NODE_ID
#define MY_NODE_ID 12
#endif
#ifndef MY_RADIO_NRF24
#define MY_RADIO_NRF24
#endif
#ifndef MY_RF24_DATARATE
#define MY_RF24_DATARATE RF24_1MBPS
#endif
#ifndef MY_RF24_PA_LEVEL
#endif
#define MY_RF24_PA_LEVEL RF24_PA_LOW
#ifndef MY_RF24_CHANNEL
#define MY_RF24_CHANNEL 5
#endif
*/

#ifndef up_button
#define up_button A0
#endif
#ifndef stop_button
#define stop_button A1
#endif
#ifndef down_button
#define down_button A2
#endif
#ifndef previous_cover
#define previous_cover A3
#endif
#ifndef group_cover
#define group_cover A5
#endif
#ifndef next_cover
#define next_cover A4
#endif
#ifndef Number_of_Covers
#define Number_of_Covers 16
#endif
#ifndef Number_of_Doors
#define Number_of_Doors 5
#endif
#ifndef door_downtime
#define door_downtime 40
#endif
#ifndef window_downtime
#define window_downtime 20
#endif
#ifndef button_press
#define button_press 1000
#endif
#ifndef shutter_change
#define shutter_change 50
#endif
#ifndef cover_state_msg_percent
#define cover_state_msg_percent 10
#endif
#ifndef update_timer
#define update_timer 120000
#endif

struct isDoorarray{
  int x[];  
};
class TimedCover {
  public:
//Test    
    TimedCover();
    void TimedCoverInz(int shutterId,int downTime);
    bool Check();
    bool OnlyMsg();
    void Do();
    void setDowntime(unsigned int downtime);
    void Status(); // give back the current status
    void msgStop();
    void msgUp();
    void msgDown();
    void msgPercent();
    void newState(int Percent);  
  private:
   void upCover();
   void downCover();
   void stopCover();
   void startMoving();
   unsigned long _starttime;
   unsigned long _endtime;
   int _goalPercent;
   int _currentPercent; 
   int _countStatusUpdate;
   int _countStatusUpdate_msg;
   int _CoverId;
   long _downTime;
   long _downTime1Percent;
   int _direction; //1 is up 2 is down
   bool _moving = false;   
   bool _changed = false;
   long _lastUpdate = 0;
   
};

class RemoteControlCover
{
  public:
    RemoteControlCover();
    void Check();
    void upCover(int CoverId);
    void downCover(int CoverId);
    void stopCover(int CoverId);
    void percentCover(int CoverId,int CoverPercent);
    void isDoorp(int CoverId);        
  private:
    int _currentCover = 1;       
    TimedCover _covers[Number_of_Covers+1];
    void setCover(int CoverId);         
};


#endif
