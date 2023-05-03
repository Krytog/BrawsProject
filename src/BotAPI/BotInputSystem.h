#include <Core/InputSystem.h>

class BotInputSystem : public InputSystem {
public:
    void ReadNewInput() override;

    static BotInputSystem& InitInstance(const sf::Window&);
private:
    explicit BotInputSystem(const sf::Window&);
    BotInputSystem(const BotInputSystem&) = delete;
    BotInputSystem& operator=(const BotInputSystem&) = delete;
};