ROOT=/home/xuanlinhha/CCXX/cmake
CLANG=$ROOT/clang+llvm-8.0.0/bin/clang
CLANG_FORMAT=$ROOT/clang+llvm-8.0.0/bin/clang-format
PROJECT=$ROOT/LLVM_Emulator
SRC=$PROJECT/src
TEST=$PROJECT/test
echo "=== PATH ==="
echo $CLANG_FORMAT
echo $SRC 
echo "============"

echo "=== CLANG COMPILE ==="
$CLANG -Isrc/include -emit-llvm -c -g -S $TEST/test.c -o $TEST/test.ll
$CLANG -Isrc/include -emit-llvm -c -g $TEST/test.c -o $TEST/test.bc
echo "====================="

echo "=== CLANG FORMAT ==="
$CLANG_FORMAT -i $SRC/include/*.h
$CLANG_FORMAT -i $SRC/execution/symexe/*.h
$CLANG_FORMAT -i $SRC/execution/symexe/*.cpp
$CLANG_FORMAT -i $SRC/execution/searcher/*.h
$CLANG_FORMAT -i $SRC/execution/searcher/*.cpp
$CLANG_FORMAT -i $SRC/execution/state/*.h
$CLANG_FORMAT -i $SRC/execution/state/*.cpp
$CLANG_FORMAT -i $SRC/execution/value/*.h
$CLANG_FORMAT -i $SRC/execution/value/*.cpp
$CLANG_FORMAT -i $SRC/execution/solver/*.h
$CLANG_FORMAT -i $SRC/execution/solver/*.cpp
$CLANG_FORMAT -i $SRC/execution/simplify/*.h
$CLANG_FORMAT -i $SRC/execution/simplify/*.cpp
$CLANG_FORMAT -i $SRC/execution/helper/*.h
$CLANG_FORMAT -i $SRC/execution/helper/*.cpp
$CLANG_FORMAT -i $SRC/tool/*.cpp
$CLANG_FORMAT -i $TEST/*.c
echo "====================="
