# HomeData
Contains a header file with all of the #defines required for my home monitoring setup.
# HomeData\_Gateway
Simple gateway taken from MySensors.  It runs on an Arduino Uno V3, using the NRF24L01+LNA+PA board, and the Arduino's onboard 3v3 regulator.
# HomeData\_Sensor
Sensor device.  These typically run on Arduino Pro Minis, and include an LM1117 3v3 regulator for the radio.  There are #defines to turn the repeater functionality on or off.  Some of my sensors are just NRF24L01 boards with PCB antennas.  Those have repeater functionality turned off.  The other devices have the NRF24L01+LNA+PA boards with a sleeve monopole antenna, and have repeater functionality on.  Those are deployed along the long axis of my house, giving me a 3-hop path from the planned instrument shelter in the garden to the gateway in my office.  All of the sensor boards currently running have a DHT22 temp/humidity sensor.  That will change slightly when I build the collection system for the garden, since it will be monitoring six DHT22s, plus a soil temp sensor, and possible other things.  That might get broken out into its own sketch.
