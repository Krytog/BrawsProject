#include "Cerebrate.h"
#include "Overmind.h"

size_t Cerebrate::GetType() const {
    return type_;
}

std::string Cerebrate::GetInfoForOvermind() {
    auto temp = std::move(buffer_);
    return SerializeInfo() + temp;
}

Cerebrate::Cerebrate(size_t type) : type_(type) {
    Overmind::GetInstance().RegisterNewCerebrate(this);
}

void Cerebrate::AddCommandToBuffer(std::string_view serialized_command) {
    buffer_ += serialized_command;
}

Cerebrate::~Cerebrate() = default;