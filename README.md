<h1 align="center">Vire</h1>
<p align="center">Vire is a C++ voxel rendering engine. It is written in C++14 and uses OpenGL for graphics.</p>
<p align="center"> <img src="https://cdn.discordapp.com/attachments/934901281703747635/937814644221960212/ouhsx8t71ge81.jpg"> </p>
<br>

## Downloads
If you'd just like to just download and try, there are precompiled binaries available for Windows and Linux which you can download here:

<table>
<tr>
<th align="center">
<img width="441" height="1">
<p> 
<small>
  <a href="https://nightly.link/JacRich/vkt/workflows/make-windows/main/windows%20mingw64.zip">Windows</a>
</small>
</p>
</th>
<th align="center">
<img width="441" height="1">
<p> 
<small>
  <a href="https://nightly.link/JacRich/vkt/workflows/make-linux/main/linux.zip">Linux</a>
</small>
</p>
</th>
</tr>
<tr>
</table>

## Compiling
### Linux
To compile the project, you will need the following dependencies:

| name | package debian based | package arch based | package fedora based |
|---|---|---|---|
|g++|g++|gcc|gcc-c++|
|libGL|libgl-dev|libglvnd|mesa-libGL-devel|
|libGLEW|libglew-dev|glew|glew-devel|
|libglfw|libglfw3-dev|glfw-wayland / glfw-x11|glfw-devel|
|libglm|libglm-dev|glm|glm-devel|
|libX11|libx11-dev|libx11|libX11-1.7.3.1-1.fc36|
|libXxf86vm|libxxf86vm-dev|libxxf86vm|libXxf86vm-devel|
|libXrandr|libxrandr-dev|libxrandr|libXrandr-devel|
|libpthread|libpthread-stubs0-dev|glibcs|libpthread-stubs-devel|
|libXi|libxi-dev|libxi|libXi-devel|
|libdl|libc6-dev|glibc|glibc-devel|
|libXinerama|libxinerama-dev|libxinerama|libXinerama|
|libXcursor|libxcursor-dev|libxcursor|libXcursor|
|libm|libc6-dev|glibc|glibc-devel|

#### Ubuntu


```
sudo apt-get -y install g++ libgl-dev libglew-dev libglfw3-dev libglm-dev libx11-dev libxxf86vm-dev libxrandr-dev libpthread-stubs0-dev libxi-dev libc6-dev libxinerama-dev libxcursor-dev libc6-dev
```
Then `git clone` this repository or download and extract the zip <br>
Finally, you should be able to cd into the repo and run `make buildlinux` <br>

The program can be started using the file in the new bin/linux folder, or through the command `make run`

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
