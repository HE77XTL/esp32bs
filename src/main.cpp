#include <Arduino.h>
#include <SPIFFS.h>

// 函数声明
void writeJavaScriptFile();
void readJavaScriptFile();

// JavaScript代码内容（从examples/r_demo.js读取）
const char* jsCode = R"(
function fileTest(){
    console.log("hello word")
    console.log(111)
}
)";

// 文件操作相关变量
unsigned long lastWriteTime = 0;
unsigned long lastReadTime = 0;
const unsigned long READ_DELAY = 60000; // 1分钟 = 60000毫秒
bool fileWritten = false;

void setup() {
  // 初始化串口通信
  Serial.begin(115200);
  
  // 等待串口稳定
  delay(1000);
  
  Serial.println("=== ESP32 JavaScript文件系统测试 ===");
  Serial.println("文件系统测试开始...");
  
  // 初始化SPIFFS文件系统
  if (!SPIFFS.begin(true)) {
    Serial.println("SPIFFS初始化失败!");
    return;
  }
  Serial.println("SPIFFS初始化成功!");
  
  // 显示文件系统信息
  size_t totalBytes = SPIFFS.totalBytes();
  size_t usedBytes = SPIFFS.usedBytes();
  Serial.printf("总空间: %u bytes\n", totalBytes);
  Serial.printf("已用空间: %u bytes\n", usedBytes);
  Serial.printf("可用空间: %u bytes\n", totalBytes - usedBytes);
  
  Serial.println("=== Setup完成 ===");
}

void loop() {
  // 添加心跳信息
  static unsigned long lastHeartbeat = 0;
  unsigned long currentTime = millis();
  
  // 每5秒输出一次心跳
  if (currentTime - lastHeartbeat >= 5000) {
    Serial.printf("心跳: %lu ms\n", currentTime);
    lastHeartbeat = currentTime;
  }
  
  // 写入JavaScript文件（只在开始时执行一次）
  if (!fileWritten) {
    writeJavaScriptFile();
    fileWritten = true;
    lastWriteTime = currentTime;
    Serial.println("JavaScript文件写入完成，等待1分钟后读取...");
  }
  
  // 1分钟后读取JavaScript文件
  if (fileWritten && (currentTime - lastWriteTime >= READ_DELAY)) {
    if (currentTime - lastReadTime >= READ_DELAY) {
      readJavaScriptFile();
      lastReadTime = currentTime;
    }
  }
  
  delay(1000); // 每秒检查一次
}

// 写入JavaScript文件函数
void writeJavaScriptFile() {
  Serial.println("\n=== 写入JavaScript文件 ===");
  
  // 打开文件进行写入
  File file = SPIFFS.open("/r_demo.js", "w");
  if (!file) {
    Serial.println("无法创建JavaScript文件!");
    return;
  }
  
  // 写入JavaScript代码
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

// 读取JavaScript文件函数
void readJavaScriptFile() {
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