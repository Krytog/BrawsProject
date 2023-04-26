#include <memory>
#include <unordered_map>
#include "Cerebrate.h"

class IFactory {
public:
    virtual Cerebrate* Create() = 0;
    virtual ~IFactory() = default;
};

template <typename T>
class Factory : public IFactory {
public:
    virtual Cerebrate* Create() override {
        return new T();
    }

    ~Factory() = default;
};

class CerebrateRegistry {
public:
    static CerebrateRegistry& GetInstance() {
        static CerebrateRegistry instance;
        return instance;
    }

    CerebrateRegistry(const CerebrateRegistry& other) = delete;
    CerebrateRegistry(CerebrateRegistry&& other) = delete;

    template <class Cerebrate>
    void RegisterClass(size_t type_id) {
        register_.insert({type_id, std::make_unique<Factory<Cerebrate>>()});
    }

    Cerebrate* GetCerebrate(size_t type_id) {
        return register_[type_id]->Create();
    }

private:
    CerebrateRegistry() = default;
    std::unordered_map<size_t, std::unique_ptr<IFactory>> register_;
};
