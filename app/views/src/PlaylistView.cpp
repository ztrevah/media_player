#include "../inc/PlaylistView.h"

#include <filesystem>
#include "../../utils/inc/sdl_utils.h"

PlaylistView::PlaylistView(PlaylistController* controller, IView* previousView) : IView(controller, previousView), 
    window(nullptr),
    renderer(nullptr),
    font(nullptr),
    smallFont(nullptr), // Font for playlist names in the sidebar
    titleRect({ 0, 0, 0, 0 }),
    addPlaylistButtonRect({ 0, 0, 0, 0 }),
    addMediaButtonRect({ 0,0,0,0 }),
    mediaFilePathRect({ 0,0,0,0}),
    mediaFilePathLabelRect({0,0,0,0}),
    screenWidth(0),
    screenHeight(0),
    titleWidth(0),
    titleHeight(0),
    buttonWidth(0),
    buttonHeight(0),
    playlistViewMode(PlaylistViewMode::DISPLAY_PLAYLIST),
    selectedPlaylistIndex(-1),
    newPlaylistName("")
{
    
}

PlaylistView::~PlaylistView() {
    cleanup();
}

void PlaylistView::display() {
    if (!initialize()) {
        return;
    }
    if (!loadResources()) {
        return;
    }

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
            else if (event.type == SDL_TEXTINPUT) {
                handleTextInput(event);
            }
            else if (event.type == SDL_KEYDOWN) {
                handleKeyDown(event);
            }
        }
        render();
    }
    //cleanup moved here
    cleanup();
    // if(this->previousView != nullptr) {
    //     this->previousView->display();
    // }
}

bool PlaylistView::initialize() {
    // Initialize SDL (already done by Application)

    // Window and Renderer
    window = SDL_CreateWindow("Playlists", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
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

bool PlaylistView::loadResources() {
    // Load font
    font = createFont(font, 20);
    if(font == nullptr) {
        cleanup();
        DisplayMessageBox("Font error: " + std::string(TTF_GetError()));
        return false;
    }

    // Load smaller font for playlist names in the sidebar
    smallFont = createFont(smallFont, 16);
    if(smallFont == nullptr) {
        cleanup();
        DisplayMessageBox("Font error: " + std::string(TTF_GetError()));
        return false;
    }

    fetchData();
    newMediaInputBuffer.clear();

    return true;
}

void PlaylistView::fetchData() {
    auto fetchedPlaylists = ((PlaylistController*) controller)->getPlaylists();
    if(selectedPlaylistIndex == -1) {
        playlists = fetchedPlaylists;
        return;
    } 

    Playlist selectedPlaylist = playlists[selectedPlaylistIndex];
    playlists = fetchedPlaylists;
    for(size_t i=0;i<playlists.size();i++) {
        if(selectedPlaylist.getName() == playlists[i].getName()) {
            selectedPlaylistIndex = i;
            break;
        }
        else if(i == playlists.size() - 1) selectedPlaylistIndex = -1;
        i++;
    }
}

void PlaylistView::handleMouseClick(const SDL_Event& event) {
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    if (playlistViewMode == PlaylistViewMode::DISPLAY_PLAYLIST) {
        if (mouseX >= addMediaButtonRect.x && mouseX <= addMediaButtonRect.x + addMediaButtonRect.w &&
            mouseY >= addMediaButtonRect.y && mouseY <= addMediaButtonRect.y + addMediaButtonRect.h) {
            // "Add Media" button clicked, add media file to the selected playlist
            if (selectedPlaylistIndex >= 0 && selectedPlaylistIndex < (int) playlists.size()) {
                if (!newMediaInputBuffer.empty())
                {
                    // Insert the new media file at the beginning of the list
                    playlists[selectedPlaylistIndex].addMediaFile(MediaFile(newMediaInputBuffer));
                    ((PlaylistController*) controller)->updatePlaylist(
                        playlists[selectedPlaylistIndex].getName(),
                        playlists[selectedPlaylistIndex].getMediaList()
                    );
                    fetchData();
                    newMediaInputBuffer.clear();
                    SDL_StopTextInput();
                }
            }
        }
        else if (mouseX >= addPlaylistButtonRect.x && mouseX <= addPlaylistButtonRect.x + addPlaylistButtonRect.w &&
            mouseY >= addPlaylistButtonRect.y && mouseY <= addPlaylistButtonRect.y + addPlaylistButtonRect.h) {
            // "Add" button clicked, switch to create playlist mode
            playlistViewMode = PlaylistViewMode::CREATE_PLAYLIST;
            newPlaylistName = ""; // Clear any previous name
            SDL_StartTextInput();
        }
        else if (mouseX >= deletePlaylistButtonRect.x && mouseX <= deletePlaylistButtonRect.x + deletePlaylistButtonRect.w &&
                    mouseY >= deletePlaylistButtonRect.y && mouseY <= deletePlaylistButtonRect.y + deletePlaylistButtonRect.h) {
            // "Delete Playlist" button clicked
            if (selectedPlaylistIndex >= 0 && selectedPlaylistIndex < (int) playlists.size()) {
                ((PlaylistController*) controller)->deletePlaylist(playlists[selectedPlaylistIndex].getName());
                fetchData();
                newMediaInputBuffer.clear();
                selectedPlaylistIndex = -1; // Reset selection
            }
        }
        else {
            // Check if a media file in the list is clicked
            for(size_t i=0;i<mediaFileRects.size();i++) {
                SDL_Rect mediaRect = mediaFileRects.at(i);
                if (mouseX >= mediaRect.x && mouseX <= mediaRect.x + mediaRect.w &&
                    mouseY >= mediaRect.y && mouseY <= mediaRect.y + mediaRect.h) {
                    if(selectedPlaylistIndex >= 0 && selectedPlaylistIndex < (int) playlists.size()) {
                        if (event.button.button == SDL_BUTTON_LEFT) {
                            // Perform actions for left click here
                            isDisplaying = false;
                            cleanup();
                            ((PlaylistController*) controller)->openMediaPlayView(
                                playlists[selectedPlaylistIndex].getName(),
                                playlists[selectedPlaylistIndex].getMediaList()[i].getFilePath()
                            );
                            if (initialize() && loadResources()) {
                                isDisplaying = true;
                            }
                        } else if (event.button.button == SDL_BUTTON_RIGHT) {
                            // Perform actions for right click here
                            isDisplaying = false;
                            cleanup();
                            ((PlaylistController*) controller)->openMetadataView(playlists[selectedPlaylistIndex].getMediaList()[i]);
                            if (initialize() && loadResources()) {
                                isDisplaying = true;
                            }
                        }
                        return;
                    }
                }
            }
            // Check if a playlist in the list is clicked
            int playlistStartY = addPlaylistButtonRect.y + addPlaylistButtonRect.h + 20;
            for (size_t i = 0; i < playlists.size(); ++i) {
                SDL_Rect playlistRect = { 50, playlistStartY + static_cast<int>(i * 30), 200, 25 }; // Example rect
                if (mouseX >= playlistRect.x && mouseX <= playlistRect.x + playlistRect.w &&
                    mouseY >= playlistRect.y && mouseY <= playlistRect.y + playlistRect.h) {
                    // Playlist clicked, switch to display content mode
                    selectedPlaylistIndex = i;
                    playlistViewMode = PlaylistViewMode::DISPLAY_PLAYLIST; //set the mode
                    newMediaInputBuffer.clear();
                    SDL_StartTextInput();   
                    break; // Exit the loop
                }
            }
        }
    }
    else if (playlistViewMode == PlaylistViewMode::CREATE_PLAYLIST) {
        if (mouseX >= addPlaylistButtonRect.x && mouseX <= addPlaylistButtonRect.x + addPlaylistButtonRect.w &&
            mouseY >= addPlaylistButtonRect.y && mouseY <= addPlaylistButtonRect.y + addPlaylistButtonRect.h) {
            // "Add" button clicked, create new playlist
            if (!newPlaylistName.empty()) {
                ((PlaylistController*) controller)->createPlaylist(newPlaylistName, {});
                playlists.push_back(Playlist(newPlaylistName, {}));
                selectedPlaylistIndex = playlists.size() - 1;
                fetchData();
                newPlaylistName = "";
                newMediaInputBuffer.clear();
                SDL_StopTextInput();
            }
        }
        else {
            int playlistStartY = addPlaylistButtonRect.y + addPlaylistButtonRect.h + 20;
            for (size_t i = 0; i < playlists.size(); ++i) {
                SDL_Rect playlistRect = { 50, playlistStartY + static_cast<int>(i * 30), 200, 25 }; // Example rect
                if (mouseX >= playlistRect.x && mouseX <= playlistRect.x + playlistRect.w &&
                    mouseY >= playlistRect.y && mouseY <= playlistRect.y + playlistRect.h) {
                    // Playlist clicked, switch to display content mode
                    selectedPlaylistIndex = i;
                    playlistViewMode = PlaylistViewMode::DISPLAY_PLAYLIST; //set the mode
                    newMediaInputBuffer.clear();
                    SDL_StartTextInput();
                    break; // Exit the loop
                }
            }
        }
    }
}

void PlaylistView::handleTextInput(const SDL_Event& event) {
    if (playlistViewMode == PlaylistViewMode::CREATE_PLAYLIST)
    {
        newPlaylistName += event.text.text;
    }
    else if (playlistViewMode == PlaylistViewMode::DISPLAY_PLAYLIST)
    {
        if(selectedPlaylistIndex >= 0 && selectedPlaylistIndex < (int) playlists.size()) {
            newMediaInputBuffer += event.text.text;
        }
    }

}

void PlaylistView::handleKeyDown(const SDL_Event& event) {
    if (event.key.keysym.sym == SDLK_BACKSPACE) {
        if (playlistViewMode == PlaylistViewMode::CREATE_PLAYLIST)
        {
            if(!newPlaylistName.empty()) newPlaylistName.pop_back();
        }
        else if (playlistViewMode == PlaylistViewMode::DISPLAY_PLAYLIST)
        {
            if(selectedPlaylistIndex >= 0 && selectedPlaylistIndex < (int) playlists.size()) {
                if(!newMediaInputBuffer.empty()) {
                    newMediaInputBuffer.pop_back();
                }
            }
        }
    }
    else if (event.key.keysym.sym == SDLK_RETURN) {
        if (playlistViewMode == PlaylistViewMode::CREATE_PLAYLIST) {
            if (!newPlaylistName.empty()) {
                ((PlaylistController*) controller)->createPlaylist(newPlaylistName, {});
                playlists.push_back(Playlist(newPlaylistName, {}));
                selectedPlaylistIndex = playlists.size() - 1;
                fetchData();
                newPlaylistName = "";
                newMediaInputBuffer.clear();
                SDL_StopTextInput();
            }
        }
        else if (playlistViewMode == PlaylistViewMode::DISPLAY_PLAYLIST)
        {
            if (selectedPlaylistIndex >= 0 && selectedPlaylistIndex < (int) playlists.size()) {
                if (!newMediaInputBuffer.empty())
                {
                    // Insert the new media file at the beginning of the list
                    playlists[selectedPlaylistIndex].addMediaFile(MediaFile(newMediaInputBuffer));
                    ((PlaylistController*) controller)->updatePlaylist(
                        playlists[selectedPlaylistIndex].getName(),
                        playlists[selectedPlaylistIndex].getMediaList()
                    );
                    fetchData();
                    newMediaInputBuffer.clear();
                    SDL_StopTextInput();
                }
            }
        }
    }
    else if (event.key.keysym.sym == SDLK_v && (SDL_GetModState() & KMOD_CTRL)) {
        if (SDL_HasClipboardText() == SDL_TRUE) {
            char* pastedText = SDL_GetClipboardText();
            if (pastedText != nullptr) {
                if (playlistViewMode == PlaylistViewMode::CREATE_PLAYLIST) {
                    newPlaylistName += pastedText;
                }
                else if (playlistViewMode == PlaylistViewMode::DISPLAY_PLAYLIST)
                {
                    if (selectedPlaylistIndex >= 0 && selectedPlaylistIndex < (int) playlists.size()) {
                        newMediaInputBuffer += pastedText;
                    }
                }
                SDL_free(pastedText);
                pastedText = nullptr;
            } else {
                std::cerr << "Error getting text from clipboard: " << SDL_GetError() << std::endl;
            }
        } else {
            std::cerr << "Clipboard is empty." << std::endl;
        }
    }
}

void PlaylistView::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_Color textColor = { 240, 240, 240, 255 };

    // Render title at the center top
    SDL_Texture* titleTexture = CreateTextTexture(renderer, font, "Playlists", textColor);
    if (!titleTexture) {
        cleanup();
        return;
    }
    SDL_QueryTexture(titleTexture, NULL, NULL, &titleWidth, &titleHeight);
    titleRect.x = (screenWidth - titleWidth) / 2;
    titleRect.y = 20;
    titleRect.w = titleWidth;
    titleRect.h = titleHeight;
    SDL_RenderCopy(renderer, titleTexture, NULL, &titleRect);

    // Add Playlist button texture
    SDL_Texture* addPlaylistButtonTexture = CreateTextTexture(renderer, font, "Add", textColor);
    if (!addPlaylistButtonTexture) {
        cleanup();
        return;
    }    
    SDL_QueryTexture(addPlaylistButtonTexture, NULL, NULL, &buttonWidth, &buttonHeight);
    addPlaylistButtonRect.x = 50;
    addPlaylistButtonRect.y = titleRect.y + titleRect.h + 20;
    addPlaylistButtonRect.w = buttonWidth;
    addPlaylistButtonRect.h = buttonHeight;
    SDL_RenderCopy(renderer, addPlaylistButtonTexture, NULL, &addPlaylistButtonRect);

    // Render list of playlists (sidebar) - This part is now always rendered
    int playlistStartY = addPlaylistButtonRect.y + addPlaylistButtonRect.h + 20;
    int i = 0;
    for (const auto& playlist : playlists) {
        SDL_Texture* playlistNameTexture = CreateTextTexture(renderer, smallFont, playlist.getName(), textColor);
        if (!playlistNameTexture) {
            std::cerr << "Error creating playlist name texture: " << playlist.getName() << std::endl;
            continue; // Skip this playlist
        }
        int pw, ph;
        SDL_QueryTexture(playlistNameTexture, NULL, NULL, &pw, &ph);
        SDL_Rect playlistRect = { 50, playlistStartY + i * 30, pw, ph };
        SDL_RenderCopy(renderer, playlistNameTexture, NULL, &playlistRect);
        SDL_DestroyTexture(playlistNameTexture);
        i++;
    }

    if (playlistViewMode == PlaylistViewMode::DISPLAY_PLAYLIST) {
        //  If in DISPLAY_PLAYLIST mode,  render the list of playlists.
        if (selectedPlaylistIndex != -1) {
            renderPlaylistContent();
        }
    }
    else if (playlistViewMode == PlaylistViewMode::CREATE_PLAYLIST) {
        renderCreatePlaylistView();
    }

    SDL_RenderPresent(renderer);
}

void PlaylistView::renderPlaylistContent() {
    SDL_Color textColor = { 240, 240, 240, 255 };

    // First, render the "Add Media" button and input field at the top
    int startX = 300;
    int startY = titleRect.y + titleRect.h + 20;

    // Render the label
    SDL_Texture* mediaFilePathLabelTexture = CreateTextTexture(renderer, font, "Enter new media file path:", textColor);
    if (!mediaFilePathLabelTexture) {
        cleanup();
        return;
    }
    SDL_QueryTexture(mediaFilePathLabelTexture, NULL, NULL, &titleWidth, &titleHeight);
    mediaFilePathLabelRect.x = startX;
    mediaFilePathLabelRect.y = startY;
    mediaFilePathLabelRect.w = titleWidth;
    mediaFilePathLabelRect.h = titleHeight;
    SDL_RenderCopy(renderer, mediaFilePathLabelTexture, NULL, &mediaFilePathLabelRect);

    // Calculate Y position for button and input field based on label
    int inputStartY = mediaFilePathLabelRect.y + mediaFilePathLabelRect.h + 10;

    SDL_Texture* addMediaButtonTexture = CreateTextTexture(renderer, font, "Add Media", textColor);
    if (!addMediaButtonTexture) {
        cleanup();
        return;
    }
    SDL_QueryTexture(addMediaButtonTexture, NULL, NULL, &buttonWidth, &buttonHeight);
    addMediaButtonRect.x = startX;
    addMediaButtonRect.y = inputStartY + 40;
    addMediaButtonRect.w = buttonWidth;
    addMediaButtonRect.h = buttonHeight;
    SDL_RenderCopy(renderer, addMediaButtonTexture, NULL, &addMediaButtonRect);
    
    // Render input field for media file path
    SDL_Texture* mediaFilePathTexture = CreateTextTexture(renderer, font, newMediaInputBuffer.c_str(), textColor);
    if (!mediaFilePathTexture) {
        if (!newMediaInputBuffer.empty())
        std::cerr << "Error creating name texture: " << newMediaInputBuffer << std::endl;
        //  Don't return, keep going
    }
    int mfWidth, mfHeight;
    SDL_QueryTexture(mediaFilePathTexture, NULL, NULL, &mfWidth, &mfHeight);
    mediaFilePathRect = { startX, inputStartY, mfWidth, mfHeight }; // Increased width for better visibility
    // SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255); // Background for input
    // SDL_RenderFillRect(renderer, &mediaFilePathRect);
    if (mediaFilePathTexture)
    {
        SDL_RenderCopy(renderer, mediaFilePathTexture, NULL, &mediaFilePathRect);
        SDL_DestroyTexture(mediaFilePathTexture);
    }

    // Now, render the list of media files
    int mediaListStartY = addMediaButtonRect.y + addMediaButtonRect.h + 20; // Start below the button
    deletePlaylistButtonRect.y = mediaListStartY;
    mediaFileRects.clear();
    if (selectedPlaylistIndex >= 0 && selectedPlaylistIndex < (int) playlists.size()) {
        const Playlist& selectedPlaylist = playlists[selectedPlaylistIndex];
        int i = 0;
        for (const auto& mediaFile : selectedPlaylist.getMediaList()) {
            std::string displayString = std::to_string(i + 1) + ". " + mediaFile.getFilePath();
            SDL_Texture* mediaTexture = CreateTextTexture(renderer, font, displayString, textColor);
            if (!mediaTexture) {
                std::cerr << "Error creating media texture: " << displayString << std::endl;
                continue;
            }
            int mw, mh;
            SDL_QueryTexture(mediaTexture, NULL, NULL, &mw, &mh);
            SDL_Rect mediaRect = { startX, mediaListStartY + i * 30, mw, mh };
            deletePlaylistButtonRect.y += 30;
            SDL_RenderCopy(renderer, mediaTexture, NULL, &mediaRect);
            mediaFileRects.push_back(mediaRect);
            SDL_DestroyTexture(mediaTexture);
            i++;
        }
    }

    SDL_Color redColor = { 255, 0, 0, 255 };
    SDL_Texture* deletePlaylistButtonTexture = CreateTextTexture(renderer, font, "Delete Playlist", redColor); // Create texture for delete button
    if (!deletePlaylistButtonTexture) {
        cleanup();
        return;
    }
    SDL_QueryTexture(deletePlaylistButtonTexture, NULL, NULL, &buttonWidth, &buttonHeight);
    deletePlaylistButtonRect.x = startX;
    deletePlaylistButtonRect.y += 50;
    deletePlaylistButtonRect.w = buttonWidth;
    deletePlaylistButtonRect.h = buttonHeight;
    SDL_RenderCopy(renderer, deletePlaylistButtonTexture, NULL, &deletePlaylistButtonRect);
}

void PlaylistView::renderCreatePlaylistView() {
    SDL_Color textColor = { 240, 240, 240, 255 };
    // Render the UI for creating a new playlist
    std::string labelText = "Enter playlist name:";
    SDL_Texture* labelTexture = CreateTextTexture(renderer, font, labelText, textColor);
    if (!labelTexture) {
        std::cerr << "Error creating label texture" << std::endl;
        return;
    }
    int labelWidth,  labelHeight;
    SDL_QueryTexture(labelTexture, NULL, NULL, &labelWidth, &labelHeight);
    SDL_Rect labelRect = { 300, titleRect.y + titleRect.h + 20, labelWidth, labelHeight };
    SDL_RenderCopy(renderer, labelTexture, NULL, &labelRect);
    SDL_DestroyTexture(labelTexture);

    // Render the input name
    SDL_Texture* nameTexture = CreateTextTexture(renderer, font, newPlaylistName.c_str(), textColor);
    if (!nameTexture) {
        if (!newPlaylistName.empty())
            std::cerr << "Error creating name texture: " << newPlaylistName << std::endl;
        //  Don't return, keep going
    }
    int nameWidth,  nameHeight;
    SDL_QueryTexture(nameTexture, NULL, NULL, &nameWidth, &nameHeight);
    newPlaylistNameRect = { 300, labelRect.y + labelHeight + 10, nameWidth, nameHeight }; // Increased width
    // SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255); // Background for input
    // SDL_RenderFillRect(renderer, &newPlaylistNameRect);
    if (nameTexture)
    {
        SDL_RenderCopy(renderer, nameTexture, NULL, &newPlaylistNameRect);
        SDL_DestroyTexture(nameTexture);
    }

    SDL_Texture* createPlaylistButtonTexture = CreateTextTexture(renderer, font, "Create Playlist", textColor);
    if (!createPlaylistButtonTexture) {
        std::cerr << "Error creating label texture" << std::endl;
        return;
    }
    SDL_QueryTexture(createPlaylistButtonTexture, NULL, NULL, &buttonWidth, &buttonHeight);
    SDL_Rect createPlaylistButtonRect({
        300,
        newPlaylistNameRect.y + newPlaylistNameRect.h + 20,
        buttonWidth,
        buttonHeight
    });
    SDL_RenderCopy(renderer, createPlaylistButtonTexture, NULL, &createPlaylistButtonRect);
}

void PlaylistView::cleanup() {
    isDisplaying = false;
    if (font) {
        TTF_CloseFont(font);
        font = nullptr; // Set to null after closing
    }
    if (smallFont) {
        TTF_CloseFont(smallFont);
        smallFont = nullptr; // Set to null after closing
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