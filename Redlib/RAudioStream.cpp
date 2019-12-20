#include "RAudioStream.h"

#include "RDebug.h"

const unsigned RAudioStream::BUFFER_SIZE = 256;

RAudioStream::RAudioStream(Api api):
    stream_(api)
{
    if(printError(stream_.getDeviceCount() < 1, "No audio devices found!"))
        return;
    parameters_.deviceId = stream_.getDefaultOutputDevice();
    parameters_.firstChannel = 0;
}

RAudioStream::RAudioStream(const RMp3 &mp3, RAudioStream::Api api):
    RAudioStream(api)
{
#ifdef R_DEBUG
    if(printError(!mp3.isValid(), "Failed open audio stream, RMp3 is invalid!")) return;
#endif
    openStream(mp3);
}

RAudioStream::RAudioStream(const RAudioStream &audio):
    RAudioStream(audio.mp3_, const_cast<RAudioStream&>(audio).stream_.getCurrentApi())
{

}

RAudioStream& RAudioStream::operator=(const RAudioStream &audio)
{
    stream_ = RAudioStreamExtern::RtAudio(const_cast<RAudioStream&>(audio).stream_.getCurrentApi());
    if(printError(stream_.getDeviceCount() < 1, "No audio devices found!"))
        return *this;

    parameters_.deviceId = stream_.getDefaultOutputDevice();
    parameters_.firstChannel = 0;

    if(audio.mp3_.isValid())
        openStream(audio.mp3_);

    return *this;
}

RAudioStream::~RAudioStream()
{
    if(isRunning()) stopStream();
    if(isOpen()) closeStream();
}

std::string RAudioStream::getCurrentApi() const
{
    RAudioStreamExtern::RtAudio *pa = const_cast<RAudioStreamExtern::RtAudio*>(&stream_);
    return stream_.getApiName(pa->getCurrentApi());
}

bool RAudioStream::isOpen() const
{
    return stream_.isStreamOpen();
}

bool RAudioStream::isRunning() const
{
    return stream_.isStreamRunning();
}

double RAudioStream::getStreamTime() const
{
    RAudioStreamExtern::RtAudio *pa = const_cast<RAudioStreamExtern::RtAudio*>(&stream_);
    return pa->getStreamTime();
}

float RAudioStream::getVolume() const
{
    return volume_;
}

void RAudioStream::setStreamTime(double time)
{
    stream_.setStreamTime(time);
}

float RAudioStream::setVolume(float volume)
{
    if(volume > 1.0f) volume = 1.0f;
    if(volume < 0.0f) volume = 0.0f;
    return volume_ = volume;
}

float RAudioStream::increaseVolume(float increase)
{
    return setVolume(volume_ + increase);
}

float RAudioStream::decreaseVolume(float decrease)
{
    return setVolume(volume_ - decrease);
}

bool RAudioStream::openStream(const RMp3 &mp3)
{
#ifdef R_DEBUG
    if(printError(stream_.isStreamOpen(), "Failed start audio stream, there is already opening!")) return false;
#endif

    mp3_ = mp3;
    parameters_.nChannels = static_cast<unsigned>(mp3.channel());

    try {
        stream_.openStream(&parameters_, nullptr, RAudioStreamExtern::RTAUDIO_SINT16, static_cast<unsigned>(mp3.hz()),
                         const_cast<unsigned*>(&BUFFER_SIZE), &playback, this);
    }
    catch(RAudioStreamExtern::RtAudioError &e) {
        e.printMessage();
        return false;
    }
    return true;
}

void RAudioStream::closeStream()
{
    if(stream_.isStreamOpen()) stream_.closeStream();
}

bool RAudioStream::startStream(unsigned repeat)
{
#ifdef R_DEBUG
    if(printError(!stream_.isStreamOpen(), "Failed start audio stream, there not have open!")) return false;
    if(printError(stream_.isStreamRunning(), "Failed start audio stream, there is already running!")) return false;
#endif

    setStreamTime();
    try {
        stream_.startStream();
        repeat_ = repeat;
    }
    catch(RAudioStreamExtern::RtAudioError &e) {
        e.printMessage();
        return false;
    }
    return true;
}

bool RAudioStream::repeatStream()
{
    return startStream(~0u);
}

bool RAudioStream::stopStream()
{
#ifdef R_DEBUG
    if(printError(!stream_.isStreamRunning(), "Failed stop audio stream, there not have star!")) return false;
#endif

    try {
        stream_.stopStream();
    }
    catch(RAudioStreamExtern::RtAudioError &e) {
        e.printMessage();
        return false;
    }

    return true;
}

bool RAudioStream::abortStream()
{
#ifdef R_DEBUG
    if(printError(!stream_.isStreamRunning(), "Failed abort audio stream, there not have star!")) return false;
#endif

    try {
        stream_.abortStream();
    }
    catch(RAudioStreamExtern::RtAudioError &e) {
        e.printMessage();
        return false;
    }

    return true;
}

int RAudioStream::playback(void *outputBuffer, void *, unsigned nBufferFrames, double streamTime,
                           RAudioStreamExtern::RtAudioStreamStatus status, void *userData)
{
    if(printError(status, "Stream underflow detected!")) return 2;

    int16_t *buffer = static_cast<int16_t*>(outputBuffer);
    RAudioStream *rAudio = static_cast<RAudioStream*>(userData);
    unsigned channels = rAudio->parameters_.nChannels;

    unsigned offset = static_cast<unsigned>(streamTime * rAudio->mp3_.hz() * channels);
    if(offset + nBufferFrames > rAudio->mp3_.samples())
    {
        if(rAudio->repeat_ == 0)
            return 1;
        else {
            --rAudio->repeat_;
            rAudio->setStreamTime();
        }
    }

    const int16_t* lastValues = rAudio->mp3_.cdata() + offset;
    float volume = rAudio->volume_;
    for(unsigned i = 0; i < nBufferFrames; ++i)
    {
        for(unsigned j = 0; j < channels; ++j)
            *buffer++ = static_cast<int16_t>(*lastValues++ * volume);
    }

    return 0;
}
