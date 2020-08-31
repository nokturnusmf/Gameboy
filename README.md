# Gameboy

A simple Gameboy and Gameboy Colour emulator.

Compatability is mixed; many games work almost perfectly while others may not work at all.


### Running

The program should be run from the command line with the path to the ROM as the first argument.


### Controls

Gameboy | Keyboard
--------|---------
A       | Space
B       | Left Shift
DPad    | WASD
Start   | Enter
Select  | Backspace


### Building

Ensure GLFW and GLEW are installed.

Running `make` will build an optimised release version. `make debug` can instead be used to build an unoptimised version suitable for debugging; `make all` will build both.

Finally, `make clean` can be used to remove all generated executables and object files.


### Screenshots

![Pokemon Gold](https://maf27.host.cs.st-andrews.ac.uk/Gameboy/Gold.png)
![Pokemon Blue](https://maf27.host.cs.st-andrews.ac.uk/Gameboy/Blue.png)
![Link's Awakening](https://maf27.host.cs.st-andrews.ac.uk/Gameboy/Zelda.png)
![Tetris](https://maf27.host.cs.st-andrews.ac.uk/Gameboy/Tetris.png)
