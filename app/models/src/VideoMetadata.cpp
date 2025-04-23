#include "../inc/VideoMetadata.h"
#include <iostream>
#include <sstream>

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/dict.h>
#include <libavutil/time.h>
}
// ------------------------ Stream Class ------------------------
VideoMetadata::Stream::Stream(const std::string& codecType, const std::string& codecName,
                             int64_t duration, int64_t bitRate)
    : codecType(codecType), codecName(codecName), duration(duration), bitRate(bitRate) {}

VideoMetadata::Stream::~Stream() {}

const std::string& VideoMetadata::Stream::getCodecType() const { return codecType; }
const std::string& VideoMetadata::Stream::getCodecName() const { return codecName; }
int64_t VideoMetadata::Stream::getDuration() const { return duration; }
int64_t VideoMetadata::Stream::getBitRate() const { return bitRate; }
const std::map<std::string, std::string>& VideoMetadata::Stream::getMetadata() const { return metadata; }

void VideoMetadata::Stream::setCodecType(const std::string& codecType) { this->codecType = codecType; }
void VideoMetadata::Stream::setCodecName(const std::string& codecName) { this->codecName = codecName; }
void VideoMetadata::Stream::setDuration(int64_t duration) { this->duration = duration; }
void VideoMetadata::Stream::setBitRate(int64_t bitRate) { this->bitRate = bitRate; }
void VideoMetadata::Stream::setMetadata(const std::map<std::string, std::string>& metadata) { this->metadata = metadata; }
void VideoMetadata::Stream::addMetadata(const std::string& key, const std::string& value) { this->metadata[key] = value; }

// ------------------------ VideoStream Class ------------------------
VideoMetadata::VideoStream::VideoStream(const std::string& codecType, const std::string& codecName,
                                        int64_t duration, int64_t bitRate,
                                        int width, int height, double frameRate)
    : Stream(codecType, codecName, duration, bitRate),
      width(width), height(height), frameRate(frameRate) {}

VideoMetadata::VideoStream::~VideoStream() {}

int VideoMetadata::VideoStream::getWidth() const { return width; }
int VideoMetadata::VideoStream::getHeight() const { return height; }
double VideoMetadata::VideoStream::getFrameRate() const { return frameRate; }

void VideoMetadata::VideoStream::setWidth(int width) { this->width = width; }
void VideoMetadata::VideoStream::setHeight(int height) { this->height = height; }
void VideoMetadata::VideoStream::setFrameRate(double frameRate) { this->frameRate = frameRate; }

// ------------------------ AudioStream Class ------------------------
VideoMetadata::AudioStream::AudioStream(const std::string& codecType, const std::string& codecName,
                                        int64_t duration, int64_t bitRate,
                                        int sampleRate, int channels)
    : Stream(codecType, codecName, duration, bitRate),
      sampleRate(sampleRate), channels(channels) {}

VideoMetadata::AudioStream::~AudioStream() {}

int VideoMetadata::AudioStream::getSampleRate() const { return sampleRate; }
int VideoMetadata::AudioStream::getChannels() const { return channels; }

void VideoMetadata::AudioStream::setSampleRate(int sampleRate) { this->sampleRate = sampleRate; }
void VideoMetadata::AudioStream::setChannels(int channels) { this->channels = channels; }

// ------------------------ VideoMetadata Class ------------------------
VideoMetadata::VideoMetadata(int64_t duration, int64_t bitRate)
    : duration(duration), bitRate(bitRate) {}

VideoMetadata::VideoMetadata(const VideoMetadata& other) : duration(other.duration), bitRate(other.bitRate), metadata(other.metadata) {
    this->streams.reserve(other.streams.size());
    for (const Stream* stream : other.streams) {
        this->streams.push_back(new Stream(*stream));
    }
}

// Move Constructor
VideoMetadata::VideoMetadata(VideoMetadata&& other) : streams(std::move(other.streams)), duration(other.duration), bitRate(other.bitRate), metadata(std::move(other.metadata)) {

}

// Copy Assignment Operator
VideoMetadata& VideoMetadata::operator=(const VideoMetadata& other) {
    if (this != &other) {
        // Clean up existing streams
        for (Stream* stream : streams) {
            delete stream;
        }
        streams.clear();

        duration = other.duration;
        bitRate = other.bitRate;
        metadata = other.metadata;

        // Copy streams
        streams.reserve(other.streams.size());
        for (const Stream* stream : other.streams) {
            if (dynamic_cast<const VideoStream*>(stream)) {
                streams.push_back(new VideoStream(*dynamic_cast<const VideoStream*>(stream)));
            } else if (dynamic_cast<const AudioStream*>(stream)) {
                streams.push_back(new AudioStream(*dynamic_cast<const AudioStream*>(stream)));
            } else if (stream) {
                streams.push_back(new Stream(*stream));
            }
        }
    }
    return *this;
}

// Move Assignment Operator
VideoMetadata& VideoMetadata::operator=(VideoMetadata&& other) {
    if (this != &other) {
        // Clean up existing streams
        for (Stream* stream : streams) {
            delete stream;
        }
        streams = std::move(other.streams);
        duration = other.duration;
        bitRate = other.bitRate;
        metadata = std::move(other.metadata);
    }
    return *this;
}

VideoMetadata::~VideoMetadata() {
    for (Stream* stream : streams) {
        if(stream) delete stream;
    }
    streams.clear();
}

const std::vector<VideoMetadata::Stream*>& VideoMetadata::getStreams() const { return streams; }
int64_t VideoMetadata::getDuration() const { return duration; }
int64_t VideoMetadata::getBitRate() const { return bitRate; }
const std::map<std::string, std::string>& VideoMetadata::getMetadata() const { return metadata; }

void VideoMetadata::setDuration(int64_t duration) { this->duration = duration; }
void VideoMetadata::setBitRate(int64_t bitRate) { this->bitRate = bitRate; }
void VideoMetadata::setMetadata(const std::map<std::string, std::string>& metadata) { this->metadata = metadata; }
void VideoMetadata::addMetadata(const std::string& key, const std::string& value) { this->metadata[key] = value; }

void VideoMetadata::addStream(Stream* stream) {
    streams.push_back(stream);
}

void VideoMetadata::addVideoStream(const std::string& codecType, const std::string& codecName,
                                    int64_t duration, int64_t bitRate,
                                    int width, int height, double frameRate) {
    streams.push_back(new VideoStream(codecType, codecName, duration, bitRate, width, height, frameRate));
}

void VideoMetadata::addAudioStream(const std::string& codecType, const std::string& codecName,
                                    int64_t duration, int64_t bitRate,
                                    int sampleRate, int channels) {
    streams.push_back(new AudioStream(codecType, codecName, duration, bitRate, sampleRate, channels));
}