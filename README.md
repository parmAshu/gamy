# Gamy - a wireless quiz show controller

**Gamy** is the system when you are organizing a quiz event. The system consists of wireless buttons used by participants/teams. There is a master button that is used by the quiz master. One system can have upto 10 button devices (upto 10 teams.) However, there can be only one master button. There is another type of device that is used in a gamy system; it's called the indication device. These devices are used to indicate the team that pressed the button first. There can be several indication devices in one gamy system.

## Working explained
---
1. A button is pressed. The device sends a packet (unicast) to the master device.
2. The master device detects the packet, records the team code, broadcasts this team code and goes to wait state (waits for its button to be pressed.)
3. The indication device picks up the broadcast and manipulates its outputs to display the same.
