ROOT=/home/xuanlinhha/CCXX/cmake
CLANG=$ROOT/clang+llvm-8.0.0/bin/clang
CLANG_FORMAT=$ROOT/clang+llvm-8.0.0/bin/clang-format
PROJECT=$ROOT/LLVM_Emulator
SRC=$PROJECT/src
TEST=$PROJECT/benchmarks
echo "=== PATH ==="
printf "clang-format: %s\n" "$CLANG_FORMAT"
printf "src: %s\n" "$SRC"
echo "============"

echo "=== FORMAT SOURCE CODE ==="
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
$CLANG_FORMAT -i $TEST/sv-comp/*.c
echo "====================="

echo "=== BUILD ==="
if [[ ! -d "build" ]]; then
    mkdir build
    cd build
	cmake ..
fi
cd build
make -j 4
cd ..
echo "====================="

