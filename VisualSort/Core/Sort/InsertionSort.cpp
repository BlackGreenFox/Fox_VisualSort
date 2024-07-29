#include "InsertionSort.h"

InsertionSort::InsertionSort()
{
    _values = nullptr;
    _paused = true;
    _i = 0;
    _j = 0;
    _delay = 0;
    _swapped = false;
    _last_step = SDL_GetTicks();
    _comparison_state = 0.0f;
    _sorted = false;
}

void InsertionSort::Init(std::vector<int>& values, int delay) {
    _values = &values;
    _paused = true;
    _i = 0;
    _j = 0;
    _delay = delay;
    _swapped = false;
    _last_step = SDL_GetTicks();
    _comparison_state = 0.0f;
    _sorted = false;
}


void InsertionSort::Step() {
    if (_paused || _values == nullptr || _i >= _values->size() - 1) {
        return;
    }

    Uint32 current_time = SDL_GetTicks();
    if (current_time - _last_step < _delay) {
        return;
    }
    _last_step = current_time;

    if (_i < _values->size()) {
        int key = (*_values)[_i];
        _j = _i - 1;

        while (_j >= 0 && (*_values)[_j] > key) {
            (*_values)[_j + 1] = (*_values)[_j];
            _j = _j - 1;
        }
        (*_values)[_j + 1] = key;

        _i++;
    }
    else {
        _sorted = true;
    }
}

void InsertionSort::Update(SDL_Renderer* renderer, int offsetX, int offsetY, int width, int height) {
    if (_values == nullptr) return;

    int margin = 10;
    int available_width = width - 2 * margin;
    int available_height = height - 2 * margin;
    int num_values = _values->size();
    int line_gap = 1;  // Gap between lines
    int line_width = (available_width - (num_values - 1) * line_gap) / num_values;
    int thick_line_width = std::max(1, line_width);

    for (int i = 0; i < num_values; ++i) {
        if (_sorted) {
            SDL_SetRenderDrawColor(renderer, 144, 238, 144, 255);  // Light green when sorted
        }
        else if (i == _j || i == _j + 1) {
            SDL_SetRenderDrawColor(renderer, 99, 220, 66, 255);  // Highlight the current sorting elements
        }
        else {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // Default color
        }
        SDL_Rect lineRect = {
            offsetX + margin + i * (thick_line_width + line_gap),
            height - margin - ((*_values)[i] * available_height / 100),
            thick_line_width,
            (*_values)[i] * available_height / 100
        };
        SDL_RenderFillRect(renderer, &lineRect);
    }
}

std::string InsertionSort::GetDescription() const {
    return "Insertion sort iterates, consuming one input element each repetition, and grows a sorted output list. At each iteration, insertion sort removes one element from the input data, finds the location it belongs within the sorted list, and inserts it there. It repeats until no input elements remain.";
}

std::vector<float> InsertionSort::GetBigOPlotData() const {
    std::vector<float> big_o_plot(_values->size());
    for (size_t i = 0; i < _values->size(); ++i) {
        big_o_plot[i] = static_cast<float>(i * i);
    }
    return big_o_plot;
}
