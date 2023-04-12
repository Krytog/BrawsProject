#include "Cerebrate.h"
#include "Overmind.h"

size_t Cerebrate::GetType() {
    return type_;
}

std::string Cerebrate::GetInfoForOvermind() {
    return SerializeInfo() + buffer_;
}

Cerebrate::Cerebrate(size_t type) : type_(type) {
    Overmind::GetInstance().RegisterNewCerebrate(this);
}

void Cerebrate::AddCommandToBuffer(const std::string& serialized_command) {
    buffer_ += serialized_command;
}