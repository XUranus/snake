#!/usr/bin/zsh
rm -rf build
mkdir build

echo 'building client...'
cmake -S ./client -B ./build
cd build && make
mv snake_client .. && cd ..

rm -rf build
mkdir build

echo 'building server...'
cmake -S ./server -B ./build
cd build && make
mv snake_server .. && cd ..

echo 'removing build cache...'
rm -rf build

echo 'Done.'