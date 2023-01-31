#pragma once

#include <unordered_set>
#include <string_view>
#include <vector>
#include <memory>
#include <unordered_map>

template <class T>
class BasicSequencer {
public:
    BasicSequencer(const std::vector<std::pair<std::string_view, T *>> &params_list,
                   bool is_cycled = true)
        : cur_object_tag_(params_list.front().first),
          start_object_tag_(params_list.front().first),
          end_object_tag_(params_list.back().first) {
        for (const auto &params : params_list) {
            AddByTag(params.first, params.second);
        }
        for (size_t it = 0; it < params_list.size() - 1; ++it) {
            AddEdgeFromTo(params_list[it].first, params_list[it + 1].first);
        }
        if (is_cycled) {
            AddEdgeFromTo(params_list.back().first, params_list.front().first);
        }
    }

    void RemoveByTag(std::string_view tag) {
        IsAt(tag);
        if (tag == start_object_tag_) {
            throw std::runtime_error("Sequncer: Unable to delete start animation object!");
        }
        Node &node = display_objects_.at(tag);
        for (const auto &incom_tag : node.incoming) {
            display_objects_[incom_tag].outcoming.erase(tag);
        }
        for (const auto &outcom_tag : node.outcoming) {
            display_objects_[outcom_tag].incoming.erase(tag);
        }
        display_objects_.erase(tag);
    }

    void AddByTag(std::string_view tag, T *ptr) {
        IsNotAt(tag);
        display_objects_[tag].object = std::unique_ptr<T>(ptr);
    }

    void AddEdgeFromTo(std::string_view from_tag, std::string_view to_tag) {
        IsAt(from_tag);
        IsAt(to_tag);
        Node &from_node = display_objects_.at(from_tag);
        Node &to_node = display_objects_.at(to_tag);
        if (!from_node.outcoming.empty()) {
            throw std::runtime_error("Sequencer: Each node can contain only one outgoing edge!");
        }
        from_node.outcoming.insert(to_tag);
        to_node.incoming.insert(from_tag);
    }

    void AddAfterByTag(std::string_view after_node_tag, std::string_view tag, T *ptr) {
        IsAt(after_node_tag);
        IsNotAt(tag);
        display_objects_[tag].object = std::unique_ptr<T>(ptr);
        Node &after_node = display_objects_.at(after_node_tag);
        Node &node = display_objects_.at(tag);

        if (!after_node.outcoming.empty()) {
            throw std::runtime_error("Sequencer: Each node can contain only one outgoing edge!");
        }
        after_node.outcoming.insert(tag);
        node.incoming.insert(after_node_tag);
    }

    void SetStartTag(std::string_view tag) {
        IsAt(tag);
        start_object_tag_ = tag;
    }

    void SetEndTag(std::string_view tag) {
        IsAt(tag);
        end_object_tag_ = tag;
    }

    void Reset() {
        cur_object_tag_ = start_object_tag_;
    }

protected:
    struct Node {
        std::unique_ptr<T> object;
        std::unordered_set<std::string_view> incoming;
        std::unordered_set<std::string_view> outcoming;
    };

    void IsAt(std::string_view tag) {
        if (!display_objects_.count(tag)) {
            throw std::runtime_error("Sequencer: No entity added with this tag!");
        }
    }

    void IsNotAt(std::string_view tag) {
        if (display_objects_.count(tag)) {
            throw std::runtime_error(
                "Sequencer: An object with the same name has already been added!");
        }
    }

    std::unordered_map<std::string_view, Node> display_objects_;
    mutable std::string_view cur_object_tag_;

    std::string_view
        start_object_tag_; /* For now, we will assume that we do not delete the start animation */
    std::string_view end_object_tag_; /* After this object is finished, the entire sequencer will be
                                         considered finished. */
};