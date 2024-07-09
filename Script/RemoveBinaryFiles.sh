#!/bin/bash

pushd .. > /dev/null

# Recursively find and delete directories named 'bin'
find . -type d -name "bin" -exec rm -rf {} +
# Recursively find and delete directories named 'bin-int'
find . -type d -name "bin-int" -exec rm -rf {} +

popd > /dev/null

read -p "Press [Enter] key to continue..."