# Description 

This firmware drives a RGB LED ring and a capacitive touch keyboard using the WEMOS D1 mini micro controller. 
It is meant to provide light below a CNC milling motor.
However, it can be also used sa decorative light.   
 
# Features

The device can be either in light or WiFi mode. 
Not both at the same time.

+ Multiple light modes
  1. multiple color modes
     1. R, G, B, W, Rainbow, Theater Chase ... 
  1. on/off and brightness adjustment
  1. angle based mode
    1. arc length can be in-/decreased
    2. arc can be rotated 
 
+ Can be accessed by WiFi  
  1. acts as WiFi access-point until WiFi is configured
  1. configuration is done via HTTP 
  1. once connection to AP is established the WiFi AP is deactivated
  1. in WiFi mode this is possible
     1. firmware upload & flash
     1. get device status

+ Keyboard Layout

|   |   |   |
|:-:|:-:|:-:|
| W | ↑ | R |
| ← | ↲ | → |
| G | ↓ | B |
| ⏻| ☀ | ☼ |

corresponds to

| symbol | on press | on double press | on long press | on extra long gpress |
| ---    | ---      | ---             | ---           | ---                  |
| W | white            | max brightness | full circle | - |
| R | red              | max brightness | full circle | - |
| G | green            | max brightness | full circle | - |
| B | blue             | max brightness | full circle | - |
| ↑ | increase arc     | - | - | - |
| ↓ | decrease arc     | - | - | - |
| ← | turn arc left    | - | - | - |
| → | turn arc right   | - | - | - |
|↲  | next cpolor mode | - | - | switch to WiFi mode |
| ⏻| lights on/off    | - | - | - |
| ☀ | darker           | - | - | - |
| ☼ | brighter         | - | - | - |

# Hardware

 + MPR121 touch keyboard
 + Adafruit NeoPixel (or similar RGB LED ring/strip)
 
## Wiring

| WEMOS D1 mini | Keyboard | LED |
|---            | ---      |---  |
| D1 (SCL)      | SCL      | SCL |
| D2 (SDA)      | SDA      | SDA |
| 3V3 (3.3V)    | VCC      | -   |
| 5V (5V)       | -        | -   |
| -             | -        | VCC (external 5V) |
| G (ground)    | GND      | GND |
