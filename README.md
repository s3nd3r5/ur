# The Royal Game of Ur

```
[ ][ ]      [ ][ ][ ][ ]
[ ][ ][ ][ ][ ][ ][ ][ ]
[ ][ ]      [ ][ ][ ][ ]
```
[Gameplay Wiki](https://en.wikipedia.org/wiki/Royal_Game_of_Ur#Gameplay)
[Play video](https://www.youtube.com/watch?v=WZskjLq040I)

## How to play

Two players face off with 7 pieces taking turns rolling the dice and progressing their pieces
to the end of the paths.

First player is on the bottom, and top player is player 2.

## Development

### Linux

Ur is built with SFML and cmake. You will need to install SFML libraries and cmake utilities.

Then configure cmake:

```
cmake .
make
```

### Windows

To build on windows you'll need to setup
the SFML include path as `SFML_DIR`

then make sure to include all the DLLs needed when running.
The ones required right now are:

 - `sfml-graphics-2.dll`
 - `sfml-system-2.dll`
 - `sfml-window-2.dll`
 - `libgcc_s_seh-1.dll`
 - `libstdc++-6.dll`
 - `libwinpthread-1.dll`

Additional SFML libs not used:

 - `sfml-audio-2.dll`
 - `sfml-network-2.dll`

On linux you can build the project for windows using mingw:

```
mkdir build-mingw
cd build-mingw
x86_64-w64-mingw-cmake -S ../ -B .
make
```

You can run your windows build via wine64 by bundling your dll and the res folder along side the exe.

### Debug

To enable debug logging use the cmake flags:

```
cmake -DCMAKE_BUILD_TYPE=Debug
```
