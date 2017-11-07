# PA10: Pinball

# Dependencies, Building, and Running

## Dependency Instructions
For both of the operating systems to run this project installation of these three programs are required [GLEW](http://glew.sourceforge.net/), [GLM](http://glm.g-truc.net/0.9.7/index.html), and [SDL2](https://wiki.libsdl.org/Tutorials).

This project uses OpenGL 3.3. Some computers, such as virtual machines in the ECC, can not run this version. In in order to run OpenGL 2.7 follow the instructions at [Using OpenGL 2.7](https://github.com/HPC-Vis/computer-graphics/wiki/Using-OpenGL-2.7)

## Building and Running
Mousewheel up/down : will zoom in and out
If you hold rightmouse : pan around a point
Up/down arrow : will zoom in and out
Left/right arrow : will pan left and right around point

W A S D : will move the cube



## EXTRA CREDIT
All of our objects are load via triangle meshes


## FEATURES
The cube can get stuck. Do try to do that. If you do you lose and will have to restart.

### Ubuntu/Linux
```bash
sudo apt-get install libglew-dev libglm-dev libsdl2-dev
sudo apt-get install libassimp-dev
sudo apt-get install libmagick++-dev
sudo apt-get install libbullet-dev
```

## Building and Running
Use cmake


### CMake Instructions
The building of the project is done using CMake, installation with apt-get or brew may be necessary. Later use with CMake and Shader files will be require the copy of a directory where those files are stored (ex. shaders). To do this in the ```add_custom_target``` function place 
```cmake
COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/shaders/ ${CMAKE_CURRENT_BINARY_DIR}/shaders
```

```bash
mkdir build
cd build
cmake ..
make
./PA8
```

### Makefile Instructions 
The makefile works as expected and must be updated with new files added in.

```bash
mkdir build
cd build
cp ../makefile .
make
./Tutorial
```

## Ubuntu.cse.unr.edu
OpenGL 3.3 will run on the [ubuntu.cse.unr.edu](https://ubuntu.cse.unr.edu/) website. To do so follow the build instructions, but when running the Tutorial executable use this line to execute.
```bash
/usr/NX/scripts/vgl/vglrun ./Tutorial
```

## Controls

```bash
Left Arrow - Rotates the object counterclockwise
Right Arrow - Rotates the object clockwise
Space - Stops the rotation of object
```

## Config File

```bash
The config file, in the config folder, contains names of objects to be loaded.
Objects are saved in the objects folder as *.obj files.
To change the object that is loaded, change the objFileName in the config file.
```

## Objects

```bash
Objects are saved in the objects folder and are referenced in the config file.
Objects are in *.obj format.
```







