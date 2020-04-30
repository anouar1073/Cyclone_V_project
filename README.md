# Cyclone_V_project

## Structural form in the board
SW1: right blinker
SW2: left blinker
KEY1: fault 1
KEY2: fault 2
KEY3: fault 3
KEY4: Emergency
SW3: front position lights
SW4: rear position lights

HARDWARE:
* NIOS II processor 
* on-chip memory
* parallel I/O port for pushbuttons
* parallel I/O port for switches 
* JTAG peripheral ( for I/O operations with PC)
* parallel I/O port for LEDs

the program main.c works as follows:

  main() functions configures the different interrupts as well as the time, then it initialises the state 
  machine
  
  pushbutton_ISR(): responds to interrupts emitted from the pushbuttons
  switch_ISR(): responds to interrupts emitted from the switches
  
  posision(): controls position lights 


