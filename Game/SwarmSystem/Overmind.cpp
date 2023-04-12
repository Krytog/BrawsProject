#include "Overmind.h"
#include <cstring>

Overmind::Overmind() : current_id_(0) {
}

Overmind& Overmind::GetInstance() {
    static Overmind overmind;
    return overmind;
}

const std::string& Overmind::GetCerebratesInfoSerialized() {
    return cerebrates_info_serialized_;
}

void Overmind::UpdateCelebratesInfo() {
    std::string buffer;
    buffer.reserve(3 * sizeof(size_t) + 3 + AVG_GETINFO_STRLEN);

    for (auto [id, cerebrate] : cerebrates_) {
        buffer += '#';

        auto buffer_sz = buffer.size();
        auto type_id = cerebrate->GetType();

        std::string cerebrate_info = cerebrate->GetInfoForOvermind();
        size_t cerebrate_info_size = cerebrate_info.size();

        buffer.resize(buffer_sz + sizeof(id) + sizeof(type_id) + sizeof(cerebrate_info_size));

        std::memcpy(&buffer[0] + buffer_sz, &id, sizeof(id));
        buffer_sz += sizeof(id);

        std::memcpy(&buffer[0] + buffer_sz, &type_id, sizeof(type_id));
        buffer_sz += sizeof(type_id);

        std::memcpy(&buffer[0] + buffer_sz, &cerebrate_info_size, sizeof(cerebrate_info_size));
        buffer_sz += sizeof(cerebrate_info_size);

        buffer += '<';
        buffer += cerebrate_info;
        buffer += '>';
    }
    cerebrates_info_serialized_ = std::move(buffer);
}

void Overmind::RegisterNewCerebrate(Cerebrate* cerebrate) {
    cerebrates_[current_id_++] = cerebrate;
}

void Overmind::ForceCerebratesExecuteCommands(const std::string& serialized_command) {
    // parsing implementations
}