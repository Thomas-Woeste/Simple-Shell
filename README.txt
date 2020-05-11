Author: Thomas Woeste
Email: tewo224@uky.edu
Date: 4/23/2020


Contents:
shell.cc: This is the implementation of the simple shell and its helper functions
Makefile: This is used to build your project. 
shell.h: Holds the definition of the command structure, and prototypes for the provided functions parse_command and free_command
parser.c: This file holds the implementations of parse_command and free_command functions used in shell.cc
test/: A directory of test inputs, expected outputs, and helper commands.
run-tests: This is a sh script to run the test and comapare your shell's output with the expected output. 


Running:
To run the program download all the provided files and place them in the same folder. Type make in the command line.
Then type ./shell if you want to hand use the shell. Type ./run-tests if you want to test the program versus the provided
tests.

Implementation Notes: 
Functions that were responsible for executing commands were the execvp() command which was used to execute the external commands that
were redirected. The chdir()  system call was used to change the directory in the program, the setenv() was used to set the environment
variable. The exit() command was used to to exit from system calls.

Limitations:
One limitation with this program was that there was a maximun of 30 arguments were allowed to be passed in to be parsed. 

The program did not fail any test cases that I saw or from any of my testing cases. 

Refrences:
I worked with Robert Bennet and Austin Arnold on the concepts that were presented in the videos like the built in CD command other similar. 

I helped Caleb Lytle fixing his cd command line arguments. 

I got the majority of the style of this program from the videos posted by Dr. Moore on youtube. 

Implementations of functions were aided by their man pages. 



