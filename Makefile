# Makefile by Igor to Jac Rich
# Below are some variables.
EXECUTABLE_NAME = "vkt"

# Added option to build with clang instead of g++ -David (B1QUAD)
# Uncomment line 7 and comment line 8 to use clang++
# CXX = "clang++"
CXX = "g++"
CPPARAMS := -g -std=gnu++14 -Wall

# Set g++ specific flags
ifeq ($(CXX),g++)
	CPPARAMS += -fdiagnostics-color=always -fdiagnostics-show-labels
endif

# Set clang++ specific flags
# ifeq used because there is no else if in GNU make
ifeq ($(CXX),clang++)
	CPPARAMS += -Wgcc-compat -Woption -fdiagnostics-show-option -fdiagnostics-show-category   
endif

LDLINUXFLAGS = -I headers -lGL -lGLEW -lglfw -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor -lm
objects = $(wildcard src/*.cpp)

# create directory bin/linux if not existent
# compile the program to the folder using g++ and use the EXECUTABLE_NAME variable as the name of the executable.
# then copy all files inside "outdata/" to the folder "bin/linux/" but not outdata itself.
buildlinux:
	@echo "Compiling for Linux"
	mkdir -p bin/linux
	$(CXX) $(objects) -g $(CPPARAMS) $(LDLINUXFLAGS) -o bin/linux/$(EXECUTABLE_NAME)
	@echo "Copying assets..."
	@cp -r assets/* bin/linux/

# if more make build commands are made for other platforms, add them here, this will run them all.
all: buildlinux

# if on linux, run the bin/linux/$(EXECUTABLE_NAME) executable.
run:
	@echo "Running Linux executable"
	(cd bin/linux && ./$(EXECUTABLE_NAME))
