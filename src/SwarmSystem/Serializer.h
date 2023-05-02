#include <string>
#include <cstring>

class Serializer {
public:
    template <typename T>
    static size_t Serialize(const T& obj, std::string* result, size_t start = 0) {
        std::string bytes;
        bytes.resize(sizeof(obj));

        // little-big endian!?

        std::memcpy(&bytes[0], &obj, sizeof(obj));
        size_t output = bytes.size();
        if (result->size() < start + output) {
            result->resize(start + output);
        }
        for (size_t i = 0; i < output; ++i) {
            (*result)[start + i] = bytes[i];
        }
        return output;
    }

    template <typename T>
    static size_t Deserialize(T& obj, std::string_view src) {
        if (sizeof(obj) != src.size()) {
            return 0;
        }

        // little-big endian!?
        std::memcpy(&obj, src.data(), sizeof(obj));
        return src.size();
    }
};
