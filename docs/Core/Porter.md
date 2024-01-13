# Porter

* `Porter` is the main class to reg users and manage game lobbies to provide multiplayer game conception and possibility to set some game settings. Here is the list of Porters' common functions:
    * `void StartRegistration()` - starts the registration process, sends to clients unique id to continue the game process.
    * `void StartHandling()` - calls after registration, receives from client request with info about game option, character type and e.t.c.
    * `void HandleConnection(uint64_t user_id, tcp::socket& connection, const Request& request)` - help method for `void StartHandling()`, realises logic that attaches game lobbies and request validation
    * `void SendInitGamePackages(const Lobby& lobby)` - calls after lobby game start was approved, sends corresponding packages to clients to notify the game was started on server.
    * `uint64_t RegId() && uint64_t RegLobbyId()` - reg unique client/lobby that was now already registered
    * `void CheckLobbiesState()` - calls every server tick to go through lobbies and check their active state. If lobby is finished it removes, if is ready calls InitGame function.