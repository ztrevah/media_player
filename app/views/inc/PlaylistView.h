// views/PlaylistView.h
#ifndef PLAYLIST_VIEW_H
#define PLAYLIST_VIEW_H

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "IView.h"
#include "../../controllers/inc/PlaylistController.h"

class PlaylistView : public IView {
    enum class PlaylistViewMode {
        DISPLAY_PLAYLIST,
        CREATE_PLAYLIST,
    };

    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* font;
    TTF_Font* smallFont; // Font for playlist names

    SDL_Rect titleRect;
    SDL_Rect newPlaylistNameRect;
    SDL_Rect addPlaylistButtonRect;
    SDL_Rect addMediaButtonRect;
    SDL_Rect mediaFilePathRect;
    SDL_Rect mediaFilePathLabelRect;
    SDL_Rect deletePlaylistButtonRect;
    std::vector<SDL_Rect> mediaFileRects;

    int screenWidth;
    int screenHeight;
    int titleWidth;
    int titleHeight;
    int buttonWidth;
    int buttonHeight;

    std::vector<Playlist> playlists;
    PlaylistViewMode playlistViewMode;
    int selectedPlaylistIndex;
    std::string newPlaylistName;
    std::string playlistNameInputBuffer;
    std::string newMediaInputBuffer;

    
public:
    PlaylistView(PlaylistController* controller, IView* previousView);
    ~PlaylistView();
    void display();
private:
    bool initialize();
    bool loadResources();
    void fetchData();
    void handleMouseClick(const SDL_Event& event);
    void handleTextInput(const SDL_Event& event);
    void handleKeyDown(const SDL_Event& event);
    void render();
    void renderPlaylistContent();
    void renderCreatePlaylistView();
    void cleanup();

};

#endif // PLAYLIST_VIEW_H