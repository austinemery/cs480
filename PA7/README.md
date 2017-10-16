### PA6 Collaborators
Jacob Lee
Cayler Miley
Austin Emery

### The buttons
```bash

S : Will start and stop the rotaion 
Right Click: Will reverse and resume the rotaion
Left Arrow Key: Will set the planet to counterclockwise rotaion
Right Arrow Key: Will set the planet to clockwise rotation.


ESC : Will end the program
```

### Use cmake to make this project

```bash
mkdir build
cd build
cmake ..
make
./PA6 --Help
```
running will help 
```bash
./PA6 --Help
```


I added a bash file to make running the program with the provided shaders and models quicker. The file is called run.sh and is the root dir.
```bash
cp ../run.sh run.sh
./run.sh
```
If I did a horrible job with that, run it like this.
```bash
./PA6 --v ../shaders/passthrough.vert --f ../shaders/passthrough.frag --m ../models/cube.obj

```

The shaders are in the shader folder
There are some cool models in the models folder that I made that you can try

Only the moon model has textures.


# PA6: ASIMP TEXTURE LOADING

```bash
The object can be changed by changing the run.sh file.
The texture is loaded from the objects .mtl file.
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



