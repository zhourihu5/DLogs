//
// Created by Darren on 2020/4/19.
//

#include "TXFileLogger.h"

static const int DEFAULT_MMAP_TIMES = 4;
static const int DEFAULT_INCREASE_SIZE = getpagesize() * 128;

TXFileLogger::TXFileLogger(const char *logPath, long fileMaxSize) {
    this->logPath = string(logPath);
    this->fileMaxSize = fileMaxSize;
    //  mmap 映射文件，分两种情况，一种是没有内容，一种是有内容
    int fileSize = getFileSize();

    increaseSize = fileMaxSize / DEFAULT_MMAP_TIMES;
    if (increaseSize < DEFAULT_INCREASE_SIZE) {
        increaseSize = fileMaxSize;
    } else if (increaseSize % getpagesize() != 0) {
        increaseSize = (increaseSize / getpagesize() + 1) * getpagesize();
    }
    // 最大的映射值, 2M
    if (fileSize == 0) {
        // 没有内容，利用 mmap 映射
        mmapSize = increaseSize;
        mmapFile(0, mmapSize);
    } else {
        // 有内容
        mmapSize = fileSize;
        // 加个逻辑不要映射整个文件，需要计算映射空间
        mmapFile(0, fileSize);
        // 需要把位置读出来
        memcpy(&dataPosition, mmapPtr + (mmapSize - 4 - 1), 4);
    }
}

TXFileLogger::~TXFileLogger() {

}

/**
 * 获取当前文件的大小
 */
int TXFileLogger::getFileSize() {
    FILE *fp = fopen(logPath.c_str(), "r");
    if (fp == NULL) {
        return 0;
    }
    fseek(fp, 0L, SEEK_END);
    int size = ftell(fp);
    fclose(fp);
    return size;
}

/**
 * 本文件的某一部分映射到内存
 * @param start 开始位置
 * @param end 结束位置
 */
void TXFileLogger::mmapFile(int start, long end) {
    int logFd = open(logPath.c_str(), O_RDWR | O_CREAT, S_IRWXU);
    int size = end - start;
    // 扩大文件的大小
    ftruncate(logFd, size);
    mmapPtr = (char *) mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, logFd, start);
    close(logFd);
}

void TXFileLogger::writeData(jbyte *data, jint dataLen) {
    if (mmapSize < dataPosition + dataLen + 4) {
        // 需要扩容 ，分两种情况，快要超过大小了，把数据写入就可以了
        // 离超过大小还很远，没有超过大小
        if (mmapPtr) {
            munmap(mmapPtr, mmapSize);
        }
        mmapSize += increaseSize;
        // 0 - mmapSize , oldmmapSize - newmmapSize
        mmapFile(0, mmapSize);
    }
    // 拷贝数据到
    memcpy(mmapPtr + dataPosition, data, dataLen);
    dataPosition += dataLen;
    // 把数据大小写入文件的末尾
    memcpy(mmapPtr + (mmapSize - 4 - 1), &dataPosition, 4);
}
