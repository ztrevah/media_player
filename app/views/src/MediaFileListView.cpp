#include "../inc/MediaFileListView.h"
#include "../../utils/inc/sdl_utils.h"

#include <sstream>

MediaFileListView::MediaFileListView(MediaFileListController* controller, IView* previousView) : IView(controller, previousView),
    window(nullptr),
    renderer(nullptr),
    font(nullptr),
    titleRect({ 0, 0, 0, 0 }),
    prevButtonRect({ 0, 0, 0, 0 }),
    nextButtonRect({ 0, 0, 0, 0 }),
    pageTextRect({ 0, 0, 0, 0 }) {

    this->currentPage = 1;
    this->pageSize = controller->getPageSize();
}

MediaFileListView::~MediaFileListView() {
    cleanup();
}

void MediaFileListView::display() {
    if (!initialize()) {
        return; // Initialization failed, error message already displayed
    }

    if (!loadResources()) {
        return; // Resource loading failed, error message already displayed
    }

    if (mediaFiles.empty()) {
        DisplayMessageBox("No media files found in the current directory.");
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

    cleanup();
    // if(this->previousView != nullptr) {
    //     this->previousView->display();
    // }
}

bool MediaFileListView::initialize() {
    // Initialize SDL (already done by Application)

    // Window and Renderer
    window = SDL_CreateWindow("Media List", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
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
    SDL_GetWindowSize(window, &screenWidth, &screenHeight);
    return true;
}

bool MediaFileListView::loadResources() {
    // Load font (use a fallback if the default isn't found)
    font = createFont(font, 20);
    if(font == nullptr) {
        cleanup();
        DisplayMessageBox("Font error: " + std::string(TTF_GetError()));
        return false;
    }

    getMediaFiles();
    return true;
}

void MediaFileListView::getMediaFiles() {
    mediaFiles.clear(); // Clear previous file list
    mediaFiles = ((MediaFileListController*) controller)->getMediaList(currentPage);
}

void MediaFileListView::handleMouseClick(const SDL_Event& event) {
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    if (mouseX >= prevButtonRect.x && mouseX <= prevButtonRect.x + prevButtonRect.w &&
        mouseY >= prevButtonRect.y && mouseY <= prevButtonRect.y + prevButtonRect.h) {
        if (currentPage > 1) {
            currentPage--;
            getMediaFiles();
        }

        return;
    }
    
    if (mouseX >= nextButtonRect.x && mouseX <= nextButtonRect.x + nextButtonRect.w &&
        mouseY >= nextButtonRect.y && mouseY <= nextButtonRect.y + nextButtonRect.h) {
        int totalPages = (((MediaFileListController*) controller)->getMediaListSize() + pageSize - 1) / pageSize;
        if (currentPage < totalPages) {
            currentPage++;
            getMediaFiles();
        }
    }

    int i = 0;
    for(const auto &r: mediaRects) {
        if (mouseX >= r.x && mouseX <= r.x + r.w && mouseY >= r.y && mouseY <= r.y + r.h) {            
            if (event.button.button == SDL_BUTTON_LEFT) {
                // Perform actions for left click here
                isDisplaying = false;
                cleanup();
                ((MediaFileListController*) controller)->openMediaPlayView(mediaFiles[i]);
                if (initialize() && loadResources()) {
                    isDisplaying = true;
                }
            } else if (event.button.button == SDL_BUTTON_RIGHT) {
                // Perform actions for right click here
                isDisplaying = false;
                cleanup();
                ((MediaFileListController*) controller)->openMetadataView(mediaFiles[i]);
                if (initialize() && loadResources()) {
                    isDisplaying = true;
                }
            }
            break;
        } 
        i++;
    }
}

void MediaFileListView::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_Color textColor = { 240, 240, 240, 255 };
    
    // Render title at the center top
    SDL_Texture* titleTexture = CreateTextTexture(renderer, font, "Media Lists", textColor);
    if (!titleTexture) {
        cleanup();
        return;
    }
    int titleWidth, titleHeight;
    SDL_QueryTexture(titleTexture, NULL, NULL, &titleWidth, &titleHeight);
    titleRect.x = (screenWidth - titleWidth) / 2;
    titleRect.y = 20;
    titleRect.w = titleWidth;
    titleRect.h = titleHeight;
    SDL_RenderCopy(renderer, titleTexture, NULL, &titleRect);
    SDL_DestroyTexture(titleTexture);

    // Calculate start and end indices for the current page
    int startIndex = (currentPage - 1) * pageSize;

    mediaRects.clear();
    // Render media files for the current page
    int startY = titleRect.y + titleHeight + 20; // Start below the title
    for (int i = 0; i < (int) mediaFiles.size(); ++i) {
        std::string displayString = std::to_string(startIndex + i + 1) + ". " + mediaFiles[i].getFilePath();
        SDL_Texture* texture = CreateTextTexture(renderer, font, displayString, textColor);
        if (!texture) {
            // Handle error, but try to continue rendering other items
            std::cerr << "Error creating texture for: " << displayString << std::endl;
            continue;
        }

        SDL_Rect rect;
        int textWidth, textHeight;
        SDL_QueryTexture(texture, NULL, NULL, &textWidth, &textHeight);
        rect.x = 50; // Left aligned
        rect.y = startY + (i) * (textHeight + 10); // Add spacing
        rect.w = textWidth;
        rect.h = textHeight;
        mediaRects.push_back(rect);
        SDL_RenderCopy(renderer, texture, NULL, &rect);
    }

    // Render pagination buttons
    int buttonWidth, buttonHeight;
    SDL_Texture* prevButtonTexture = CreateTextTexture(renderer, font, "Prev", textColor);
    if (!prevButtonTexture) {
        cleanup();
        return;
    }
    SDL_QueryTexture(prevButtonTexture, NULL, NULL, &buttonWidth, &buttonHeight);
    prevButtonRect.x = 50;
    prevButtonRect.y = screenHeight - buttonHeight - 20;
    prevButtonRect.w = buttonWidth;
    prevButtonRect.h = buttonHeight;
    SDL_RenderCopy(renderer, prevButtonTexture, NULL, &prevButtonRect);
    SDL_DestroyTexture(prevButtonTexture);

    SDL_Texture* nextButtonTexture = CreateTextTexture(renderer, font, "Next", textColor);
    if (!nextButtonTexture) {
        cleanup();
        return;
    }
    SDL_QueryTexture(nextButtonTexture, NULL, NULL, &buttonWidth, &buttonHeight);
    nextButtonRect.x = screenWidth - 50 - buttonWidth;
    nextButtonRect.y = screenHeight - buttonHeight - 20;
    nextButtonRect.w = buttonWidth;
    nextButtonRect.h = buttonHeight;
    SDL_RenderCopy(renderer, nextButtonTexture, NULL, &nextButtonRect);
    SDL_DestroyTexture(nextButtonTexture);

    // Render page number
    std::stringstream ss;
    ss << "Page: " << currentPage << " of " << (((MediaFileListController*) controller)->getMediaListSize() + pageSize - 1) / pageSize;
    std::string pageText = ss.str();
    SDL_Texture* pageTextTexture = CreateTextTexture(renderer, font, pageText, textColor);
    if (!pageTextTexture) {
        std::cerr << "Error creating page text texture" << std::endl;
        cleanup();
        return;
    }
    int pageTextWidth, pageTextHeight;
    SDL_QueryTexture(pageTextTexture, NULL, NULL, &pageTextWidth, &pageTextHeight);
    pageTextRect.x = (screenWidth - pageTextWidth) / 2;
    pageTextRect.y = screenHeight - pageTextHeight - 20;
    pageTextRect.w = pageTextWidth;
    pageTextRect.h = pageTextHeight;
    SDL_RenderCopy(renderer, pageTextTexture, NULL, &pageTextRect);
    SDL_DestroyTexture(pageTextTexture);

    SDL_RenderPresent(renderer);
}

void MediaFileListView::cleanup() {
    isDisplaying = false;
    if (font) {
        TTF_CloseFont(font);
        font = nullptr; // Set to null after closing
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

