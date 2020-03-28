ROOT="$(dirname `pwd`)"
CLANG=$ROOT/llvm/bin/clang
CLANG_FORMAT=$ROOT/llvm/bin/clang-format
PROJECT=$ROOT/LLVM_Emulator
SRC=$PROJECT/src
BENCHMARKS=$PROJECT/benchmarks
echo "=== PATHS ==="
printf "clang-format: %s\n" "$CLANG_FORMAT"
printf "src: %s\n" "$SRC"
echo "============="

echo "=== FORMAT SOURCE CODE ==="
$CLANG_FORMAT -i $SRC/include/*.h
$CLANG_FORMAT -i $SRC/execution/helper/*.h
$CLANG_FORMAT -i $SRC/execution/helper/*.cpp
$CLANG_FORMAT -i $SRC/execution/searcher/*.h
$CLANG_FORMAT -i $SRC/execution/searcher/*.cpp
$CLANG_FORMAT -i $SRC/execution/simplify/*.h
$CLANG_FORMAT -i $SRC/execution/simplify/*.cpp
$CLANG_FORMAT -i $SRC/execution/solver/*.h
$CLANG_FORMAT -i $SRC/execution/solver/*.cpp
$CLANG_FORMAT -i $SRC/execution/state/*.h
$CLANG_FORMAT -i $SRC/execution/state/*.cpp
$CLANG_FORMAT -i $SRC/execution/symexe/*.h
$CLANG_FORMAT -i $SRC/execution/symexe/*.cpp
$CLANG_FORMAT -i $SRC/execution/value/*.h
$CLANG_FORMAT -i $SRC/execution/value/*.cpp
$CLANG_FORMAT -i $SRC/tool/*.cpp

# echo "=== FORMAT BENCHMARKS (optional) ==="
# $CLANG_FORMAT -i $BENCHMARKS/*.c
# $CLANG_FORMAT -i $BENCHMARKS/sv-comp/*.c
echo "=========================="

echo "=== BUILD ==="
if [[  ! -d "$PROJECT/build"  ]]; then
	mkdir $PROJECT/build
	cd $PROJECT/build
	cmake ..
else
	cd $PROJECT/build
fi
make -j4
echo "============="

echo "=== COMPILE TEST ==="
$CLANG -Isrc/include -emit-llvm -c -O0 -g -S $BENCHMARKS/test.c -o $BENCHMARKS/test.ll
$CLANG -Isrc/include -emit-llvm -c -O0 -g $BENCHMARKS/test.c -o $BENCHMARKS/test.bc
echo "===================="

echo "=== RUN TEST ==="
$PROJECT/build/bin/sym $BENCHMARKS/test.bc
echo "================"