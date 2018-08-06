Different (and naive) implements of data structure for collision detection.

This project is inspired by https://github.com/ttvd/spatial-collision-datastructures. I have used his code in "Core.cpp" (on visualizing the scene).

The performance is much better than the above one. Mainly because I removed the polymorphism of "point". After all, "you don't pay for what you don't use". You can extend and reuse the code by using PointTraits\<T\>. I believe that **template** is the correct way to write a cpp program. 

# Building
(need C++17, opengl, openmp)
```shell
mkdir build && cd build
cmake ..
make
```
And we will get two executable files. One is TestMain, for testing the correctness and speed of the implements. One is GlutMain, for visualizing the scene. For the latter one, you can use different cmd args to launch it. For description of the args, type
```
./GlutMain --help
```