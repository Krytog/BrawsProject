#include "DrawTextHelper.h"
#include <ostream>
#include <string_view>
#include <iostream>

 DrawTextHelper::DrawTextHelper(TextSprite* text, const size_t& clockwise_angle): text_(text), angle_(clockwise_angle) {}

void DrawTextHelper::Paint(Painter *painter) const {
    auto real_painter = dynamic_cast<QPainter*>(painter);
    const auto& size = text_->GetSize();
    const auto& pos = text_->GetPosition().GetCoordinates();
    auto data = text_->GetText().data();

    real_painter->save();

    real_painter->translate(pos.first, -pos.second);
    real_painter->rotate(360 - angle_);
    real_painter->translate(- (static_cast<int64_t>(size.first) / 2),
         - (static_cast<int64_t>(size.second) / 2));

    QFontMetrics font_metrics(real_painter->font());
    real_painter->scale(size.first / font_metrics.horizontalAdvance(data),
         size.second / font_metrics.height());

    real_painter->drawText(QRectF(0, 0, size.first, size.second), data);
    real_painter->restore();
}