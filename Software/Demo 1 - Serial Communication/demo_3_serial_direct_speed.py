"""
List ports
Try to send SIGN\0 and see what ports are responsive
Open the responsive port


---
Command 
Set target velocity of left and right wheels. It is limited by the acceleration of the driver, a safety and continuity measure.

Using this command sets a constant velocity, and bypasses the motion orchestration. Set speed to zero or ask for STOP to stop.

It has S8 arguments, range -127 to +127.

SBC REQUEST: ```VR%sL%s/0```

BOARD ANSWER: none
---

The code sends every 250ms 
increase speed on R by increment of 10 up to 50
Then decrement in speed -10 down to -50
then increment of +10 up to 0
then do the same on L speed
repeat

Stop when a keybord stroke is pressed


"""

import serial
import serial.tools.list_ports
import time
import keyboard  # pip install keyboard

n_speed_increment = 1
n_speed_max = 10
t_sleep = 0.15

def find_responsive_ports():
    """
    Scans all available serial ports and identifies those that respond to "SIGN\0".
    Returns:
        list: A list of device paths (strings) for responsive serial ports.
    """
    ports = serial.tools.list_ports.comports()
    responsive_ports = []
    for port_info in ports:
        try:
            with serial.Serial(
                port_info.device,
                baudrate=250000,
                timeout=0.5
            ) as ser:
                print(f"Testing port: {port_info.device}")
                ser.write(b"SIGN\0")
                time.sleep(0.5)
                response = ser.read_all()
                if response:
                    print(f"Response from {port_info.device}: {response}")
                    responsive_ports.append(port_info.device)
        except serial.SerialException as e:
            print(f"Failed to open {port_info.device}: {e}")
    return responsive_ports

def send_wheel_commands(port):
    """
    Sends velocity commands to the specified serial port using status variables.
    Args:
        port (str): The device path of the serial port.
    """
    try:
        with serial.Serial(port, baudrate=250000, timeout=1) as ser:
            print(f"Connected to {port}. Press any key to stop...")

            # Status variables
            motor_index = 0  # 0: right motor, 1: left motor
            phase = 0        # 0: ramp up, 1: ramp down, 2: ramp up to zero
            speed = 0

            while True:
                #if keyboard.is_pressed('any'):
                #    print("Stopping...")
                #    return

                # Update speed based on phase and motor
                if phase == 0:  # Ramp up
                    speed += n_speed_increment
                    if speed > n_speed_max:
                        speed = n_speed_max
                        phase = 1
                elif phase == 1:  # Ramp down
                    speed -= n_speed_increment
                    if speed < -n_speed_max:
                        speed = -n_speed_max
                        phase = 2
                elif phase == 2:  # Ramp up to zero
                    speed += n_speed_increment
                    if speed > 0:
                        speed = 0
                        phase = 0
                        motor_index = 1 - motor_index  # Switch motor

                # Send command
                if motor_index == 0:  # Right motor
                    command = f"VR{speed}L0\0".encode()
                else:  # Left motor
                    command = f"VR0L{speed}\0".encode()

                ser.write(command)
                print(f"Sent: {command}")
                time.sleep(t_sleep)

    except serial.SerialException as e:
        print(f"Error: {e}")

if __name__ == "__main__":
    responsive_ports = find_responsive_ports()
    if not responsive_ports:
        print("No responsive ports found.")
    else:
        print("\nResponsive ports:")
        for port in responsive_ports:
            print(port)
        send_wheel_commands(responsive_ports[0])
