#pragma once

// 하드웨어 장치 인터페이스
class FlashMemoryDevice {
public:
    virtual ~FlashMemoryDevice() = default;
    virtual unsigned char read(long address) = 0;
    virtual void write(long address, unsigned char data) = 0;
    virtual void readAndPrinter(long startAddress, long endAddress) = 0;
};

// 디바이스 드라이버 클래스
class DeviceDriver
{
public:
    explicit DeviceDriver(FlashMemoryDevice* hardware);
    int read(long address);
    void write(long address, int data);
    void readAndPrinter(long startAddress, long endAddress);

protected:
    FlashMemoryDevice* m_hardware;
};
