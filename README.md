# DIY Digital Dice (d10000)

​	This is an Arduino based DIY digital dice that, as standard, can generate a random number between 0 and 4 /6 /8 /10 /12 /20 /100 /1000 *or* 10000 along with a coin flip between heads and tails (1/2.) Using a gyroscope the device can sense its rotational acceleration around any one of the three axis and generate a random number when a specified threshold is reached. Originally created to use along side with an extended 'chaos magic' in DnD 5e. Despite this the dice can function as a physical random number generator for any situation with little modification to the Arduino code if wanted.



### Prerequisites

###### Software:

- Access to a 3d printer (... *cm* x ... *cm* x ... *cm*  **min print size**) and slicer
- 3D modelling program *(Fusion 360)*
- Arduino IDE *(or other Arduino compatible IDE's)*
  - LiquidCrystal.h
  - Wire.h
- EDA software Autodesk 'Eagle' *(alternatively 'Easy EDA')*

###### Components [Standard]:

- Arduino Nano
- 8x2 liquid crystal display
- MPU-6050 Gyroscope
- 2x 10k Resistor *THT*
- 1x 10k potentiometer 
- 2x 100nF non polarized capacitor (5.8mm *THT*) 
- 1x Speaker
- 2x Momentary Buttons <u>[hole mountable]</u> 
- 1x Toggle Switch / Button <u>[hole mountable]</u> 
- 1x 1s rechargeable Lithium Polymer battery and USB charging circuit 
  *<u>(Easily found from butchering a 5v battery charger)</u>*
- 1x PCB (*see production files*)
- 1x 3D printed shell
- (screws, nuts and washers) ...



### Contributing

All contributions are accepted with no requirements for permission from authors.



### Version

Release 0.1 **[Board Design & Program]** (08.03.2020)



### Authors

- **Sebastian Whitehead** - *Initial work* [*Version 1.0*]

  

### License

This Project is licensed under the MIT License - See the LICENSE.md file for details



### Acknowledgments

-  Dejan - howtomechatronics.com - parts of the Gyroscope code

