# Engine

* `Engine` is the main class in the game. It handles the creation and destruction of all objects, allows to execute delayed callbacks and add events. Engine is split into `ClientEngine` and `ServerEngine` due to Client-Server architecture of the game. Here is the list of Engines' common functions:
    * `void DestroyEvent(Event* event)` - destroys event if present.
    * `CollisionSystem::PossiblePosition CheckTYPECollision(GameObject* first, GameObject* second) const`, where TYPE is Physical of Trigger - check collision of TYPE between two game objects
    * 