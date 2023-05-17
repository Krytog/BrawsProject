#pragma once
#include <QImage>
#include <unordered_map>

class RecourcesInitializer {
public:
    static RecourcesInitializer &GetInstance();

    const QImage& GetImage(std::string_view path_to_image);

    ~RecourcesInitializer() = default;

private:
    std::unordered_map<std::string, QImage> images_;
private:
    RecourcesInitializer() = default;

    RecourcesInitializer(const RecourcesInitializer &other) = delete;
    RecourcesInitializer(RecourcesInitializer &&other) = delete;

    RecourcesInitializer &operator=(const RecourcesInitializer &other) = delete;
    RecourcesInitializer &operator=(RecourcesInitializer &&other) = delete;
};