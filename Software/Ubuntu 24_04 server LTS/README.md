# INSTALL UBUNTU

Install Ubuntu 24.04 LTS and setup network and drives for success

## BURN USB INSTALLATION MEDIA

![](/Software/Ubuntu%2024_04%20server%20LTS/Images/2025-11-22-T1149%20Installation%20Media.jpg)

## POWER

- Connect KVM
- Power on
- Push power button

![](/Software/Ubuntu%2024_04%20server%20LTS/Images/2025-11-22-T1149%20Power%20Latte%20Panda.jpg)

## BOOT ORDER

Access bios with ```del``` key

![](/Software/Ubuntu%2024_04%20server%20LTS/Images/2025-11-22-T1147%20Access%20Bios.png)

Check hardware, there should be the SSD and WiFi radio, here you can check ram configuration and more.

![](/Software/Ubuntu%2024_04%20server%20LTS/Images/2025-11-22-T1148%20NVME%20storage.png)

Change boot order to put the installation media first

![](/Software/Ubuntu%2024_04%20server%20LTS/Images/2025-11-22-T1148%20Bios%20Boot%20Order.png)

Save and exit with ```F10``` key yes and enter

System will reboot

## INSTALL WIZARD

It'll ask you a number of options, select ```try or install ubuntu server```

Language screen, pick english

![](/Software/Ubuntu%2024_04%20server%20LTS/Images/2025-11-22-T115017%20Wizard%20Language.png)

Driver screen, tell ubuntu it can use third party drivers. It's an option for open source purists I don't care about.

![](/Software/Ubuntu%2024_04%20server%20LTS/Images/2025-11-22-T1150%20Wizard%20Drivers.png)

## INSTALL WIZARD - NETWORK (CRITICAL)

This is really important, Ubuntu has severe deficiencies.

First make sure the hardware is set, if you add or remove radios, it'll change names and brick everything.

Also make sure you set it right here, if you try to modify cloud init, it'll brick the interfaces, or worse.

It has to be done once, here and right, and never change.

It should list the embedded ethernet interface, and the addon wifi radio

![](/Software/Ubuntu%2024_04%20server%20LTS/Images/2025-11-22-T1151%20Wizard%20Network.png)

On the wifi menu, you can list network SSID to make sure it's working. Select

![](/Software/Ubuntu%2024_04%20server%20LTS/Images/2025-11-22-T1152%20Wizard%20Network%20SSID.png)

Punch in credentials, the KVM has a paste field to make that convenient

![](/Software/Ubuntu%2024_04%20server%20LTS/Images/2025-11-22-T1152%20Wizard%20Network%20SSID%20B.png)

Wait a few seconds, and it should show you the IP address if everything worked

![](/Software/Ubuntu%2024_04%20server%20LTS/Images/2025-11-22-T1153%20Wizard%20Network%20SSID%20C.png)

If you got here, this is good, it should save lots of pain later

## INSTALL WIZARD - FOLDER STRUCTURE (CRITICAL)

This is really important

- we want the operating system on the EMCC module that is super fast.
- We want an external disk to be mounted so that I can dump LLM models and big stulff in there

First step it'll ask you to format the external disk, leave as one big chunk without LVM (not really sure what it does, virtual disks or something)

![](/Software/Ubuntu%2024_04%20server%20LTS/Images/2025-11-22-T1155%20Wizard%20Disk%20LVM%20NO.png)

The default folder structure is a MESS. It uses the big disk as root and isn't mounting the EMCC disk

![](/Software/Ubuntu%2024_04%20server%20LTS/Images/2025-11-22-T1155%20Wizard%20Disk%20Default%20Mess.png)

Go to root and unmount it, then go to the big disk, format it as ext 4 and mount it as mnt ext-data

![](/Software/Ubuntu%2024_04%20server%20LTS/Images/2025-11-22-T1201%20Wizard%20Disk%20Unmount%20root.png)

Carefully, CAREFULLY do it for the three folder structures. EFI you can leave as is, but is paramount the EMMC is for root, and the big drive is mounted in mnt as something sensible like ```ext-drive```

I tried to do this inside the OS, and uses a long hash and is a mess, I would need to learn to do it properly, but the wizard seems to already do so properly, get it right here. 

![](/Software/Ubuntu%2024_04%20server%20LTS/Images/2025-11-22-T1201%20Wizard%20Disk%20Mount%20Right.png)

If the folder structure is nailed down, continue forward.

![](/Software/Ubuntu%2024_04%20server%20LTS/Images/2025-11-22-T1201%20Wizard%20Disk%20Done.png)

## INSTALL WIZARD - HOSTNAME AND USER

Now give the machine a sensible hostname, and a username and password

![](/Software/Ubuntu%2024_04%20server%20LTS/Images/2025-11-22-T1202%20Wizard%20Hostname%20User.png)



## INSTALL WIZARD - FINISH AND REBOOT

Activate openssh

![](/Software/Ubuntu%2024_04%20server%20LTS/Images/2025-11-22-T1202%20Wizard%20SSH.png)

Third Party Drivers

![](/Software/Ubuntu%2024_04%20server%20LTS/Images/2025-11-22-T1202%20Wizard%20Third%20Party%20Drivers.png)

This page lets you install snap of common applications, so far I don't need anything from here

![](/Software/Ubuntu%2024_04%20server%20LTS/Images/2025-11-22-T1203%20Wizard%20Snap.png)

Installing

![](/Software/Ubuntu%2024_04%20server%20LTS/Images/2025-11-22T1204%20Wizard%20Install%20Progress.png)

When done, it'll add a reboot button, press it

![](/Software/Ubuntu%2024_04%20server%20LTS/Images/2025-11-22T1208%20Wizard%20Install%20Done.png)

It'll ask to remove the install media, do it and continue

![](/Software/Ubuntu%2024_04%20server%20LTS/Images/2025-11-22T1212%20Wizard%20Remove%20Media%20Reboot.png)


## LOGIN AND TEST PERIPHERALS

Login with your user

Test several things. Internet connection, external drive, dev tree, networks

```
ls /mnt
ls /dev
ip a
ping www.google.com
```


![](/Software/Ubuntu%2024_04%20server%20LTS/Images/2025-11-22-T1311%20Test%20ping%20dev%20drive%20network.png)


## UPDATE AND UPGRADE

```
sudo apt update
sudo apt upgrade
```

![](/Software/Ubuntu%2024_04%20server%20LTS/Images/2025-11-22-T1222%20AptUpdate%20Upgrade.png)
