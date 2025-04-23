#include "../inc/MediaPlayView.h"
#include "../../utils/inc/utils.h"
#include "../../utils/inc/sdl_utils.h"

#include <atomic>
#include <sstream>

MediaPlayView::MediaPlayView(MediaPlayController* controller, IView* previousView) : IView(controller, previousView),
    screenWidth(600), 
    screenHeight(400), 
    window(nullptr), 
    renderer(nullptr), 
    font(nullptr), 
    textColor({220, 220, 220, 255}),
    filePathRect({0,0,0,0}),
    playlistRect({0,0,0,0}),
    timeRect({0,0,0,0}),
    prevButtonRect({0,0,0,0}),
    playPauseButtonRect({0,0,0,0}),
    nextButtonRect ({0,0,0,0})
{

}

MediaPlayView::~MediaPlayView() {
    cleanup();
}

void MediaPlayView::display() {
    if (!initialize()) {
        return; // Initialization failed, error message already displayed
    }

    if (!loadResources()) {
        return; // Resource loading failed, error message already displayed
    }

    // Main event loop
    SDL_Event event;
    isDisplaying = true;
    while (isDisplaying) {
        while (SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                isDisplaying = false;
            }
            handleEvent(event);
        }
        render();
        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }

    cleanup();
    // if(this->previousView != nullptr) {
    //     this->previousView->display();
    // }
}

bool MediaPlayView::initialize() {
    // Initialize SDL (already done by Application)

    window = SDL_CreateWindow("Media Player", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer creation failed: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        return false;
    }
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255); // Dark background
    return true;
}

bool MediaPlayView::loadResources() {
    font = createFont(font, 16);
    if(font == nullptr) {
        cleanup();
        DisplayMessageBox("Font error: " + std::string(TTF_GetError()));
        return false;
    }
    return true;
}

void MediaPlayView::handleEvent(const SDL_Event& event) {
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        handleMouseClick(event);
    }
    // Add other event handling here (keyboard, etc.)
}

void MediaPlayView::handleMouseClick(const SDL_Event& event) {
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    if (isMouseOver(mouseX, mouseY, prevButtonRect)) {
        std::cout << "Previous button clicked" << std::endl;
        // Implement previous functionality
        ((MediaPlayController*) controller)->previous();
    } else if (isMouseOver(mouseX, mouseY, playPauseButtonRect)) {
        std::cout << "Play/Pause button clicked" << std::endl;
        // Implement play/pause functionality
        if(((MediaPlayController*) controller)->getCurrentMusic() != nullptr) {
            ((MediaPlayController*) controller)->pause();
            return;
        }
        ((MediaPlayController*) controller)->play();
    } else if (isMouseOver(mouseX, mouseY, nextButtonRect)) {
        std::cout << "Next button clicked" << std::endl;
        // Implement next functionality
        ((MediaPlayController*) controller)->next();
    }
}

bool MediaPlayView::isMouseOver(int mouseX, int mouseY, const SDL_Rect& rect) const {
    return (mouseX >= rect.x && mouseX < rect.x + rect.w &&
            mouseY >= rect.y && mouseY < rect.y + rect.h);
}

int MediaPlayView::getTextHeight(const std::string& text, TTF_Font* currentFont) const {
    int w, h;
    TTF_SizeText(currentFont, text.c_str(), &w, &h);
    return h;
}

SDL_Texture* MediaPlayView::renderText(const std::string& text, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    if (!surface) {
        // std::cerr << "TTF_RenderText_Solid Error: " << TTF_GetError() << std::endl;
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!texture) {
        // std::cerr << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
    }
    return texture;
}

SDL_Texture* MediaPlayView::createButtonTexture(const std::string& text) {
    SDL_Color buttonColor = { 100, 100, 100, 255 };
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), { 255, 255, 255, 255 });
    if (!textSurface) {
        std::cerr << "TTF_RenderText_Solid Error: " << TTF_GetError() << std::endl;
        return nullptr;
    }
    SDL_Surface* buttonSurface = SDL_CreateRGBSurface(0, textSurface->w + 20, textSurface->h + 10, 32, 0, 0, 0, 0);
    if (!buttonSurface) {
        std::cerr << "SDL_CreateRGBSurface Error: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(textSurface);
        return nullptr;
    }
    SDL_FillRect(buttonSurface, NULL, SDL_MapRGB(buttonSurface->format, buttonColor.r, buttonColor.g, buttonColor.b));
    SDL_Rect textRect = { 10, 5, textSurface->w, textSurface->h };
    SDL_BlitSurface(textSurface, NULL, buttonSurface, &textRect);
    SDL_FreeSurface(textSurface);
    SDL_Texture* buttonTexture = SDL_CreateTextureFromSurface(renderer, buttonSurface);
    SDL_FreeSurface(buttonSurface);
    return buttonTexture;
}

void MediaPlayView::render() {
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255); // Dark background
    SDL_RenderClear(renderer);

    int filePathW = 0, filePathH = 0, playlistW = 0, playlistH = 0, timeW = 0, timeH = 0; 
    int prevW = 0, prevH = 0, playW = 0, playH = 0, nextW = 0, nextH = 0;
    int centerX = screenWidth / 2;
    int startY = 50;
    int buttonSpacing = 20;
    auto playState = ((MediaPlayController*) controller)->getPlayState();
    auto currentTime = ((MediaPlayController*) controller)->getCurrentTime();
    
    SDL_Texture* filePathTexture = nullptr;
    if(!playState.getPlayedMedia().getFilePath().empty()) {
        filePathTexture = renderText("File: " + playState.getPlayedMedia().getFilePath(), textColor);
    }
    else {
        filePathTexture = renderText("", textColor);
    }
    if(filePathTexture) {
        SDL_QueryTexture(filePathTexture, NULL, NULL, &filePathW, &filePathH);
        filePathRect = { centerX - (filePathW / 2), startY, filePathW, filePathH };
        SDL_RenderCopy(renderer, filePathTexture, NULL, &filePathRect);
        SDL_DestroyTexture(filePathTexture);
    }
    
    SDL_Texture* playlistTexture = nullptr;
    if(!playState.getPlaylistFrom().empty()) {
        playlistTexture = renderText("Playlist: " + playState.getPlaylistFrom(), textColor);
    } else {
        playlistTexture = renderText("", textColor);
    }
    if(playlistTexture) {
        SDL_QueryTexture(playlistTexture, NULL, NULL, &playlistW, &playlistH);
        playlistRect = { centerX - (playlistW / 2), startY + filePathH + 10, playlistW, playlistH };
        SDL_RenderCopy(renderer, playlistTexture, NULL, &playlistRect);
        SDL_DestroyTexture(playlistTexture);
    }
    
    if(currentTime.first == -1 || currentTime.second == -1 ) {
        currentTime.first = 0;
        currentTime.second = 0;
    }
    SDL_Texture* timeTexture = renderText(
        secondsToHHMMSS(currentTime.first) + " / " + secondsToHHMMSS(currentTime.second), 
        textColor
    );
    if(timeTexture) {
        SDL_QueryTexture(timeTexture, NULL, NULL, &timeW, &timeH);
        timeRect = { centerX - (timeW / 2), startY + filePathH + playlistH + 20, timeW, timeH };
        SDL_RenderCopy(renderer, timeTexture, NULL, &timeRect);
        SDL_DestroyTexture(timeTexture);
    }
    
    SDL_Texture* previousButtonTexture = createButtonTexture("Previous");
    SDL_Texture* playPauseButtonTexture = createButtonTexture("Play/Pause");
    SDL_Texture* nextButtonTexture = createButtonTexture("Next");
    if(previousButtonTexture && playPauseButtonTexture && nextButtonTexture) {
        SDL_QueryTexture(previousButtonTexture, NULL, NULL, &prevW, &prevH);
        SDL_QueryTexture(playPauseButtonTexture, NULL, NULL, &playW, &playH);
        SDL_QueryTexture(nextButtonTexture, NULL, NULL, &nextW, &nextH);

        int totalButtonWidth = prevW + buttonSpacing + playW + buttonSpacing + nextW;
        int buttonsStartX = centerX - (totalButtonWidth / 2);

        prevButtonRect = { buttonsStartX, startY + filePathH + playlistH + timeH + 40, prevW, prevH };
        SDL_RenderCopy(renderer, previousButtonTexture, NULL, &prevButtonRect);
        SDL_DestroyTexture(previousButtonTexture);

        playPauseButtonRect = { buttonsStartX + prevW + buttonSpacing, startY + filePathH + playlistH + timeH + 40, playW, playH };
        SDL_RenderCopy(renderer, playPauseButtonTexture, NULL, &playPauseButtonRect);
        SDL_DestroyTexture(playPauseButtonTexture);

        nextButtonRect = { buttonsStartX + prevW + buttonSpacing + playW + buttonSpacing, startY + filePathH + playlistH + timeH + 40, nextW, nextH };
        SDL_RenderCopy(renderer, nextButtonTexture, NULL, &nextButtonRect);
        SDL_DestroyTexture(nextButtonTexture);
    }
}

void MediaPlayView::cleanup() {
    isDisplaying = false;
    if (font) {
        TTF_CloseFont(font);
        font = nullptr;
    }
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
}
