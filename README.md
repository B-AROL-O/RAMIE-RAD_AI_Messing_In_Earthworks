# RAMIE-RAD_AI_Messing_In_Earthworks

A local Gradio MCP server controlling a robot platform

## Scope

The scope of this repository is to document the Hugging Face MCP 1° birthday hackaton

The scope of this repository is to document all instruction to replicate in full the robot project

Work is divided in two parts:

- Work that is qualifying for the hackaton itself, that was started 2025-11-14, this includes learning GRADIO, learning MPC, preparing the MCP tools, interfacing ollama with AI models needed

- Work that is not qualifying for the hackaton and was recycled from previous projects and is in this repository in order to make a complete set of instructions to replicate this project. This include the electronics, the mechanical part of the robot, learning the compute board, and learning that was done for ollama testing of the microphone

This felt like a good chance to gather all the disparate documentation and work done over the years in one tidy set of insttructions that can be followed to make one complete platform end to end with the GRADIO/MCP/OLLAMA software being the keystone that glue it all together.

## Documentation

Designs marked with '*(not qualifying for hackaton)* ' are older recycled design that are here for documentation, instruction and completeness purposes and are not qualifying for the scope of the hackaton. 

### Board Motor Screen

Interfaces with a UART bidirectional link with the SBC, answer to a serial protocol, controls two servos and a LCD screen. Choosen because I had a bounch assembled in a drawer and working firmware to start from.

- [ELN: Board Motor Screen](/Electronics/Board%20motor%20screen) *(not qualifying for hackaton)*
- [FW: Board Motor Screen](/Firmware/Motor%20Board%20Atmel%20AT324PA) *(not qualifying for hackaton)*

### SBC Latte Panda Mu N100 16GB

I lost count of the SBCs I tested for my ultimate goal of making a general robot that expresses a degree of general intelligence with local compute only. The N100 16GB could be it, it has the memory, bandwidth, compute and power envelope to run useful LLMs on the go.

### Mechanical Design

I design in OpenSCAD as I like making code. I designed the 18650 Battery holder parametric design, and a platform for the Latte Panda Lite Board.

- [MEC: 18650 Battery Holder]() *(not qualifying for hackaton)*



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