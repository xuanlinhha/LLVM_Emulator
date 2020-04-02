ROOT="$(dirname $(dirname `pwd`))"
CLANG=$ROOT/llvm/bin/clang
PROJECT=$ROOT/LLVM_Emulator
SRC=$PROJECT/src
BENCHMARKS=$PROJECT/benchmarks
echo "=== PATHS ==="
printf "src: %s\n" "$SRC"
echo "============="



echo "=== Compile sample.c ==="
$CLANG -I$ROOT/src/include -emit-llvm -c -O0 -g -S sample.c -o sample.ll
$CLANG -I$ROOT/src/include -emit-llvm -c -O0 -g sample.c -o sample.bc
echo "===================="

echo "=== Run sample ==="
$PROJECT/build/bin/sym sample.bc
echo "================"