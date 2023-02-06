#include "Canv.h"
#include "Impls.h"

void Canv::Draw(Image image) {
    canv_impl->Draw(image.GetImpl());
}