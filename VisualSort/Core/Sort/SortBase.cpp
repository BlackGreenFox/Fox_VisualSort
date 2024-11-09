#include "SortBase.h"

namespace FoxSort {

    Uint32 SortBase::GetLastStep() const {
        return _last_step;
    }

    void SortBase::Pause() {
        _paused = !_paused;
    }

    void SortBase::SetDelay(int delay) {
        _delay = delay;
    }

    float SortBase::GetComparisonState() const {
        return _comparison_state;
    }

    bool SortBase::IsSorted() const {
        return _sorted;
    }

    void SortBase::Update(SDL_Renderer* renderer, int offsetX, int offsetY, int width, int height) {
        if (_values == nullptr) return;

        int margin = 10;
        int available_width = width - 2 * margin;
        int available_height = height - 2 * margin;
        int num_values = _values->size();
        int line_gap = 1;
        int line_width = (available_width - (num_values - 1) * line_gap) / num_values;
        int thick_line_width = std::max(1, line_width);

 
        for (int i = 0; i < num_values; ++i) {
             if (_sorted && !Style::Gradient){
                 SDL_SetRenderDrawColor(renderer, static_cast<Uint8>(Style::SortComplete[0] * 255),
                     static_cast<Uint8>(Style::SortComplete[1] * 255),
                     static_cast<Uint8>(Style::SortComplete[2] * 255),
                     static_cast<Uint8>(Style::SortComplete[3] * 255));
            }
            else if (i == _j || i == _j + 1) {
                SDL_SetRenderDrawColor(renderer, static_cast<Uint8>(Style::SortCurrentLine[0] * 255),
                    static_cast<Uint8>(Style::SortCurrentLine[1] * 255),
                    static_cast<Uint8>(Style::SortCurrentLine[2] * 255),
                    static_cast<Uint8>(Style::SortCurrentLine[3] * 255));
            }
            else if (Style::Gradient) {
                float t = static_cast<float>((*_values)[i]) / 100.0f;
                Uint8 r = static_cast<Uint8>((1 - t) * Style::SortGradienStartLine[0] * 255 + t * Style::SortGradienEndLine[0] * 255);
                Uint8 g = static_cast<Uint8>((1 - t) * Style::SortGradienStartLine[1] * 255 + t * Style::SortGradienEndLine[1] * 255);
                Uint8 b = static_cast<Uint8>((1 - t) * Style::SortGradienStartLine[2] * 255 + t * Style::SortGradienEndLine[2] * 255);
                Uint8 a = static_cast<Uint8>((1 - t) * Style::SortGradienStartLine[3] * 255 + t * Style::SortGradienEndLine[3] * 255);

                SDL_SetRenderDrawColor(renderer, r, g, b, a);
            }
            else {
                SDL_SetRenderDrawColor(renderer, static_cast<Uint8>(Style::SortLine[0] * 255),
                    static_cast<Uint8>(Style::SortLine[1] * 255),
                    static_cast<Uint8>(Style::SortLine[2] * 255),
                    static_cast<Uint8>(Style::SortLine[3] * 255));
            }

            // Line Draw
            SDL_Rect lineRect = {
                offsetX + margin + i * (thick_line_width + line_gap),
                height - margin - ((*_values)[i] * available_height / 100),
                thick_line_width,
                (*_values)[i] * available_height / 100
            };
            SDL_RenderFillRect(renderer, &lineRect);
        }
    }

}