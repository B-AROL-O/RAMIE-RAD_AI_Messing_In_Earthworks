# Wheeled Platform

This was lots of work

## SPECIFICATIONS

- Latte Panda Lite Board
- 2S2P battery holder
- Ping Pong pivot ball
- HS422 wheeled servo motors
- AT324 LCD motor controller

## Pivot Ball

I did a fancy geometry to hold a 40mm ping pong ball as pivot. The three petals cut into the base and serve as springs to pull out the ball and hold it gently letting it rotate. All with the heights nailed down to get it leveled.

![](/Mechanical/Wheeled%20Platform%20OpenSCAD/Images/2025-11-06T1304%20Pivot%20SCAD.png)

## 2S2P Inverted Quad 18650 Holder

Another fancy geometry to hold four batteries with parametric size inverted with the tollerances nailed down so it stays when inverted but can be pulled out. Needs support to print inverted.

I considered various layouts

For a Raspberry Pi dual battery design I can shrink it enormously, that's what I started with, but then pivoted to Latte Panda Mu Board

This version has two banks of two batteries, but ultimately is a lot wider.

![](/Mechanical/Wheeled%20Platform%20OpenSCAD/Images/2025-11-06-T1357_2X2S1P.png)

Ultimately I decided to do it along the width in between wheels and pivot. There is a variation where the batteries stays lengthwise but it makes it much bigger

![](/Mechanical/Wheeled%20Platform%20OpenSCAD/Images/2025-11-08-T0952_M2S2P_Middle.png)

Then I designed an inverted battery pack to load from below. Much harder to design but worth it.

![](/Mechanical/Wheeled%20Platform%20OpenSCAD/Images/2025-11-08-T1148_Below_Loading.png)

## HS422 Servo Holder

I have pillars serving as servo holders, I did all the parameters to nail down tollerances and height to keep level, and merged with the SBC pillars.

![](/Mechanical/Wheeled%20Platform%20OpenSCAD/Images/2025-08-27T1552_HS422_pillar.png)

## AT324 Board

Constructing a sloped wall to support the board

![](/Mechanical/Wheeled%20Platform%20OpenSCAD/Images/)

## Final Assembly

I did a bath tub style holder with all the components and it turned out okay

![](/Mechanical/Wheeled%20Platform%20OpenSCAD/Images/2025-11-08-T1352_Final_Assembly.png)

Generated the STL and Printed with my Creality K2 with auto loader. I decided to use PETG transparent filament and it came out great! The tollerances are PERFECT.

![](/Mechanical/Wheeled%20Platform%20OpenSCAD/Images/2025-11-08-T1437_Slicing.png)

### TOP VIEW

![](/Mechanical/Wheeled%20Platform%20OpenSCAD/Images/2025-11-08-T2104_assembled_powered_above.jpg)

### BOTTOM VIEW

![](/Mechanical/Wheeled%20Platform%20OpenSCAD/Images/2025-11-08-T2104_assembled_powered_below.jpg)