# VS Code

Use VS code on host machine with remote SSH plugin to evelop and run code directly on the SBC using the WiFi radio

## SBC IP address

The SBC needs to be in the same subnet as the host machine

There are many ways to find the IP addres of the SBC
- TODO: I want to display the IP address of the interfaces on the microcontroller interface
- On the router find out the DHCP assignment
- Use the debug serial interface TTYS0 connect with putty and find the address with the command ```ip a```

## VS Code Host

Use Remote SSH extension

![](/Software/VS%20Code%20Remote%20Development%20on%20SBC/Images/2025-11-23-T0945%20Extension.png)


There will be a button on bottom left ```><``` -> on top center ```Connect to host``` -> ```sona@192.168.1.227```

![](/Software/VS%20Code%20Remote%20Development%20on%20SBC/Images/2025-11-23-T0942%20Connect%20to%20Host.png)

Once connected select ```Linux``` as platform

Then on the center page option open folder and select the home folder of the SBC, ```Trust Authors```

![](/Software/VS%20Code%20Remote%20Development%20on%20SBC/Images/2025-11-23-T0956%20Connected%20to%20SBC%20Home.png)

You are now connected to the SBC and can do remote development and launch applications directly on the SBC from the host
