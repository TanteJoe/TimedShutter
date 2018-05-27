/*
  Shutter.cpp - Library for Controlling Shutters by knowing Time to be closed completly.
  Created by Bernhard Schubel, Mary 5, 2018.
  Released into the public domain.
*/
/*

*/
#include "Arduino.h"
#include "shutter.h"

RemoteControlCover::RemoteControlCover() {

  for (int i = 0; i < Number_of_Covers; i++) {
    _covers[i].TimedCoverInz(i + 1 , window_downtime );
  }
  for (int i = 0; i < Number_of_Doors; i++) {
      if (is_door[i]-1<Number_of_Covers) {
        isDoor(is_door[i]);                  
    }
     else {
       Serial.print("Door to big:");
        Serial.println(is_door[i]);  
     }
  }
  

}
void RemoteControlCover::Check() {

  for (int i = 0; i < Number_of_Covers; i++) {
    if (_covers[i].Check() == 1 ) {
        if ( _covers[i].OnlyMsg() != 1 ) {
      setCover(i + 1);
        }
      _covers[i].Do();
    }
  }

}
void RemoteControlCover::upCover(int CoverId) {
    _covers[CoverId - 1].newState(-20);
}
void RemoteControlCover::downCover(int CoverId) {
  
  _covers[CoverId - 1].newState(120);


}
void RemoteControlCover::stopCover(int CoverId) {
  
  _covers[CoverId - 1].newState(500);

}
void RemoteControlCover::percentCover(int CoverId, int CoverPercent) {
  _covers[CoverId - 1].newState(CoverPercent-1);
}
void RemoteControlCover::setCover(int CoverId) {
  int tmp_state;
  tmp_state = CoverId - _currentCover;
  
  if ( tmp_state > 0 ) {
    for (int sensor = 0; sensor < tmp_state; sensor++) {
      delay(shutter_change);
      digitalWrite(next_cover, LOW);
      delay(shutter_change);
      digitalWrite(next_cover, HIGH);
      _currentCover = _currentCover + 1;
    }
  }
  if ( tmp_state < 0 ) {
    for (int sensor = 0; sensor < (tmp_state * -1); sensor++) {
      delay(shutter_change);
      digitalWrite(previous_cover, LOW);
      delay(shutter_change);
      digitalWrite(previous_cover, HIGH);
      _currentCover = _currentCover - 1;
    }
  }
}
void RemoteControlCover::isDoor(int CoverId) {  
  _covers[CoverId - 1].setDowntime(door_downtime);
  
}
TimedCover::TimedCover() {
 _goalPercent = 0;
  _currentPercent = 0;
  setDowntime(0);
  _downTime1Percent = 0;
  _direction = 0; //1 is up 2 is down
  _moving = 0;
  _changed = 0;

}
void TimedCover::TimedCoverInz(int shutterId, int downTime) {
  _CoverId=shutterId;
  setDowntime(downTime);

}
bool TimedCover::Check() {
  bool result;
  if ( (_changed == 1) || (_moving == 1) ) {
    result = 1;
  }
  else {
      if ( (_changed == 0 ) && (_moving == 0 ) && (_currentPercent>100) ) {
          _currentPercent=100;
      }
      if ( (_changed == 0 ) && (_moving == 0 ) && (_currentPercent<0) ) {
          _currentPercent=0;
      }
    result = 0;
  }  
  if (_lastUpdate <= millis()) {
      Status();
      _lastUpdate = millis() + update_timer;
  }
  return result;
}
bool TimedCover::OnlyMsg() {
  bool result;
  result = 0;
  if ((_changed == 0) && (_moving == 1) && (millis() <= _endtime) ) {    
      result=1;        
  }  
  return result;
}
void TimedCover::Do() {
  if ((_changed == 1)) {
    if (_goalPercent > _currentPercent) {
      downCover();
    }
    if (_goalPercent < _currentPercent) {
      upCover();
    }
    if (_goalPercent == _currentPercent) {
      stopCover();
    }
  }  
  else if ((_changed == 0) && (_moving == 1) ) {
    if (millis() <= _endtime) {
      if (millis() > (_starttime + (_downTime1Percent * _countStatusUpdate * cover_state_msg_percent))) {        
        _currentPercent = _currentPercent + (cover_state_msg_percent * _direction);        
        _countStatusUpdate = _countStatusUpdate + 1;
        msgPercent();
      }
    }
    else {
      stopCover();
    }    
  }
}
void TimedCover::setDowntime(unsigned int downtime) {
  _downTime = downtime * 1000;
  _downTime1Percent = downtime * 10;   
}
void TimedCover::startMoving() {
  _starttime = millis();
  _endtime = millis() + (((_goalPercent - _currentPercent) * _direction) * _downTime1Percent);
  _changed = 0;
  _moving = 1;
  _countStatusUpdate = 1;
}
void TimedCover::upCover() {  
  digitalWrite(up_button, LOW);
  delay(button_press);
  digitalWrite(up_button, HIGH);
  _direction = -1;
  startMoving();
  msgUp();
}
void TimedCover::downCover() {
  digitalWrite(down_button, LOW);
  delay(button_press);
  digitalWrite(down_button, HIGH);
  _direction = 1;
  startMoving();
  msgDown();
}
void TimedCover::stopCover() {
  digitalWrite(stop_button, LOW);
  delay(button_press);
  digitalWrite(stop_button, HIGH);
  _starttime = 0;
  _endtime = 0;
  _changed = 0;
  _moving = 0;
  msgStop();
  msgPercent();
}
void TimedCover::Status(){
    if ((_changed != 1) && (_moving != 1)){
       msgStop();
       msgPercent(); 
    }
    
}
void TimedCover::msgDown(){  
  MyMessage msg(_CoverId, V_DOWN);  
  send(msg.set(1));    
}
void TimedCover::msgUp(){  
  MyMessage msg(_CoverId, V_UP);  
  send(msg.set(1));         
}
void TimedCover::msgStop(){ 
  MyMessage msg(_CoverId, V_STOP);   
  send(msg.set(1));        
}
void TimedCover::msgPercent(){
int tmp_percent;
  MyMessage msg(_CoverId, V_PERCENTAGE);
  tmp_percent=_currentPercent;
  if ( _currentPercent > 100) {
      tmp_percent=100;
  }
  if ( _currentPercent < 0) {
      tmp_percent=0;
  }
  send(msg.set(tmp_percent));         
}
void TimedCover::newState(int Percent) {  
  if ( Percent != 500 ) {
    _goalPercent = Percent;
  }
  else {
    _goalPercent = _currentPercent;
    _endtime = millis();
  }
  _changed = 1;  
}


