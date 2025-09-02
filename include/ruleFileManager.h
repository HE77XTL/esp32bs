#ifndef RULE_FILE_MANAGER_H
#define RULE_FILE_MANAGER_H

#include <Arduino.h>
#include <SPIFFS.h>

class RuleFileManager {
private:
    // 私有成员变量
    bool fileWritten;
    unsigned long lastWriteTime;
    unsigned long lastReadTime;
    const unsigned long READ_DELAY;
    
    // 新增：文件列表功能相关变量
    bool fileListShown;
    unsigned long beginTime;
    const unsigned long LIST_DELAY = 10000; // 10秒延迟
    
    // 私有方法
    void writeJavaScriptFile();
    void readJavaScriptFile();
    void listAllFiles(); // 新增：列出所有文件的方法

public:
    // 构造函数
    RuleFileManager();
    
    // 公共接口
    void begin();
    void update();
    bool shouldReadFile() const;
    bool isFileWritten() const;
};

#endif // RULE_FILE_MANAGER_H
