# Riddle Room Simulation

This repository contains code that simulates a riddle room scenario involving prisoners and boxes.

## Description

The code includes functions to simulate a riddle room, calculate statistics for the room, calculate success rates, and implement a "nice guard" intervention.

## Functions

- `simulateRoom`: Simulates the riddle room scenario. Takes an array of boxes, number of prisoners, and number of trials as input. Returns true if all prisoners are successful, and false otherwise.

- `statsRoom`: Calculates and displays statistics for a riddle room. Counts the number of prisoners who find their slips, the number of loops in the room, the smallest and longest loop lengths, and the contents of the longest loop.

- `successRooms`: Calculates the success rate of 1000 riddle rooms. Returns the ratio of successful rooms to the total number of rooms.

- `niceGuard`: Implements a "nice guard" intervention to help prisoners escape from rooms with loops longer than the number of trials.
