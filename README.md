# OpenGL Bouncing DVD Logo

Welcome to the OpenGL Bouncing DVD Logo project! This C++ application showcases a classic DVD logo bouncing around the screen, using OpenGL for graphics rendering. Watch as the logo changes color upon collision with the screen edges, bringing back nostalgic screensaver vibes.

![Alt Text](https://media.giphy.com/media/v1.Y2lkPTc5MGI3NjExanNlY3FwanZvaW51eDZobmQyaXp2bTF3Y3VseGUwNnl1dzZ5MjhsZCZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9Zw/P0N8Ghf7nfc87He749/giphy.gif)


## Prerequisites

- C++ compiler (GCC/Clang on Linux, MSVC on Windows)
- CMake
- OpenGL libraries
- GLEW
- GLFW
- GLM

## Building and Running

### Windows

1. Clone the repository:
   ```sh
   git clone https://github.com/Tlamir/OpenGL_DVD_Logo.git
   cd OpenGL_DVD_Logo
2.Open the solution in Visual Studio.

3.Build and run the project.
### Linux/Mac
1.Install the required libaries:
 ```sh
   sudo apt-get install cmake libglew-dev libglfw3-dev libglm-dev
```
2.Clone the repository:
 ```sh
  git clone https://github.com/Tlamir/OpenGL_DVD_Logo.git
   cd OpenGL_DVD_Logo
   cd OpenglApp
```
3.Create a build directory and navigate to it:
 ```sh
mkdir build
cd build
```
4.Run CMake to configure the project
```
cmake ..
```
5.Build the project:
```
make
```
6.Run the executable
```
./OpenGLApp
```
