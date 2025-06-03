#!/usr/bin/env sh 

if [ -z "$1" ]; then
	echo "No operation specified!"
	exit 1
fi

# installing homebrew 
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
eval "$(/opt/homebrew/bin/brew shellenv)"

# installing dependencies
brew install --formula cmake git libpng
brew upgrade

export REPO="$(pwd)"
export CONFIGURATION_OPTIONS='-DCMAKE_PREFIX_PATH=/opt/homebrew -DPNG_PNG_INCLUDE_DIR=/opt/homebrew/include -DPNG_LIBRARY=/opt/homebrew/lib/libpng.dylib'

cmake -P "cicd/cmake/$1.cmake"
