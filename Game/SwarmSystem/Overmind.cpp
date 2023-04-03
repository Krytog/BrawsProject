#include "Overmind.h"

Overmind::Overmind(): current_id_(0) {}

Overmind &Overmind::GetInstance() {
    static Overmind overmind;
    return overmind;
}

const std::string& Overmind::GetCerebratesInfoSerialized() {
    return cerebrates_info_serialized_;
}

void Overmind::UpdateCelebratesInfo() {
    cerebrates_info_serialized_.clear();
    for (auto& [id, cerebrate] : cerebrates_) {
        // THIS IS TEMPORARY IMPLEMENTATION, SHOULD BE CHANGED IN THE NEAREST FUTURE
        cerebrates_info_serialized_ += '#';
        cerebrates_info_serialized_ += std::to_string(cerebrate->GetType());
        cerebrates_info_serialized_ += '<';
        cerebrates_info_serialized_ += cerebrate->GetInfoForOvermind();
        cerebrates_info_serialized_ += '>';
    }
}

void Overmind::RegisterNewCerebrate(Cerebrate *cerebrate) {
    cerebrates_[current_id_++] = cerebrate;
}

void Overmind::ForceCerebratesExecuteCommands(const std::string &serialized_command) {
    //parsing implementations
}