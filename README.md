
# Kazmath

Kazmath is a simple 3D maths library written in C. It was initially coded for use in my book, Beginning OpenGL Game Programming - Second edition, but rapidly gained a life of its own. Kazmath is now used by many different projects, and apparently is used in 25% of the worlds mobile games (yeah, I don't believe it either - but it's used in Cocoas2d-x).

If you are using Kazmath in a project, I'd be happy to hear about it. Find me on Twitter as @kazade. 

# Compiling

Compiling is simple using CMake, on Linux:

    mkdir build
    cd build
    cmake ..
    make
    sudo make install

On other platforms use CMake to generate an appropriate project file.

If you want to build shared libraries you should pass `-DBUILD_SHARED_LIBS=YES` to the cmake command

# Contributing

There are many improvements that could be made to kazmath, including:

 - More tests
 - More optimizations
 - More functions, and primitives
 - More bindings (e.g. Python)

All contributions are welcome. Just send me a pull request on GitHub and I'll review and probably merge! 

# Donations

If you like what I do, I'm happy to receive donations in DOGE to this address: D6uwEfChxWmWjFGs4jT8VRtabYaza5oAGx :)

Thank you!
