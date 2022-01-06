
#ifndef ASYNC_FILE_H
#define ASYNC_FILE_H


#include <filesystem>
#include <map>
#include <cstring>
#include <fstream>
#include <sstream>
#include <iostream>
#include <future>
#include <string_view>
#include <uwebsockets/HttpResponse.h>
#include <spdlog/spdlog.h>

#include "singleton.hpp"


class AsyncFile
{
private:
    // 缓存
    std::string cache;
    // 缓存处对于文件起始的偏移
    int cacheOffset;
    // 是否已经缓存
    bool hasCache;
    // 文件的大小
    int fileSize;
    // 文件的名字
    std::string fileName;
public:
    AsyncFile(std::string fileName) : fileName(fileName) {
        cache.resize(1024 * 1024);
    }

    /* 获取已经缓存的数据 */
    std::string_view peek_data_already_cached(int offset) {

        /* 判断请求是否在缓存中 */
        if (hasCache && offset >= cacheOffset && ((offset - cacheOffset) < cache.length())) {

            int chunkSize = std::min<int>(fileSize - offset, cache.length() - offset + cacheOffset);

            return std::string_view(cache.data() + offset - cacheOffset, chunkSize);
        }
        else {
            //请求的文件超出缓存
            spdlog::debug("The requested file is out of cache");
            return std::string_view(nullptr, 0);
        }
    }

    /* 异步请求偏移量处的更多数据 */
    void async_request(int offset, std::function<void(std::string_view)> cb) {

    }


    int getFileSize() {
        return fileSize;
    }


};


class AsyncFileManager : public TSingleton<AsyncFileManager>
{
private:
    std::map<std::string_view, AsyncFile*> asyncFiles;
    std::string root_path;
public:

    template <bool SSL>
    static void streamFile(uWS::HttpResponse<SSL>* res, AsyncFile* asyncFileReader) {

        /* 获取缓存内容,这里并没有异步或io操作 */
        std::string_view chunk = asyncFileReader->peek_data_already_cached(res->getWriteOffset());

        // 如果chunk的长度不为0 且 写入成功
        if (!chunk.length() || res->tryEnd(chunk, asyncFileReader->getFileSize()).first) {
            
            //到这里证明一个 chunk 发送成功了


            // 如果chunk的长度小于文件的应有大小,也就是说需要发送更多数据
            if (chunk.length() < asyncFileReader->getFileSize()) {

                //启动异步请求
                asyncFileReader->async_request(
                    res->getWriteOffset(),
                    [res, asyncFileReader](std::string_view chunk) {
                        if (!chunk.length()) {
                            res->close();
                        }
                        else {
                            streamFile(res, asyncFileReader);
                        }
                    }
                );
            }
        }
        else {
            /* We failed writing everything, so let's continue when we can */
            res->onWritable([res, asyncFileReader](int offset) {

                // här kan skiten avbrytas!

                AsyncFileStreamer::streamFile(res, asyncFileReader);
                // todo: I don't really know what this is supposed to mean?
                return false;
                })->onAborted([]() {
                    std::cout << "ABORTED!" << std::endl;
                    }
                );
        }
    }



    template <bool SSL>
    void streamFile(uWS::HttpResponse<SSL>* res, std::string_view url) {
        auto it = asyncFiles.find(url);
        if (it == asyncFiles.end()) {
            spdlog::warn("The requested file could not be found:{:s}", url);
        }
        else {
            streamFile(res, it->second);
        }
    }
};





struct AsyncFileReader {
private:
    // 缓存
    std::string cache;
    // 缓存处对于文件起始的偏移
    int cacheOffset;
    // 是否已经缓存
    bool hasCache;
    // 文件的大小
    int fileSize;
    // 文件的名字
    std::string fileName;

public:

    AsyncFileReader(std::string fileName) : fileName(fileName) {
        cache.resize(1024 * 1024);
    }

    /* 获取已经缓存的数据 */
    std::string_view peek_data_already_cached(int offset) {

        /* 判断请求是否在缓存中 */
        if (hasCache && offset >= cacheOffset && ((offset - cacheOffset) < cache.length())) {
            
            int chunkSize = std::min<int>(fileSize - offset, cache.length() - offset + cacheOffset);

            return std::string_view(cache.data() + offset - cacheOffset, chunkSize);
        }
        else {
            //请求的文件超出缓存
            spdlog::debug("The requested file is out of cache");
            return std::string_view(nullptr, 0);
        }
    }

    /* Asynchronously request more data at offset */
    void async_request(int offset, std::function<void(std::string_view)> cb) {

        // in this case, what do we do?
        // we need to queue up this chunk request and callback!
        // if queue is full, either block or close the connection via abort!
        if (!hasCache) {
            // already requesting a chunk!
            std::cout << "ERROR: already requesting a chunk!" << std::endl;
            return;
        }

        // disable cache
        hasCache = false;

        
    }

    /* Abort any pending async. request */
    void abort() {

    }

    int getFileSize() {
        return fileSize;
    }
};









#endif // !ASYNC_FILE_H