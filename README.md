---
title: RAMIE-RAD AI Messing In Earthworks
emoji: 🐨
colorFrom: gray
colorTo: pink
sdk: gradio
sdk_version: 5.49.1
app_file: app.py
pinned: false
license: cc-by-sa-4.0
short_description: A local Gradio MCP server controlling a robot platform
tags:
  - mcp-in-action-track-creative
---


# RAMIE

- **Radical**
- **AI**
- **Messing**
- **In**
- **Earthworks**

A local Gradio MCP AI powered wheeled robot, capable of listening, speaking and executing commands, in the vision of the B-AROL-O team

*TRIVIA: The B-AROL-O team likes to use names of Italian wines as acronyms for robots. [RAMIE](https://nebbiaeluce.com/producer/ramie-perosa-argentina/) is a local red wine from my village*

*TRIVIA: "Vino" is the Italian word for Wine. Intel OpenVINO AI runtime is the perfect runtime to run a robot named after an italian wine by italian developers*

## Scope

The scope of this repository is to document the Hugging Face MCP 1° birthday hackaton

The scope of this repository is to document all instruction to replicate in full the robot project

Work is divided in two parts:

- Work that is qualifying for the hackaton itself, that was started 2025-11-14, this includes learning GRADIO, learning MPC, preparing the MCP tools, interfacing ollama with AI models needed

- Work that is not qualifying for the hackaton and was recycled from previous projects and is in this repository in order to make a complete set of instructions to replicate this project. This include the electronics, the mechanical part of the robot, learning the compute board, and learning that was done for ollama testing of the microphone

This felt like a good chance to gather all the disparate documentation and work done over the years in one tidy set of insttructions that can be followed to make one complete platform end to end with the GRADIO/MCP/OLLAMA software being the keystone that glue it all together.

# Documentation

Designs marked with '*(not qualifying for hackaton)* ' are older recycled design that are here for documentation, instruction and completeness purposes and are not qualifying for the scope of the hackaton.  They are either taken as is, or lightly/heavily adapted to serve for the scope of the Hackaton.

## Mechanical Design

I design in OpenSCAD as I like making geometries as code and I like doing everything parametric.

I decided that the robots should be powered by 18650 batteries. Target configuration is 2S2P. For RAMIE I went with an inverted design that is soldered 4S1P to get 14.8V nominal voltage to be compatible with the input regulator for the Latte Panda Lite Board.

- [MEC: 18650 Battery Holder](/Mechanical/Battery%20Holder%2018650%20OpenSCAD) *(not qualifying for hackaton)*
- [MEC: Wheeled Platform](/Mechanical/Wheeled%20Platform%20OpenSCAD) *(not qualifying for hackaton)*

The robot platform is the one I am designing for Latte Panda Lite Board powered AI robots, it was a good fit for this hackaton

![](/Mechanical/Wheeled%20Platform%20OpenSCAD/Images/2025-11-08-T1437_Slicing.png)

 
## Board Motor Screen

Interfaces with a UART bidirectional link with the SBC, answer to a serial protocol, controls two servos and a LCD screen. Choosen because I had a bounch assembled in a drawer and working firmware to start from.

- [ELN: Board Motor Screen](/Electronics/Board%20motor%20screen) *(not qualifying for hackaton)*
- [FW: Board Motor Screen](/Firmware/Motor%20Board%20Atmel%20AT324PA) *(not qualifying for hackaton)*
- [FW: Test Bench Serial Communication](/Firmware/Test%20Bench%20Parser) *(not qualifying for hackaton)*
- [FW: Test Bench Motion Orchestrator](/Firmware/Test%20bench%20motion%20orchestration)
- [SW: Python demos to test communication between Host computer and board](/Software/Demo_Host_Serial_Communication/)
- [SW: Python demos to test communication between SBC and board](/Software/Demo_SBC_Serial_Communication/)

![](/Firmware/Images/2025-11-16_13_41_IMG_20251116_134154.jpg)

# SBC Latte Panda Mu N100 16GB

I lost count of the SBCs I tested for my ultimate goal of making a general robot that expresses a degree of general intelligence with local compute only. The N100 16GB could be it, it has the memory, bandwidth, compute and power envelope to run useful LLMs on the go.

![](/Electronics/Images/2025-11-19-T1545_electronics_latte_panda.jpg)

## Luckfox PicoKVM

I always have trouble accessing SBC headless, I finally got a 49€ KVM to mitigate the issue and free myself from using discrete keyboard and mouse

[Luckfox PicoKVM Instructions](/Software/Luckfox%20PicoKVM)

![](/Software/Luckfox%20PicoKVM/Images/2025-11-22_11_11_IMG_20251122_111122.jpg)

## INSTALL UBUNTU

Install Ubuntu 24.04 Server LTS on the SBC and configure network and storage

[Ubuntu 24.04 Server Install Instructions](/Software/Ubuntu%2024_04%20server%20LTS)

![](/Software/Ubuntu%2024_04%20server%20LTS/Images/2025-11-22-T1311%20Test%20ping%20dev%20drive%20network.png)

## Luckfox PicoKVM

I always have trouble accessing SBC headless, I finally got a 49€ KVM to mitigate the issue and free myself from using discrete keyboard and mouse

[Luckfox PicoKVM Instructions](/Software/Luckfox%20PicoKVM)

![](/Software/Luckfox%20PicoKVM/Images/2025-11-22_11_11_IMG_20251122_111122.jpg)

## TTYS0 SSH PUTTY SERIAL

Configure the GRUB to use ttyS0 as UART serial interface to let the host connect via putty via USB to UART FTDI adapter

[TTYS0 SERIAL GRUB CONFIGURATION](/Software/ttys0%20SSH)

![](/Electronics/TTYS0%20PUTTY/2025-11-22-T1001%20TTYS0%20Connected.jpg)

## VS Code Remote

Use VS code on host machine with remote SSH plugin to evelop and run code directly on the SBC via WiFi radio

[VS Code Remote Instructions](/Software/VS%20Code%20Remote%20Development%20on%20SBC)

![](/Software/VS%20Code%20Remote%20Development%20on%20SBC/Images/2025-11-23-T0956%20Connected%20to%20SBC%20Home.png)


# Battery Teleoperation SSH WiFI

Final test with the robot under battery power with SBC, AT324, WiFI and motors running with SSH teleoperation form keyboard

https://github.com/user-attachments/assets/86d634cf-5507-45de-8ebf-013d29c1c32a


# Ollama GRADIO LLamaindex

The robot hosts and run locally GRADIO

There are two chats

One is the chat to ask about RAMIE capability using LLamaindex to rag the specifications

The second has a tool that attaches with a MCP tool to the motors and genereate serial command for the robot to execute

![](/Software/Demo_ollama/Images/2025-11-29-T1731%20GRADIO%20Interface.png)

![](/Software/Demo_ollama/Images/2025-11-29-T1740%20HTOP.png)







# Contributors

- [Orso Eric](https://github.com/OrsoEric)

## Useful Links

- [Huggingface Hackaton](https://huggingface.co/MCP-1st-Birthday) 
- [Huggingface Example Track 2 Creative](https://huggingface.co/spaces/ysharma/sample5-hackathon-space-submission/blob/main/README.md)
- [Huggingface space](https://huggingface.co/spaces/MCP-1st-Birthday/RAMIE-RAD_AI_Messing_In_Earthworks)
- [Huggingface Organization](https://huggingface.co/RAMIE-RAD-AI-Messing-In-Earthworks)
- [Github repository](https://github.com/B-AROL-O/RAMIE-RAD_AI_Messing_In_Earthworks)
- [Github project board](https://github.com/orgs/B-AROL-O/projects/5)

## TRACK

Track 2: MCP in Action

mcp-in-action-track-creative





# Copyright and License

Copyright (C) 2025, B-AROL-O Team, all rights reserved.

NOTE: This repository is based upon https://github.com/arol-polito/python-project-template.
Source code license

The source code contained in this repository and the executable distributions are licensed under the terms of the MIT license as detailed in the LICENSE file.
Documentation license

CC BY-SA 4.0

Please note that your contribution to the project Documentation is licensed under a Creative Commons Attribution-Share Alike 4.0 License. see https://creativecommons.org/licenses/by-sa/4.0/