#!/bin/bash
set +x

pushd .. > /dev/null
vendor/premake/bin0/premake5.exe gmake2
popd > /dev/null

read -p "Press [Enter] key to continue..."
