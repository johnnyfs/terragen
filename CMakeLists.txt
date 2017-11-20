cmake_minimum_required(VERSION 3.8)
project(untitled1)

# Windows DLLs
configure_file(C:/Libraries/SDL2-2.0.5/i686-w64-mingw32/bin/SDL2.dll SDL2.dll COPYONLY)
configure_file(C:/MinGW/bin/glew32.dll glew32.dll COPYONLY)

# Shaders
configure_file(vertex.glsl vertex.glsl COPYONLY)
configure_file(fragment.glsl fragment.glsl COPYONLY)

# Flags
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_FLAGS “${CMAKE_CXX_FLAGS}”)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lmingw32")

# Directories
include_directories("C:/Libraries/SDL2-2.0.5/i686-w64-mingw32/include/")
link_directories("C:/Libraries/SDL2-2.0.5/i686-w64-mingw32/lib")

# Target
set(SOURCE_FILES main.c matrices.c matrices.h vectors.h util.h shaders.h frame.c frame.h buttons.h)
add_executable(untitled1 ${SOURCE_FILES})
target_link_libraries(untitled1 SDL2main SDL2 opengl32 glew32)
