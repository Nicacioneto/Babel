# Babel - Tiamat Team
[![Build Status](https://travis-ci.org/Nicacioneto/Babel.svg?branch=master)](https://travis-ci.org/Nicacioneto/Babel)



Singleplayer game like Sci-fi. The mankind wanders the universe looking for a new habitable planet. They found an unknown planet with a big and strange tower.

The challenge is explore the tower and the planet and expand your resources, but be careful with the mysteries of this new planet.

Usage
------------
Follow this steps to play the game:

Clone the repository:
```
git clone https://github.com/ije-tiamat/babel.git
```

Installing the Engine:
```
$ cd game/lib
$ tar -vzxf ijengine-0.0.5.tar.gz
$ make
$ sudo make install
```

Compiling and opening the game:
```
$ cd ..
$ make
$ bin/babel
```

Enjoy!

Requirements
------------
It's necessary the following packages to compile the game:

- Simple DirectMedia Layer (SDL2) (2.0.3)
    * maintainer: http://www.libsdl.org/download-2.0.php
    * debian/ubuntu: sudo apt-get install libsdl2-dev

- TrueType Font library (SDL2-ttf) (2.0.12)
    * maintainer: http://www.libsdl.org/projects/SDL_ttf/
    * debian/ubuntu: sudo apt-get install libsdl2-ttf-dev

- Image loading library (SDL2-image) (2.0.0)
    * maintainer: http://www.libsdl.org/projects/SDL_image/
    * debian/ubuntu: sudo apt-get install libsdl2-image-dev

- Mixer library (SDL-mixer) (2.0.0)
    * maintainer: http://www.libsdl.org/projects/SDL_mixer/
    * debian/ubuntu: sudo apt-get install libsdl2-mixer-dev


Ou instale o nosso pacote debian, baixando o arquivo babel_1.0_all.deb.
Após a instalação do pacote sigua os passos para execução:

$ cd /usr/bin/babel
$ bin/babel
