#include "Overmind.h"
#include <cstring>
#include <unordered_set>

#include "Register.h"

namespace {
    bool FunctorAlwaysTrue(Cerebrate* first, Cerebrate* second) {
        return true;
    }
}

enum {
    BEGIN_ENTITY_TOKEN = '#',
    OPEN_INFO_TOKEN = '<',
    CLOSE_INFO_TOKEN = '>'
};

Overmind::Overmind() : current_id_(0) {
}

Overmind& Overmind::GetInstance() {
    static Overmind overmind;
    return overmind;
}

const std::string& Overmind::GetCerebratesInfoSerialized() {
    return cerebrates_info_serialized_;
}

void Overmind::UpdateCerebratesInfo() {
    UpdateCerebratesInfo(nullptr, FunctorAlwaysTrue);
}

size_t Overmind::RegisterNewCerebrate(Cerebrate* cerebrate) {
    size_t out = current_id_;
    cerebrates_[current_id_++] = cerebrate;
    return out;
}

#include <iostream>

void Overmind::ForceCerebratesExecuteCommands(std::string_view serialized_command) {
    std::cout << cerebrates_.size() << std::endl;
    size_t beg = 0, ptr = 1;
    while (ptr < serialized_command.size()) {
        while (ptr < serialized_command.size() && serialized_command[ptr] != BEGIN_ENTITY_TOKEN) {
            ++ptr;
        }

        size_t id;
        size_t type_id;
        size_t cerebrate_info_size;

        if (beg + 1 + sizeof(id) + sizeof(type_id) + sizeof(cerebrate_info_size) > serialized_command.size()) {
            beg = ptr++;
            continue;
        }

        std::memcpy(&id, &serialized_command[beg + 1], sizeof(id));

        std::memcpy(&type_id, &serialized_command[beg + 1 + sizeof(id)], sizeof(type_id));

        std::memcpy(&cerebrate_info_size, &serialized_command[beg + 1 + sizeof(id) + sizeof(type_id)],
                    sizeof(cerebrate_info_size));

        if (cerebrates_.contains(id)) {
            if (cerebrates_.at(id)->GetType() != type_id) {
                std::cout << "AT " << id << " EXPECTED " << cerebrates_.at(id)->GetType() << " GOT " << type_id << std::endl;
                throw std::runtime_error("SwarmSystem corrupted data: cerebrates type mismatch");
            }
            cerebrates_.at(id)->ForcePossessedExecuteCommand(
                serialized_command.substr(beg + 2 + 3 * sizeof(size_t), cerebrate_info_size));
        } else {
            std::cout << current_id_ << " CREATING AT INDEX " << id << " OF TYPE " << type_id << std::endl;
            auto new_cerebrate = CerebrateRegistry::GetInstance().GetCerebrate(type_id);
            if (new_cerebrate) {
                cerebrates_[id] = new_cerebrate;
                cerebrates_.at(id)->ForcePossessedExecuteCommand(
                        serialized_command.substr(beg + 2 + 3 * sizeof(size_t), cerebrate_info_size));
            } else {
                // corrupted data case
            }
        }

        beg = ptr++;
    }
}

void Overmind::ActualizeCerebrates(std::string_view serialized_command) {
    std::unordered_set<size_t> set;
    for (size_t i = 0; i < serialized_command.size(); ++i) {
        if (serialized_command[i] == BEGIN_ENTITY_TOKEN && i + 1 < serialized_command.size()) {
            size_t id;
            if (i + 1 + sizeof(id) > serialized_command.size()) {
                continue;
            }
            std::memcpy(&id, &serialized_command[i + 1], sizeof(id));
            set.insert(id);
        }
    }
    for (auto& elem : cerebrates_) {
        if (!set.contains(elem.first)) {
            cerebrates_.erase(elem.first);
            delete elem.second;
        }
    }
}

Overmind::~Overmind() {
    for (auto& elem : cerebrates_) {
        delete elem.second;
    }
}

void Overmind::DestroyCerebrate(size_t cerebrate_id) {
    if (!cerebrates_.contains(cerebrate_id)) {
        return;
    }
    delete cerebrates_.at(cerebrate_id);
    cerebrates_.erase(cerebrate_id);
}

void Overmind::RegisterNewPlayer(uint64_t id, size_t cerebrate_id) {
    players_cerebrate_[id] = cerebrate_id;
}

Cerebrate *Overmind::GetPlayersCerebrate(uint64_t id) {
    if (!players_cerebrate_.contains(id)) {
        return nullptr;
    }
    return cerebrates_.at(players_cerebrate_.at(id));
}

void Overmind::DeletePlayerFromRegistry(uint64_t id) {
    if (!players_cerebrate_.contains(id)) {
        return;
    }
    players_cerebrate_.erase(id);
}

void Overmind::UpdateCerebratesInfo(Cerebrate* target, bool (*functor)(Cerebrate*, Cerebrate*)) {
    std::string buffer;
    buffer.reserve(3 * sizeof(size_t) + 3 + AVG_GETINFO_STRLEN);

    for (auto [id, cerebrate] : cerebrates_) {
        if (!functor(target, cerebrate)) {
            continue;
        }

        buffer += BEGIN_ENTITY_TOKEN;

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
//        buffer_sz += sizeof(cerebrate_info_size);

        buffer += OPEN_INFO_TOKEN;
        buffer += cerebrate_info;
        buffer += CLOSE_INFO_TOKEN;
    }
    cerebrates_info_serialized_ = std::move(buffer);
}

Cerebrate* Overmind::GetCerebrateWithId(size_t id) const {
    if (cerebrates_.contains(id)) {
        return cerebrates_.at(id);
    }
    return nullptr;
}