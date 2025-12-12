

I was trying to test out C++ modules with this project, but encountered some problems along the way.  
I couldn't get CMake to work properly with modules, so building is available from cli using modern enough compiler.

```bash
    clang++ HamurabiGame.cppm -std=c++23 --precompile -o HamurabiGame.pcm
    clang++ main.cpp -std=c++23 -fmodule-file=HamurabiGame=HamurabiGame.pcm HamurabiGame.pcm -o Main
```

I was using MacOS and Apple Clang does not support CPP modules at all, so I had to use llvm version from brew. You will probably need to check if your path is right, but here are the example commands used by me. 

```bash
    /opt/homebrew/opt/llvm/bin/clang++ HamurabiGame.cppm -std=c++23 --precompile -o HamurabiGame.pcm
    /opt/homebrew/opt/llvm/bin/clang++ main.cpp -std=c++23 -fmodule-file=HamurabiGame=HamurabiGame.pcm HamurabiGame.pcm -o Main
```