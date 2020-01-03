#include "RPackFile.h"
#include "RDebug.h"
#include "RThread.h"

#include <fstream>

uint64_t RPackFile::generateCheckCode(const RByte *buffer, size_t size)
{
    static const int concurrency = std::thread::hardware_concurrency() > 0 ? std::thread::hardware_concurrency(): 1;
    size_t num = size / 64;
    const uint64_t *p = reinterpret_cast<const uint64_t*>(buffer);
    int count = 1;
    if(num > 1000) count = concurrency;

    std::vector<uint64_t> check(count, 0);
    size_t range = num / count;

    { //线程开始
    std::vector<RThread> threads(count);
    for(int i = 1; i < count; ++i)
    {
        threads[i] = RThread([&check, i, range](const uint64_t *p)
        {
            for(size_t j = 0; j < range; ++j)
                check[i] ^= *p++;
        },
        p+range*i);
    }

    for(size_t j = 0; j < range; ++j)
        check[0] ^= *p++;
    } //线程结束

    uint64_t result = 0;
    for(int i = 0; i < count; ++i)
        result ^= check[i];

    return result;
}

RPackFile::RPackFile():
    path_(),
    fileInfo_()
{

}

RPackFile::RPackFile(const std::string &path):
    RPackFile()
{
    std::ifstream file(path, std::ios::binary);
    if(!file)
    {
#ifdef R_DEBUG
        RDebug() << "Pack file done not exist: " + path;
#endif
        path_ = path;
        return;
    }

    std::unique_ptr<PackInfo[]> info;
    PackFileHead head;
    //若状态被置为failbit或badbit，则抛出异常
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        file.read(reinterpret_cast<char*>(&head), sizeof(PackFileHead));
        //无论是否压缩，检测文件头是否有效
        if(head.headformat == headformat)
        {
            info = std::unique_ptr<PackInfo[]>(new PackInfo [head.fileNum]);
            file.read(reinterpret_cast<char*>(info.get()), sizeof(PackInfo) * head.fileNum);
            for(size_t i = 0; i < head.fileNum; ++i)
            {
                RByte *data = new RByte [info[i].size];
                file.read(reinterpret_cast<char*>(data), info[i].size);
                fileInfo_.emplace(info[i].nameHash, FileInfo{ info[i].size, info[i].check, std::unique_ptr<RByte[]>(data) });
            }
        }
        else return;

        file.close();
    }
    catch(std::ifstream::failure &e)
    {
        file.close();
        printError("Error reading pack file in " + path);
    }

    path_ = path;
}

const RPackFile::FileInfo *RPackFile::getFileInfo(const std::string &file)
{
    uint64_t hash = std::hash<std::string>()(file);
    auto it = fileInfo_.find(hash);
    if(it != fileInfo_.end())
        return &it->second;

    return nullptr;
}

void RPackFile::packing(const RByte *buffer, size_t size, const std::string &sourceName)
{
    RByte *p = new RByte [size];
    for(size_t i = 0; i < size; ++i)
        p[i] = buffer[i];
    packing(std::unique_ptr<RByte[]>(p), size, sourceName);
}

void RPackFile::packing(std::unique_ptr<RByte[]> &&buffer, size_t size, const std::string &sourceName)
{
    uint64_t hash = std::hash<std::string>()(sourceName);
    size_t check = generateCheckCode(buffer.get(), size);

    auto it = fileInfo_.find(hash);
    if(it != fileInfo_.end())
    {
        if(check != it->second.check)
            it->second = { size, check, std::move(buffer) };
    } else
    {
        fileInfo_.emplace(hash, FileInfo{ size, check, std::move(buffer) });
    }
}

bool RPackFile::packing(const std::string &sourcePath)
{
    std::ifstream file(sourcePath, std::ios::binary | std::ios::ate);
    if(!file)
    {
#ifdef R_DEBUG
        printError("File done not exist: " + sourcePath);
#endif
        return false;
    }
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);

    RByte *data = new RByte [size];
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        file.read(reinterpret_cast<char*>(data), size);
        file.close();
    }
    catch(std::ifstream::failure &f)
    {
        file.close();
        printError("Error reading file in " + sourcePath);
        return false;
    }

    packing(std::unique_ptr<RByte[]>(data), size, sourcePath);
    return true;
}

bool RPackFile::save()
{
    std::ofstream file(path_, std::ios::binary);
    if(printError(!file, "Failure save pack file <" + path_ + "> in opening!"))
        return false;

    file.exceptions(std::ofstream::failbit | std::ofstream::badbit);
    try {
        //写入文件头
        PackFileHead head { headformat,
                    static_cast<uint32_t>(fileInfo_.size()) };
        file.write(reinterpret_cast<char*>(&head), sizeof(PackFileHead));
        for(auto &info : fileInfo_)
        {
            PackInfo pkgInfo { info.first, info.second.size, info.second.check };
            file.write(reinterpret_cast<char*>(&pkgInfo), sizeof(PackInfo));
        }
        //写入数据部分
        for(auto &info : fileInfo_)
        {
            char *buf = reinterpret_cast<char*>(info.second.data.get());
            size_t size = info.second.size;
            file.write(buf, size);
        }

        file.close();
    }
    catch(std::ofstream::failure &f)
    {
        file.close();
        printError("Failure save pack file <" + path_ + "> in reading!");
        return false;
    }

    RDebug() << "Save pack file as <" + path_ + '>';
    return true;
}

bool RPackFile::fileCheck(const RPackFile &pack)
{
    if(fileInfo_.size() != pack.fileInfo_.size())
        return false;

    for(auto & [name, info] : fileInfo_)
    {
        auto it = pack.fileInfo_.find(name);
        if(it != pack.fileInfo_.end())
        {
            if(it->second.check != info.check)
                return false;
        }
        else
        {
            return false;
        }
    }

    return true;
}

bool RPackFile::fileCheck(const std::string &file)
{
    return fileCheck(RPackFile(file));
}
