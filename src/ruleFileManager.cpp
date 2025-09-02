#include "ruleFileManager.h"

// 构造函数
RuleFileManager::RuleFileManager() 
    : fileWritten(false), lastWriteTime(0), lastReadTime(0), READ_DELAY(60000),
      fileListShown(false), beginTime(0) {
}

// 初始化方法
void RuleFileManager::begin() {
    Serial.println("=== RuleFileManager 初始化 ===");
    beginTime = millis(); // 记录开始时间
    Serial.println("文件管理器初始化完成");
}

// 更新方法（在loop中调用）
void RuleFileManager::update() {
    unsigned long currentTime = millis();
    
    // 新增：10秒后列出所有文件（只执行一次）
    if (!fileListShown && (currentTime - beginTime >= LIST_DELAY)) {
        listAllFiles();
        fileListShown = true;
    }
    
    // 写入JavaScript文件（只在开始时执行一次）
    if (!fileWritten) {
        writeJavaScriptFile();
        fileWritten = true;
        lastWriteTime = currentTime;
        Serial.println("JavaScript文件写入完成，等待1分钟后读取...");
    }
    
    // 1分钟后读取JavaScript文件
    if (fileWritten && shouldReadFile()) {
        readJavaScriptFile();
        lastReadTime = currentTime;
    }
}

// 检查是否应该读取文件
bool RuleFileManager::shouldReadFile() const {
    unsigned long currentTime = millis();
    return (currentTime - lastWriteTime >= READ_DELAY) && 
           (currentTime - lastReadTime >= READ_DELAY);
}

// 检查文件是否已写入
bool RuleFileManager::isFileWritten() const {
    return fileWritten;
}

// 写入JavaScript文件（私有方法）
void RuleFileManager::writeJavaScriptFile() {
    Serial.println("\n=== 写入JavaScript文件 ===");
    
    // 打开文件进行写入
    File file = SPIFFS.open("/r_demo.js", "w");
    if (!file) {
        Serial.println("无法创建JavaScript文件!");
        return;
    }
    
    // 写入JavaScript代码内容
    const char* jsCode = R"(
function fileTest(){
    console.log("hello word")
    console.log(111)
}
)";
    
    if (file.print(jsCode)) {
        Serial.println("JavaScript代码写入成功!");
        Serial.println("写入内容:");
        Serial.println(jsCode);
        Serial.printf("文件大小: %u bytes\n", file.size());
    } else {
        Serial.println("JavaScript代码写入失败!");
    }
    
    // 关闭文件
    file.close();
    Serial.println("=== JavaScript文件写入完成 ===\n");
}

// 读取JavaScript文件（私有方法）
void RuleFileManager::readJavaScriptFile() {
    Serial.println("\n=== 读取JavaScript文件内容 ===");
    
    // 检查文件是否存在
    if (!SPIFFS.exists("/r_demo.js")) {
        Serial.println("JavaScript文件不存在!");
        return;
    }
    
    // 打开文件进行读取
    File file = SPIFFS.open("/r_demo.js", "r");
    if (!file) {
        Serial.println("无法打开JavaScript文件!");
        return;
    }
    
    // 显示文件信息
    Serial.printf("文件大小: %u bytes\n", file.size());
    Serial.println("JavaScript代码内容:");
    Serial.println("----------------------------------------");
    
    // 读取并显示JavaScript代码
    while (file.available()) {
        String line = file.readStringUntil('\n');
        Serial.println(line);
    }
    
    Serial.println("----------------------------------------");
    
    // 关闭文件
    file.close();
    Serial.println("=== JavaScript文件读取完成 ===\n");
}

// 列出所有文件（私有方法）
void RuleFileManager::listAllFiles() {
    Serial.println("\n=== 列出SPIFFS中的所有文件 ===");
    
    // 检查SPIFFS是否可用
    if (!SPIFFS.begin()) {
        Serial.println("SPIFFS不可用!");
        return;
    }
    
    // 打开根目录
    File root = SPIFFS.open("/");
    if (!root) {
        Serial.println("无法打开根目录!");
        return;
    }
    
    if (!root.isDirectory()) {
        Serial.println("根目录不是一个目录!");
        root.close();
        return;
    }
    
    // 遍历所有文件
    File file = root.openNextFile();
    bool hasFiles = false;
    
    Serial.println("文件列表:");
    Serial.println("----------------------------------------");
    
    while (file) {
        if (!file.isDirectory()) {
            hasFiles = true;
            Serial.printf("文件名: %s, 大小: %u bytes\n", file.name(), file.size());
        }
        file = root.openNextFile();
    }
    
    if (!hasFiles) {
        Serial.println("无");
    }
    
    Serial.println("----------------------------------------");
    
    // 关闭根目录
    root.close();
    Serial.println("=== 文件列表显示完成 ===\n");
}
