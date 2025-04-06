all: create_dirs $(TARGET)

create_dirs:
	if not exist build mkdir build
	if not exist bin mkdir bin

# Compiler and flags
CC=cl
CFLAGS=/nologo /EHsc /DUNICODE /D_UNICODE /W3 /Zi /MD /Fo"build\\"
LDFLAGS=/link user32.lib gdi32.lib /OUT:bin\\TransparentWindowApp.exe

# Output
TARGET=bin\\TransparentWindowApp.exe

# Object files in build folder
OBJS=build\\source.obj build\\TransparentWindow.obj

# Default rule
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS)

build\\source.obj: source.cpp TransparentWindow.h
	$(CC) $(CFLAGS) /c source.cpp

build\\TransparentWindow.obj: TransparentWindow.cpp TransparentWindow.h
	$(CC) $(CFLAGS) /c TransparentWindow.cpp

clean:
	del /Q build\\*.obj build\\*.pdb bin\\*.exe
