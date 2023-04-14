#include <memory>
#include <map>
#include "Cerebrate.h"

class IFactory {
public:
    virtual std::unique_ptr<Cerebrate> Create() = 0;
    virtual ~IFactory() = default;
};

template <typename T>
class Factory : public IFactory {
public:
    virtual std::unique_ptr<Cerebrate> Create() override {
        return std::make_shared<T>();
    }

    ~Factory() = default;
};

class CerebrateRegistry {
public:
    static CerebrateRegistry& GetInstance() {
        static CerebrateRegistry instance;
        return instance;
    }

    template <class Cerebrate>
    void RegisterClass(size_t type_id) {
        register_.insert({type_id, std::make_unique<Factory<TestClass>>()});
    }

    std::unique_ptr<Cerebrate> GetCerbrate(size_t type_id) {
        return register_[type_id]->Create();
    }

private:
    std::unordered_map<size_t, std::unique_ptr<IFactory>> register_;
};
