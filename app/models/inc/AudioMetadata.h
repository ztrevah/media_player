#ifndef AUDIO_METADATA_H
#define AUDIO_METADATA_H

#include <map>
#include <string>

class AudioMetadata {
public:
    class AudioTag {
    private:
        std::string title;
        std::string artist;
        std::string album;
        std::string comment;
        std::string genre;
        unsigned int year;
        unsigned int track;

    public:
        AudioTag(
            std::string title = "",
            std::string artist = "",
            std::string album = "",
            std::string comment = "",
            std::string genre = "",
            unsigned int year = 0,
            unsigned int track = 0
        );
        ~AudioTag();

        std::string getTitle() const;
        void setTitle(const std::string& newTitle);

        std::string getArtist() const;
        void setArtist(const std::string& newArtist);

        std::string getAlbum() const;
        void setAlbum(const std::string& newAlbum);

        std::string getComment() const;
        void setComment(const std::string& newComment);

        std::string getGenre() const;
        void setGenre(const std::string& newGenre);

        unsigned int getYear() const;
        void setYear(unsigned int newYear);

        unsigned int getTrack() const;
        void setTrack(unsigned int newTrack);
    };

    class AudioProperties {
    private:
        int length;
        int bitRate;
        int sampleRate;
        int channels;

    public:
        AudioProperties(int length = 0, int bitRate = 0, int sampleRate = 0, int channels = 0);
        ~AudioProperties();

        int getLength() const;
        void setLength(int newLength);

        int getBitRate() const;
        void setBitRate(int newBitRate);

        int getSampleRate() const;
        void setSampleRate(int newSampleRate);

        int getChannels() const;
        void setChannels(int newChannels);
    };

private:
    AudioTag* audioTag;
    AudioProperties* audioProperties;
    // std::map<std::string, std::string> otherProperties;

public:

    AudioMetadata();
    AudioMetadata(const AudioMetadata& metadata);
    AudioMetadata(AudioMetadata&& metadata);
    ~AudioMetadata();

    AudioTag* getAudioTag();
    void setAudioTag(AudioTag* newAudioTag);
    AudioProperties* getAudioProperties();
    void setAudioProperties(AudioProperties* newAudioProperties);

    // void setOtherProperties(const std::map<std::string, std::string>& properties);
    // void insertOtherProperty(const std::string& key, const std::string& value);
    // void deleteOtherProperty(const std::string& key);
    // const std::map<std::string, std::string>& getOtherProperties() const;

    AudioMetadata& operator=(const AudioMetadata& otherMetadata);
    AudioMetadata& operator=(AudioMetadata&& otherMetadata);
};

#endif // AUDIO_METADATA_H