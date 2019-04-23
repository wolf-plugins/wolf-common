make clean

export LD_LIBRARY_PATH=/lib:/usr/lib:/usr/local/lib
export CC=stoat-compile
export CXX=stoat-compile++

make

stoat --recursive .