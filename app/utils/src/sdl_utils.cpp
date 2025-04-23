#include "../inc/sdl_utils.h"

#include <iostream>

void DisplayMessageBox(const std::string& message) {
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", message.c_str(), nullptr);
}

SDL_Texture* CreateTextTexture(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), color);
    if (!surface) {
        // std::cerr << "TTF_RenderText_Blended Error: " << TTF_GetError() << std::endl;
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!texture) {
        std::cerr << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
    }
    return texture;
}

TTF_Font* createFont(TTF_Font* font, int fontSize) {
    TTF_OpenFont("OpenSans-Regular.ttf", fontSize); // Try Open Sans
    if (!font) {
        font = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", fontSize); // Fallback to DejaVu Sans (Linux)
        if (!font) {
            font = TTF_OpenFont("/Library/Fonts/Arial Unicode.ttf", fontSize); // Fallback to Arial Unicode (Mac)
            if (!font) {
                std::cerr << "Could not open font: " + std::string(TTF_GetError()) +
                    "\n  Please ensure that OpenSans-Regular.ttf, DejaVuSans.ttf, or Arial Unicode.ttf is available.";
            }
        }
    }
    return font;
}