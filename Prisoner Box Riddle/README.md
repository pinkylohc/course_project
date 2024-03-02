# Riddle Room Simulation

This repository contains code that simulates a riddle room scenario involving prisoners and boxes.

## Description

The code includes functions to simulate a riddle room, calculate statistics for the room, calculate success rates, and implement a "nice guard" intervention.

## Functions

- `simulateRoom`: Simulates the riddle room scenario. Takes an array of boxes, number of prisoners, and number of trials as input. Returns true if all prisoners are successful, and false otherwise.

- `statsRoom`: Calculates and displays statistics for a riddle room. Counts the number of prisoners who find their slips, the number of loops in the room, the smallest and longest loop lengths, and the contents of the longest loop.

- `successRooms`: Calculates the success rate of 1000 riddle rooms. Returns the ratio of successful rooms to the total number of rooms.

- `niceGuard`: Implements a "nice guard" intervention to help prisoners escape from rooms with loops longer than the number of trials.

## Usage

- 'Input':
The program will prompt you to enter the number of prisoners, the number of boxes each prisoner can open, and the seed for randomization. The number of prisoners should be between 1 and 1000, the number of boxes should be between 1 and 1000, and the seed can be any non-negative integer. If you enter 0 as the seed, the system will randomize it for you.

- 'Options':
Once the program is running, you will be presented with several options to choose from. Each option corresponds to a specific functionality of the program. Enter the number corresponding to the desired option to proceed.

- 'Output':
The program will display different outputs depending on the chosen option. For example, if you select the option to print the boxes, the program will show the contents of the boxes. If you choose to simulate a room, the program will indicate whether all prisoners were able to find their slip or not. The specific output for each option will be explained in the program's execution.
