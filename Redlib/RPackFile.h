#ifndef RPACKFILE_H
#define RPACKFILE_H

#include <string>
#include <map>
#include <memory>
#include <unordered_map>

using RByte = unsigned char;

class RPackFile
{
    //Rpack文件头数据
    struct PackFileHead
    {
        uint32_t headformat;    //文件头校验码
        uint32_t fileNum;       //包内文件数量
    };

    //包内各文件数据
    struct PackInfo
    {
        uint64_t nameHash;  //文件名的哈希值
        uint64_t size;      //文件大小
        uint64_t check;     //文件校验码
    };

    //加载后储存各文件数据
    struct FileInfo
    {
        size_t size = 0;
        size_t check = 0;
        std::unique_ptr<RByte[]> data;
    };

public:
    static uint64_t generateCheckCode(const RByte *buffer, size_t size);

    RPackFile();
    RPackFile(const std::string &path);

    bool isEmpty() const { return fileInfo_.size() == 0; }
    const FileInfo* getFileInfo(const std::string &file);

    //指针版本会发生拷贝，智能指针版本不会
    void packing(const RByte *buffer, size_t size, const std::string &sourceName);
    void packing(std::unique_ptr<RByte[]> &&buffer, size_t size, const std::string &sourceName);
    bool packing(const std::string &sourcePath);
    bool save();
    bool fileCheck(const RPackFile &pack);
    bool fileCheck(const std::string &file);

private:
    //文件头格式校验码 1110 1100 1000 0000 1000 1100 1110 0000
    static constexpr uint32_t headformat = 0xec808ce0;

    std::string path_;
    std::unordered_map<uint64_t, FileInfo> fileInfo_;
};

#endif // RPACKFILE_H
