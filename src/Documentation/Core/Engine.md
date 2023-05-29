# Engine

* `Engine` is the main class in the game. It handles the creation and destruction of all objects, allows to execute delayed callbacks and add events. Engine is split into `ClientEngine` and `ServerEngine` due to Client-Server architecture of the game. Here is the list of Engines' common functions:
    * `void DestroyEvent(Event* event)` - destroys event if present.
    * `CollisionSystem::PossiblePosition CheckPhysicalCollision(GameObject* first, GameObject* second) const` - check physical collision between two objects.
    * `CollisionSystem::PossiblePosition CheckTriggerCollision(GameObject* first, GameObject* second) const` - check trigger collision between two objects.
    * `CollisionSystem::CollisionsInfoArray GetAllCollisions(GameObject* game_object) const` - get all collisions with game object.
    * `CollisionSystem::CollisionsInfoArray GetPhysicalCollisions(GameObject* game_object) const` - get all physical collisions with game object.
    * `CollisionSystem::CollisionsInfoArray GetTriggerCollisions(GameObject* game_object) const` - get all trigger collisions with game object.
    * `CollisionSystem::CollisionsInfoArray GetAllCollisionsWithTag(GameObject* game_object,
      std::string_view string) const` - get all collisions between game object and objects of exact tag
    * `template <typename U>
      CollisionSystem::CollisionsInfoArray GetAllCollisionsWithType(GameObject* game_object) const` - get all collisions between game object and objects of exact type