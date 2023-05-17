#pragma once

enum CharacterCommands {
    COMMAND_CAPTURE_VIEWPORT = 'V',
    COMMAND_RECEIVE_DAMAGE = 'D',
    COMMAND_SHOOT = 'S',
    COMMAND_ON_WIN = 'W',
    COMMAND_ON_LOSE = 'L',
    COMMAND_HEAL = 'H'
};

enum ProjectileCommands {
    COMMAND_HIT_DESTROY = 'H',
};