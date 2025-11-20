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

### COMMANDS

Commands are simply executed by sending an appropriate ASCII string with terminator via UART link.

| SBC Message | Microcontroller Response | Robot Motion | 
|-------------|-------------|--------------|
| ```P/0``` | 0           | STOP         |

## COMMAND PING

This command simply refreshes the communication timeout. 

SBC REQUEST: ```P/0```

BOARD ANSWER: none

## SIGN

This command Ask the board to send the signature, it can be the name of the robot

SBC REQUEST: ```SIGN/0```

BOARD ANSWER: ```RAMIE/0```

## REV

This command ask the board for the firmware revision. It's a date in yyyy-mm-dd ISO format

SBC REQUEST: ```REV/0```

BOARD ANSWER: ```2025-11-16/0```

## STOP

This command is the emergency stop. It stops the motors bypassing the speed and acceleration limits.

SBC REQUEST: ```STOP/0```

BOARD ANSWER: ```STOP/0```

TODO: implements the limit removal

##  SET VELOCITY

Set target velocity of left and right wheels. It is limited by the acceleration of the driver, a safety and continuity measure.

Using this command sets a constant velocity, and bypasses the motion orchestration. Set speed to zero or ask for STOP to stop.

It has S8 arguments, range -127 to +127.

SBC REQUEST: ```VR%sL%s/0```

BOARD ANSWER: none

##  SET TIMED VELOCITY

This specify a time and two velocities. This will engage the motion orchestration, allowing the SBC to queue up motions in a queue, so that when a new speed is sent, it's queued up for execution.

Robot will automatically stop once the last command in the queue is executed.

It's done it this way so that the SBC driver doesn't have to care for the real time orchestration of motion sequences.

SBC REQUEST: ```VT%uR%sL%s```

BOARD ANSWER: none

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

I need to decide how big is an orchestration step. It should be a finite multiple of 50Hz. I'm thinking 1 mean 100ms. So the maximum 255 means 25.5s

