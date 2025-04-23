// views/MediaPlayView.h
#ifndef MEDIA_PLAY_VIEW
#define MEDIA_PLAY_VIEW

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "IView.h"
#include "../../controllers/inc/MediaPlayController.h"

class MediaPlayView : public IView {
    int screenWidth;
    int screenHeight;
    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* font;
    SDL_Color textColor;
    SDL_Rect filePathRect;
    SDL_Rect playlistRect;
    SDL_Rect timeRect;
    SDL_Rect prevButtonRect;
    SDL_Rect playPauseButtonRect;
    SDL_Rect nextButtonRect;
public:
    MediaPlayView(MediaPlayController* controller, IView* previousView);
    ~MediaPlayView();
    void display(); // Display the main menu

    bool initialize();
    bool loadResources();
    void handleEvent(const SDL_Event& event);
    void handleMouseClick(const SDL_Event& event);
    bool isMouseOver(int mouseX, int mouseY, const SDL_Rect& rect) const;
    int getTextHeight(const std::string& text, TTF_Font* currentFont) const;
    SDL_Texture* renderText(const std::string& text, SDL_Color color);
    SDL_Texture* createButtonTexture(const std::string& text);
    void render();
    void cleanup();
};

#endif // MEDIA_PLAY_VIEW