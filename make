#!/bin/bash

NAME="FreboNS"
VERSION="1.0"

PARAMS="-Iinclude/ -Llib/ -I/usr/include/mysql -L/usr/lib/mysql -L/usr/include/mysql -lmysqlclient -g1 -lSegFault"

install_depend() {
	echo "Could not find dependency '$1'!"
	echo -n "Do you want to install it [y/n]: "
	read installdep

	if [ "$installdep" = "y" ]; then
		echo "Installing $1..."

		APT=`which apt-get`
		YUM=`which yum`

		if [ "$APT" -eq 1 ]; then
			apt-get install -y $1
		elif [ "$YUM" -eq 1 ]; then
			yum install -y $1
		else
			echo "Unknown package manager!"
			exit 0
		fi
	else
		exit 0
	fi

	echo "$1 Installed!"
}

echo "   ____        __        _  ______"
echo "  / __/______ / /  ___  / |/ / __/"
echo " / _// __/ -_) _ \\/ _ \/    /\\ \\"
echo "/_/ /_/  \\__/_.__/\\___/_/|_/___/"
echo "        Building: $NAME"
echo "         version: $VERSION"
echo "     Custom make by Trigger"
echo
echo Started: $(date)
echo

if [ "$1" = "clean" ]; then
	echo "Deleting objects..."
	rm -r build/*.o
	echo "Deleting documentation..."
	rm -r docs/doxygen/html
	rm -r docs/doxygen/latex
	exit 0
fi

if [ "$1" = "distclean" ]; then
	echo "Deleting objects..."
	rm -r build/*.o
	echo "Deleting target..."
	rm -r bin/frebons
	echo "Deleting documentation..."
	rm -r docs/doxygen/html
	rm -r docs/doxygen/latex

fi

if [ "$1" = "install" ]; then
	echo "Install not implemented."
	exit 0
fi

DEPBUILD=`dpkg-query -l | grep "build-essential" | wc -l`
DEPMYSQL=`dpkg-query -l | grep "libmysqlclient-dev" | wc -l`

if [ -z "$DEPBUILD" ] || [ "$DEPBUILD" -eq 0 ]; then
	install_depend "build-essential"
fi

if [ -z "$DEPMYSQL" ] || [ "$DEPMYSQL" -eq 0 ]; then
	install_depend "libmysqlclient-dev"
fi

#Compile sources to objects
echo "Compiling..."
for i in src/*.cpp
do
        g++ -c $i $PARAMS
done

echo "Linking..."

#Prepare all objects
for os in *.o
do
        OUTS="$OUTS $os"
done

#Compiling
g++ -o bin/frebons $OUTS $PARAMS
echo "Done... bin/frebons"

#Move objects to build directory
mv *.o build/

#Finished
