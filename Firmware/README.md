# Firmware

I started from Unit Zero 2019 firmware and the uniparser I built over years. Fixed lots of issues glued together and I have the firmware

I spent many years improving on the uniparser library that allows to register a string and a callback with % argument decoding and it works great! Building custom parsers every time was tedious, and the automatic callback with argument is great at reducing work to do new firmware, it works really well.

TODO

# SBC <-> Board Servomotor LCD Communication

I use a UART 250Kb/s link

The communication is done in clear ASCII characters, easy to do by hand

I use the uniparser library that allows to link commands with % delimiter like a printf to do automatic callback functions with arguments that works really well

Define the callback functions

```cpp
extern void handle_ping(void);

extern void handle_sign(void);

extern void handle_revision(void);

extern void handle_stop(void);

extern void handle_set_velocity(int8_t right_speed, int8_t left_speed);

extern void handle_set_velocity_timed(uint8_t time, int8_t right_speed, int8_t left_speed);
```

Register what command triggers what callback function

```cpp
parser.add_cmd( "P", (void *)&handle_ping );

parser.add_cmd( "F", (void *)&handle_sign );

parser.add_cmd( "REV", (void *)&handle_revision );

parser.add_cmd( "STOP", (void *)&handle_stop );

parser.add_cmd( "VR%sL%s", (void *)&handle_set_velocity );

parser.add_cmd( "T%uVR%sL%s", (void *)&handle_set_velocity_timed );
```

From here the uniparser will take care of everything, and automatically do the function call once the command is received

## TIMEOUT

The firmware is configured to kill the motors if no communications is received, assuming that something went wrong.

TODO: Is it implemented? check.

## PING

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

