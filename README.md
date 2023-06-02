# Brawls 

Brawls is a simple multiplayer game to play with your friends. Play as a team or alone, collect boosters and fight your enemies!

## Setup

Для сборки всего проекта используется CMake. Для начала требуется склонировать репозиторий, и зайти в его корень ```.../BrawsProject```. Далее:
  * Созаём директорию build: ```mkdir build```
  * Заходим в неё: ```cd build```
  * Генерируем make files: ```cmake ..```
  * Собираем Client target: ```make Client```
Теперь, при желании поиграть на удалённом сервере достатчно запустить программу ```./Client```. При желании запустить сервер на локальной машине требуется также собрать таргет сервера: ```make Server```, поменять в ```.../BrawsProject/src/NormInfrastructure/GameInfo.h``` константу ```GAME_HOST``` на ```localhost```. Далее нужно локально запустить сервер ```./Server```, после чего можно запускать клиентов.

