// views/MetadataView.h
#ifndef METADATA_VIEW_H
#define METADATA_VIEW_H

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "IView.h"
#include "../../controllers/inc/MetadataController.h"

class MetadataView : public IView {
    int screenWidth;
    int screenHeight;
    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* font;
    int fontSize;
    SDL_Color textColor;
    MediaFile currentFile;
public:
    MetadataView(MetadataController* controller, IView* previousView);
    ~MetadataView();
    void display();
    // void openDisplayAudioMetadataView();
    // void openDisplayVideoMetadataView();
    // void openEditAudioMetadataView();
    // void openEditVideoMetadataView();
private:
    void render();
    void cleanup();
    bool initialize();
    bool loadResources();
    void renderTextAndPosition(const std::string& text, int x, int& y, SDL_Color color);
    void renderAudioMetadata(AudioMetadata& metadata, int x, int& y);
    void renderVideoMetadata(const VideoMetadata& metadata, int x, int& y);
};

#endif // METADATA_VIEW_H