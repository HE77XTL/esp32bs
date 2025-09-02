#include "systemInfoManager.h"
#include "esp_heap_caps.h"

// 构造函数
SystemInfoManager::SystemInfoManager() {
}

// 初始化方法
void SystemInfoManager::begin() {
    Serial.println("=== SystemInfoManager 初始化 ===");
    
    // 初始化SPIFFS文件系统
    if (!SPIFFS.begin(true)) {
        Serial.println("SPIFFS初始化失败!");
        return;
    }
    Serial.println("SPIFFS初始化成功!");
    
    Serial.println("=== SystemInfoManager 初始化完成 ===");
}

// 显示所有系统信息
void SystemInfoManager::displayAllSystemInfo() {
    Serial.println("=== 系统信息总览 ===");
    
    displayBasicSystemInfo();
    displayMemoryInfo();
    displayPSRAMInfo();
    displaySPIFFSInfo();
    
    Serial.println("=== 系统信息显示完成 ===");
}

// 显示基本系统信息
void SystemInfoManager::displayBasicSystemInfo() {
    Serial.println("--- 基本系统信息 ---");
    Serial.printf("芯片型号: %s\n", ESP.getChipModel());
    Serial.printf("芯片版本: %d\n", ESP.getChipRevision());
    Serial.printf("芯片核心数: %d\n", ESP.getChipCores());
    Serial.printf("CPU频率: %u MHz\n", ESP.getCpuFreqMHz());
    Serial.printf("Flash大小: %u bytes (%.2f MB)\n", 
                 ESP.getFlashChipSize(), ESP.getFlashChipSize() / 1024.0 / 1024.0);
}

// 显示内存信息
void SystemInfoManager::displayMemoryInfo() {
    Serial.println("--- 内存信息 ---");
    Serial.printf("内置SRAM总容量: %u bytes (%.2f KB)\n", 
                 ESP.getHeapSize(), ESP.getHeapSize() / 1024.0);
    Serial.printf("内置SRAM可用: %u bytes (%.2f KB)\n", 
                 ESP.getFreeHeap(), ESP.getFreeHeap() / 1024.0);
    Serial.printf("内置SRAM使用率: %.1f%%\n", 
                 (1.0 - (float)ESP.getFreeHeap() / ESP.getHeapSize()) * 100);
}

// 显示PSRAM信息
void SystemInfoManager::displayPSRAMInfo() {
    Serial.println("--- PSRAM信息 ---");
    
    // 检查编译时配置
    #ifdef BOARD_HAS_PSRAM
        Serial.println("编译时已启用PSRAM支持");
    #else
        Serial.println("编译时未启用PSRAM支持");
    #endif
    
    if (psramFound()) {
        Serial.println("PSRAM 状态：已检测到并初始化成功");
        
        // 读取 PSRAM 总容量
        size_t psram_total = heap_caps_get_total_size(MALLOC_CAP_SPIRAM);
        Serial.printf("PSRAM 总容量：%d 字节 = %.2f KB = %.2f MB\n",
               psram_total,
               (float)psram_total / 1024,
               (float)psram_total / (1024 * 1024));
        
        // 读取 PSRAM 当前可用容量
        size_t psram_free = heap_caps_get_free_size(MALLOC_CAP_SPIRAM);
        Serial.printf("PSRAM 可用容量：%d 字节 = %.2f KB = %.2f MB\n",
               psram_free,
               (float)psram_free / 1024,
               (float)psram_free / (1024 * 1024));
        
        // 验证 PSRAM 内存分配
        const size_t alloc_size = 10 * 1024; // 10KB
        void *psram_buf = heap_caps_malloc(alloc_size, MALLOC_CAP_SPIRAM);
        
        if (psram_buf != NULL) {
            Serial.printf("PSRAM 内存分配测试：成功分配 %d 字节\n", alloc_size);
            
            // 分配后再次读取可用容量
            size_t psram_free_after = heap_caps_get_free_size(MALLOC_CAP_SPIRAM);
            Serial.printf("分配后 PSRAM 可用容量：%.2f KB\n", (float)psram_free_after / 1024);
            
            // 释放分配的内存
            heap_caps_free(psram_buf);
            Serial.printf("PSRAM 内存释放：已释放 %d 字节\n", alloc_size);
        } else {
            Serial.println("PSRAM 内存分配测试：失败");
        }
    } else {
        Serial.println("PSRAM 状态：未检测到 PSRAM");
        Serial.println("原因分析：");
        Serial.println("1. 此板子型号为 ESP32-S3-DevKitC-1-N8");
        Serial.println("2. N8 表示 No PSRAM（无PSRAM）");
        Serial.println("3. 如需PSRAM，请使用 R8 版本板子");
        Serial.println("4. 当前使用内置SRAM：320KB");
    }
}

// 显示SPIFFS信息
void SystemInfoManager::displaySPIFFSInfo() {
    Serial.println("--- SPIFFS文件系统信息 ---");
    
    size_t totalBytes = SPIFFS.totalBytes();
    size_t usedBytes = SPIFFS.usedBytes();
    Serial.printf("SPIFFS总空间: %u bytes (%.2f MB)\n", totalBytes, totalBytes / 1024.0 / 1024.0);
    Serial.printf("已用空间: %u bytes (%.2f KB)\n", usedBytes, usedBytes / 1024.0);
    Serial.printf("可用空间: %u bytes (%.2f KB)\n", totalBytes - usedBytes, (totalBytes - usedBytes) / 1024.0);
    Serial.printf("使用率: %.1f%%\n", (float)usedBytes / totalBytes * 100);
}

// 公共接口方法
bool SystemInfoManager::hasPSRAM() const {
    return psramFound();
}

size_t SystemInfoManager::getTotalMemory() const {
    return ESP.getHeapSize();
}

size_t SystemInfoManager::getFreeMemory() const {
    return ESP.getFreeHeap();
}

size_t SystemInfoManager::getPSRAMSize() const {
    if (psramFound()) {
        return heap_caps_get_total_size(MALLOC_CAP_SPIRAM);
    }
    return 0;
}

size_t SystemInfoManager::getFreePSRAM() const {
    if (psramFound()) {
        return heap_caps_get_free_size(MALLOC_CAP_SPIRAM);
    }
    return 0;
}
