```cpp
/**********************************************************************************
 *  CircularMotionQueue.hpp
 *
 *  Micro‑controller friendly FIFO queue that stores wheel speed/duration structures.
 **********************************************************************************/

#pragma once

#include <cstdint>

/*--------------------------------------------------------------
 *  Forward declaration of the element type.
 *  The actual definition must be visible before the queue is used.
 --------------------------------------------------------------*/
struct St_wheel_speed_duration;

/**********************************************************************************
 *  @namespace MotionQueue
 *
 *  @brief Namespace that contains all public interfaces for the motion‑queue
 *         implementation.  Keeps the global namespace free from name clashes
 *         and makes it obvious that the types belong to this library.
 **********************************************************************************/
namespace MotionQueue {

/**********************************************************************************
 *  @class Cl_motion_queue
 *
 *  @brief Circular FIFO queue with a fixed size that stores wheel speed/duration
 *         structures.  The queue is designed for micro‑controller environments,
 *         therefore no dynamic allocation is used and all operations are
 *         constexpr‑friendly.
 *
 *  The queue uses an array of @p St_wheel_speed_duration objects, two indices
 *  (`g_u8_bottom` / `g_u8_top`) to keep track of the oldest element and the next
 *  free slot respectively, and a run flag (`g_x_run`).  All member functions are
 *  defined inline in the header because they are small and benefit from being
 *  compiled into every translation unit that uses them.
 *
 *  @note   The queue is intentionally “small‑footprint”: all indices and flags
 *          use `uint8_t`, which is sufficient for the maximum size of 16.
 *
 **********************************************************************************/
class Cl_motion_queue
{
public:
    /*-----------------------------------------------------------------------
     *  Public constants
     *-----------------------------------------------------------------------*/
    /** @brief Size of the internal circular buffer. */
    static constexpr uint8_t C_Q_SIZE = 16;

    /*-----------------------------------------------------------------------
     *  Construction / Destruction
     *-----------------------------------------------------------------------*/

    /**
     * @brief Default constructor.
     *
     * Initializes an empty queue and clears the run flag.
     *
     * @post Queue is empty.
     * @pre None.
     */
    inline Cl_motion_queue()
        : g_u8_bottom{0},
          g_u8_top{0},
          g_x_run{false}
    {}

    /*-----------------------------------------------------------------------
     *  Queue manipulation
     *-----------------------------------------------------------------------*/

    /**
     * @brief Push a new element into the queue.
     *
     * @param i_rst_item Reference to the wheel speed/duration structure to
     *                   be queued.  The object is copied into the internal
     *                   buffer.
     *
     * @return `true` if the operation failed because the queue is full,
     *         otherwise `false`.
     *
     * @post If the return value is `false`, `i_rst_item` occupies the next
     *       free slot in the circular buffer.
     */
    inline bool push(const St_wheel_speed_duration &i_rst_item)
    {
        const uint8_t u8_next = (this->g_u8_top + 1u) % this->C_Q_SIZE;

        // Queue full?
        if (u8_next == this->g_u8_bottom)
            return true;   // FAIL

        this->gst_queue[this->g_u8_top] = i_rst_item;
        this->g_u8_top                   = u8_next;
        return false;  // OK
    }

    /**
     * @brief Peek the oldest element without removing it.
     *
     * @param o_rst_item Reference to a structure that will receive the
     *                   queued values.  If the queue is empty, the object
     *                   is filled with zeros.
     *
     * @return `true` if the queue was empty (output contains zeroes),
     *         otherwise `false`.
     */
    inline bool peek(St_wheel_speed_duration &o_rst_item) const
    {
        if (this->empty())
        {
            o_rst_item.s8_speed_right = 0;
            o_rst_item.s8_speed_left  = 0;
            o_rst_item.u8_duration   = 0;
            return true;   // FAIL
        }

        o_rst_item = this->gst_queue[this->g_u8_bottom];
        return false;  // OK
    }

    /**
     * @brief Remove the oldest element from the queue.
     *
     * @return `true` if the queue was empty (nothing removed),
     *         otherwise `false`.
     */
    inline bool pop(void)
    {
        if (this->empty())
            return true;   // FAIL

        this->g_u8_bottom = (this->g_u8_bottom + 1u) % this->C_Q_SIZE;
        return false;  // OK
    }

    /**
     * @brief Reset the queue to an empty state.
     *
     * All indices are set back to zero, discarding any stored elements.
     *
     * @return Always `false` (operation never fails).
     */
    inline bool flush(void)
    {
        this->g_u8_bottom = 0;
        this->g_u8_top    = 0;
        return false;  // OK
    }

    /*-----------------------------------------------------------------------
     *  Execution helpers
     *-----------------------------------------------------------------------*/

    /**
     * @brief Enable or disable execution of the queued commands.
     *
     * While disabled, `execute_time_step()` will always return zero speeds.
     *
     * @param i_x_run `true` to start executing, `false` to pause.
     *
     * @return Always `false` (operation never fails).
     */
    inline bool set_run(bool i_x_run)
    {
        this->g_x_run = i_x_run;
        return false;  // OK
    }

    /**
     * @brief Execute a single time‑step.
     *
     * The function copies the current wheel speeds and remaining duration into
     * `o_rst_out`.  If the queue is empty or execution is paused, zero speeds
     * are returned.  When an element finishes (duration reaches zero) it is
     * automatically removed so that the next call processes the following
     * command.
     *
     * @param o_rst_out Reference to a structure that will receive the current
     *                   wheel speed and remaining duration values.
     *
     * @return `true` if execution was paused or the queue was empty (output
     *         contains zeroes), otherwise `false`.
     */
    inline bool execute_time_step(St_wheel_speed_duration &o_rst_out)
    {
        if (!this->g_x_run || this->empty())
        {
            o_rst_out.s8_speed_right = 0;
            o_rst_out.s8_speed_left  = 0;
            o_rst_out.u8_duration   = 0;
            return true;          // queue empty or paused
        }

        /* Active element is at the head of the queue */
        St_wheel_speed_duration &rst_item =
            this->gst_queue[this->g_u8_bottom];

        /* Copy current state to output */
        o_rst_out.s8_speed_right = rst_item.s8_speed_right;
        o_rst_out.s8_speed_left  = rst_item.s8_speed_left;
        o_rst_out.u8_duration    = rst_item.u8_duration;

        /* Decrement duration of the active element */
        if (rst_item.u8_duration > 0u)
            --rst_item.u8_duration;

        /* If finished, pop it */
        if (rst_item.u8_duration == 0u)
            this->pop();

        return false; // OK
    }

    /**
     * @brief Check whether the queue is empty.
     *
     * @return `true` if there are no elements in the queue,
     *         otherwise `false`.
     */
    inline bool empty(void) const { return this->g_u8_bottom == this->g_u8_top; }

private:
    /*-----------------------------------------------------------------------
     *  Queue storage
     *-----------------------------------------------------------------------*/
    St_wheel_speed_duration gst_queue[C_Q_SIZE];

    /**
     * @brief Index of the oldest element (head of the queue).
     *
     * When `empty()` is true, this index points to the next slot to be read.
     */
    uint8_t g_u8_bottom;

    /**
     * @brief Index of the next free slot (tail of the queue).
     *
     * The element at this index has not yet been written to.
     */
    uint8_t g_u8_top;

    /** @brief Flag that controls whether execution is active. */
    bool g_x_run;
};

} // namespace MotionQueue
```
