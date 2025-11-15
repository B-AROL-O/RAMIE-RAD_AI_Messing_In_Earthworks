# Circular Motion Queue

It's a C++ class that has a circular queue of a structure with three fields
struct st_wheel_speed_duration
	int8_t s8_speed_right
	int8_t s8_speed_left
	uint8_t u8_duration

The queue is a FIFO, it's meant to store a sequence of motions with a duration

The class has a statically allocated vector that serves as circular queue

It has an index pointing to bottom of the queue and an index pointing to the top of the queue

It has methods to push a structure, peek the structure and pop the structure from the queue

It has a method for execution execute_time_step, each time it is called it decreases duration in the top element by one. The user must provide a reference of a st_wheel_speed_duration structure that the method will fill with the updated top element. If duration is zero on the top element, it is popped and the next element becomes active

There are protections. execute_time_step on Empty queue should fill zero speed and return true.

executing push on full queue should return true (fail) and not touch the queue

There is a method to flush the queue by setting the indexes of top and bottom to zero

There is a bool x_run and a pause, unpause method. execute_time_step will fill zero speed if the motion is paused

use stdint library with explicit types, this is microcontroller code and should not use dynamic allocation nor pointers too much

as for names, i_ for function inputs. explicit type in the name int8_t s8_temp
