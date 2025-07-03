#include <iostream>
#include "device_driver.h"

class Application {
public:
	Application(DeviceDriver* dd) : dd{ dd } {}

    void readAndPrint(long startAddr, long endAddr) {
        for (long addr = startAddr; addr <= endAddr; addr++) {
            int ret = dd->read(addr);
            std::cout << ret << "\n";
        }
    }
    void writeAll(int value) {
        for (long addr = 0x00; addr <= 0x04; addr++) {
            dd->write(addr, value);
        }
    }
private:
    DeviceDriver* dd;
};