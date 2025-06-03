#!/usr/bin/env sh 

if [ -z "$1" ]; then
	echo "No operation specified!"
	exit 1
fi

aptdeps="cmake git libpng-dev"
case "$1" in
	'docs'|'pack')
		aptdeps="$aptdeps doxygen graphviz"
		;;
esac

# installing dependencies
sudo apt install $aptdeps

export REPO="$(pwd)"

cmake -P "cicd/cmake/$1.cmake"
