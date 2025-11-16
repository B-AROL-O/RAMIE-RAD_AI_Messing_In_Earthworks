# DEMO 1 SERIAL COMMUNICATION

## INSTRUCTIONS

UV Virtual environment

```cmd
uv venv ToB --python 3.13

ToB\Scripts\activate 

uv pip install pyserial
```

Check Python version

```cmd
(ToB)> python --version
Python 3.13.3
```

## DEMO 1 - ENUMERATE SERIAL PORTS

The pyserial has a function to list available ports

```python
import serial

#Location of the library on disk
print(serial.__file__)

import serial.tools.list_ports

#Enumerate available ports
ports = serial.tools.list_ports.comports()
for port in ports:
    print(port.device)
```

OUTPUT

```cmd
>python demo_1_enumerate_serial.py
F:\Data\Project\Project OrangeBot\RAMIE-RAD_AI_Messing_In_Earthworks\Software\Demo 1 - Serial Communication\ToB\Lib\site-packages\serial\__init__.py
COM1
COM5
```

Under windows COM1 is to be avoided. It should be COM5.

## ALREADY RESERVED PORT

If the port is open, there will be a permission error

```cmd
something like permission error code 5, didn't save it
```

## DEMO 2 - ASK BOARD SIGNATURE

I'm vibe coding the demos, this worked first shot, I had to fix the baud rate.

Enumerate ports
For each
Open them
Send "SIGN\0"
Listen for a string answer for 1 second non blocking
Enumerate all the ports that answered

CODE

```python

import serial
import serial.tools.list_ports
import time

def find_responsive_ports():
    """
    Scans all available serial ports and identifies those that respond to a specific command.

    This function iterates through all detected serial ports, attempts to open each one,
    sends a command ("SIGN\0"), and checks for a response. Ports that respond are added
    to a list of responsive ports.

    Returns:
        list: A list of device paths (strings) for responsive serial ports.
    """
    # Get a list of all available serial ports
    ports = serial.tools.list_ports.comports()
    responsive_ports = []

    for port_info in ports:
        try:
            # Attempt to open the current serial port
            with serial.Serial(
                port_info.device,
                baudrate=250000,
                timeout=1  # Timeout for read operations
            ) as ser:
                print(f"Testing port: {port_info.device}")

                # Send the command "SIGN\0" to the device
                ser.write(b"SIGN\0")

                # Wait for 1 second to allow the device to process the command
                time.sleep(1)

                # Read all available data from the serial port
                response = ser.read_all()

                # If a response is received, print it and add the port to the responsive list
                if response:
                    print(f"Response from {port_info.device}: {response}")
                    responsive_ports.append(port_info.device)

        except serial.SerialException as e:
            # Print an error message if the port cannot be opened
            print(f"Failed to open {port_info.device}: {e}")

    return responsive_ports

if __name__ == "__main__":
    # Find and print all responsive serial ports
    responsive_ports = find_responsive_ports()
    print("\nResponsive ports:")
    for port in responsive_ports:
        print(port)
```

RESPONSE

```
(ToB) F:\Data\Project\Project OrangeBot\RAMIE-RAD_AI_Messing_In_Earthworks\Software\Demo 1 - Serial Communication>python demo_2_ask_signature.py
Failed to open COM1: Cannot configure port, something went wrong. Original message: OSError(22, 'The parameter is incorrect.', None, 87)
Testing port: COM5
Response from COM5: b'RAMIE\x00FF'

Responsive ports:
COM5
```