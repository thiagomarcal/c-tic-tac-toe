"# c-tic-tac-toe"

Simple Tic-Tac-Toe Game using C11 with SDL for Testing Purposes

Notes:
    CLion already has a builtin CMAKE and Ninja installation binaries

build pipeline: 
    CMAKE with Ninja (Testing Default Clion with VisualStudio 2022 compiler)

platform: x64 Windows With VS 2022 compiler installed

````
CLion run the commands below behind the curtains:

build:
    cmake.exe -DCMAKE_BUILD_TYPE=Debug "-DCMAKE_MAKE_PROGRAM=<path to ninja-build install binary>" -G Ninja -S <path to the root project> -B <path-to-project-root>\cmake-build-debug

run:
    cmake.exe --build <path-to-project-root>\cmake-build-debug --target tic-tac-toe -j 9
````
