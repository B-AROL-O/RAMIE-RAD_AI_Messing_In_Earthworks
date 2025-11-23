# Latte Panda Mu Lite Board

I have three modules

N100 8GB Active Cooling

N100 16GB Passive Cooling

N305 16GB (BRICKED Sent back to DF Robot)



---

# Power

| Voltage  | Current | Power | Note |
|-------------|-------------|--------------|-|
| 18V |  | STOP         |
| +10         | +10         | FORWARD 100mm/s          |
| -10         | -10         | BACKWARD 100mm/s          |
| -10         | +10         | COUNTERCLOCKWISE 180°/s |
| +10         | -10         | CLOCKWISE 180°/s |



# Battery Teleoperation SSH WiFI

Final test with the robot under battery power with SBC, AT324, WiFI and motors running with SSH teleoperation form keyboard

https://github.com/user-attachments/assets/86d634cf-5507-45de-8ebf-013d29c1c32a



## SETUP UART ACCESS TO TTYS0 DEBUG UART

Modify GRUB so that the operating system connects tothe serial port for access

```
cat /etc/default/grub
sudo nano /etc/default/grub
```

Edit the GRUB to make sure the following lines woirk

```
 GNU nano 7.2                                                                                                          
/etc/default/grub *
# If you change this file, run 'update-grub' afterwards to update
# /boot/grub/grub.cfg.
# For full documentation of the options in this file, see:
#   info -f grub -n 'Simple configuration'

GRUB_DEFAULT=0
GRUB_TIMEOUT_STYLE=hidden
GRUB_TIMEOUT=0
GRUB_DISTRIBUTOR=`( . /etc/os-release; echo ${NAME:-Ubuntu} ) 2>/dev/null || echo Ubuntu`

 #2025-11-22 Adding the serial port access via ttyS0 that I ound is the debug consol
GRUB_CMDLINE_LINUX_DEFAULT="console=ttyS0,115200n8"
GRUB_TERMINAL="console serial"
GRUB_SERIAL_COMMAND="serial --speed=115200 --unit=0 --word=8 --parity=no --stop=1"


# If your computer has multiple operating systems installed, then you
# probably want to run os-prober. However, if your computer is a host
# for guest OSes installed via LVM or raw disk devices, running
# os-prober can cause damage to those guest OSes as it mounts
# filesystems to look for things.
#GRUB_DISABLE_OS_PROBER=false

# Uncomment to enable BadRAM filtering, modify to suit your needs
# This works with Linux (no patch required) and with any kernel that obtains
# the memory map information from GRUB (GNU Mach, kernel of FreeBSD ...)
#GRUB_BADRAM="0x01234567,0xfefefefe,0x89abcdef,0xefefefef"

# Uncomment to disable graphical terminal
#GRUB_TERMINAL=console

# The resolution used on graphical terminal
# note that you can use only modes which your graphic card supports via VBE
# you can see them in real GRUB with the command `vbeinfo'
#GRUB_GFXMODE=640x480

# Uncomment if you don't want GRUB to pass "root=UUID=xxx" parameter to Linux
#GRUB_DISABLE_LINUX_UUID=true

# Uncomment to disable generation of recovery mode menu entries
#GRUB_DISABLE_RECOVERY="true"

# Uncomment to get a beep at grub start
#GRUB_INIT_TUNE="480 440 1"
```

Update grub, whatever that means

```
sudo update-grub
```

Log

```
sona@sona-lp-n100-8gb:~$ sudo update-grub
Sourcing file `/etc/default/grub'
Generating grub configuration file ...
Found linux image: /boot/vmlinuz-6.14.0-32-generic
Found initrd image: /boot/initrd.img-6.14.0-32-generic
Found linux image: /boot/vmlinuz-6.14.0-29-generic
Found initrd image: /boot/initrd.img-6.14.0-29-generic
Found memtest86+ 64bit EFI image: /boot/memtest86+x64.efi
Warning: os-prober will not be executed to detect other bootable partitions.
Systems on them will not be added to the GRUB boot configuration.
Check GRUB_DISABLE_OS_PROBER documentation entry.
Adding boot menu entry for UEFI Firmware Settings ...
done
```

Reboot and see if you bricked the OS


```
sudo reboot
```

So, it works and doesn't. On the screen I see ```unsupported port speed``` and on the serial terminal on the host I see that at 9600 I can see the dump

I redid the instruction setting the speed to 115200


---

#### 2025-11-22-T0900 Test Board 2

I powered bypassing the jack and the power button does not work

I connected jack, disconnected uC serial, and it takes a while to power on

I see nothing on the debug uart ttys0

Screen shows no sign of activity

A single click on the power button will shut down the SBC

Shutdown doesn't work



#### 2025-11-22-T1041 Test Board 1

Powered

Checksum error a few reboots

Powered on turn on screen

tested debug uart, see nothing


---