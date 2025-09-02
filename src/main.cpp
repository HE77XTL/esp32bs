#include <Arduino.h>
#include "ruleFileManager.h"
#include "systemInfoManager.h"

// 创建管理器实例
SystemInfoManager systemInfo;
RuleFileManager fileManager;

void setup() {
  // 初始化串口通信
  Serial.begin(115200);
  
  // 等待串口稳定
  delay(1000);
  
  Serial.println("=== ESP32 JavaScript文件系统测试 ===");
  Serial.println("系统初始化开始...");
  
  // 初始化系统信息管理器（包含SPIFFS初始化）
  systemInfo.begin();
  
  // 显示所有系统信息
  systemInfo.displayAllSystemInfo();
  
  // 初始化文件管理器
  fileManager.begin();
  
  Serial.println("=== Setup完成 ===");
}

void loop() {
  // 添加心跳信息
  static unsigned long lastHeartbeat = 0;
  unsigned long currentTime = millis();
  
  // 每5秒输出一次心跳
  if (currentTime - lastHeartbeat >= 5000) {
    Serial.printf("心跳: %lu ms\n", currentTime);
    
    // 显示内存状态
    Serial.printf("内置SRAM可用: %.2f KB\n", systemInfo.getFreeMemory() / 1024.0);
    if (systemInfo.hasPSRAM()) {
      Serial.printf("PSRAM可用: %.2f KB\n", systemInfo.getFreePSRAM() / 1024.0);
    }
    
    lastHeartbeat = currentTime;
  }
  
  // 更新文件管理器状态
  fileManager.update();
  
  delay(1000); // 每秒检查一次
}