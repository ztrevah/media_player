// views/HomeView.cpp
#include "../inc/HomeView.h"
#include "../../utils/inc/sdl_utils.h"
#include "../../Application.h"

#include "../../controllers/inc/MediaFileListController.h"
#include "../../controllers/inc/PlaylistController.h"
#include "../../controllers/inc/MediaPlayController.h"


HomeView::HomeView(HomeController* controller, IView* previousView) : IView(controller, previousView), 
    window(nullptr), renderer(nullptr), font(nullptr), dirFont(nullptr), dirRect({0,0,0,0}) {

}

HomeView::~HomeView() {
    cleanup();
}

void HomeView::display() {
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
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                handleMouseClick(event);
            }
        }
        render();
    }

    // cleanup is handled by the destructor
    cleanup();
    // if(this->previousView != nullptr) {
    //     this->previousView->display();
    // }
}

bool HomeView::initialize() {
    // Initialize SDL (already done by Application)

    // Get the current directory
    currentDir = Application::getStartDirectory();

    // Window and Renderer
    window = SDL_CreateWindow("SDL Media Player", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        DisplayMessageBox("SDL_CreateWindow failed: " + std::string(SDL_GetError()));
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        DisplayMessageBox("SDL_CreateRenderer failed: " + std::string(SDL_GetError()));
        return false;
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black background
    return true;
}

bool HomeView::loadResources() {
    // Load font (use a fallback if the default isn't found)
    font = createFont(font, 24);
    if(font == nullptr) {
        cleanup();
        DisplayMessageBox("Font error: " + std::string(TTF_GetError()));
        return false;
    }
    textColor = { 240, 240, 240, 255 }; // Slightly off-white text for better contrast on dark background.

    // Load a smaller font for the directory
    dirFont = createFont(dirFont, 18);
    if(dirFont == nullptr) {
        cleanup();
        DisplayMessageBox("Font error: " + std::string(TTF_GetError()));
        return false;
    }

    // Menu Options
    menuOptions = {
        "Media List",
        "Playlists",
        "Media Player",
        "Exit"
    };

    // Rectangles for menu options
    menuRects.resize(menuOptions.size());
    menuWidth = 250; // Increased width
    menuHeight = 50; // Increased height
    startX = 50; // Align to the left, reduced padding
    startY = 150; // Increased startY to move menu down
    yOffset = menuHeight + 15; // Increased spacing

    for (size_t i = 0; i < menuOptions.size(); ++i) {
        menuRects[i] = { startX, startY + static_cast<int>(i * yOffset), menuWidth, menuHeight };
    }

    // Rectangle for current directory
    dirRect = { startX, 50, menuWidth * 2, menuHeight }; // Align with menu, increased width
    return true;
}

void HomeView::handleMouseClick(const SDL_Event& event) {
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    for (size_t i = 0; i < menuOptions.size(); ++i) {
        if (mouseX >= menuRects[i].x && mouseX <= menuRects[i].x + menuRects[i].w &&
            mouseY >= menuRects[i].y && mouseY <= menuRects[i].y + menuRects[i].h) {
            if (i == 3) { // "Exit" option
                SDL_Event quitEvent;
                quitEvent.type = SDL_QUIT;
                SDL_PushEvent(&quitEvent); // Use SDL_PushEvent to post a quit event.
            }
            //  Add functionality for other menu options here
            else if (i == 0)
            {
                //Media List
                isDisplaying = false;
                cleanup();
                ((HomeController*) this->controller)->openMediaFileListView();

                if (initialize() && loadResources()) {
                    isDisplaying = true;
                }
            }
            else if (i == 1)
            {
                //Playlists
                isDisplaying = false;
                cleanup();
                ((HomeController*) this->controller)->openPlaylistView();
                if (initialize() && loadResources()) {
                    isDisplaying = true;
                }
            }
            else if (i == 2)
            {
                // Media Player
                isDisplaying = false;
                cleanup();
                ((HomeController*) this->controller)->openMediaPlayView();
                if (initialize() && loadResources()) {
                    isDisplaying = true;
                }
            }
        }
    }
}

void HomeView::render() {
    // Clear the screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Ensure background is black each frame.
    SDL_RenderClear(renderer);

    // Render menu options and directory
    for (size_t i = 0; i < menuOptions.size(); ++i) {
        SDL_Texture* texture = CreateTextTexture(renderer, font, menuOptions[i], textColor);
        if (!texture) {
            cleanup();
            DisplayMessageBox("Failed to create menu texture");
            return; // Or handle the error as appropriate
        }
        renderTextWithBackground(texture, menuRects[i], false); // false for not centered
        SDL_DestroyTexture(texture);
    }

    // Handle long directory string
    int maxDirWidth = menuWidth * 2; // Or some other maximum width
    std::string displayedDir = currentDir;
    int dirTextureWidth, dirTextureHeight;
    SDL_Texture* dirTexture = CreateTextTexture(renderer, dirFont, "Current Directory: " + currentDir, textColor);
    if (!dirTexture) {
        cleanup();
        DisplayMessageBox("Failed to create directory texture");
        return; // Or handle the error
    }
    SDL_QueryTexture(dirTexture, NULL, NULL, &dirTextureWidth, &dirTextureHeight);

    if (dirTextureWidth > maxDirWidth) {
        // Truncate the directory string
        int truncationLength = 40; // Show last 40 characters
        if ((int) currentDir.length() > truncationLength) {
            displayedDir = "..." + currentDir.substr(currentDir.length() - truncationLength);
        }
        //Create new texture with truncated string
        SDL_DestroyTexture(dirTexture);
        dirTexture = CreateTextTexture(renderer, dirFont, "Current Directory: " + displayedDir, textColor); // Use dirFont
        if (!dirTexture) {
            cleanup();
            DisplayMessageBox("Failed to create directory texture");
            return;
        }
    }
    renderTextWithBackground(dirTexture, dirRect, false); // false for not centered
    SDL_DestroyTexture(dirTexture);

    // Update the screen
    SDL_RenderPresent(renderer);
}

void HomeView::cleanup() {
    isDisplaying = false;
    if (font) {
        TTF_CloseFont(font);
        font = nullptr; // Set to null after closing
    }
    if (dirFont) {
        TTF_CloseFont(dirFont);
        dirFont = nullptr; // Set to null after closing
    }
    if(renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    if(window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
}

void HomeView::renderTextWithBackground(SDL_Texture* texture, SDL_Rect rect, bool center) {
    // Draw a slightly rounded background
    SDL_Rect bgRect = rect;
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 200); // Darker background for menu items
    SDL_RenderFillRect(renderer, &bgRect);

    // Get the texture and its size
    int textWidth, textHeight;
    SDL_QueryTexture(texture, NULL, NULL, &textWidth, &textHeight);

    // Calculate centering or left alignment
    int x;
    if (center) {
        x = rect.x + (rect.w - textWidth) / 2;
    }
    else
    {
        x = rect.x + 10; // Left alignment with 10px padding.
    }
    int y = rect.y + (rect.h - textHeight) / 2;

    // Create a new rect for the text
    SDL_Rect textRect = {x, y, textWidth, textHeight};

    // Adjust textRect width to fit inside rect
    if (textWidth > rect.w) {
        textRect.w = rect.w - 20; // Leave some padding (10 on each side)
    }

    SDL_RenderCopy(renderer, texture, nullptr, &textRect);
}
