//
// Created by Darren on 2020/4/19.
//

#ifndef DLOGS_TXFILELOGGER_H
#define DLOGS_TXFILELOGGER_H

#include <string>
#include <unistd.h>
#include <malloc.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <jni.h>
using namespace std;

class TXFileLogger {
public:
    TXFileLogger(const char* logPath, long fileMaxSize);
    ~TXFileLogger();

    void writeData(jbyte *data, jint dataLen);

private:
    string logPath;
    long fileMaxSize;
    /**
     * 每次映射的大小
     */
    long increaseSize;
    /**
     * 映射大小
     */
    long mmapSize;
    /**
     * 映射的首地址
     */
    char* mmapPtr;
    /**
     * 当前写入的位置
     */
    long dataPosition = 0;

    int getFileSize();

    void mmapFile(int start, long end);
};


#endif //DLOGS_TXFILELOGGER_H
