// views/HomeView.h
#ifndef HOME_VIEW_H
#define HOME_VIEW_H

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>

#include "IView.h"
#include "../../controllers/inc/HomeController.h"

class HomeView : public IView {
    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* font;
    TTF_Font* dirFont; // Font for directory path
    std::vector<SDL_Rect> menuRects;
    std::string currentDir;
    std::vector<std::string> menuOptions;
    int menuWidth;
    int menuHeight;
    int startX;
    int startY;
    int yOffset;
    SDL_Color textColor;
    SDL_Rect dirRect; // Added dirRect as a member variable
public:
    HomeView(HomeController* controller, IView* previousView);
    ~HomeView();
    void display(); // Display the main menu
private:
    bool initialize();
    bool loadResources();
    void handleMouseClick(const SDL_Event& event);
    void render();
    void cleanup();
    void renderTextWithBackground(SDL_Texture* texture, SDL_Rect rect, bool center);
};

#endif // HOME_VIEW_H