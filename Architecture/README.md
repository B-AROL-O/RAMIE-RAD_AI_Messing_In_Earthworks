# ARCHITECTURE

## RAMIE

RAMIE is an acronym for:
- RAD Radical
- AI
- Messing
- In
- Earthworks

RAMIE is an MVP AI robot meant to use local 4B LLMs as poor's man AGI and achieve general robotics to an extent.

RAMIE showcasses an end to end STT LLM TTS workflow using MCP tools to sense the robot state and control the robot actions.

RAMIE is menat to run 100% locally without internet connection, all within a small mobile robots, cost, power, memory and compute envelopes.

It has proven most challenging finding an SBC capable of such feats.

## SPECIFICATIONS

- Power: 40 W
- Batttery Duration: ? h
- DoF: 2
- Complexity: MVP

# HARDWARE ARCHITECTURE

## SBC

The single board computer takes care of high level user application and running AI models and required python glue.

RAMIE is powered by the ```Latte Panda Mu N100 16GB``` mounted on a ```Latte Panda Mu Lite Carrier Board```

This board was choosen because of a combination of RAM, RAM bandwidth, compute, power envelope and Intel being one of the two companies in the world that knows how to write drivers and ML frameworks.

## Microcontroller

RAMIE is equipped with a microcontroller board takes care of real time applications, like motor control.

- ```Atmel 324 PA``` 32KB Flash, 4KB SRAM
- ```LCD 2x16``` Display board status
- ```UART 250Kb/s``` Link with the SBC

## Motors

RAMIE moves using two continuous rotation servos using PPM protocol and two wheels with pivot.

Motors are labeled left and right, in this order. The table below will show the robot action when a given speed is sent to the microcontroller board.

| Left Speed  | Right Speed | Robot Motion | 
|-------------|-------------|--------------|
| 0           | 0           | STOP         |
| +10         | +10         | FORWARD 100mm/s          |
| -10         | -10         | BACKWARD 100mm/s          |
| -10         | +10         | COUNTERCLOCKWISE 180°/s |
| +10         | -10         | CLOCKWISE 180°/s |

## Communication Protocol SBC <-> 

Hardware link uses full duplex 250Kb/s link on UART ```ttys4```

The protocol is meant to allow the SBC to read the robot state, and emit robot actions, with the microcontroller firmware taking care of real time operation.

### FEATURE: COMMUNICATION TIMEOUT

The firmware is configured to STOP the motors automatically if no communications is received within a given time limit.

This feature is meant to mitigate the scenario where the SBC application has stalled and the robot will keep executing the last given command, preventing a collision and making it convenient to rearm the robot.

Typical timeout configuration is 1000ms

### FEATURE: DIRECT MODE VS. TIMED MODE

I thought a lot about it, and designed to set of velocity commands to be sent to the SBC.

- Direct Mode: The SBC send a velocity for left and right wheel, and the wheel will move at that speed unless commanded otherwise.
- Timed Mode: The SBC send a timed velocity command that specify a duration in servo step (50Hz). The microcontroller has a queue. This allows the SBC to send a number of motion commands and the microcontroller will execute the commands in sequence for the specified time, then when queue is empty, stop the motors.

Direct Mode is convenient for teleoperation, by binding a velocity to a keystroke down, and velocity zero to keystroke up.

Timed Mode is convenient for LLM orchestration, where the LLM can come up in a single query with a number of motion instructions, and the microcontroller will execute them in sequence. THe SBC isn't going to be convenient for real time anyway, let the microcontroller deal with it.

The microcontroller will seamless switch mode based on last received command, switching will flush the queue of timed commands.

### UART Command Reference (SBC <-> MICROCONTROLLER)

*All messages are ASCII strings terminated by the literal sequence `"/0"` (slash zero). The SBC always sends the terminator; the micro‑controller never sends any data except the replies listed below.*

**RAMIE UART Command Reference (SBC ↔ MC‑U)**  

| Command | Syntax | Arguments | Meaning | Reply (if any) |
|---------|--------|-----------|---------|----------------|
| **PING** – refresh communication timeout | `P/0` | – | Resets the internal timeout timer. | None |
| **SIGN** – get robot signature | `SIGN/0` | – | Returns a string identifying the robot. | `RAMIE/0` |
| **REV** – firmware revision | `REV/0` | – | Returns date of current firmware (ISO‑8601). | `2025-11-16/0` |
| **STOP** – emergency stop | `STOP/0` | – | Immediately cuts power to both motors, bypassing acceleration limits and flushes the motion queue. | `STOP/0` |
| **SET VELOCITY** – set constant wheel speed (bypass orchestration) | `VR%sL%s/0` | `R`: right‑wheel speed  (–127 … +127)<br>`L`: left‑wheel speed  (–127 … +127) | Motor controller accepts the two signed bytes and drives each wheel at the requested PWM value. The values are clipped to the legal range. | None |
| **SET TIMED VELOCITY** – enqueue a motion segment | `VT%uR%sL%s/0` | `T`: duration (steps) at 50Hz <br>`R`, `L`: wheel speeds as above | The controller stores the tuple *(right, left, duration)* in its internal queue. When no other command is received, it executes each queued segment sequentially and stops automatically after the last step. Duration is an integer multiple of **1 step = 20 ms** (i.e., one 50Hz cycle). | None |
| **GET BATTERY** – read battery voltage | `GETBAT/0` | – | ? | ? |


### Common Notes

- **Terminators** – Every command string must end with the literal characters `/0`. The micro‑controller responds in exactly the same way (if a reply is defined).  
- **Error Handling** – If an unknown or malformed command arrives, the micro‑controller sends back `ERR/0`. No other action is taken.  
- **Speed Units** – The signed byte values represent PPM 
- **Queue Size** – Timed Mode supports up to 16 motion commands before overflowing. Overflow behaviour is flush.
- **Safety** – The servo driver implements a change in command limit from time step to time step, that is speed for position servos, and acceleration for speed servos.


#### Example Session

```
# Reset timeout
P/0

# Query signature
SIGN/0   → RAMIE/0

# Set constant speeds for 2 seconds
VR10L-10/0
VT20R20L20/0      # queue: forward 3 s (since duration=30 steps)
```

The test bench shows the response to orchestration

```
//MOTIONS QUEUED UP
St_wheel_speed_duration mov1{ 10, -10, 5 };   // spin in place for 5 steps
St_wheel_speed_duration mov2{ 20,  20, 3 };    // forward for 3 steps
St_wheel_speed_duration mov3{-15, 15, 4 };     // spin opposite direction
```

```
//SPEED OF THE MOTORS AT EACH ORCHESTRATION STEP
Step:   1 | R:    10 | L:   -10
Step:   2 | R:    10 | L:   -10
Step:   3 | R:    10 | L:   -10
Step:   4 | R:    10 | L:   -10
Step:   5 | R:    10 | L:   -10
Step:   6 | R:    20 | L:    20
Step:   7 | R:    20 | L:    20
Step:   8 | R:    20 | L:    20
Step:   9 | R:   -15 | L:    15
Step:  10 | R:   -15 | L:    15
Step:  11 | R:   -15 | L:    15
Step:  12 | R:   -15 | L:    15
```
