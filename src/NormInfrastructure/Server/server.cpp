#include "Porter.h"

int main() {
    Porter::GetInstance().StartRegistration();
    Porter::GetInstance().StartHandling();
    while (true) {
        Porter::GetInstance().CheckLobbiesState();
    }
}