#ifndef SDL_UTILS_H
#define SDL_UTILS_H

#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

void DisplayMessageBox(const std::string& message);
SDL_Texture* CreateTextTexture(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, SDL_Color color);
TTF_Font* createFont(TTF_Font* font, int fontSize);

#endif