#include "BubbleSort.h"
#include <algorithm>
#include <SDL.h>

BubbleSort::BubbleSort() : values_(nullptr), paused_(true), i_(0), j_(0), swapped_(false), delay_(300), last_step_(0), comparison_state_(0.0f), sorted_(false) {}

void BubbleSort::Init(std::vector<int>& values) {
    values_ = &values;
    paused_ = false;
    i_ = 0;
    j_ = 0;
    swapped_ = false;
    last_step_ = SDL_GetTicks();
    comparison_state_ = 0.0f;
    sorted_ = false;
}

Uint32 BubbleSort::GetLastStep() const {
    return last_step_;
}

void BubbleSort::Step() {
    if (paused_ || values_ == nullptr || i_ >= values_->size() - 1) {
        return;
    }

    Uint32 current_time = SDL_GetTicks();
    if (current_time - last_step_ < delay_) {
        return;
    }
    last_step_ = current_time;

    if (j_ < values_->size() - i_ - 1) {
        // Increment comparison counter
        if ((*values_)[j_] > (*values_)[j_ + 1]) {
            std::swap((*values_)[j_], (*values_)[j_ + 1]);
            swapped_ = true;
            comparison_state_++;  // Reset comparison counter on swap
        }
        else {
            comparison_state_ = 0;
        }
        j_++;
    }
    else {
        if (!swapped_) {
            i_ = values_->size();
            sorted_ = true;  // Indicate that the array is sorted
        }
        j_ = 0;
        i_++;
        swapped_ = false;
    }
}

void BubbleSort::Pause() {
    paused_ = !paused_;
}

void BubbleSort::SetDelay(int delay) {
    delay_ = delay;
}

float BubbleSort::GetComparisonState() const {
    return comparison_state_;
}

bool BubbleSort::IsSorted() const {
    return sorted_;
}

void BubbleSort::Update(SDL_Renderer* renderer, int offsetX, int offsetY, int width, int height) {
    if (values_ == nullptr) return;

    int margin = 10;
    int available_width = width - 2 * margin;
    int available_height = height - 2 * margin;
    int num_values = values_->size();
    int line_gap = 1;  // Gap between lines
    int line_width = (available_width - (num_values - 1) * line_gap) / num_values;
    int thick_line_width = std::max(1, line_width);

    for (int i = 0; i < num_values; ++i) {
        if (sorted_) {
            SDL_SetRenderDrawColor(renderer, 144, 238, 144, 255);  // Light green when sorted
        }
        else if (i == j_ || i == j_ + 1) {
            SDL_SetRenderDrawColor(renderer, 99, 220, 66, 255);  // Highlight the current sorting elements
        }
        else {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // Default color
        }
        SDL_Rect lineRect = {
            offsetX + margin + i * (thick_line_width + line_gap),
            height - margin - ((*values_)[i] * available_height / 100),
            thick_line_width,
            (*values_)[i] * available_height / 100
        };
        SDL_RenderFillRect(renderer, &lineRect);
    }
}
