# 18650 Battery Holder

This is a design for a robot battery holder


# Battery with PCB protection

Length measured to the tip of the button

| Battery              | Diameter [mm] | Length [mm] | Capacity [mAh] | Note |
|----------------------|---------------|-------------|----------------|------|
| UltraFlre LI IONBRC  |  18           | 680         | 4800           | I think the capacity is a scam|
| RSPRO LI ION PCB     |  18           | 710         | 2600           | |

# Battery Tab 

[RS Component Buy](https://it.rs-online.com/web/p/contatti-per-batterie/3247303)

Size 11x11mm slightly bigger

| Loading     | Length [mm] |
|-------------|-------------|
| Unloaded    | 8 mm        |
| Loaded      | 3 mm        |

# OpenSCAD

Modeling of the parts

## Tab with Spring

![](/Mechanical/Battery%20Holder%2018650%20OpenSCAD/Images/Screenshot%202025-11-02%20112939.png)

## Endcap for the battery holder

OpenSCAD

![](/Mechanical/Battery%20Holder%2018650%20OpenSCAD/Images/Screenshot%202025-11-02%20120854.png)
![](/Mechanical/Battery%20Holder%2018650%20OpenSCAD/Images/Screenshot%202025-11-02%20121316.png)

Printed to test tollerances

![](/Mechanical/Battery%20Holder%2018650%20OpenSCAD/Images/2025-11-02-T1256.jpg)

## Holder 1S1P Single Battery

S mean series, P means parallel

3.7V nominal
4.2V max

![](/Mechanical/Battery%20Holder%2018650%20OpenSCAD/Images/2025-11-02-T1357-S1P1-Spring-Compressed.png)

![](/Mechanical/Battery%20Holder%2018650%20OpenSCAD/Images/2025-11-02-T1400%20Holder%20S1P1.png)

First test, it's close. Fix:
- I did double spring that's not smart, I do have a button contact for the positive tab
- The center wing is too weak, I need to make it longer
- Ticker construction?

![](/Mechanical/Battery%20Holder%2018650%20OpenSCAD/Images/2025-11-02-T_1600%20Test%20Golder%20S1P1.jpg)

![](/Mechanical/Battery%20Holder%2018650%20OpenSCAD/Images/2025-11-02-T1601%20Test%20Golder%20S1P1.jpg)

## Holder 2S1P Dual Battery

![](/Mechanical/Battery%20Holder%2018650%20OpenSCAD/Images/2025-11-06-T1219_scad_2s1p.png)

![](/Mechanical/Battery%20Holder%2018650%20OpenSCAD/Images/2025-11-06-T1209_holder_2s1p.jpg)

## Holder 2S2P Quad Battery

3.7 * 4 = 14.8V nominal
4.2 * 4 = 16.8V max

![](/Mechanical/Battery%20Holder%2018650%20OpenSCAD/Images/2025-11-06-T1600_scad_holder_2s2p.png)

## Holder 2S2P Quad Battery Inverted

This is a fancy design, I did work to do the rounding to have an inverted holder.

Flanges have perfect tollerance.

TODO: I could tighten the spriing a little more, and I need to work better on holes for the wires. I could think of a lid? It holds well so it's not needed.

![](/Mechanical/Battery%20Holder%2018650%20OpenSCAD/Images/2025-11-15-T1207%2018650%202S2P%204S1P%20Inverted%20Holder.jpg)