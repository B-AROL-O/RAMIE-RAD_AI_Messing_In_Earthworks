# TTYS0 SSH

Configure the UART Debug port ```ttys0``` as serial terminal so I can connect with Putty

## ELECTRICAL CONNECTION

I soldered an adapter

![](/Electronics/TTYS0%20PUTTY/2025-11-22-T0951%20Build%20Adapter.jpg)

Connected to the debug port marked ```Uart Debug``` Note that it has the wires in different order to the other UART ports above

![](/Electronics/TTYS0%20PUTTY/2025-11-22-T1001%20TTYS0%20Connected.jpg)

Use an FTDI to attach to USB

![](/Electronics/TTYS0%20PUTTY/2025-11-22-T1002%20FTDI%20USB%20to%20UART%20adapter.jpg)

## MODIFY GRUB

The grub is what setup the debug port as terminal

### READ GRUB

```GRUB_CMDLINE_LINUX_DEFAULT=""``` tells us that it is not configured

```
sona@lpmu8gb:~$ cat /etc/default/grub
# If you change this file, run 'update-grub' afterwards to update
# /boot/grub/grub.cfg.
# For full documentation of the options in this file, see:
#   info -f grub -n 'Simple configuration'

GRUB_DEFAULT=0
GRUB_TIMEOUT_STYLE=hidden
GRUB_TIMEOUT=0
GRUB_DISTRIBUTOR=`( . /etc/os-release; echo ${NAME:-Ubuntu} ) 2>/dev/null || echo Ubuntu`
GRUB_CMDLINE_LINUX_DEFAULT=""
GRUB_CMDLINE_LINUX=""

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

### CHANGE GRUB TO ACTIVATE SERIAL AS TERMINAL

```
sona@lpmu8gb:~$ sudo nano /etc/default/grub
[sudo] password for sona:
sona@lpmu8gb:~$ cat /etc/default/grub
# If you change this file, run 'update-grub' afterwards to update
# /boot/grub/grub.cfg.
# For full documentation of the options in this file, see:
#   info -f grub -n 'Simple configuration'

GRUB_DEFAULT=0
GRUB_TIMEOUT_STYLE=hidden
GRUB_TIMEOUT=0
GRUB_DISTRIBUTOR=`( . /etc/os-release; echo ${NAME:-Ubuntu} ) 2>/dev/null || echo Ubuntu`
#2025-11-22 Setup ttyS0 as serial terminal on debug port at 115200 baud rate
#higher speed not supported
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

### APPLY GRUB

Activate terminal, and listen to the output

Apply GRUB and reboot

```
sona@lpmu8gb:~$ sudo update-grub
[sudo] password for sona:
Sourcing file `/etc/default/grub'
Generating grub configuration file ...
Found linux image: /boot/vmlinuz-6.8.0-88-generic
Found initrd image: /boot/initrd.img-6.8.0-88-generic
Warning: os-prober will not be executed to detect other bootable partitions.
Systems on them will not be added to the GRUB boot configuration.
Check GRUB_DISABLE_OS_PROBER documentation entry.
Adding boot menu entry for UEFI Firmware Settings ...
done
sona@lpmu8gb:~$ sudo reboot

Broadcast message from root@lpmu8gb on pts/0 (Sat 2025-11-22 12:45:47 UTC):

The system will reboot now!

sona@lpmu8gb:~$
```

![](/Software/ttys0%20SSH/Images/2025-11-22-T1346%20Terminal%20Listen%20TTYS0.png)


### PUTTY CONNECTS VIA SERIAL

Remember to disconnect from terminal from the serial interface

CRIT SUCCESS! Now I can connect to the Latte Panda via serial interface via putty. This should mitigate the network getting bricked.

![](/Software/ttys0%20SSH/Images/2025-11-22T1349%20Putty%20Configuration.png)

![](/Software/ttys0%20SSH/Images/2025-11-22-T1349%20Putty%20Connection.png)



