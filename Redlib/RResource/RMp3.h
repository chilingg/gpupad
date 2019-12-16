#ifndef RMP3_H
#define RMP3_H

#include "RResource.h"
#include <memory>

namespace RMp3Extern {
#include "minimp3.h"
#include "minimp3_ex.h"
}

class RMp3 : public RResource
{
    using DecoderDeleter = std::unique_ptr<RMp3Extern::mp3dec_t>;
    friend void swap(RMp3 &img1, RMp3 &img2);
public:
    RMp3();
    RMp3(const std::string &path, const std::string &name);
    RMp3(const RMp3 &mp3);
    RMp3(const RMp3 &&mp3);
    RMp3& operator=(RMp3 mp3);
    ~RMp3();
    void swap(RMp3 &mp3);

    bool isValid() const;
    unsigned hz() const;
    size_t samples() const;
    int channel() const;
    int16_t* data();
    const int16_t* cdata() const;

    bool load(std::string path);
    void freeMp3();

protected:
    void copyOnWrite();

private:
    static DecoderDeleter decoder;

    unsigned hz_;
    size_t samples_;
    int channel_;
    std::shared_ptr<RMp3Extern::mp3d_sample_t> data_;
};

void swap(RMp3 &img1, RMp3 &img2);

#endif // RMP3_H
