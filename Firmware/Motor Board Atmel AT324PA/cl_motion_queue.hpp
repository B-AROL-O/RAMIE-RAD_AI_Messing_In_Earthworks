/*  CircularMotionQueue.hpp
 *
 *  Micro‑controller friendly FIFO queue that stores wheel speed/duration
 *  structures.
 */

#include <stdint.h>

/*--------------------------------------------------------------
 *  Circular queue class
 *--------------------------------------------------------------*/

class Cl_motion_queue
{
    public:
        /* public constants ---------------------------------------------------*/

        //Queue Size
        static const uint8_t cu8_queue_size = 16;

        /* constructor --------------------------------------------------------*/
        inline Cl_motion_queue()
        {
            this->g_u8_bottom = 0;
            this->g_u8_top = 0;
            this->g_x_run = false;
            return;
        }

        /*--------------------------------------------------------------
         *  Queue manipulation
         *--------------------------------------------------------------*/

        /* push a new element. Returns true if the queue is full (push fails) */
        inline bool push(const St_wheel_speed_duration &i_rst_item)
        {
            uint8_t u8_next = (this->g_u8_top >= this->cu8_queue_size)?(0):(this->g_u8_top + 1u);

            /* queue full */
            if (u8_next == this->g_u8_bottom)
                return true; //FAIL

            this->gst_queue[this->g_u8_top] = i_rst_item;
            this->g_u8_top = u8_next;
            return false; //OK
        }

        /* peek the oldest element without removing it.
         * If the queue is empty, the returned object has zero speeds. */
        inline bool peek( St_wheel_speed_duration &o_rst_item ) const
        {
            if (this->empty() == true)
            {
                o_rst_item.s8_speed_right = 0;
                o_rst_item.s8_speed_left  = 0;
                o_rst_item.u8_duration = 0;
                return true; //FAIL
            }
            o_rst_item = this->gst_queue[this->g_u8_bottom];
            return false; //OK
        }

        /* pop the oldest element. Returns true if an element was removed,
         * otherwise false (queue empty). */
        inline bool pop(void)
        {
            if (this->empty())
                return true; //FAIL

            this->g_u8_bottom = (this->g_u8_bottom + 1u) % this->cu8_queue_size;
            return false; //OK
        }

        /* flush queue – reset to empty state. */
        inline bool flush(void)
        {
            this->g_u8_bottom = 0;
            this->g_u8_top    = 0;
            return false; //OK
        }

		//Return the number of motions queued
		inline uint8_t get_num_element()
		{
			return ( (this->g_u8_top >= this->g_u8_bottom) ? (this->g_u8_top-this->g_u8_bottom) : (this->cu8_queue_size-this->g_u8_bottom+this->g_u8_top) );
		}

        /*--------------------------------------------------------------
         *  Execution helpers
         *--------------------------------------------------------------*/

        inline bool set_run( bool i_x_run )
        {
            this->g_x_run = i_x_run;
            return false; //OK
        }

        /* execute one time‑step.
         *
         * i_rst_out will be filled with the current wheel speeds and
         * duration remaining. If the queue is empty or paused,
         * zero speeds are returned and the function returns true.
         *
         * When a command finishes (duration reaches 0) it is removed
         * automatically for the next call.
         */
        inline bool execute_time_step(St_wheel_speed_duration &i_rst_out)
        {
            if ( (this->g_x_run==false) || (this->empty()==true) )
            {
                i_rst_out.s8_speed_right = 0;
                i_rst_out.s8_speed_left  = 0;
                i_rst_out.u8_duration = 0;
                return true;          /* queue empty or paused */
            }

            /* active element is at the head of the queue */
            St_wheel_speed_duration &rst_item = this->gst_queue[this->g_u8_bottom];

            /* copy current state to output */
            i_rst_out.s8_speed_right = rst_item.s8_speed_right;
            i_rst_out.s8_speed_left  = rst_item.s8_speed_left;
            i_rst_out.u8_duration = rst_item.u8_duration;

            /* decrement duration of the active element */
            if (rst_item.u8_duration > 0u)
            {
                rst_item.u8_duration--;
            }

            /* if finished, pop it */
            if (rst_item.u8_duration == 0u)
            {
                this->pop();
            }

            return false; //OK
        }

        inline bool empty(void) const
        {
            return this->g_u8_bottom == this->g_u8_top;
        }

    private:
        /* queue storage ---------------------------------------------*/
        St_wheel_speed_duration gst_queue[cu8_queue_size];

        /* indices: g_u8_bottom → oldest element
         *           g_u8_top    → next free slot                */
        uint8_t g_u8_bottom;
        uint8_t g_u8_top;

        /* run flag ---------------------------------------------------*/
        bool g_x_run;   /* true = executing, false = paused */


};
