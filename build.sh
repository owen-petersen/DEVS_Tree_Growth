if [ -d "build" ]; then rm -Rf bin; fi
if [ -d "bin" ]; then rm -Rf bin; fi
rm *.csv
mkdir -p build
cd build || exit
rm -rf *
cmake .. 
make || (echo "Compilation failed." && return)
cd ..
echo Compilation done. Executable in the bin folder