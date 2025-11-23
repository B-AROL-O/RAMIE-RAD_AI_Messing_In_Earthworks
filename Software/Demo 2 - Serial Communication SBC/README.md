# PYTHON

## UV VENV

Install virtual environment for python

```
sudo snap install astral-uv --classic
```

```
sona@lpmu8gb:~/Demo 2 - Serial Communication SBC$ sudo snap install astral-uv --classic
2025-11-23T09:38:36Z INFO
Waiting for automatic snapd
restart...
astral-uv 0.9.5 from Alex Lowe (lengau) installed
```

## Create folder

Make sure you create a folder for the pythoin demos to run

```
sudo mkdir demo_serial_communication
cd demo_serial_communication
```

## UV TOML 

The most convenient way for it to autogenerate the virtual environment is to generate a toml project file and run uv with the command ```uv run```

This will make sure that with a single line a venv is created with the right python version and dependencies

```pyproject.toml```

```python
[project]
name = "demo_serial_communication_sbc_electronics"
version = "0.1.0"
requires-python = ">=3.13"
dependencies = [
    "pyserial",
]
```

## DEMO ENUMERATE SERIAL PORTS

Now, the first application tests the ports enumerated in /dev that are serial ports

```demo_1_enumerate_serial.py```

```
import serial
print(serial.__file__)

import serial.tools.list_ports

ports = serial.tools.list_ports.comports()
for port in ports:
    print(port.device)
```

Execute the script

```
uv run demo_1_enumerate_serial.py 
```

The first run will create ```.venv``` with the local libraries and interpreter and execute. In this screen I displayed the location of the library to make sure it's executing from .venv

```
uv run demo_1_enumerate_serial.py 
Using CPython 3.14.0
Creating virtual environment at: .venv
Installed 1 package in 16ms
/home/sona/Demo 2 - Serial Communication SBC/.venv/lib/python3.14/site-packages/serial/__init__.py
/dev/ttyS31
/dev/ttyS30
/dev/ttyS29
/dev/ttyS28
/dev/ttyS27
/dev/ttyS26
/dev/ttyS25
/dev/ttyS24
/dev/ttyS23
/dev/ttyS22
/dev/ttyS21
/dev/ttyS20
/dev/ttyS19
/dev/ttyS18
/dev/ttyS17
/dev/ttyS16
/dev/ttyS15
/dev/ttyS14
/dev/ttyS13
/dev/ttyS12
/dev/ttyS11
/dev/ttyS10
/dev/ttyS9
/dev/ttyS8
/dev/ttyS7
/dev/ttyS3
/dev/ttyS2
/dev/ttyS1
/dev/ttyS6
/dev/ttyS5
/dev/ttyS4
/dev/ttyS0
```

In the Latte Panda Mu Lite Board, serial port S0 is the debug interface that is connected by GRUB to the linux serial terminal

[Linux Grub TTYS0 Instructions](/Software/ttys0%20SSH)

While S4 to S6 are three serial ports available to user, I diagnosed them with an oscilloscope and terminal to make sure they work

TODO: add documentation for the serial ports?

#### Download single folder from github (FAIL)

```
wget https://github.com/B-AROL-O/RAMIE-RAD_AI_Messing_In_Earthworks/tree/Master/Software/Demo%201%20-%20Serial%20Communication%20Host


wget https://github.com/B-AROL-O/RAMIE-RAD_AI_Messing_In_Earthworks/archive/refs/heads/Master.zip -O repo.zip

sona@lpmu8gb:~$ wget https://github.com/B-AROL-O/RAMIE-RAD_AI_Messing_In_Earthworks/archive/refs/heads/Master.zip -O repo.zip
--2025-11-23 09:30:26--  https://github.com/B-AROL-O/RAMIE-RAD_AI_Messing_In_Earthworks/archive/refs/heads/Master.zip
Resolving github.com (github.com)... 140.82.121.4
Connecting to github.com (github.com)|140.82.121.4|:443... connected.
HTTP request sent, awaiting response... 404 Not Found
2025-11-23 09:30:26 ERROR 404: Not Found.

```