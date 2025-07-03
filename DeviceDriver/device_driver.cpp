#include "device_driver.h"
#include <stdexcept>
#include <vector>
#include <iostream>

class ReadFiveTimeFail : public std::exception {};
class ReadAddrFromStartToEnd : public std::exception {};

DeviceDriver::DeviceDriver(FlashMemoryDevice* hardware) : m_hardware(hardware)
{
}


int DeviceDriver::read(long address)
{
    int result = (int)m_hardware->read(address);

    for (int i = 1; i < 5; ++i)
    {
        if (m_hardware->read(address) != result)
        {
            throw ReadFiveTimeFail();
        }
    }
    return result;
}

void DeviceDriver::write(long address, int data)
{
    int result = (int)m_hardware->read(address);
    if (result != 0xFF) {
        throw ReadFiveTimeFail();
    }
    m_hardware->write(address, static_cast<unsigned char>(data));
}


