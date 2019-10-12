# A sample LLVM emulator

This project requires LLVM & Z3. Supposed that our working folder named `ROOT`.

```
ROOT $ ls
clang+llvm-8.0.0    LLVM_Emulator    z3
```

##### LLVM & Z3

Download Pre-Built Binaries of LLVM-8.0.0 from here: http://releases.llvm.org/download.html

Get Z3 from here: https://github.com/Z3Prover/z3. Checkout to a stable version & build with CMake

```
ROOT/z3 $ git clean -nx src
ROOT/z3 $ git clean -fx src
ROOT/z3 $ mkdir build
ROOT/z3 $ cd build
ROOT/z3 $ cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release ../
ROOT/z3 $ make -j8
```

##### Build LLVM_Emulator
Firstly, adjust the paths of LLVM & Z3 in these two lines of `ROOT/LLVM_Emulator/src/tool/CMakeLists.txt`:

```
set(LLVM_INSTALL_DIR "ROOT/clang+llvm-8.0.0" CACHE STRING "Path to LLVM folder")
set(Z3_INSTALL_DIR "ROOT/z3/build" CACHE STRING "Path to Z3 folder")
```

##### Steps to build:

```
ROOT/LLVM_Emulator $ mkdir build
ROOT/LLVM_Emulator $ cd build
ROOT/LLVM_Emulator/build $ cmake ..
ROOT/LLVM_Emulator/build $ make -j8
```

##### Run & Debug

```
ROOT/LLVM_Emulator/build $ ./bin/sym <input.bc> <program_args>
ROOT/LLVM_Emulator/build $ gdb --args ./bin/sym <input.bc> <program_args>
```
