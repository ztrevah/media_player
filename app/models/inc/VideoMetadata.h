#ifndef VIDEOMETADATA_H
#define VIDEOMETADATA_H

#include <iostream>
#include <string>
#include <vector>
#include <map>

class VideoMetadata {
public:
    class Stream {
    protected:
        std::string codecType;
        std::string codecName;
        int64_t duration;
        int64_t bitRate;
        std::map<std::string, std::string> metadata;

    public:
        // Default Constructor
        Stream() : duration(0), bitRate(0) {}

        // Constructor
        Stream(const std::string& codecType, const std::string& codecName,
               int64_t duration, int64_t bitRate);

        // Destructor
        virtual ~Stream();

        // Getters
        const std::string& getCodecType() const;
        const std::string& getCodecName() const;
        int64_t getDuration() const;
        int64_t getBitRate() const;
        const std::map<std::string, std::string>& getMetadata() const;

        // Setters
        void setCodecType(const std::string& codecType);
        void setCodecName(const std::string& codecName);
        void setDuration(int64_t duration);
        void setBitRate(int64_t bitRate);
        void setMetadata(const std::map<std::string, std::string>& metadata);
        void addMetadata(const std::string& key, const std::string& value);
    };

    class VideoStream : public Stream {
    private:
        int width;
        int height;
        double frameRate;

    public:
        // Default Constructor
        VideoStream() : width(0), height(0), frameRate(0.0) {}

        // Constructor
        VideoStream(const std::string& codecType, const std::string& codecName,
                    int64_t duration, int64_t bitRate,
                    int width, int height, double frameRate);

        // Destructor
        ~VideoStream() override;

        // Getters
        int getWidth() const;
        int getHeight() const;
        double getFrameRate() const;

        // Setters
        void setWidth(int width);
        void setHeight(int height);
        void setFrameRate(double frameRate);
    };

    class AudioStream : public Stream {
    private:
        int sampleRate;
        int channels;

    public:
        // Default Constructor
        AudioStream() : sampleRate(0), channels(0) {}

        // Constructor
        AudioStream(const std::string& codecType, const std::string& codecName,
                    int64_t duration, int64_t bitRate,
                    int sampleRate, int channels);

        // Destructor
        ~AudioStream() override;

        // Getters
        int getSampleRate() const;
        int getChannels() const;

        // Setters
        void setSampleRate(int sampleRate);
        void setChannels(int channels);
    };

private:
    std::vector<Stream*> streams;
    int64_t duration;
    int64_t bitRate;
    std::map<std::string, std::string> metadata;

public:
    // Default Constructor
    VideoMetadata() : duration(0), bitRate(0) {}

    // Constructor
    VideoMetadata(int64_t duration, int64_t bitRate);
    VideoMetadata(const VideoMetadata& metadata);
    VideoMetadata(VideoMetadata&& metadata);

    VideoMetadata& operator=(const VideoMetadata& other);
    VideoMetadata& operator=(VideoMetadata&& other);

    // Destructor
    ~VideoMetadata();

    // Getters
    const std::vector<Stream*>& getStreams() const;
    int64_t getDuration() const;
    int64_t getBitRate() const;
    const std::map<std::string, std::string>& getMetadata() const;

    // Setters
    void setDuration(int64_t duration);
    void setBitRate(int64_t bitRate);
    void setMetadata(const std::map<std::string, std::string>& metadata);
    void addMetadata(const std::string& key, const std::string& value);

    // Methods to add streams
    void addStream(Stream* stream);
    void addVideoStream(const std::string& codecType, const std::string& codecName,
                        int64_t duration, int64_t bitRate,
                        int width, int height, double frameRate);
    void addAudioStream(const std::string& codecType, const std::string& codecName,
                        int64_t duration, int64_t bitRate,
                        int sampleRate, int channels);
};

class VideoMetadataException : public std::exception {
public:
    VideoMetadataException(const std::string& message) : message(message) {}
    const char* what() const noexcept override { return message.c_str(); }
private:
    std::string message;
};

#endif // VIDEOMETADATA_H