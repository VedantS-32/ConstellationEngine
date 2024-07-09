#!/bin/bash

pushd .. > /dev/null

# Recursively find and delete files named 'Makefile'
find . -type f -name "Makefile" -exec rm -f {} +

popd > /dev/null

read -p "Press [Enter] key to continue..."
