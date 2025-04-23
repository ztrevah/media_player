#include "../inc/MetadataView.h"

#include "../../models/inc/AudioMetadata.h"
#include "../../models/inc/VideoMetadata.h"
#include "../../utils/inc/utils.h"
#include "../../utils/inc/sdl_utils.h"

#include <functional>
#include <iomanip>

MetadataView::MetadataView(MetadataController* controller, IView* previousView) : IView(controller, previousView),
    screenWidth(640), screenHeight(600), window(nullptr), renderer(nullptr), font(nullptr), fontSize(16) 
{
    currentFile = ((MetadataController*) this->controller)->getCurrentMediaFile();
}

MetadataView::~MetadataView() {
    cleanup();
}

void MetadataView::display() {
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
        }
        render();
    }

    cleanup();
    // if(this->previousView != nullptr) {
    //     this->previousView->display();
    // }
}

bool MetadataView::initialize() {
    // Initialize SDL (already done by Application)

    // Window and Renderer
    window = SDL_CreateWindow("Metadata Viewer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        
        // DisplayMessageBox("SDL_CreateWindow failed: " + std::string(SDL_GetError())); // Assuming DisplayMessageBox exists
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        
        // DisplayMessageBox("SDL_CreateRenderer failed: " + std::string(SDL_GetError())); // Assuming DisplayMessageBox exists
        return false;
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black background
    return true;
}

bool MetadataView::loadResources() {
    // Load font (use a fallback if the default isn't found)
    font = createFont(font, 16);
    if(font == nullptr) {
        cleanup();
        DisplayMessageBox("Font error: " + std::string(TTF_GetError()));
        return false;
    }
    textColor = { 240, 240, 240, 255 }; // Slightly off-white text
    return true;
}

void MetadataView::render() {
    // Clear the screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    int yOffset = 20;
    int xOffset = 20;

    SDL_Texture* filePathTexture = CreateTextTexture(renderer, font, "File: " + this->currentFile.getFilePath(), { 255, 255, 0, 255 });
    if (filePathTexture) {
        SDL_Rect rect = { xOffset, yOffset, 0, 0 };
        SDL_QueryTexture(filePathTexture, nullptr, nullptr, &rect.w, &rect.h);
        SDL_RenderCopy(renderer, filePathTexture, nullptr, &rect);
        yOffset += rect.h + 10;
        SDL_DestroyTexture(filePathTexture);
    }

    if(getFileType(currentFile.getFilePath()) == "audio") {
        AudioMetadata audioData = ((MetadataController*) this->controller)->getAudioMetadata(currentFile.getFilePath());
        renderAudioMetadata(audioData, xOffset, yOffset);
    }
    else if(getFileType(currentFile.getFilePath()) == "video") {
        VideoMetadata videoData = ((MetadataController*) this->controller)->getVideoMetadata(currentFile.getFilePath());
        renderVideoMetadata(videoData, xOffset, yOffset);
    }
    else {
        SDL_Texture* unknownTexture = CreateTextTexture(renderer, font,"--- Unsupported file type ---", { 255, 255, 0, 255 });
        if (unknownTexture) {
            SDL_Rect rect = { xOffset, yOffset, 0, 0 };
            SDL_QueryTexture(unknownTexture, nullptr, nullptr, &rect.w, &rect.h);
            SDL_RenderCopy(renderer, unknownTexture, nullptr, &rect);
            yOffset += rect.h + 10;
            SDL_DestroyTexture(unknownTexture);
        }
    }       

    // Update the screen
    SDL_RenderPresent(renderer);
}

void MetadataView::cleanup() {
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

void MetadataView::renderTextAndPosition(const std::string& text, int x, int& y, SDL_Color color) {
    SDL_Texture* texture = CreateTextTexture(renderer, font, text, { 255, 255, 0, 255 });
    if (texture) {
        SDL_Rect rect = { x, y, 0, 0 };
        SDL_QueryTexture(texture, nullptr, nullptr, &rect.w, &rect.h);
        SDL_RenderCopy(renderer, texture, nullptr, &rect);
        y += rect.h;
        SDL_DestroyTexture(texture);
    }
}

void MetadataView::renderAudioMetadata(AudioMetadata& metadata, int x, int& y) {
    SDL_Texture* audioHeaderTexture = CreateTextTexture(renderer, font,"--- Audio Metadata ---", { 255, 255, 0, 255 });
    if (audioHeaderTexture) {
        SDL_Rect rect = { x, y, 0, 0 };
        SDL_QueryTexture(audioHeaderTexture, nullptr, nullptr, &rect.w, &rect.h);
        SDL_RenderCopy(renderer, audioHeaderTexture, nullptr, &rect);
        y += rect.h + 10;
        SDL_DestroyTexture(audioHeaderTexture);
    }

    SDL_Color textColor = { 255, 255, 255, 255 };

    AudioMetadata::AudioTag* tag = metadata.getAudioTag();
    AudioMetadata::AudioProperties* properties = metadata.getAudioProperties();

    if (tag) {
        renderTextAndPosition("Title: " + tag->getTitle(), x, y, textColor);
        renderTextAndPosition("Artist: " + tag->getArtist(), x, y, textColor);
        renderTextAndPosition("Album: " + tag->getAlbum(), x, y, textColor);
        renderTextAndPosition("Comment: " + tag->getComment(), x, y, textColor);
        renderTextAndPosition("Genre: " + tag->getGenre(), x, y, textColor);
        renderTextAndPosition("Year: " + std::to_string(tag->getYear()), x, y, textColor);
        renderTextAndPosition("Track: " + std::to_string(tag->getTrack()), x, y, textColor);
    }

    if (properties) {
        renderTextAndPosition("Length: " + std::to_string(properties->getLength()), x, y, textColor);
        renderTextAndPosition("Bit Rate: " + std::to_string(properties->getBitRate()), x, y, textColor);
        renderTextAndPosition("Sample Rate: " + std::to_string(properties->getSampleRate()), x, y, textColor);
        renderTextAndPosition("Channels: " + std::to_string(properties->getChannels()), x, y, textColor);
    }
}

void MetadataView::renderVideoMetadata(const VideoMetadata& metadata, int x, int& y) {
    SDL_Texture* videoHeaderTexture = CreateTextTexture(renderer, font, "--- Video Metadata ---", { 255, 255, 0, 255 });
    if (videoHeaderTexture) {
        SDL_Rect rect = { x, y, 0, 0 };
        SDL_QueryTexture(videoHeaderTexture, nullptr, nullptr, &rect.w, &rect.h);
        SDL_RenderCopy(renderer, videoHeaderTexture, nullptr, &rect);
        y += rect.h + 10;
        SDL_DestroyTexture(videoHeaderTexture);
    }

    SDL_Color textColor = { 255, 255, 255, 255 };

    renderTextAndPosition("Duration: " + std::to_string(metadata.getDuration()), x, y, textColor);
    renderTextAndPosition("Bit Rate: " + std::to_string(metadata.getBitRate()), x, y, textColor);

    const auto& metadataMap = metadata.getMetadata();
    for (const auto& pair : metadataMap) {
        renderTextAndPosition(pair.first + ": " + pair.second, x, y, textColor);
    }

    const auto& streams = metadata.getStreams();
    for (const auto& stream : streams) {
        renderTextAndPosition("Codec Type: " + stream->getCodecType(), x, y, textColor);
        renderTextAndPosition("Codec Name: " + stream->getCodecName(), x, y, textColor);
        renderTextAndPosition("Stream Duration: " + std::to_string(stream->getDuration()), x, y, textColor);
        renderTextAndPosition("Stream Bit Rate: " + std::to_string(stream->getBitRate()), x, y, textColor);

        if (VideoMetadata::VideoStream* videoStream = dynamic_cast<VideoMetadata::VideoStream*>(stream)) {
            renderTextAndPosition("  Width: " + std::to_string(videoStream->getWidth()), x + 20, y, textColor);
            renderTextAndPosition("  Height: " + std::to_string(videoStream->getHeight()), x + 20, y, textColor);
            renderTextAndPosition("  Frame Rate: " + std::to_string(videoStream->getFrameRate()), x + 20, y, textColor);
        } else if (VideoMetadata::AudioStream* audioStream = dynamic_cast<VideoMetadata::AudioStream*>(stream)) {
            renderTextAndPosition("  Sample Rate: " + std::to_string(audioStream->getSampleRate()), x + 20, y, textColor);
            renderTextAndPosition("  Channels: " + std::to_string(audioStream->getChannels()), x + 20, y, textColor);
        }

        const auto& streamMetadata = stream->getMetadata();
        for (const auto& pair : streamMetadata) {
            renderTextAndPosition("    " + pair.first + ": " + pair.second, x + 40, y, textColor);
        }
    }
}
