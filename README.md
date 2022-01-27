<h1 align="center">VKT</h1>
<p align="center">VkT is a C++ voxel rendering engine. It is written in C++14 and uses OpenGL for graphics.</p>
<p align="center"> <img src="https://cdn.discordapp.com/attachments/934901281703747635/935902064242987108/unknown.png"> </p>
<br>

## Downloads
If you'd just like to just download and try, there are precompiled binaries available for Windows and Linux which you can download here:

### [Windows](https://nightly.link/garv-shah/vkt/workflows/make-windows/main/windows.zip)

### [Linux](https://nightly.link/garv-shah/vkt/workflows/make-linux/main/linux.zip)

## Compiling
### Linux
To compile the project, you will need the following dependencies:

| name | package debian based | package arch based |
|---|---|---|
|g++|g++|gcc|
|libGL|libgl-dev|libglvnd|
|libGLEW|libglew-dev|glew|
|libglfw|libglfw3-dev|glfw-wayland / glfw-x11|
|libglm|libglm-dev|glm|
|libX11|libx11-dev|libx11|
|libXxf86vm|libxxf86vm-dev|libxxf86vm|
|libXrandr|libxrandr-dev|libxrandr|
|libpthread|libpthread-stubs0-dev|glibcs|
|libXi|libxi-dev|libxi|
|libdl|libc6-dev|glibc|
|libXinerama|libxinerama-dev|libxinerama|
|libXcursor|libxcursor-dev|libxcursor|
|libm|libc6-dev|glibc|

### Windows
To compile on Windows, you'll need to follow these steps:

1. Download MSYS2 from https://www.msys2.org/ and follow steps 1-7 on the main page
2. Run this command in the MSYS2 shell: `pacman -S mingw-w64-x86_64-glfw mingw-w64-x86_64-glew mingw-w64-x86_64-glm`
3. Add the folder `msys64/mingw64/bin` to PATH
4. Either `git clone` this repository or download and extract the zip
5. In the MinGW shell, `cd` to the repository and run `make buildwin_msys`

The program can be started using the .exe in the new bin/windows folder, or through the command `make run`

## Contributing
To contribute to the project, submit a pull request.
