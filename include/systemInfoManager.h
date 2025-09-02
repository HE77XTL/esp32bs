#ifndef SYSTEM_INFO_MANAGER_H
#define SYSTEM_INFO_MANAGER_H

#include <Arduino.h>
#include <SPIFFS.h>

class SystemInfoManager {
private:
    // 私有方法
    void displayBasicSystemInfo();
    void displayPSRAMInfo();
    void displaySPIFFSInfo();
    void displayMemoryInfo();
    
public:
    // 构造函数
    SystemInfoManager();
    
    // 公共接口
    void begin();
    void displayAllSystemInfo();
    bool hasPSRAM() const;
    size_t getTotalMemory() const;
    size_t getFreeMemory() const;
    size_t getPSRAMSize() const;
    size_t getFreePSRAM() const;
};

#endif // SYSTEM_INFO_MANAGER_H
