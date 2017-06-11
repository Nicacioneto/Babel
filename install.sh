#!/bin/bash

clear

sudo apt-get update
echo "Installing Simple DirectMedia Layer (SDL2) (2.0.3)"
sudo apt-get install libsdl2-dev
echo "Installing TrueType Font library (SDL2-ttf) (2.0.12)"
sudo apt-get install libsdl2-ttf-dev
echo "Installing Image loading library (SDL2-image) (2.0.0)"
sudo apt-get install libsdl2-image-dev
echo "Installing Mixer library (SDL-mixer) (2.0.0)"
sudo apt-get install libsdl2-mixer-dev

cd game/lib
tar -vzxf ijengine-0.0.5.tar.gz
make
sudo make install

cd ..
make
bin/babel

echo "Saindo..."
exit;
