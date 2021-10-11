package com.tencent.logs;

import java.io.File;

/**
 * C++ 尽量写简单,定制化操作留给 java 层来做
 * 同步锁，文件目录创建
 */
public class FileLogger {
    private long mNativePtr;

    public FileLogger(File logFile, long fileMaxSize) {
        mNativePtr = nativeCreate(logFile.getAbsolutePath(), fileMaxSize);
    }

    /**
     * 在 c++ 层创建一个 FileLogger 对象
     */
    private native long nativeCreate(String absolutePath, long fileMaxSize);

    public void write(String text) {
        byte[] data = text.getBytes();
        nWrite(mNativePtr, data, data.length);
    }

    private native void nWrite(long mNativePtr, byte[] data, int dataLen);
}
