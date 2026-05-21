# Memory Allocation Simulator

## Overview

Memory Allocation Simulator is a command-line C++ program that simulates how an operating system manages memory through virtual address spaces, paging, and a Memory Management Unit (MMU). It models a 64 MB physical memory space and lets users create and terminate processes, allocate and free typed variables, and write values to virtual memory addresses — all translated to physical memory through a configurable page table.

The project was developed as an OS concepts exercise and includes an MMU, a page table, and a REPL-style prompt for issuing memory commands.

## Features

* Simulated 64 MB physical memory backed by a raw byte array
* Configurable page size passed as a command-line argument
* Virtual-to-physical address translation via a page table
* MMU tracking of processes, variables, and virtual address spaces
* Support for multiple data types: char, short, int, float, long, and double
* Per-process memory layout with TEXT, GLOBALS, STACK, and heap segments
* Interactive REPL with commands to create, allocate, set, free, terminate, and print

## Technologies Used

* C++ (C++20)
* Custom MMU and page table implementation
* Linux build toolchain (g++, Make)

## Installation Instructions

1. Clone this repository: `git clone https://github.com/your-username/os-memsim.git`
2. Navigate into the project directory: `cd os-memsim`
3. Build the project: `make`

## Usage Instructions

1. Run the simulator with a page size argument: `./bin/memsim <page_size>`
2. Use the interactive prompt to issue memory commands:
   * `create <text_size> <data_size>` — initialize a new process
   * `allocate <PID> <var_name> <data_type> <num_elements>` — allocate a variable on the heap
   * `set <PID> <var_name> <offset> <value(s)>` — write values to a variable
   * `free <PID> <var_name>` — deallocate a variable
   * `terminate <PID>` — kill a process and release all its memory
   * `print mmu` — display the MMU memory table
   * `print page` — display the page table
   * `print processes` — list all active PIDs
3. Type `exit` to quit the simulator.
