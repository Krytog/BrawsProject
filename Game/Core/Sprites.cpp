#include "Sprites.h"

#include <list>
#include <memory>
#include <queue>

#include "Canvas.h"

class VisibleObjectImpl {
public:
    VisibleObjectImpl(const Position* pos, const size_t& width, const size_t& height,
                      const size_t& render_level, bool is_cycled = true)
        : pos_(*pos), width_(width), height_(height), render_level_(render_level), is_cycled_(is_cycled){};

    size_t GelRenderLevel() const {
        return render_level_;
    }

    bool IsFinished() const {
        return is_finished_;
    }

    bool IsAbleToInterrupt() const {
        return is_able_to_interrupt_;
    }

    virtual void UpdatePosition(const Position& position) = 0;
    virtual void Translate(const Vector2D& vector2D) = 0;

    virtual void RenderIt(Canvas* canvas) = 0;
    virtual void Reset() = 0;

protected:
    Position pos_; /* Coincides with the center of the object, that is, the usual local Position of
        the GameObject, where Render will draw it */

    size_t width_;
    size_t height_;

    size_t render_level_;

    bool is_finished_ = false; /* Object animation complete */
    bool is_cycled_ =
        true; /* Object animation is cycled, only meaningful for animated sprites and sequncers */
    bool is_able_to_interrupt_ =
        false; /* Animation/Change of VisibleObject can be interrupted at the moment */
};

///////////////////////////////////////////////////////////////////////

class StaticSpriteImpl : public VisibleObjectImpl {
public:
    StaticSpriteImpl(const Position* pos, const size_t& width, const size_t& height,
                     std::string_view path_to_file, const size_t& render_level)
        : VisibleObjectImpl(pos, width, height, render_level), image_(pos_, width, height) {
        try {
            image_.LoadFromFile(path_to_file);
        } catch (...) {
            image_.SetDefaultImage();
        }
    }

    void RenderIt(Canvas* canvas) override {
        canvas->Draw(&image_);
        if (!is_finished_) {
            is_finished_ = true;
        }
    }

    void UpdatePosition(const Position& position) override {
        pos_ = position;
        image_.UpdatePosition(position);
    }

    void Translate(const Vector2D& vector2D) override {
        pos_.Translate(vector2D);
        image_.Translate(vector2D);
    }

    void Reset() override {
        is_finished_ = false;
    }

private:
    Canvas::Image image_;
};

StaticSprite::StaticSprite(const Position* pos, const size_t& width, const size_t& height,
                           std::string_view path_to_file, const size_t& render_level) {
    impl_ = std::make_unique<StaticSpriteImpl>(pos, width, height, path_to_file, render_level);
}

void StaticSprite::UpdatePosition(const Position& position) {
    impl_->UpdatePosition(position);
}

void StaticSprite::Translate(const Vector2D& vector2D) {
    impl_->Translate(vector2D);
}

void StaticSprite::RenderIt(Canvas* canvas) const {
    impl_->RenderIt(canvas);
}

size_t StaticSprite::GetRenderLevel() const {
    return impl_->GelRenderLevel();
}

bool StaticSprite::IsFinished() const {
    return impl_->IsFinished();
}

bool StaticSprite::IsAbleToInterrupt() const {
    return impl_->IsAbleToInterrupt();
}

void StaticSprite::Reset() {
    impl_->Reset();
}

StaticSprite::~StaticSprite() noexcept = default;

/////////////////////////////////////////////////////////////////

class AnimatedSpriteImpl : public VisibleObjectImpl {
public:
    AnimatedSpriteImpl(const Position* pos, const size_t& width, const size_t& height,
                       std::string_view path_to_file, const size_t& render_level,
                       const size_t& tics_per_frame, const size_t& frames_count_width,
                       const size_t& frames_count_height, const std::unordered_set<size_t>& interrupt_points,
                       bool is_cycled)
        : VisibleObjectImpl(pos, width, height, render_level, is_cycled),
          image_(pos_, width, height),
          tics_per_frame_(tics_per_frame),
          frames_count_width_(frames_count_width),
          frames_count_height_(frames_count_height),
          interrupt_points_(interrupt_points) {
        try {
            static_image_.LoadFromFile(path_to_file);
        } catch (...) {
            static_image_.SetDefaultImage();
        }
        kFrameSizeX = static_cast<double>(static_image_.GetRealSize().first) / frames_count_width_;
        kFrameSizeY = static_cast<double>(static_image_.GetRealSize().second) / frames_count_height;
    };

    void RenderIt(Canvas* canvas) override {
        if (is_finished_) {
            canvas->Draw(&image_);
            return;
        }

        if (++current_rate_ == tics_per_frame_) { /* Frame ended */
            current_rate_ = 0;
            if (++current_frame_x_ == frames_count_width_) { /* The last frame in the line is played */
                current_frame_x_ = 0;
                if (++current_frame_y_ ==
                    frames_count_height_) { /* The last frame in the whole sprite has been played */
                    current_frame_y_ = 0;
                    if (!is_cycled_) { /* Just mark as finished with no change of frame and return */
                        is_finished_ = true;
                        canvas->Draw(&image_);
                        return;
                    }
                }
            }
        }

        const size_t current_frame = (current_frame_y_ * frames_count_width_) + current_frame_x_;
        is_able_to_interrupt_ = false;
        if (current_rate_ == tics_per_frame_ - 1 &&
            interrupt_points_.contains(
                current_frame)) { /* One of the interrupt-allowed frames has been played */
            is_able_to_interrupt_ = true;
        }

        image_.LoadFromStaticImage(
            static_image_, Position(0, 0),
            Position(current_frame_x_ * kFrameSizeX, current_frame_y_ * kFrameSizeY),
            Position((current_frame_x_ + 1) * kFrameSizeX, (current_frame_y_ + 1) * kFrameSizeY), kFrameSizeX,
            kFrameSizeY);
        canvas->Draw(&image_);
    }

    void UpdatePosition(const Position& position) override {
        pos_ = position;
        image_.UpdatePosition(position);
    }

    void Translate(const Vector2D& vector2D) override {
        pos_.Translate(vector2D);
        image_.Translate(vector2D);
    }

    void Reset() override {
        current_frame_x_ = 0;
        current_frame_y_ = 0;
        current_rate_ = 0;
        is_finished_ = false;
        is_able_to_interrupt_ = false;
    }

private:
    Canvas::StaticImage static_image_;  // All frames
    Canvas::Image image_;               // Current frame image

    const size_t frames_count_width_;
    const size_t frames_count_height_;

    const size_t tics_per_frame_;

    size_t current_frame_x_ = 0;
    size_t current_frame_y_ = 0;

    size_t current_rate_ = 0;

    std::unordered_set<size_t> interrupt_points_;

    // Frame size on real image
    size_t kFrameSizeX;
    size_t kFrameSizeY;
};

AnimatedSprite::AnimatedSprite(const Position* pos, const size_t& width, const size_t& height,
                               std::string_view path_to_file, const size_t& render_level,
                               const size_t& tics_per_frame, const size_t& frames_count_width,
                               const size_t& frames_count_height,
                               const std::unordered_set<size_t>& interrupt_points, bool is_cycled) {
    impl_ = std::make_unique<AnimatedSpriteImpl>(pos, width, height, path_to_file, render_level,
                                                 tics_per_frame, frames_count_width, frames_count_height,
                                                 interrupt_points, is_cycled);
}

void AnimatedSprite::RenderIt(Canvas* canvas) const {
    impl_->RenderIt(canvas);
}

void AnimatedSprite::UpdatePosition(const Position& position) {
    impl_->UpdatePosition(position);
}

void AnimatedSprite::Translate(const Vector2D& vector2D) {
    impl_->Translate(vector2D);
}

size_t AnimatedSprite::GetRenderLevel() const {
    return impl_->GelRenderLevel();
}

bool AnimatedSprite::IsFinished() const {
    return impl_->IsFinished();
}

bool AnimatedSprite::IsAbleToInterrupt() const {
    return impl_->IsAbleToInterrupt();
}

void AnimatedSprite::Reset() {
    impl_->Reset();
}

AnimatedSprite::~AnimatedSprite() noexcept = default;

//////////////////////////////////////////////////////////////////////////

/* In this implementation, a pimple is not used for, because while it is meaningless, it is desirable to use
 * this pattern in the future */
AnimationSequencer::AnimationSequencer(
    const std::vector<std::pair<std::string_view, VisibleObject*>>& params_list,
    const std::unordered_set<std::string_view>& interrupt_points, bool is_cycled)
    : BasicSequencer<VisibleObject>(params_list, is_cycled), interrupt_points_(interrupt_points){};

void AnimationSequencer::SwitchAnimationTo(std::string_view tag, SwitchOption option) {
    if (option == SwitchOption::FORCE) {
        cur_object_tag_ = tag;
        std::queue<SwitchInfo> empty_queue;
        std::swap(switch_queue_, empty_queue); /* Fast clear */
        return;
    }
    switch_queue_.push({tag, option});
}

void AnimationSequencer::RenderIt(Canvas* canvas) const {
    const Node& cur_node = display_objects_.at(cur_object_tag_);
    if (switch_queue_.empty()) {
        if (cur_node.object->IsFinished() && !cur_node.outcoming.empty()) {
            cur_node.object->Reset();
            cur_object_tag_ = *cur_node.outcoming.begin();
        }
    } else if (switch_queue_.front().option == SwitchOption::SOFT && cur_node.object->IsFinished()) {
        cur_node.object->Reset();
        cur_object_tag_ = switch_queue_.front().tag;
        switch_queue_.pop();
    } else if (switch_queue_.front().option == SwitchOption::MIXED &&
               (cur_node.object->IsAbleToInterrupt() || cur_node.object->IsFinished())) {
        cur_node.object->Reset();
        cur_object_tag_ = switch_queue_.front().tag;
        switch_queue_.pop();
    }
    display_objects_.at(cur_object_tag_).object->RenderIt(canvas);
}

void AnimationSequencer::UpdatePosition(const Position& position) {
    for (auto& [tag, node] : display_objects_) {
        node.object->UpdatePosition(position);
    }
}

void AnimationSequencer::Translate(const Vector2D& vector2D) {
    for (auto& [tag, node] : display_objects_) {
        node.object->Translate(vector2D);
    }
}

bool AnimationSequencer::IsFinished() const {
    return display_objects_.at(end_object_tag_).object->IsFinished();
}

bool AnimationSequencer::IsAbleToInterrupt() const {
    return display_objects_.at(cur_object_tag_).object->IsFinished() &&
           interrupt_points_.contains(
               cur_object_tag_); /* The animation of the object has played up to the last frame and the object
                                    itself is marked as valid for interruption */
}

size_t AnimationSequencer::GetRenderLevel() const {
    return display_objects_.at(cur_object_tag_).object->GetRenderLevel();
}

void AnimationSequencer::Reset() {
    BasicSequencer::Reset(); /* Sets the start animation to the place of the current one */
}