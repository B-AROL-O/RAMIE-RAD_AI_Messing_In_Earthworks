/*
 *  Test bench for CircularMotionQueue
 *
 *  Compile with:
 *      g++ -std=c++11 main.cpp -o testbench
 */

#include <iostream>
#include "St_wheel_speed_duration.h"
#include "cl_motion_queue.hpp"

using std::cout;
using std::endl;

/* helper to print a motion i_rst_item */
void show_item(const St_wheel_speed_duration &i_rst_item, uint8_t u8_step)
{
    cout << "Step " << u8_step
         << ":  R=" << static_cast<int>(i_rst_item.s8_speed_right)
         << ", L=" << static_cast<int>(i_rst_item.s8_speed_left)
         << ", dur=" << static_cast<unsigned>(i_rst_item.u8_duration)
         << endl;
}

int main()
{
    Cl_motion_queue cl_queue;

    /* ---------- 1. Feed a bunch of motions -------------------- */
    St_wheel_speed_duration mov1{ 10, -10, 5 };   // spin in place for 5 steps
    St_wheel_speed_duration mov2{ 20,  20, 3 };    // forward for 3 steps
    St_wheel_speed_duration mov3{-15, 15, 4 };     // spin opposite direction

    cl_queue.push(mov1);
    cl_queue.push(mov2);
    cl_queue.push(mov3);

    cl_queue.set_run(true);

    /* ---------- 2. Execute time‑steps until the cl_queue is empty */
    cout << "=== Running motions ===" << endl;
    St_wheel_speed_duration st_wheel_speed_duration{0,0,0};
    uint8_t u8_step = 1;
    while (cl_queue.empty() == false)
    {
        bool x_fail = cl_queue.execute_time_step(st_wheel_speed_duration);
        show_item(st_wheel_speed_duration, u8_step);

        u8_step++;
    }

    //--------------------------------------------------------------------------
    //	PAUSE/UNPAUSE EXECUTION
    //--------------------------------------------------------------------------

    cout << "\n=== Pause test ===" << endl;
    cl_queue.push(mov1);   // push another command

    // run one u8_step
    cl_queue.execute_time_step(st_wheel_speed_duration);
    show_item(st_wheel_speed_duration, 1);

    // pause
    cl_queue.set_run(false);
    cl_queue.execute_time_step(st_wheel_speed_duration);
    show_item(st_wheel_speed_duration, 2);          // should be all zeros

    // unpause and continue
    cl_queue.set_run(true);
    while (!cl_queue.empty())
    {
        u8_step++;
        cl_queue.execute_time_step(st_wheel_speed_duration);
        show_item(st_wheel_speed_duration, u8_step);
    }

    //--------------------------------------------------------------------------
    //	FLUSH TEST
    //--------------------------------------------------------------------------

    cout << "\n=== Flush test ===" << endl;
    cl_queue.flush();
    bool empty_after_flush = cl_queue.empty();
    cout << "Queue empty after flush? "
         << (empty_after_flush ? "YES" : "NO") << endl;

    /* ---------- 4. Test full‑cl_queue handling --------------------- */
    cout << "\n=== Full cl_queue test ===" << endl;
    const uint8_t n = Cl_motion_queue::cu8_queue_size;
    bool push_failed = false;

    for (uint8_t u8_cnt = 0; u8_cnt < n + 1; ++u8_cnt)
    {
        St_wheel_speed_duration st_dummy{ static_cast<int8_t>(u8_cnt), 0, 1 };
        if (cl_queue.push(st_dummy))
        {
            push_failed = true;
        }
    }

    cout << "Push failed on full cl_queue? "
         << (push_failed ? "YES" : "NO") << endl;



    return 0;
}
