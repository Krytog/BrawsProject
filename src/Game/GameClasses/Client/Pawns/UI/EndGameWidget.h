#pragma once

#include <Core/ClientEngine.h>

class EndGameWidget : public VisibleGameObject {
public:
    enum Status {
        WIN, LOSE
    };

    void OnUpdate() override;

    EndGameWidget(EndGameWidget::Status status);
};
