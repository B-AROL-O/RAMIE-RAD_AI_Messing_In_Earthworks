# PYTHON

This section is about demos to interface the SBC with the AT324 and test serial communication and commands

## Serial Ports

The Latte Panda Mu and the Lite Carrier boards have a number of serial interfaces

I need to find out how they are mapped to linux ubuntu and how to use them

## Dev Tree

ttySxxx should be serial interfaces, on the pi they are also mapped to ttyAMAxxx

```
sona@sona-lp-n100-8gb:~$ ls /dev/
autofs           i2c-7         nvme0     tty24  tty59      ttyS6
block            i2c-8         nvme0n1   tty25  tty6       ttyS7
btrfs-control    i2c-9         nvram     tty26  tty60      ttyS8
bus              initctl       port      tty27  tty61      ttyS9
char             input         ppp       tty28  tty62      udmabuf
console          kmsg          psaux     tty29  tty63      uhid
core             kvm           ptmx      tty3   tty7       uinput
cpu              log           pts       tty30  tty8       urandom
cpu_dma_latency  loop0         random    tty31  tty9       userfaultfd
cuse             loop1         rfkill    tty32  ttyprintk  userio
disk             loop10        rtc       tty33  ttyS0      vcs
dma_heap         loop11        rtc0      tty34  ttyS1      vcs1
dri              loop2         shm       tty35  ttyS10     vcs2
drm_dp_aux0      loop3         snapshot  tty36  ttyS11     vcs3
ecryptfs         loop4         snd       tty37  ttyS12     vcs4
fb0              loop5         stderr    tty38  ttyS13     vcs5
fd               loop6         stdin     tty39  ttyS14     vcs6
full             loop7         stdout    tty4   ttyS15     vcsa
fuse             loop8         tpm0      tty40  ttyS16     vcsa1
gpiochip0        loop9         tpmrm0    tty41  ttyS17     vcsa2
hidraw0          loop-control  tty       tty42  ttyS18     vcsa3
hidraw1          mapper        tty0      tty43  ttyS19     vcsa4
hpet             mcelog        tty1      tty44  ttyS2      vcsa5
hugepages        mei0          tty10     tty45  ttyS20     vcsa6
hwrng            mem           tty11     tty46  ttyS21     vcsu
i2c-0            mmcblk0       tty12     tty47  ttyS22     vcsu1
i2c-1            mmcblk0boot0  tty13     tty48  ttyS23     vcsu2
i2c-10           mmcblk0boot1  tty14     tty49  ttyS24     vcsu3
i2c-11           mmcblk0p1     tty15     tty5   ttyS25     vcsu4
i2c-12           mmcblk0p2     tty16     tty50  ttyS26     vcsu5
i2c-13           mmcblk0rpmb   tty17     tty51  ttyS27     vcsu6
i2c-14           mqueue        tty18     tty52  ttyS28     vfio
i2c-15           mtd           tty19     tty53  ttyS29     vga_arbiter
i2c-2            mtd0          tty2      tty54  ttyS3      vhci
i2c-3            mtd0ro        tty20     tty55  ttyS30     vhost-net
i2c-4            net           tty21     tty56  ttyS31     vhost-vsock
i2c-5            ng0n1         tty22     tty57  ttyS4      zero
i2c-6            null          tty23     tty58  ttyS5      zfs
```

## UBUNTU SERIAL PORT PERMISSION

The library is ```pyserial``` if you try to access ports on Linux it will fail

```
Failed to open /dev/ttyS4: [Errno 13] could not open port /dev/ttyS4: [Errno 13] Permission denied: '/dev/ttyS4'
```

You need to give permission from your USER to the ports.

Commands let you see what the premission group is

Ubuntu Desktop

```
PermissionError: [Errno 13] Permission denied: '/dev/ttyS1'

sona@sona-lp-n100-8gb:~$ ls -l /dev/ttyS0
crw-rw---- 1 root dialout 4, 64 Sep 21 09:09 /dev/ttyS0
```

Ubuntu Server

```
sona@lpmu8gb:~$ ls -l /dev/ttyS0
crw------- 1 sona tty 4, 64 Nov 23 11:11 /dev/ttyS0
```

Permission for those ports is group dialout, so I add those permissions to my user

```
sudo usermod -a -G dialout $USER
sudo reboot
```

With that access to ports from user is allowed.

Remember that ```/dev/ttyS0``` is the port that Ubuntu uses for the serial console

## SERIAL PORT SPEED (Windows vs Linux)

Under windows I'm free to give the baud rate I want, I tested 250Kb/s.

Under Linux it behaves differently. There seems to be a discretization of baud for no good reasons. I know the Intel hardware can do 250Kb/s under windows, but under Linux if I request that it does 9600 b/s.

![](/Software/Demo_SBC_Serial_Communication/Images/2025-11-23-T1343%20Linux%209600%20baud.png)

I need to request likely from this table I found in pyserial for posix.

Right now I selected 115200b/s on Linux

![](/Software/Demo_SBC_Serial_Communication/Images/2025-11-23-T1354%20Linux%20115200%20baud.png)

```
#serialposix.py:96
BAUDRATE_CONSTANTS = {
            0:       0o000000,  # hang up
            50:      0o000001,
            75:      0o000002,
            110:     0o000003,
            134:     0o000004,
            150:     0o000005,
            200:     0o000006,
            300:     0o000007,
            600:     0o000010,
            1200:    0o000011,
            1800:    0o000012,
            2400:    0o000013,
            4800:    0o000014,
            9600:    0o000015,
            19200:   0o000016,
            38400:   0o000017,
            57600:   0o010001,
            115200:  0o010002,
            230400:  0o010003,
            460800:  0o010004,
            500000:  0o010005,
            576000:  0o010006,
            921600:  0o010007,
            1000000: 0o010010,
            1152000: 0o010011,
            1500000: 0o010012,
            2000000: 0o010013,
            2500000: 0o010014,
            3000000: 0o010015,
            3500000: 0o010016,
            4000000: 0o010017
        }
```

There is an issue with speed on the microcontroller, there I do have coarser discretization because of clock speed and UART hadware, I can select 113636 and it is compatible with 115200.

```
//56818
//#define UART_UBRR			23

//113636
#define UART_UBRR			10

//125000
//#define UART_UBRR			9

//138888
//#define UART_UBRR			8

//156 250
//#define UART_UBRR			7

//250 000 	
//#define UART_UBRR			4
```

It's 20MHz / 16 / (UBRR+1). There is a fast mode, setting the prescaler to 8, but it halves the sampling and can result in more errors.

I'll need to go faster for the final application, targeting 500Kb/s.

## LOOPBACK JUMPER

A trick to test the SBC communication is to do a jumper and do loopback, so it removes the speed mismatch. It's useful to debug if the problem is in software, in this case it wasn't.

![](/Software/Demo_SBC_Serial_Communication/Images/2025-11-23-T1324%20Test%20Loopback%20SBC%20ttyS4.jpg)

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

TODO: How do I clone just some of the code? Perhaps I do a release?

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




## DEMO 1 - ENUMERATE SERIAL PORTS

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

TODO: add documentation for the serial ports here?



## DEMO 2 - PORT SCAN

```
uv run demo_2_ask_signature.py 
```

Output if user lacks permission for dialout

```
$ uv run demo_2_ask_signature.py 
/home/sona/Demo 2 - Serial Communication SBC/demo_2_ask_signature.py:18: SyntaxWarning: "\D" is an invalid escape sequence. Such sequences will not work in the future. Did you mean "\\D"? A raw string is also an option.
  (ToB) F:\Data\Project\Project OrangeBot\RAMIE-RAD_AI_Messing_In_Earthworks\Software\Demo 1 - Serial Communication>python demo_2_ask_signature.py
Failed to open /dev/ttyS31: [Errno 13] could not open port /dev/ttyS31: [Errno 13] Permission denied: '/dev/ttyS31'
Failed to open /dev/ttyS30: [Errno 13] could not open port /dev/ttyS30: [Errno 13] Permission denied: '/dev/ttyS30'
Failed to open /dev/ttyS29: [Errno 13] could not open port /dev/ttyS29: [Errno 13] Permission denied: '/dev/ttyS29'
Failed to open /dev/ttyS28: [Errno 13] could not open port /dev/ttyS28: [Errno 13] Permission denied: '/dev/ttyS28'
Failed to open /dev/ttyS27: [Errno 13] could not open port /dev/ttyS27: [Errno 13] Permission denied: '/dev/ttyS27'
Failed to open /dev/ttyS26: [Errno 13] could not open port /dev/ttyS26: [Errno 13] Permission denied: '/dev/ttyS26'
Failed to open /dev/ttyS25: [Errno 13] could not open port /dev/ttyS25: [Errno 13] Permission denied: '/dev/ttyS25'
Failed to open /dev/ttyS24: [Errno 13] could not open port /dev/ttyS24: [Errno 13] Permission denied: '/dev/ttyS24'
Failed to open /dev/ttyS23: [Errno 13] could not open port /dev/ttyS23: [Errno 13] Permission denied: '/dev/ttyS23'
Failed to open /dev/ttyS22: [Errno 13] could not open port /dev/ttyS22: [Errno 13] Permission denied: '/dev/ttyS22'
Failed to open /dev/ttyS21: [Errno 13] could not open port /dev/ttyS21: [Errno 13] Permission denied: '/dev/ttyS21'
Failed to open /dev/ttyS20: [Errno 13] could not open port /dev/ttyS20: [Errno 13] Permission denied: '/dev/ttyS20'
Failed to open /dev/ttyS19: [Errno 13] could not open port /dev/ttyS19: [Errno 13] Permission denied: '/dev/ttyS19'
Failed to open /dev/ttyS18: [Errno 13] could not open port /dev/ttyS18: [Errno 13] Permission denied: '/dev/ttyS18'
Failed to open /dev/ttyS17: [Errno 13] could not open port /dev/ttyS17: [Errno 13] Permission denied: '/dev/ttyS17'
Failed to open /dev/ttyS16: [Errno 13] could not open port /dev/ttyS16: [Errno 13] Permission denied: '/dev/ttyS16'
Failed to open /dev/ttyS15: [Errno 13] could not open port /dev/ttyS15: [Errno 13] Permission denied: '/dev/ttyS15'
Failed to open /dev/ttyS14: [Errno 13] could not open port /dev/ttyS14: [Errno 13] Permission denied: '/dev/ttyS14'
Failed to open /dev/ttyS13: [Errno 13] could not open port /dev/ttyS13: [Errno 13] Permission denied: '/dev/ttyS13'
Failed to open /dev/ttyS12: [Errno 13] could not open port /dev/ttyS12: [Errno 13] Permission denied: '/dev/ttyS12'
Failed to open /dev/ttyS11: [Errno 13] could not open port /dev/ttyS11: [Errno 13] Permission denied: '/dev/ttyS11'
Failed to open /dev/ttyS10: [Errno 13] could not open port /dev/ttyS10: [Errno 13] Permission denied: '/dev/ttyS10'
Failed to open /dev/ttyS9: [Errno 13] could not open port /dev/ttyS9: [Errno 13] Permission denied: '/dev/ttyS9'
Failed to open /dev/ttyS8: [Errno 13] could not open port /dev/ttyS8: [Errno 13] Permission denied: '/dev/ttyS8'
Failed to open /dev/ttyS7: [Errno 13] could not open port /dev/ttyS7: [Errno 13] Permission denied: '/dev/ttyS7'
Failed to open /dev/ttyS3: [Errno 13] could not open port /dev/ttyS3: [Errno 13] Permission denied: '/dev/ttyS3'
Failed to open /dev/ttyS2: [Errno 13] could not open port /dev/ttyS2: [Errno 13] Permission denied: '/dev/ttyS2'
Failed to open /dev/ttyS1: [Errno 13] could not open port /dev/ttyS1: [Errno 13] Permission denied: '/dev/ttyS1'
Failed to open /dev/ttyS6: [Errno 13] could not open port /dev/ttyS6: [Errno 13] Permission denied: '/dev/ttyS6'
Failed to open /dev/ttyS5: [Errno 13] could not open port /dev/ttyS5: [Errno 13] Permission denied: '/dev/ttyS5'
Failed to open /dev/ttyS4: [Errno 13] could not open port /dev/ttyS4: [Errno 13] Permission denied: '/dev/ttyS4'
Testing port: /dev/ttyS0
```

Output if user has permission for dialout, but I had a problem of not receiving

```
sona@lpmu8gb:~$ cd Demo\ 2\ -\ Serial\ Communication\ SBC/
sona@lpmu8gb:~/Demo 2 - Serial Communication SBC$ uv run demo_2_ask_signature.py 
/home/sona/Demo 2 - Serial Communication SBC/demo_2_ask_signature.py:18: SyntaxWarning: "\D" is an invalid escape sequence. Such sequences will not work in the future. Did you mean "\\D"? A raw string is also an option.
  (ToB) F:\Data\Project\Project OrangeBot\RAMIE-RAD_AI_Messing_In_Earthworks\Software\Demo 1 - Serial Communication>python demo_2_ask_signature.py
Failed to open /dev/ttyS31: Could not configure port: (5, 'Input/output error')
Failed to open /dev/ttyS30: Could not configure port: (5, 'Input/output error')
Failed to open /dev/ttyS29: Could not configure port: (5, 'Input/output error')
Failed to open /dev/ttyS28: Could not configure port: (5, 'Input/output error')
Failed to open /dev/ttyS27: Could not configure port: (5, 'Input/output error')
Failed to open /dev/ttyS26: Could not configure port: (5, 'Input/output error')
Failed to open /dev/ttyS25: Could not configure port: (5, 'Input/output error')
Failed to open /dev/ttyS24: Could not configure port: (5, 'Input/output error')
Failed to open /dev/ttyS23: Could not configure port: (5, 'Input/output error')
Failed to open /dev/ttyS22: Could not configure port: (5, 'Input/output error')
Failed to open /dev/ttyS21: Could not configure port: (5, 'Input/output error')
Failed to open /dev/ttyS20: Could not configure port: (5, 'Input/output error')
Failed to open /dev/ttyS19: Could not configure port: (5, 'Input/output error')
Failed to open /dev/ttyS18: Could not configure port: (5, 'Input/output error')
Failed to open /dev/ttyS17: Could not configure port: (5, 'Input/output error')
Failed to open /dev/ttyS16: Could not configure port: (5, 'Input/output error')
Failed to open /dev/ttyS15: Could not configure port: (5, 'Input/output error')
Failed to open /dev/ttyS14: Could not configure port: (5, 'Input/output error')
Failed to open /dev/ttyS13: Could not configure port: (5, 'Input/output error')
Failed to open /dev/ttyS12: Could not configure port: (5, 'Input/output error')
Failed to open /dev/ttyS11: Could not configure port: (5, 'Input/output error')
Failed to open /dev/ttyS10: Could not configure port: (5, 'Input/output error')
Failed to open /dev/ttyS9: Could not configure port: (5, 'Input/output error')
Failed to open /dev/ttyS8: Could not configure port: (5, 'Input/output error')
Failed to open /dev/ttyS7: Could not configure port: (5, 'Input/output error')
Failed to open /dev/ttyS3: Could not configure port: (5, 'Input/output error')
Failed to open /dev/ttyS2: Could not configure port: (5, 'Input/output error')
Failed to open /dev/ttyS1: Could not configure port: (5, 'Input/output error')
Testing port: /dev/ttyS6
Testing port: /dev/ttyS5
Testing port: /dev/ttyS4
Testing port: /dev/ttyS0
```

## DEMO 3 - CONTINUOUS TRANSMISSION

I closed the cable with a jumper to test problems, no problems. It turns out it was speed under linux.

```
Response from /dev/ttyS4: b'SIGN\x00'
Testing port: /dev/ttyS4
Response from /dev/ttyS4: b'SIGN\x00'
Testing port: /dev/ttyS4
Response from /dev/ttyS4: b'SIGN\x00'
Testing port: /dev/ttyS4
Response from /dev/ttyS4: b'SIGN\x00'
Testing port: /dev/ttyS4
Response from /dev/ttyS4: b'SIGN\x00'
Testing port: /dev/ttyS4
Response from /dev/ttyS4: b'SIGN\x00'
Testing port: /dev/ttyS4
Response from /dev/ttyS4: b'SIGN\x00'
Testing port: /dev/ttyS4
Response from /dev/ttyS4: b'SIGN\x00'
Testing port: /dev/ttyS4
Response from /dev/ttyS4: b'SIGN\x00'
Testing port: /dev/ttyS4
```

Connected the electronics

I moved application to 115200 b/s
I moved the firmware to 113636 b/s
Now it communicates fine.
I'll need to significantly accelerate the communication for the future

```
esting port: /dev/ttyS4
Response from /dev/ttyS4: b'RAMIE\x00'
Testing port: /dev/ttyS4
Response from /dev/ttyS4: b'RAMIE\x00'
Testing port: /dev/ttyS4
Response from /dev/ttyS4: b'RAMIE\x00'
Testing port: /dev/ttyS4
```

# DEMO 4 - Direct speed


Works really well, it finds a serial port that answer with something, and uses that to communicate with the board

```
sona@lpmu8gb:~/Demo 2 - Serial Communication SBC$ uv run demo_4_serial_direct_speed.py 
Failed to open /dev/ttyS31: Could not configure port: (5, 'Input/output error')
Failed to open /dev/ttyS30: Could not configure port: (5, 'Input/output error')
Failed to open /dev/ttyS29: Could not configure port: (5, 'Input/output error')
Failed to open /dev/ttyS28: Could not configure port: (5, 'Input/output error')
Failed to open /dev/ttyS27: Could not configure port: (5, 'Input/output error')
Failed to open /dev/ttyS26: Could not configure port: (5, 'Input/output error')
Failed to open /dev/ttyS25: Could not configure port: (5, 'Input/output error')
Failed to open /dev/ttyS24: Could not configure port: (5, 'Input/output error')
Failed to open /dev/ttyS23: Could not configure port: (5, 'Input/output error')
Failed to open /dev/ttyS22: Could not configure port: (5, 'Input/output error')
Failed to open /dev/ttyS21: Could not configure port: (5, 'Input/output error')
Failed to open /dev/ttyS20: Could not configure port: (5, 'Input/output error')
Failed to open /dev/ttyS19: Could not configure port: (5, 'Input/output error')
Failed to open /dev/ttyS18: Could not configure port: (5, 'Input/output error')
Failed to open /dev/ttyS17: Could not configure port: (5, 'Input/output error')
Failed to open /dev/ttyS16: Could not configure port: (5, 'Input/output error')
Failed to open /dev/ttyS15: Could not configure port: (5, 'Input/output error')
Failed to open /dev/ttyS14: Could not configure port: (5, 'Input/output error')
Failed to open /dev/ttyS13: Could not configure port: (5, 'Input/output error')
Failed to open /dev/ttyS12: Could not configure port: (5, 'Input/output error')
Failed to open /dev/ttyS11: Could not configure port: (5, 'Input/output error')
Failed to open /dev/ttyS10: Could not configure port: (5, 'Input/output error')
Failed to open /dev/ttyS9: Could not configure port: (5, 'Input/output error')
Failed to open /dev/ttyS8: Could not configure port: (5, 'Input/output error')
Failed to open /dev/ttyS7: Could not configure port: (5, 'Input/output error')
Failed to open /dev/ttyS3: Could not configure port: (5, 'Input/output error')
Failed to open /dev/ttyS2: Could not configure port: (5, 'Input/output error')
Failed to open /dev/ttyS1: Could not configure port: (5, 'Input/output error')
Testing port: /dev/ttyS6
Testing port: /dev/ttyS5
Testing port: /dev/ttyS4
Response from /dev/ttyS4: b'RAMIE\x00'
Testing port: /dev/ttyS0

Responsive ports:
/dev/ttyS4
Connected to /dev/ttyS4. Press any key to stop...
Sent: b'VR1L0\x00'
Sent: b'VR2L0\x00'
Sent: b'VR3L0\x00'
Sent: b'VR4L0\x00'
Sent: b'VR5L0\x00'
Sent: b'VR6L0\x00'
Sent: b'VR7L0\x00'
Sent: b'VR8L0\x00'
Sent: b'VR9L0\x00'
Sent: b'VR10L0\x00'
Sent: b'VR10L0\x00'
Sent: b'VR9L0\x00'
Sent: b'VR8L0\x00'
Sent: b'VR7L0\x00'
Sent: b'VR6L0\x00'
Sent: b'VR5L0\x00'
Sent: b'VR4L0\x00'
Sent: b'VR3L0\x00'
Sent: b'VR2L0\x00'
Sent: b'VR1L0\x00'
Sent: b'VR0L0\x00'
Sent: b'VR-1L0\x00'
Sent: b'VR-2L0\x00'
```

![](/Software/Demo_SBC_Serial_Communication/Images/2025-11-23_14_11_IMG_20251123_141159.jpg)

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