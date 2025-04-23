#include "../inc/AudioMetadata.h"

// AudioTag Implementation
AudioMetadata::AudioTag::AudioTag(std::string title, std::string artist, std::string album, std::string comment, std::string genre, unsigned int year, unsigned int track)
    : title(title), artist(artist), album(album), comment(comment), genre(genre), year(year), track(track) {}

AudioMetadata::AudioTag::~AudioTag() {}

std::string AudioMetadata::AudioTag::getTitle() const { return title; }
void AudioMetadata::AudioTag::setTitle(const std::string& newTitle) { title = newTitle; }

std::string AudioMetadata::AudioTag::getArtist() const { return artist; }
void AudioMetadata::AudioTag::setArtist(const std::string& newArtist) { artist = newArtist; }

std::string AudioMetadata::AudioTag::getAlbum() const { return album; }
void AudioMetadata::AudioTag::setAlbum(const std::string& newAlbum) { album = newAlbum; }

std::string AudioMetadata::AudioTag::getComment() const { return comment; }
void AudioMetadata::AudioTag::setComment(const std::string& newComment) { comment = newComment; }

std::string AudioMetadata::AudioTag::getGenre() const { return genre; }
void AudioMetadata::AudioTag::setGenre(const std::string& newGenre) { genre = newGenre; }

unsigned int AudioMetadata::AudioTag::getYear() const { return year; }
void AudioMetadata::AudioTag::setYear(unsigned int newYear) { year = newYear; }

unsigned int AudioMetadata::AudioTag::getTrack() const { return track; }
void AudioMetadata::AudioTag::setTrack(unsigned int newTrack) { track = newTrack; }

// AudioProperties Implementation
AudioMetadata::AudioProperties::AudioProperties(int length, int bitRate, int sampleRate, int channels)
    : length(length), bitRate(bitRate), sampleRate(sampleRate), channels(channels) {}

AudioMetadata::AudioProperties::~AudioProperties() {}

int AudioMetadata::AudioProperties::getLength() const { return length; }
void AudioMetadata::AudioProperties::setLength(int newLength) { length = newLength; }

int AudioMetadata::AudioProperties::getBitRate() const { return bitRate; }
void AudioMetadata::AudioProperties::setBitRate(int newBitRate) { bitRate = newBitRate; }

int AudioMetadata::AudioProperties::getSampleRate() const { return sampleRate; }
void AudioMetadata::AudioProperties::setSampleRate(int newSampleRate) { sampleRate = newSampleRate; }

int AudioMetadata::AudioProperties::getChannels() const { return channels; }
void AudioMetadata::AudioProperties::setChannels(int newChannels) { channels = newChannels; }

// AudioMetadata Implementation
AudioMetadata::AudioMetadata() : audioTag(new AudioTag()), audioProperties(new AudioProperties()) {}

AudioMetadata::~AudioMetadata() {
    if(audioTag){
        delete audioTag;
    }
    if(audioProperties){
        delete audioProperties;
    }
}

// Copy Constructor
AudioMetadata::AudioMetadata(const AudioMetadata& metadata) : audioTag(nullptr), audioProperties(nullptr) {
    if (metadata.audioTag) {
        audioTag = new AudioTag(*metadata.audioTag);
    }
    if (metadata.audioProperties) {
        audioProperties = new AudioProperties(*metadata.audioProperties);
    }
}

// Move Constructor
AudioMetadata::AudioMetadata(AudioMetadata&& metadata) : audioTag(metadata.audioTag), audioProperties(metadata.audioProperties) {
    metadata.audioTag = nullptr;
    metadata.audioProperties = nullptr;
}

// Copy Assignment Operator
AudioMetadata& AudioMetadata::operator=(const AudioMetadata& otherMetadata) {
    if (this != &otherMetadata) {
        delete audioTag;
        delete audioProperties;
        audioTag = nullptr;
        audioProperties = nullptr;

        if (otherMetadata.audioTag) {
            audioTag = new AudioTag(*otherMetadata.audioTag);
        }
        if (otherMetadata.audioProperties) {
            audioProperties = new AudioProperties(*otherMetadata.audioProperties);
        }
    }
    return *this;
}

AudioMetadata& AudioMetadata::operator=(AudioMetadata&& otherMetadata) {
    if (this != &otherMetadata) {
        delete audioTag;
        delete audioProperties;

        audioTag = otherMetadata.audioTag;
        audioProperties = otherMetadata.audioProperties;

        otherMetadata.audioTag = nullptr;
        otherMetadata.audioProperties = nullptr;
    }
    return *this;
}

AudioMetadata::AudioTag* AudioMetadata::getAudioTag() { return audioTag; }

void AudioMetadata::setAudioTag(AudioMetadata::AudioTag* newAudioTag) {
    if(audioTag){
        delete audioTag;
    }
    audioTag = newAudioTag;
}

AudioMetadata::AudioProperties* AudioMetadata::getAudioProperties() { return audioProperties; }

void AudioMetadata::setAudioProperties(AudioMetadata::AudioProperties* newAudioProperties) {
    if(audioProperties){
        delete audioProperties;
    }
    audioProperties = newAudioProperties;
}

// void AudioMetadata::setOtherProperties(const std::map<std::string, std::string>& properties) {
//     otherProperties = properties;
// }

// void AudioMetadata::insertOtherProperty(const std::string& key, const std::string& value) {
//     otherProperties[key] = value;
// }

// void AudioMetadata::deleteOtherProperty(const std::string& key) {
//     otherProperties.erase(key);
// }

// const std::map<std::string, std::string>& AudioMetadata::getOtherProperties() const { return otherProperties; }