# Babel
Jogo para a disciplina de Introdução aos Jogos Eletrônicos

Equipe Tiamat

---

[![Throughput Graph](https://graphs.waffle.io/ije-tiamat/babel/throughput.svg)](https://waffle.io/ije-tiamat/babel/metrics)

[![Stories in Ready](https://badge.waffle.io/ije-tiamat/babel.png?label=ready&title=Ready)](http://waffle.io/ije-tiamat/babel)
[![Stories in Progress](https://badge.waffle.io/ije-tiamat/babel.png?label=in%20progress&title=In%20Progress)](http://waffle.io/ije-tiamat/babel) 
[![Stories in Check](https://badge.waffle.io/ije-tiamat/babel.png?label=check&title=Check)](http://waffle.io/ije-tiamat/babel)

---

Uso
------------
Para rodar o jogo é necessário realizar a instalação prévia da engine. Siga os passos:
```
$ git clone http://github.com/ije-tiamat/ije.git
$ cd engine
$ sudo make install
```
```
$ git clone https://github.com/ije-tiamat/babel.git
$ cd game
$ make
$ bin/babel
```

Requisitos
------------
Antes de rodar Babel, é necessário instalar os seguintes pacotes:
- Simple DirectMedia Layer (SDL2) (2.0.3)
    * mantenedor: http://www.libsdl.org/download-2.0.php
    * debian/ubuntu: sudo apt-get install libsdl2-dev

- TrueType Font library (SDL2-ttf) (2.0.12)
    * mantenedor: http://www.libsdl.org/projects/SDL_ttf/
    * debian/ubuntu: sudo apt-get install libsdl2-ttf-dev

- Image loading library (SDL2-image) (2.0.0)
    * mantenedor: http://www.libsdl.org/projects/SDL_image/
    * debian/ubuntu: sudo apt-get install libsdl2-image-dev

- Mixer library (SDL-mixer) (2.0.0)
    * mantenedor: http://www.libsdl.org/projects/SDL_mixer/
    * debian/ubuntu: sudo apt-get install libsdl2-mixer-dev
