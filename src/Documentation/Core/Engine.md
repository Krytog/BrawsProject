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
    * `    template <typename Callable, typename... Args>
      void Invoke(const std::chrono::milliseconds& milliseconds, Callable&& cb, Args&&... args)` - add delayed callback to queue (in milliseconds)
    * `template <typename Callable, typename... Args>
      void Invoke(const uint64_t ticks_count, Callable&& cb, Args&&... args)` - add delayed callback (in Engine ticks)
    * `template <...>
      Event* CreateEvent(Predicate&& pr, PredicateArgsTuple&& pr_args, [Invoker* invoker,] Callable&& cb,
      CallableArgsTuple&& cb_args, EventStatus status)` - add event to Engine
    * `uint64_t GetTicksCount() const` - get current ticks count
    * `void ClearAll()` - delete all objects, events and delayed callbacks from Engine
    * `void ExecuteUpdates()` - update game objects by executing GameObject::OnUpdate() method
    * `void TryExecuteDelayedCallbacks()` - execute delayed callbacks 
    * `void TryExecuteEvents()` - execute events
    * `void IncreaseTicksCount()` - increase Engine ticks count
    * `bool IsActive() const` - check if Engine is active
    * `void SetActiveOn()` - set Engine active
    * `void SetActiveOff()` - set Engine active off
    * `void DebugInfo() const` - for debug purposes
