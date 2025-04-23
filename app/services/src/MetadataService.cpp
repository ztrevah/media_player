#include "../inc/MetadataService.h"

#include <taglib/tag.h>
#include <taglib/fileref.h>
#include <taglib/mpegfile.h>
#include <taglib/flacfile.h>
#include <taglib/wavfile.h>
#include <taglib/tpropertymap.h>

#include <iostream>
#include <sstream>

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/dict.h>
#include <libavutil/time.h>
}

MetadataService::MetadataService() {

}

MetadataService::~MetadataService() {

}

AudioMetadata MetadataService::getAudioMetadata(MediaFile mediaFile) {
    AudioMetadata metadata;
    TagLib::FileRef f(mediaFile.getFilePath().c_str());

    if (f.isNull()) {
        throw MetadataServiceException("Error reading audio metadata of the file: " + mediaFile.getFilePath());
    }

    TagLib::Tag *tag = f.tag();
    if (tag) {
        metadata.getAudioTag()->setTitle(tag->title().toCString(true));
        metadata.getAudioTag()->setArtist(tag->artist().toCString(true));
        metadata.getAudioTag()->setAlbum(tag->album().toCString(true));
        metadata.getAudioTag()->setGenre(tag->genre().toCString(true));
        metadata.getAudioTag()->setYear(tag->year());
        metadata.getAudioTag()->setTrack(tag->track());
        metadata.getAudioTag()->setComment(tag->comment().toCString(true));
    } else {
        metadata.setAudioTag(nullptr);
    }

    TagLib::AudioProperties *properties = f.audioProperties();
    if (properties) {
        metadata.getAudioProperties()->setBitRate(properties->bitrate());
        metadata.getAudioProperties()->setChannels(properties->channels());
        metadata.getAudioProperties()->setLength(properties->lengthInMilliseconds());
        metadata.getAudioProperties()->setSampleRate(properties->sampleRate());
    } else {
        metadata.setAudioProperties(nullptr);
    }

    return metadata;
}

void MetadataService::updateAudioMetadata(MediaFile mediaFile, AudioMetadata metadata) {
    TagLib::FileRef f(mediaFile.getFilePath().c_str());
    if (f.isNull()) {
        throw MetadataServiceException("Error: Cannot read audio metadata of the file " + mediaFile.getFilePath() + " for updating.");
    }
    TagLib::Tag *tag = f.tag();
    if(!tag) {
        throw MetadataServiceException("Error: Cannot read audio metadata of the file " + mediaFile.getFilePath() + " for updating.");
    }

    tag->setTitle(metadata.getAudioTag()->getTitle());
    tag->setArtist(metadata.getAudioTag()->getArtist());
    tag->setAlbum(metadata.getAudioTag()->getAlbum());
    tag->setGenre(metadata.getAudioTag()->getGenre());
    tag->setTrack(metadata.getAudioTag()->getTrack());
    tag->setYear(metadata.getAudioTag()->getYear());
    tag->setComment(metadata.getAudioTag()->getComment());

    if(!f.save()) {
        throw MetadataServiceException("Error: Cannot update the file metadata.");
    }
}

VideoMetadata MetadataService::getVideoMetadata(MediaFile mediaFile) {
    AVFormatContext* formatContext = nullptr;
    int ret;

    if ((ret = avformat_open_input(&formatContext, mediaFile.getFilePath().c_str(), nullptr, nullptr)) < 0) {
        char* buffer = (char*) calloc(AV_ERROR_MAX_STRING_SIZE, sizeof(char));
        av_make_error_string(buffer, AV_ERROR_MAX_STRING_SIZE, ret);
        std::stringstream ss;
        ss << "Could not open source file " << mediaFile.getFilePath() << ": " << buffer << std::endl;
        free(buffer);

        throw VideoMetadataException(ss.str());
    }

    // Get stream information
    if ((ret = avformat_find_stream_info(formatContext, nullptr)) < 0) {
        char* buffer = (char*) calloc(AV_ERROR_MAX_STRING_SIZE, sizeof(char));
        av_make_error_string(buffer, AV_ERROR_MAX_STRING_SIZE, ret);
        std::stringstream ss;
        ss << "Could not find stream information: " << buffer << std::endl;
        avformat_close_input(&formatContext);
        free(buffer);

        throw VideoMetadataException(ss.str());
    }

    // Populate VideoMetadata object
    VideoMetadata videoMetadata;
    videoMetadata.setDuration(formatContext->duration / AV_TIME_BASE); // Convert to seconds
    videoMetadata.setBitRate(formatContext->bit_rate);

    // Read global metadata
    AVDictionaryEntry* tag = nullptr;
    while ((tag = av_dict_get(formatContext->metadata, "", tag, AV_DICT_IGNORE_SUFFIX))) {
        videoMetadata.addMetadata(tag->key, tag->value);
    }

    // Iterate through streams
    for (unsigned int i = 0; i < formatContext->nb_streams; i++) {
        AVStream* stream = formatContext->streams[i];
        AVCodecParameters* codecParameters = stream->codecpar;

        std::string codecType;
        switch (codecParameters->codec_type) {
            case AVMEDIA_TYPE_VIDEO:
                codecType = "video";
                break;
            case AVMEDIA_TYPE_AUDIO:
                codecType = "audio";
                break;
            default:
                codecType = "unknown";
                break;
        }

        if (codecType == "video") {
            VideoMetadata::VideoStream* videoStream = new VideoMetadata::VideoStream
            (
                codecType,
                avcodec_find_decoder(codecParameters->codec_id)->long_name,
                (double)stream->duration * av_q2d(stream->time_base),
                codecParameters->bit_rate,
                codecParameters->width,
                codecParameters->height,
                (stream->avg_frame_rate.num && stream->avg_frame_rate.den) ? av_q2d(stream->avg_frame_rate) : 0.0
            );

            //Stream metadata
            while ((tag = av_dict_get(stream->metadata, "", tag, AV_DICT_IGNORE_SUFFIX))) {
                videoStream->addMetadata(tag->key, tag->value);
            }
            videoMetadata.addStream(videoStream);

        } 
        else if (codecType == "audio") {
            VideoMetadata::AudioStream* audioStream = new VideoMetadata::AudioStream
            (
                codecType,
                avcodec_find_decoder(codecParameters->codec_id)->long_name,
                (double)stream->duration * av_q2d(stream->time_base),
                codecParameters->bit_rate,
                codecParameters->sample_rate,
                codecParameters->ch_layout.nb_channels
            );
            while ((tag = av_dict_get(stream->metadata, "", tag, AV_DICT_IGNORE_SUFFIX))) {
                audioStream->addMetadata(tag->key, tag->value);
            }
            videoMetadata.addStream(audioStream);
        } 
        else {
            VideoMetadata::Stream* genericStream = new VideoMetadata::Stream(
            codecType,
            avcodec_find_decoder(codecParameters->codec_id)->long_name,
            (double)stream->duration * av_q2d(stream->time_base),
            codecParameters->bit_rate);
            while ((tag = av_dict_get(stream->metadata, "", tag, AV_DICT_IGNORE_SUFFIX))) {
                genericStream->addMetadata(tag->key, tag->value);
            }
            videoMetadata.addStream(genericStream);
        }
    }

    avformat_close_input(&formatContext);
    return videoMetadata;
}

// Not supported yet
void MetadataService::updateVideoMetadata(MediaFile mediaFile, VideoMetadata) {
    return;
}

