# TimedShutter
Control remoteControl for shutters via Mysensor 
This Project gives the possbility to Controll Shutter via vendors Remotcontroll (because of RollingCode).

Goal:
 - Shutter status in Percent should be adjustable
 - The remote control selects the right shutter by "pressing" the next/before button

You have to know:
 - Time in seconds to close a door/window shutter
 - The buttons on the remot needs to be set on GDN or VCC

Working Description:

Ever Shutter has a known status by the controller and if the controller gets the Signal lower the shutter it will navigate on the remote to the selected shutter and start down and will stop after the time to set the shutter to the correct status.
Example: 
 - Shutter 2 and 3 need to be closed 30 seconds
 - Shutter 3 should be closed 50%
 - Shutter 2 should be closed complete (is interpreted as 120%)
 - What is controler doing:
 - press 3 times the next button on the remote
 - press start lower shutter 3
 - press once the last button
 - press start shutter 2
 - after 15 seconds press next button
 - press stop button
 - send msg to gatway with status stoped and reached percent
 - after 36 seconds press last button
 - press stop button
 - send msg to gateway with status stoped and reched 100 %
