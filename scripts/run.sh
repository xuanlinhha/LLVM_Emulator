ROOT=/home/xuanlinhha/CCXX/cmake
CLANG=$ROOT/clang+llvm-8.0.0/bin/clang
CLANG_FORMAT=$ROOT/clang+llvm-8.0.0/bin/clang-format
PROJECT=$ROOT/LLVM_Emulator
SRC=$PROJECT/src
TEST=$PROJECT/benchmarks

echo "=== COMPILE TEST ==="
$CLANG -Isrc/include -emit-llvm -c -O0 -g -S $TEST/test.c -o $TEST/test.ll
$CLANG -Isrc/include -emit-llvm -c -O0 -g $TEST/test.c -o $TEST/test.bc
echo "====================="

echo "=== RUN ==="
$PROJECT/build/bin/sym $TEST/test.bc
echo "====================="
