#include "gmock/gmock.h"
#include "device_driver.h"
#include "application.cpp"

using namespace testing;

// FlashMemoryDevice 인터페이스에 대한 Mock 클래스
class MockFlashMemoryDevice : public FlashMemoryDevice {
public:
    //flash_memory_device.h 를 mock 함수로 재정의
	MOCK_METHOD(unsigned char, read, (long address), (override));
	MOCK_METHOD(void, write, (long address, unsigned char data), (override));
    MOCK_METHOD(void, readAndPrinter, (long startAddress, long endAddress), (override));
};

// 테스트 케이스 1: Read가 5번 모두 같은 값을 반환하여 성공하는 시나리오
TEST(DeviceDriver, ReadFiveTimesAndReturnAllSame) {
    //1. Mocking Class 객체를 하나 만들어주고
    MockFlashMemoryDevice mock_hardware;
    //2. 그 Mock 객체로 DeviceDriver 객체를 초기화
    DeviceDriver driver{ &mock_hardware };

    //3. address 임의 설정해주고
    long address = 0x100;
    //4. expected value 임의 설정 해주고
    unsigned char expected_value = 0xAB;

    //5. address를 read로 전달했을때 5번 모두 return 값이 expected_value가 되게 해줘 라고 미리 선언
    EXPECT_CALL(mock_hardware, read(address))
        .Times(5)
        .WillRepeatedly(Return(expected_value));

    //6. 실제 driver의 read 동작 확인 -> device_driver.cpp의 read -> m_hardware->read() -> mock_hardware->read
    int data = driver.read(address);

    //7. 반환된 값이 예상 값과 일치하는지 확인
    EXPECT_EQ(expected_value, data);
}

// 테스트 케이스 2: Read 도중 다른 값이 반환되어 실패하는 시나리오
TEST(DeviceDriver, ReadFiveTimesAndReturnForSameOneDiff) {
    //1. Mocking Class 객체를 하나 만들어주고
    MockFlashMemoryDevice mock_hardware;
    //2. 그 Mock 객체로 DeviceDriver 객체를 초기화
    DeviceDriver driver{ &mock_hardware };

    //3. address 임의 설정해주고
    long address = 0x200;
    //4. expected value 두개를 임의 설정 해주고
    unsigned char val1 = 0xAB;
    unsigned char val2 = 0xCD; // 다른 값

    //5. mock hardware을 호출했을때 read()의 결과는 내가 지정한 값으로 해줘 라고 설정
    EXPECT_CALL(mock_hardware, read(address))
        .WillOnce(Return(val1))
        .WillOnce(Return(val1))
        .WillOnce(Return(val1))
        .WillOnce(Return(val2)) // 4번째에 다른 값 반환
        .WillRepeatedly(Return(val1));

    //6.read 메소드 호출 시 ReadFailException이 발생하는지 확인
    EXPECT_THROW(driver.read(address), std::exception);
}

// 테스트 케이스 3: Write를 Fail 하는 시나리오
TEST(DeviceDriver, FailWriteToHardware) {
    //1. Mocking Class 객체를 하나 만들어주고
    MockFlashMemoryDevice mock_hardware;
    //2. 그 Mock 객체로 DeviceDriver 객체를 초기화
    DeviceDriver driver{ &mock_hardware };

    //3. address 임의 설정해주고
    long address = 0xAA;
    //4. write 할 data 임의 설정 해주고
    int data_to_write = 0x12;
    //5. read가 0x56을 반환하도록 설정
    EXPECT_CALL(mock_hardware, read(address))
        .WillOnce(Return(0x56));

    EXPECT_THROW(driver.write(address, data_to_write), std::exception);
}

// 테스트 케이스 4: Write를 Success 하는 시나리오
TEST(DeviceDriver, SuccessWriteToHardware) {
    //1. Mocking Class 객체를 하나 만들어주고
    MockFlashMemoryDevice mock_hardware;
    //2. 그 Mock 객체로 DeviceDriver 객체를 초기화
    DeviceDriver driver{ &mock_hardware };

    //3. address 임의 설정해주고
    long address = 0xAA;
    //4. write 할 data 임의 설정 해주고
    int data_to_write = 0x12;
    //5. read가 0x12을 반환하도록 설정
    EXPECT_CALL(mock_hardware, read(address))
        .WillOnce(Return(0xFF));
    //6. 그다음 write가 수행
    EXPECT_CALL(mock_hardware, write(address, static_cast<unsigned char>(data_to_write)))
        .Times(1);

    EXPECT_NO_THROW(driver.write(address, data_to_write), std::exception);
}

// 테스트 케이스 5: start address 부터 end address 까지 모두 읽는 시나리오
TEST(DeviceDriver, ReadAddrFromStartToEnd) {
    //1. Mocking Class 객체를 하나 만들어주고
    MockFlashMemoryDevice mock_hardware;
    //2. 그 Mock 객체로 DeviceDriver 객체를 초기화
    DeviceDriver driver{ &mock_hardware };
    //3. app 객체 초기화
    Application app{ &driver };

    //3. start/end address 임의 설정해주고
    long start_address = 0x100;
    long end_address = 0x104;
    //4. write 할 data 임의 설정 해주고
    int data_to_write = 0x12;
    //4. expected value 임의 설정 해주고
    unsigned char expected_value = 0xAB;

    //5. 
    EXPECT_CALL(mock_hardware, read(_))
        .Times(25)
        .WillRepeatedly(Return(0xFA));
    
    app.readAndPrint(start_address, end_address);
}

// 테스트 케이스 6: WriteAll
TEST(DeviceDriver, WriteAll) {
    //1. Mocking Class 객체를 하나 만들어주고
    MockFlashMemoryDevice mock_hardware;
    //2. 그 Mock 객체로 DeviceDriver 객체를 초기화
    DeviceDriver driver{ &mock_hardware };
    //3. app 객체 초기화
    Application app{ &driver };

    //3. start/end address 임의 설정해주고
    long start_address = 0x100;
    long end_address = 0x104;
    //4. write 할 data 임의 설정 해주고
    int data_to_write = 0x12;
    //4. expected value 임의 설정 해주고
    unsigned char expected_value = 0xAB;

    //5. 
    EXPECT_CALL(mock_hardware, read(_))
        .Times(5)
        .WillRepeatedly(Return(0xFF));

    app.writeAll(0xA7l);
}



int main() {
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}