#define MY_DEBUG
#if !defined(MY_NODE_ID)
#define MY_NODE_ID 12
#endif
#if !defined(MY_RADIO_NRF24)
#define MY_RADIO_NRF24
#endif
#if !defined(MY_RF24_DATARATE)
#define MY_RF24_DATARATE RF24_2MBPS
#endif
#if !defined(MY_RF24_PA_LEVEL)
#endif
#define MY_RF24_PA_LEVEL RF24_PA_LOW
#if !defined(MY_RF24_CHANNEL)
#define MY_RF24_CHANNEL 5
#endif
#define stop_button A1
#define down_button A2

#define previous_cover A3
#define group_cover A5
#define next_cover A4

#define Number_of_Covers 12

#define door_downtime 27
#define window_downtime 20

#define button_press 1000
#define shutter_change 50

#define rf_power 6
#define cover_state_msg_percent 10
#define update_timer 120000

#define Number_of_Doors 5
#define isDoorarray is_door {1,2,3,4,12};
static const uint8_t analog_pins[] = {A0, A1, A2, A3, A4, A5, A6};


#include <MySensors.h>
#include <shutter.h>

RemoteControlCover remote;

void presentation() {
  // Send the sketch version information to the gateway and Controller
  sendSketchInfo("Roladensteuerung", "2.0");

  for (int cover = 1; cover <= Number_of_Covers; cover++) {
    // Register all sensors to gw (they will be created as child devices)
    present(cover, S_COVER);
    //pimatic don't recognise a Dimmer for covers so it provides also a Dimmer
    present(cover, S_DIMMER);
  }
}
void before() {
  remote= RemoteControlCover();
}
void setup() {
  for (int outp = 0 ; outp <= 6; outp++ ) {
    digitalWrite(analog_pins[outp], HIGH);
    pinMode(analog_pins[outp], OUTPUT);
  }
  
  pinMode(rf_power, OUTPUT);
  //Power Remotecontroll on
  digitalWrite(rf_power, HIGH);
  delay(button_press);
  //Set Remotecontroll in singel mode after boot
  digitalWrite(group_cover, LOW);
  delay(shutter_change);
  digitalWrite(group_cover, HIGH);
  //RemoteControlCover loc_remote[]
  // put your setup code here, to run once:
  //loc_remote[Number_of_Covers];

  






}

void loop() {

  remote.Check();

}
void receive(const MyMessage &message) {
  int sid = message.sensor;

  if (message.type == V_UP) {
    remote.upCover(sid);
  }
  if (message.type == V_DOWN) {
    remote.downCover(sid);
  }
  if (message.type == V_STOP) {
    remote.stopCover(sid);
  }
  if (message.type == V_PERCENTAGE) {
    remote.percentCover(sid,message.getInt());
  }
}
