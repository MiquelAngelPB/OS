# Experimental Operating System (OS)
This repository contains an experimental "operating system" project developed with the objective of learning the internal workings of operating systems and software at a low level.
Note that it is still in progress, and many features are incomplete, unstable, or subject to change.


## Current Features
- Custom x86 bootloader written in Assembly
- Basic 32-bit protected mode kernel
- Simple shell / command line interface (WIP)
<img width="45%" alt="Shell_Help" src="https://github.com/user-attachments/assets/4ab3064e-a07e-45ed-a65d-35845ea68c10" />
<img width="45%" alt="Terry_A_Davis" src="https://github.com/user-attachments/assets/24b3744b-af5d-4ec3-8852-72d6f9edae3c" />

- Minimal kernel API (prototype syscall-like interface)
- Other programs:

  - Raycasting engine (WIP)
<img width="50%" alt="Raycaster" src="https://github.com/user-attachments/assets/f1d2cc60-9200-445e-ae9c-ab84384d2aa5" />

  - Text editor (WIP)
<img width="50%" alt="Text" src="https://github.com/user-attachments/assets/70a7ef21-91bc-4056-9313-9f2e79d5cfd2" />

Running and testing is currently done with QEMU.

*(More details will be added as the project evolves).*

## Motivation
The purpose of this project is to:
- Experiment with bootloaders, kernel development, and low-level hardware interaction
- Explore x86 architecture and bare-metal programming
- Practice programming in assembly and C

## Disclaimer
This project is not intended to be an example of professional-grade code, it does not necessarily follow standard conventions, best practices, or clean architecture principles.
The project was created as a personal learning exercise, please do not consider this repository as a reference for production systems, security-critical software, ect.
