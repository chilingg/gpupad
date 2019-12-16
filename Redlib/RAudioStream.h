#ifndef RAUDIOSTREAM_H
#define RAUDIOSTREAM_H

#include "RtAudio.h"
#include "RResource/RMp3.h"

class RAudioStream
{
public:
    using Api = RAudioStreamExtern::RtAudio::Api;

    RAudioStream(Api api = Api::UNSPECIFIED);
    RAudioStream(const RMp3 &mp3, Api api = Api::UNSPECIFIED);
    RAudioStream(const RAudioStream &audio);
    RAudioStream& operator=(const RAudioStream &audio);
    ~RAudioStream();

    std::string getCurrentApi() const;
    bool isOpen() const;
    bool isRunning() const;
    double getStreamTime() const;

    void setStreamTime(double time = 0.0);

    bool openStream(const RMp3 &mp3);
    void closeStream();
    bool startStream(unsigned repeat = 0);
    bool repeatStream();
    bool stopStream();
    bool abortStream();

private:
    static int playback(void *outputBuffer, 
                        void *inputBuffer, 
                        unsigned nBufferFrames,
                        double streamTime, 
                        RAudioStreamExtern::RtAudioStreamStatus status,
                        void *userData);

    static const unsigned BUFFER_SIZE;

    RAudioStreamExtern::RtAudio stream_;
    RAudioStreamExtern::RtAudio::StreamParameters parameters_;
    RMp3 mp3_;
    unsigned repeat_ = 0;
};

#endif // RAUDIOSTREAM_H
