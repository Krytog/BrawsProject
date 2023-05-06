#include "Render.h"


Render& Render::GetInstance() {
    static Render event_handler;
    return event_handler;
}