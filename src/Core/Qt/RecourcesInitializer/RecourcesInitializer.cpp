#include "RecourcesInitializer.h"

RecourcesInitializer &RecourcesInitializer::GetInstance() {
    static RecourcesInitializer instance;
    return instance;
}

const QImage& RecourcesInitializer::GetImage(std::string_view path_to_image) {
    if (images_.contains(path_to_image.data())) {
        return images_[path_to_image.data()];
    }
    images_[path_to_image.data()] = QImage(path_to_image.data());
    return images_.at(path_to_image.data());
}
