# Operating System

Using Ubuntu 24.04 LTS

## UART TERMINAL

Ubuntu is hooked to a serial port to do the UART terminal, it is more reliable than screen, keyboard ethernet and wifi

```

```







# CONFIGURE NETWORK

For reasons I do not understand the network interfaces change name and stop working...

Powered

![](/Software/Image/2025-11-19-T1622_ramie_powered.jpg)

I had to plug screen and keyboard

![](/Software/Image/2025-11-19-T1620_configure_network.jpg)

The trick is to add a line to ```/etc/netplan/50-cloud-init.yaml```

```
sona@lpn10016gb:~$ ip a
1: lo: <LOOPBACK,UP,LOWER_UP> mtu 65536 qdisc noqueue state UNKNOWN group defaul                                                    t qlen 1000
    link/loopback 00:00:00:00:00:00 brd 00:00:00:00:00:00
    inet 127.0.0.1/8 scope host lo
       valid_lft forever preferred_lft forever
    inet6 ::1/128 scope host noprefixroute
       valid_lft forever preferred_lft forever
2: enp3s0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc fq_codel state UP gr                                                    oup default qlen 1000
    link/ether d2:f7:f7:c0:0c:13 brd ff:ff:ff:ff:ff:ff
    inet 192.168.1.240/23 metric 100 brd 192.168.1.255 scope global dynamic enp3                                                    s0
       valid_lft 99850sec preferred_lft 99850sec
    inet6 fe80::d0f7:f7ff:fec0:c13/64 scope link
       valid_lft forever preferred_lft forever
3: wlp2s0: <BROADCAST,MULTICAST> mtu 1500 qdisc noop state DOWN group default ql                                                    en 1000
    link/ether 10:b1:df:bd:b3:b9 brd ff:ff:ff:ff:ff:ff
sona@lpn10016gb:~$ sudo cat /etc/netplan/50-cloud-init.yaml
[sudo] password for sona:
network:
  version: 2
  ethernets:
    enp3s0:
      dhcp4: true
```

# CONFIGURE WIFI

```
sona@lpn10016gb:~$ ip a
1: lo: <LOOPBACK,UP,LOWER_UP> mtu 65536 qdisc noqueue state UNKNOWN group default qlen 1000
    link/loopback 00:00:00:00:00:00 brd 00:00:00:00:00:00
    inet 127.0.0.1/8 scope host lo
       valid_lft forever preferred_lft forever
    inet6 ::1/128 scope host noprefixroute
       valid_lft forever preferred_lft forever
2: enp3s0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc fq_codel state UP group default qlen 1000
    link/ether d2:f7:f7:c0:0c:13 brd ff:ff:ff:ff:ff:ff
    inet 192.168.1.240/23 metric 100 brd 192.168.1.255 scope global dynamic enp3s0
       valid_lft 97509sec preferred_lft 97509sec
    inet6 fe80::d0f7:f7ff:fec0:c13/64 scope link
       valid_lft forever preferred_lft forever
3: wlp2s0: <BROADCAST,MULTICAST> mtu 1500 qdisc noop state DOWN group default qlen 1000
    link/ether 10:b1:df:bd:b3:b9 brd ff:ff:ff:ff:ff:ff
sona@lpn10016gb:~$ ^C
sona@lpn10016gb:~$ ls
test_microphone
sona@lpn10016gb:~$ ^C
sona@lpn10016gb:~$ sudo nano /etc/netplan/50-cloud-init.yaml
[sudo] password for sona:
sona@lpn10016gb:~$ sudo cat /etc/netplan/50-cloud-init.yaml
network:
  version: 2
  ethernets:
    enp3s0:
      dhcp4: true
  wifis:
    wlp2s0:
      dhcp4: true
      access-points:
        "xxx":
          password: "yyy"
```

Doesn't work, so I search for debug

```
sudo netplan --debug generate

```

Apply without rebooting

```
sudo netplan apply
sudo systemctl restart NetworkManager
```

Of course it bricked the network -.-


### Luckfox PicoKVM

I always have trouble accessing SBC headless, I finally got a 49€ KVM to mitigate the issue and free myself from using discrete keyboard and mouse

[Luckfox PicoKVM](/Software/Luckfox%20PicoKVM/README.md)
