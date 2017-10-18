### PA7 Collaborators
Jacob Lee
Cayler Miley
Austin Emery

### The buttons
```bash

V : Will change the view from top down to selection 
C : Closer top down view
- : Slows down simulation
+ : Speeds up simulation
Left Arrow Key: changes planets in selection mode
Right Arrow Key: changes planets in selection mode


ESC : Will end the program
```

### Use cmake to make this project

```bash
mkdir build
cd build
cmake ..
make
./PA7
```


# PA7: SOLAR SYSTEM

```bash
The objects and textures can be changed in the config file
```

# Dependencies, Building, and Running

## Dependency Instructions
For both of the operating systems to run this project installation of these three programs are required [GLEW](http://glew.sourceforge.net/), [GLM](http://glm.g-truc.net/0.9.7/index.html), and [SDL2](https://wiki.libsdl.org/Tutorials).

This project uses OpenGL 3.3. Some computers, such as virtual machines in the ECC, can not run this version. In in order to run OpenGL 2.7 follow the instructions at [Using OpenGL 2.7](https://github.com/HPC-Vis/computer-graphics/wiki/Using-OpenGL-2.7)

### Ubuntu/Linux
```bash
sudo apt-get install libglew-dev libglm-dev libsdl2-dev libassimp-dev
sudo apt-get install libmagick++-dev
```



