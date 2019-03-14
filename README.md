# MTrack #
## The programmer's Video Compositor ##
Mtrack is a really simple Open Source NLE with video compositor elements, like Adobe After Effects or HitFilm. It's designed to all run on the GPU so it's blazingly fast. It uses OpenGL to render the 3D graphics and video and GTK3 for the user interface. Due to the awesome benefit afforded by writing your own shaders, Mtrack allows the artist access to the actual shader that is processed on both the video and the individual objects that have been added to the scene, differentiating it from most editing packages.

### TODO Features ###
- Load & Edit together video clips
- Video effect & transition presets
- Add 3D Objects
- Spatial reconstruction and camera tracking
- Custom shader construction for programmers, plus preset video and object shaders

## Build Information ##

### Linux ###
The build process on linux is quite simple (Debian-based distros)
1. Install the dependencies

`sudo apt install build-essential cmake libopencv-dev libgl1-mesa-dev libgtkmm-3.0-dev libglew2.0`
2. Clone the repository and enter the root directory

`git clone <uri>`

`cd mtrack`
3. Make a directory called build, enter it then run cmake in the root directory.

`mkdir build && cd build`

`cmake ../`
4. Build mtrack and run it

`make && ./mtrack`
    
### Windows ###
How to build on windows using Visual Studio and msys2
1. Install CMake and add to PATH
2. Install [msys2](http://www.msys2.org/)
    - Run the msys2 bash
    - Install the 64-bit version of mingw64
    
    `pacman -S mingw-w64-x86_64-gcc`
    - Install required dependencies
    
    `pacman -S mingw-w64-x86_64-gtkmm3`
    
    `pacman -S pkg-config`
    
    `pacman -S mingw-w64-x86_64-opencv`
    
    `pacman -S mingw-w64-x86_64-glew`
3. Open the CMakeLists.txt file in Visual Studio, then configure to use CMake and MinGW as a compiler.
4. Alternatively, use CMake and the Mingw64 shell to build

`mkdir build && cd build`

`cmake ../ -G 'MinGW Makefiles'`

`mingw32-make && ./mtrack`
