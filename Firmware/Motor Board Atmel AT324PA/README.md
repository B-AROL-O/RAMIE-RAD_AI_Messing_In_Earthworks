# Firmware Board Motor Screen

I decided to do an MVP without anything fancy.

This firmware takes commands from the UART interface and controls a screen and two continuous rotation PPM servomotors.

## IDE and Programmer

This firmware is uploaded to the Motor Board Screen via Risp MKII programmer. It is an older IDE that works fine as Atmel was acquired by Microchip and slowly the products were merged.

- [IDE Atmel Microchip Studio](https://www.microchip.com/en-us/tools-resources/develop/microchip-studio)
- [Programmer AVRISP MKII](https://www.microchip.com/en-us/development-tool/atavrisp2)


## PIN Used

```cpp
//PA0			: PPM Wheel Right
//PA1			: PPM Wheel Right

//PC0			: LED#
//PC1			: LCD_PWR#
//PC2			: LCD_EN
//PC3			: LCD_RS
//PC4			: LCD_D4
//PC5			: LCD_D5
//PC6			: LCD_D6
//PC7			: LCD_D7

//PD0			: UC_RXI SBC_TXO 250Kbs
//PD1			: UC_TXO SBC_RXI 250Kbs
```

## COMMUNICATION PROTOCOL

TODO: Details the commands properly with examples

```cpp
//Ping: No action. Effect is to reset the connection timeout
UART_CMD_PING		, 'P', '\0',
//Sign: Ask for board signature
UART_CMD_SIGN		, 'F', '\0',
//Sign: Ask for firmware revision
UART_CMD_REVISION	, 'REV', '\0',
//STOP: set speed 0 to the motors
UART_CMD_STOP		, 'STOP', '\0',
//VELOCITY: R right engine L left engine (last forever until overwritten)
UART_CMD_SET_VELOCITY     , 'V', 'R', '%', 's', 'L', '%', 's', '\0',
//VELOCITY TIMED: R right engine L left engine T time of the motion
UART_CMD_SET_VELOCITY_TIMED , 'V', 'R', '%', 's', 'L', '%', 's', 'T', '%', 'u', \0',
//Dictionary terminator
'\0'
```