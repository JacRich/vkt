# Below are some variables.
EXECUTABLE_NAME = "vkt"
CPPARAMS = -O2 -std=gnu++14 -fdiagnostics-color=always -fdiagnostics-show-labels -Wall -Wno-unused-result
LDLINUXFLAGS = -I headers -lGL -lGLEW -lglfw -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor -lm
LDWINDOWSFLAGS = -I headers -DGLEW_STATIC -L lib -static-libstdc++ -static-libgcc -lopengl32 -lglfw3 -lgdi32 -lm -Wl,--stack,8388608 -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic
objects = $(wildcard src/*.cpp)
objectsWin = headers/GL/glew.c

# creates directory bin/linux if not existent
# compiles the program to the folder using g++ and use the EXECUTABLE_NAME variable as the name of the executable.
# then copy all files inside "assets/" to the folder "bin/linux/" but not assets itself.
buildlinux:
	@echo "Compiling for Linux"
	@mkdir -p bin/linux
	g++ $(objects) -g $(CPPARAMS) $(LDLINUXFLAGS) -o bin/linux/$(EXECUTABLE_NAME)
	@echo "Copying assets..."
	@cp -r assets/* bin/linux

# creates directory bin/windows if not existent
# compiles the program to the folder using MinGW g++ and use the EXECUTABLE_NAME variable as the name of the executable.
# then copy all files inside "assets/" to the folder "bin/windows/" but not assets itself.
# and copies glew32.dll from bin to bin/windows
buildwin:
	@echo "Compiling for Windows \033[1;31m!Experimental!\033[0m"
	@mkdir -p bin/windows
	x86_64-w64-mingw32-g++ $(objects) $(objectsWin) -g $(CPPARAMS) $(LDWINDOWSFLAGS) -o bin/windows/$(EXECUTABLE_NAME)
	@echo "Copying assets..."
	@cp -r assets/* bin/windows
	@cp lib/libwinpthread-1.dll bin/windows

# Command for building to windows from windows (MSYS2)
buildwin_msys:
	@echo "Compiling for Windows \033[1;31m!Experimental!\033[0m"
	@mkdir -p bin/windows

	g++ $(objects) $(objectsWin) -g $(CPPARAMS) $(LDWINDOWSFLAGS) -o bin/windows/$(EXECUTABLE_NAME)
	@echo "Copying assets..."
	@cp -r assets/* bin/windows/

# if more make Linux build commands are made for other platforms, add them here, this will run them all.
all: buildlinux buildwin

# if on linux, cd to bin/linux/ and run ./$(EXECUTABLE_NAME) executable, this is needed since
# when running from the terminal, the current directory is the root of the project.
# if on windows, cd to bin/windows/ and run ./$(EXECUTABLE_NAME).exe executable.
ifeq ($(OS),Windows_NT)
run:
	@echo "Running on Windows"
	@cd bin/windows/; ./$(EXECUTABLE_NAME).exe
else
run:
	@echo "Running on Linux"
	@cd bin/linux && ./$(EXECUTABLE_NAME)
endif
