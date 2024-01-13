# Brawls 

Brawls is a simple multiplayer duel game with sprite-based 2D graphics. It's built using ```C++```, with ```Qt``` handling the graphics and ```Boost``` managing the networking. The game itself is based on custom game engine framework and replication system, which can be reused for a number of different projects.

The game has a client-server architecture. All gameplay logic is executed on a dedicated server, while game clients are responsible for visualizing the gameplay and handling player input.

In addition to standard keyboard and mouse controls, the game supports control by a third-party bot programs, which can be written in any programming language.


## Setup

The project can be built with ```CMake```. It is assumed that ```Qt5``` and ```Boost``` are already installed. To begin, you need to clone this repository and navigate to its root ```.../BrawsProject/```. Then, follow the standard command sequence:
  * Create a build directory: ```mkdir build```
  * Enter it: ```cd build```
  * Generate make files: ```cmake ..```
  * Build Client target: ```make Client```

Now, to play the game on a remote server, simply run the compiled program: ```./Client```.

If you want to run the game on your own server, follow these steps. Build the server target: ```make Server```. In the client source code, you need to change the default server IP to your server IP. To do so, you need to change the constant ```GAME_HOST``` in ```.../BrawsProject/src/NormInfrastructure/GameInfo.h```. Specifically, if you want to play the game locally, use ```localhost``` as the IP. Then rebuild the client: ```make Client```. To start the server, simply run the server program: ```./Server```. Once the server is running, you can run the clients and start playing. 

## Gameplay

Some screenshots of the game graphics:
|  |  |
|--|--|
![gameplay screenshot 1](docs/Images/gameplay1.png) | ![gameplay screenshot 2](docs/Images/gameplay2.png)
![gameplay screenshot 3](docs/Images/gameplay3.png) | ![gameplay screenshot 4](docs/Images/gameplay4.png)

Two characters appear at opposite ends of the map at the beginning of the match. There are various boosters scattered across the game arena. Additionally, the arena features different obstacles, shelters, and destructible objects. Over time, the arena boundaries start to constrict, forcing the characters closer to each other.

Character attacks inflict damage on other characters. Each character has a limited number of attacks, but they recharge over time if the player refrains from attacking. Character health slowly regenerates if they avoid taking damage for a certain period. The last surviving player wins the match.
