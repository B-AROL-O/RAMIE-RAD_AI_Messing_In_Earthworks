import serial
import sys
import select
import tty
import termios
import time

# Configuration
g_s_port = "/dev/ttyS4"
g_n_baudrate = 115200
g_n_speed = 5
g_n_sleep = 0.1

def send_wheel_command(ser, left_speed, right_speed):
    """Send velocity commands to the motors."""
    command = f"VR{right_speed}L{left_speed}\0".encode()
    ser.write(command)
    print(f"Sent: VR{right_speed}L{left_speed}")

def get_key():
    """Read a single keypress from stdin (non-blocking)."""
    fd = sys.stdin.fileno()
    old_settings = termios.tcgetattr(fd)
    try:
        tty.setraw(fd)
        rlist, _, _ = select.select([sys.stdin], [], [], 0.1)
        if rlist:
            key = sys.stdin.read(1)
            return key
    finally:
        termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)
    return None

def main():
    try:
        with serial.Serial(
            g_s_port,
            baudrate=g_n_baudrate,
            timeout=1  # Timeout for read operations
        ) as ser:
            print(f"Connected to {g_s_port}. Use wasd to control. Press ESC or q to quit.")
            left_speed = 0
            right_speed = 0

            while True:
                key = get_key()
                if key == 'w':  # Up arrow
                    left_speed = g_n_speed
                    right_speed = g_n_speed
                elif key == 's':  # Down arrow
                    left_speed = -g_n_speed
                    right_speed = -g_n_speed
                elif key == 'a':  # Left arrow
                    left_speed = -g_n_speed
                    right_speed = +g_n_speed
                elif key == 'd':  # Right arrow
                    left_speed = +g_n_speed
                    right_speed = -g_n_speed
                elif key == 'q':  # ESC or 'q'
                    print("Exiting...")
                    break
                else:
                    left_speed = 0
                    right_speed = 0
                

                send_wheel_command(ser, left_speed, right_speed)
                time.sleep(g_n_sleep)

    except serial.SerialException as e:
        print(f"Serial error: {e}")
    except Exception as e:
        print(f"Error: {e}")

if __name__ == "__main__":
    main()
