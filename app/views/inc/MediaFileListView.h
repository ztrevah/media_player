// views/MediaFileListView.h
#ifndef MEDIA_FILE_LIST_VIEW_H
#define MEDIA_FILE_LIST_VIEW_H

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "IView.h"
#include "../../controllers/inc/MediaFileListController.h"

class MediaFileListView : public IView {
    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* font;
    std::vector<SDL_Rect> mediaRects;
    std::vector<MediaFile> mediaFiles;
    int currentPage;
    int pageSize;
    SDL_Rect titleRect;
    SDL_Rect prevButtonRect;
    SDL_Rect nextButtonRect;
    SDL_Rect pageTextRect;
    int screenWidth;
    int screenHeight;
public:
    MediaFileListView(MediaFileListController* controller, IView* previousView);
    ~MediaFileListView();
    void display(); // Display the media file list
private:
    bool initialize();
    bool loadResources();
    void getMediaFiles();
    void handleMouseClick(const SDL_Event& event);
    void render();
    void cleanup();
};

#endif // MediaList_VIEW_H