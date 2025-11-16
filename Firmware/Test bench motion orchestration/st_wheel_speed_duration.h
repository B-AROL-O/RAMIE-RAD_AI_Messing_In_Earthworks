#include <cstdint>

/*--------------------------------------------------------------
 *  Data structure to be queued
 *--------------------------------------------------------------*/

struct St_wheel_speed_duration
{
    int8_t s8_speed_right;
    int8_t s8_speed_left;
    uint8_t u8_duration;          /* duration in time steps */

};
