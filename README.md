# A sample LLVM emulator

This project requires LLVM & Z3. Supposed that our working folder named `ROOT`.

```
ROOT $ ls
llvm    LLVM_Emulator    z3
```

##### LLVM & Z3

Download Pre-Built Binaries of LLVM-8.0.0 & extract to folder `llvm`:

[https://releases.llvm.org/8.0.0/clang+llvm-8.0.0-x86_64-linux-gnu-ubuntu-16.04.tar.xz](https://releases.llvm.org/8.0.0/clang+llvm-8.0.0-x86_64-linux-gnu-ubuntu-16.04.tar.xz)


Get Z3 source from here & extract to folder `z3`:

[https://github.com/Z3Prover/z3/archive/z3-4.8.7.tar.gz](https://github.com/Z3Prover/z3/archive/z3-4.8.7.tar.gz)

Build Z3 with Makefiles:

```
ROOT/z3 $ mkdir build
ROOT/z3 $ cd build
ROOT/z3 $ cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release ../
ROOT/z3 $ make -j8
```

##### Download & Build LLVM_Emulator

```
ROOT $ git clone git@github.com:xuanlinhha/LLVM_Emulator.git
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
