#include <string>
#include <cstring>

class Serializer {
public:
    template <typename T>
    static size_t Serialize(const T& obj, std::string* result) {
        std::string bytes;
        bytes.resize(sizeof(obj));

        // little-big endian!?

        std::memcpy(&bytes[0], &obj, sizeof(obj));
        *result = std::move(bytes);
        return result->size();
    }

    template <typename T>
    static size_t Deserialize(T& obj, const std::string& src) {
        if (sizeof(obj) != src.size()) {
            return 0;
        }

        // little-big endian!?
        std::memcpy(&obj, src.data(), sizeof(obj));
        return src.size();
    }
};
