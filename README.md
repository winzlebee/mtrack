# MTrack #
## The programmer's Video Compositor ##
Mtrack is a really simple video compositor. It also allows you to easily input 3d objects (OBJ files) and animate them in 3D. It uses basic OpenGL to render the 3D graphics and GTK3 for the user interface. Due to the awesome benefit afforded by writing your own shaders, Mtrack allows the artist access to the actual shader that is processed on both the video and the individual objects that have been added to the scene, differentiating it from most editing packages.

### TODO Features ###
- Load Video & Add 3D objects
- 3D Object animation
- Spatial reconstruction and camera tracking
- Custom shader construction for programmers, plus preset video and object shaders

## Build Information ##

### Linux ###
The build process on linux is quite simple (Debian-based distros)
1. Install the dependencies
    - sudo apt install build-essential cmake libopencv-dev libgl1-mesa-dev libgtkmm-3.0-dev
2. Run cmake in the root directory
    - cd <main directory>
    - cmake .
3. Build mtrack and run it
    - make && ./mtrack
    
### Windows ###
How to build on windows using Visual Studio and msys2
1. Install [msys2](http://www.msys2.org/)
    - Run the msys2 bash
    - Install the 64-bit version of mingw64
    pacman -S mingw-w64-x86_64-gcc
    - Install required dependencies
    pacman -S mingw-w64-x86_64-gtkmm3
    pacman -S pkg-config
    pacman -S mingw-w64-x86_64-opencv
    
2. Open the CMakeLists.txt file in Visual Studio, then hit the build button.