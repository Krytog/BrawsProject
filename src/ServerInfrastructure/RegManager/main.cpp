#include "Porter.h"

int main() {
    Porter::GetInstance().StartRegistration();
    while (true) {
        Porter::GetInstance().CheckLobbiesState();
    }
}