#pragma once

#include <Core/ClientEngine.h>

class EndGameWidget : public PersistentVisibleObject {
public:
    enum Status {
        WIN, LOSE
    };

    EndGameWidget(EndGameWidget::Status status);
};
