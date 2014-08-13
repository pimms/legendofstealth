#!/bin/bash

if [ ! -f "gtest/build/libgtest.a" ]; 
then
	echo "Building the Google Test library..."
	cd "gtest" && ./configure && mkdir -p build && cd build && cmake .. && make;
fi
