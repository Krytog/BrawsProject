#include "EndGameWidget.h"

#define DELAY_TIME 3000

EndGameWidget::EndGameWidget() {
    // bruh;
    ClientEngine::GetInstance().Invoke(std::chrono::milliseconds(DELAY_TIME), &ClientEngine::SetActiveOff, &ClientEngine::GetInstance());
}