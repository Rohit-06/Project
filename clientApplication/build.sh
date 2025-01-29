cd ../client
mkdir -p build
cd build
rm -rf *
cmake ..
make

make install

cp ../bin/* ../../clientApplication/
