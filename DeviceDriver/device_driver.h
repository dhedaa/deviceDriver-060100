#pragma once

// �ϵ���� ��ġ �������̽�
class FlashMemoryDevice {
public:
    virtual ~FlashMemoryDevice() = default;
    virtual unsigned char read(long address) = 0;
    virtual void write(long address, unsigned char data) = 0;
    virtual void readAndPrinter(long startAddress, long endAddress) = 0;
};

// ����̽� ����̹� Ŭ����
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
