#include "RMp3.h"

#include <RDebug.h>

RMp3::DecoderDeleter RMp3::decoder;

RMp3::RMp3():
    RResource("UnnameMp3")
{

}

RMp3::RMp3(const std::string &path, const std::string &name):
    RResource(name)
{
    load(path);
}

RMp3::RMp3(const RMp3 &mp3):
    RResource(mp3),
    hz_(mp3.hz_),
    samples_(mp3.samples_),
    channel_(mp3.channel_),
    data_(mp3.data_)
{

}

RMp3::RMp3(const RMp3 &&mp3):
    RResource(std::move(mp3)),
    hz_(mp3.hz_),
    samples_(mp3.samples_),
    channel_(mp3.channel_),
    data_(mp3.data_)
{

}

RMp3 &RMp3::operator=(RMp3 mp3)
{
    swap(mp3);
    return *this;
}

RMp3::~RMp3()
{

}

void RMp3::swap(RMp3 &mp3)
{
    RResource::swap(mp3);
    using std::swap;
    swap(hz_, mp3.hz_);
    swap(samples_, mp3.samples_);
    swap(channel_, mp3.channel_);
    swap(data_, mp3.data_);
}

bool RMp3::isValid() const
{
    return data_ != nullptr;
}

unsigned RMp3::hz() const
{
    return hz_;
}

size_t RMp3::samples() const
{
    return samples_;
}

int RMp3::channel() const
{
    return channel_;
}

int16_t *RMp3::data()
{
    copyOnWrite();
    return data_.get();
}

const int16_t *RMp3::cdata() const
{
    return data_.get();
}

bool RMp3::load(std::string path)
{
    path = checkFilePath(path);
    if(path.empty())
        return false;

    if(!decoder)
    {
        decoder.reset(new RMp3Extern::mp3dec_t);
        mp3dec_init(decoder.get());
    }

    RMp3Extern::mp3dec_file_info_t info;
    if(printError(mp3dec_load(decoder.get(), path.c_str(), &info, nullptr, nullptr), "Failed to loac mp3:" + path + " in " + nameID()))
        return false;

    hz_ = static_cast<unsigned>(info.hz);
    samples_ = info.samples;
    channel_ = info.channels;
    data_.reset(info.buffer, free);

    return true;
}

void RMp3::freeMp3()
{
    data_.reset();
}

void RMp3::copyOnWrite()
{
    if(data_.unique() || data_ == nullptr) return;

    size_t size = samples_ * sizeof(RMp3Extern::mp3d_sample_t);
    RMp3Extern::mp3d_sample_t *source = data_.get();
    RMp3Extern::mp3d_sample_t *dest = static_cast<RMp3Extern::mp3d_sample_t*>(malloc(size));
    std::memcpy(dest, source, size);
    data_.reset(dest, free);
}

