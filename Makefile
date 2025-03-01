.PHONY: all clean run

# Define required environment variables
#------------------------------------------------------------------------------------------------
# Define target platform: PLATFORM_DESKTOP, PLATFORM_RPI, PLATFORM_DRM, PLATFORM_ANDROID, PLATFORM_WEB
PLATFORM              ?= PLATFORM_DESKTOP

# Define project variables
PROJECT_NAME          ?= BMPopener
PROJECT_VERSION       ?= 0.1
PROJECT_BUILD_PATH    ?= .
RAYLIB_PATH           ?= ../../raylib

# Locations of raylib.h and libraylib.a/libraylib.so
# NOTE: Those variables are only used for PLATFORM_OS: LINUX, BSD
RAYLIB_INCLUDE_PATH   ?= /usr/local/include
RAYLIB_LIB_PATH       ?= /usr/local/lib

# Library type compilation: STATIC (.a) or SHARED (.so/.dll)
RAYLIB_LIBTYPE        ?= STATIC

# Build mode for project: DEBUG or RELEASE
BUILD_MODE            ?= RELEASE

PLATFORM_OS = LINUX
PLATFORM_SHELL = sh

# Define raylib release directory for compiled library
RAYLIB_RELEASE_PATH 	?= $(RAYLIB_PATH)/src

# Define default C compiler: CC
#------------------------------------------------------------------------------------------------
CC = gcc

# Define default make program: MAKE
#------------------------------------------------------------------------------------------------
MAKE ?= make

# Define compiler flags: CFLAGS
#------------------------------------------------------------------------------------------------
#  -O1                  defines optimization level
#  -g                   include debug information on compilation
#  -s                   strip unnecessary data from build
#  -Wall                turns on most, but not all, compiler warnings
#  -std=c99             defines C language mode (standard C from 1999 revision)
#  -std=gnu99           defines C language mode (GNU C from 1999 revision)
#  -Wno-missing-braces  ignore invalid warning (GCC bug 53119)
#  -Wno-unused-value    ignore unused return values of some functions (i.e. fread())
#  -D_DEFAULT_SOURCE    use with -std=c99 on Linux and PLATFORM_WEB, required for timespec
CFLAGS = -std=c99 -Wall -Wno-missing-braces -Wunused-result -D_DEFAULT_SOURCE

ifeq ($(BUILD_MODE),DEBUG)
    CFLAGS += -g -D_DEBUG
endif

# Additional flags for compiler (if desired)
#CFLAGS += -Wextra -Wmissing-prototypes -Wstrict-prototypes
ifeq ($(RAYLIB_LIBTYPE),STATIC)
    CFLAGS += -D_DEFAULT_SOURCE
endif
ifeq ($(RAYLIB_LIBTYPE),SHARED)
    # Explicitly enable runtime link to libraylib.so
    CFLAGS += -Wl,-rpath,$(RAYLIB_RELEASE_PATH)
endif

# Define include paths for required headers: INCLUDE_PATHS
# NOTE: Some external/extras libraries could be required (stb, physac, easings...)
#------------------------------------------------------------------------------------------------
INCLUDE_PATHS = -I. -I$(RAYLIB_PATH)/src -I$(RAYLIB_PATH)/src/external -I$(RAYLIB_PATH)/src/extras

# Define additional directories containing required header files
INCLUDE_PATHS += -I$(RAYLIB_INCLUDE_PATH)

# Define library paths containing required libs: LDFLAGS
#------------------------------------------------------------------------------------------------
LDFLAGS = -L. -L$(RAYLIB_RELEASE_PATH) -L$(RAYLIB_PATH)/src
LDFLAGS += -L$(RAYLIB_LIB_PATH)

# Define libraries required on linking: LDLIBS
# NOTE: To link libraries (lib<name>.so or lib<name>.a), use -l<name>
#------------------------------------------------------------------------------------------------
# Libraries for Debian GNU/Linux desktop compiling
# NOTE: Required packages: libegl1-mesa-dev
#
LDLIBS = -lraylib -lGL -lm -lpthread -ldl -lrt

# On X11 requires also below libraries
LDLIBS += -lX11
# NOTE: It seems additional libraries are not required any more, latest GLFW just dlopen them
#LDLIBS += -lXrandr -lXinerama -lXi -lXxf86vm -lXcursor

ifeq ($(RAYLIB_LIBTYPE),SHARED)
    LDLIBS += -lc
endif

# Define source code object files required
#------------------------------------------------------------------------------------------------
PROJECT_SOURCE_FILES ?= \
    src/main.c \

# Define all object files from source files
OBJS = $(patsubst %.c, %.o, $(PROJECT_SOURCE_FILES))


# Define processes to execute
#------------------------------------------------------------------------------------------------
MAKEFILE_PARAMS = $(PROJECT_NAME)

# Default target entry
# NOTE: We call this Makefile target or Makefile.Android target
all:
	$(MAKE) $(MAKEFILE_PARAMS)

# Project target defined by PROJECT_NAME
$(PROJECT_NAME): $(OBJS)
	$(CC) -o $(PROJECT_NAME)$(EXT) $(OBJS) $(CFLAGS) $(INCLUDE_PATHS) $(LDFLAGS) $(LDLIBS) -D$(PLATFORM) -g

# Compile source files
# NOTE: This pattern will compile every module defined on $(OBJS)
%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS) $(INCLUDE_PATHS) -D$(PLATFORM) -g

run:
	$(MAKE) $(MAKEFILE_PARAMS)

ifeq ($(PLATFORM),PLATFORM_DESKTOP)
    ifeq ($(PLATFORM_OS),WINDOWS)
		$(PROJECT_NAME)
    endif
    ifeq ($(PLATFORM_OS),LINUX)
		./$(PROJECT_NAME)
    endif
    ifeq ($(PLATFORM_OS),OSX)
		./$(PROJECT_NAME)
    endif
endif

.PHONY: clean_shell_cmd clean_shell_sh

# Clean everything
clean:	clean_shell_$(PLATFORM_SHELL)
	@echo Cleaning done

clean_shell_sh:
	    find . -type f -executable -delete
	    rm -fv *.o
