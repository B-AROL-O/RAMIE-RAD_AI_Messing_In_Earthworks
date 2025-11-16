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

## Documentation

Designs marked with '*(not qualifying for hackaton)* ' are older recycled design that are here for documentation, instruction and completeness purposes and are not qualifying for the scope of the hackaton.  They are either taken as is, or lightly/heavily adapted to serve for the scope of the Hackaton.

### Mechanical Design

I design in OpenSCAD as I like making geometries as code and I like doing everything parametric.

I decided that the robots should be powered by 18650 batteries. Target configuration is 2S2P. For RAMIE I went with an inverted design that is soldered 4S1P to get 14.8V nominal voltage to be compatible with the input regulator for the Latte Panda Lite Board.

- [MEC: 18650 Battery Holder](/Mechanical/Battery%20Holder%2018650%20OpenSCAD) *(not qualifying for hackaton)*
- [MEC: Wheeled Platform](/Mechanical/Wheeled%20Platform%20OpenSCAD) *(not qualifying for hackaton)*

The robot platform is the one I am designing for Latte Panda Lite Board powered AI robots, it was a good fit for this hackaton

![](/Mechanical/Wheeled%20Platform%20OpenSCAD/Images/2025-11-08-T1437_Slicing.png)


### Board Motor Screen

Interfaces with a UART bidirectional link with the SBC, answer to a serial protocol, controls two servos and a LCD screen. Choosen because I had a bounch assembled in a drawer and working firmware to start from.

- [ELN: Board Motor Screen](/Electronics/Board%20motor%20screen) *(not qualifying for hackaton)*
- [FW: Board Motor Screen](/Firmware/Motor%20Board%20Atmel%20AT324PA) *(not qualifying for hackaton)*
- [FW: Test Bench Serial Communication](/Firmware/Test%20Bench%20Parser) *(not qualifying for hackaton)*
- [FW: Test Bench Motion Orchestrator](/Firmware/Test%20bench%20motion%20orchestration)
- [SW: Python demos to test communication between computer and board](/Software/Demo%201%20-%20Serial%20Communication)

![](/Electronics/Board%20motor%20screen/2025-11-15-T0928%20Motor%20Control.jpg)

### SBC Latte Panda Mu N100 16GB

I lost count of the SBCs I tested for my ultimate goal of making a general robot that expresses a degree of general intelligence with local compute only. The N100 16GB could be it, it has the memory, bandwidth, compute and power envelope to run useful LLMs on the go.


## Contributors

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