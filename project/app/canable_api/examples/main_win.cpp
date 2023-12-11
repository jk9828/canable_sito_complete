/*
#include "can_api.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <thread>
#include <chrono>
#include <atomic>

#ifndef PER
#define GREAT 1
#define PER 
GREAT
#endif

std::atomic<bool> stopCan = false;

std::stringstream strstr_rd;

void canReadProc(CanInterface* device)
{
    int i = 0;
    while (!stopCan)
    {
        CanFrame frame = device->readCanData(2);
        if (frame.can_dlc != 0)
            //std::cout << frame;
            strstr_rd << std::dec << i++ << " " << frame;
    }
}

#if 0 //for eyoubot planatery motor

int main()
{

    // for windows you can specify the baudrate to the NewCanDevice function

    std::stringstream strstr;

    CanInterface* device = NewCanDevice();
    device->Open(0, 1000000);

    // start the read thread
    std::thread readProc(canReadProc, device);

    std::cout << "Writing CAN data.\n";
    uint8_t data[8];
    int param32 = 0;
    uint8_t motor_enable[6] = { 0x01, 0x10, 0x00, 0x00, 0x00, 0x01 };
    uint8_t motor_disable[6] = { 0x01, 0x10, 0x00, 0x00, 0x00, 0x00 };
    uint8_t mode_speed[6] = { 0x01, 0x0F, 0x00, 0x00, 0x00, 0x03 };

    // motor open
    for (uint32_t j = 1; j <= 2; j++)
    {
        uint8_t id = j & 0xFF;
        // set speed mode
        if (device->writeCanData(id, 6, motor_enable) < 0)
        {
            std::cerr << "Error writing to CAN bus.\n";
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        if (device->writeCanData(id, 6, mode_speed) < 0)
        {
            std::cerr << "Error writing to CAN bus.\n";
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    getchar();

    //// move to zero position
    //for (uint32_t j = 11; j <= 14; j++)
    //{
    //    uint8_t id = j & 0xFF;
    //    if (device->writeCanData(id, 8, data) < 0)
    //    {
    //        std::cerr << "Error writing to CAN bus.\n";
    //    }
    //    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    //}

    //getchar();

    // speed up
    param32 = 0;

    for (int i = 0; i < 1000; i++)
    {
        // 10 rpm = 10 * 65536 / 60;

        param32 += 100;

        for (int j = 1; j <= 2; j++)
        {
            data[0] = 0x01;
            data[1] = 0x09; // speed
            data[2] = (param32 >> 24) & 0xFF;
            data[3] = (param32 >> 16) & 0xFF;
            data[4] = (param32 >> 8) & 0xFF;
            data[5] = param32 & 0xFF;

            uint8_t id = j & 0xFF;

            if (device->writeCanData(id, 6, data) < 0)
            {
                std::cerr << "Error writing to CAN bus.\n";
            }
            //std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        //std::cout << "Speed = " << std::dec << param32 << std::endl;
        strstr << "Speed = " << std::dec << param32 << std::endl;        
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(10000));

    // speed down
    for (int i = 0; i < 1000; i++)
    {
        param32 -= 100;

        for (int j = 1; j <= 2; j++)
        {
            data[0] = 0x01;
            data[1] = 0x09; // speed
            data[2] = (param32 >> 24) & 0xFF;
            data[3] = (param32 >> 16) & 0xFF;
            data[4] = (param32 >> 8) & 0xFF;
            data[5] = param32 & 0xFF;

            uint8_t id = j & 0xFF;

            if (device->writeCanData(id, 6, data) < 0)
            {
                std::cerr << "Error writing to CAN bus.\n";
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        //std::cout << "Speed = " << std::dec << param32 << std::endl;
        strstr << "Speed = " << std::dec << param32 << std::endl;        
    }

    std::cout << strstr.str();
    std::cout << strstr_rd.str();

    getchar();


    for (uint32_t j = 1; j <= 2; j++)
    {
        uint8_t id = j & 0xFF;
        if (device->writeCanData(id, 6, motor_disable) < 0)
        {
            std::cerr << "Error writing to CAN bus.\n";
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    stopCan = true;
    readProc.join();

    device->Close();
    delete device;
}

#endif

#if 1 // for sito harmonic drive
int main() 
{

    // for windows you can specify the baudrate to the NewCanDevice function
    
    std::stringstream strstr;

    CanInterface * device = NewCanDevice();
    device->Open(0, 1000000);

    // start the read thread
    std::thread readProc(canReadProc, device);

    std::cout << "Writing CAN data.\n";
    uint8_t data[8];
    int param32 = 0;
    int param16 = 500;
    data[0] = 0xFF;
    data[1] = 0x82;
    data[2] = (param32 >> 24) & 0xFF;
    data[3] = (param32 >> 16) & 0xFF;
    data[4] = (param32 >> 8) & 0xFF;
    data[5] = param32 & 0xFF;
    data[6] = (param16 >> 8) & 0xFF;
    data[7] = param16 & 0xFF;
    uint8_t motor_open[6] = { 0xFF, 0x80, 0x00, 0x00, 0x00, 0x01};
    uint8_t motor_off[6] = { 0xFF, 0x80, 0x00, 0x00, 0x00, 0x00 };

    // motor open
    for (uint32_t j = 11; j <= 14; j++)
    {
        uint8_t id = j & 0xFF;
        if (device->writeCanData(id, 6, motor_open) < 0)
        {
            std::cerr << "Error writing to CAN bus.\n";
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    getchar();

    //// move to zero position
    //for (uint32_t j = 11; j <= 14; j++)
    //{
    //    uint8_t id = j & 0xFF;
    //    if (device->writeCanData(id, 8, data) < 0)
    //    {
    //        std::cerr << "Error writing to CAN bus.\n";
    //    }
    //    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    //}

    //getchar();

    // speed up
    for (int i = 0; i < 500; i++)
    {
        param32 += 10;
        param16 = 0;

        for (int j = 11; j <= 14; j++)
        {
            data[0] = 0xFF; // 255
            data[1] = 0x86; // speed mode
            data[2] = (param32 >> 24) & 0xFF; // 24 만틈 비트 이동 11111111 과 & 연산 -> 뒤 8자리 남음
            data[3] = (param32 >> 16) & 0xFF; // 16 만틈 비트 이동 11111111 과 & 연산 -> 중간 8자리 남음
            data[4] = (param32 >> 8) & 0xFF; // 8 만틈 비트 이동 11111111 과 & 연산 -> 앞 8자리 남음
            data[5] = param32 & 0xFF; // 11111111 과 & 연산 -> 맨앞 8자리 남음
            data[6] = (param16 >> 8) & 0xFF; // 11111111 과 & 연산 -> 뒤 8자리 남음
            data[7] = param16 & 0xFF; // 11111111 과 & 연산 -> 앞 8자리 남음

            uint8_t id = j & 0xFF;

            if (device->writeCanData(id, 6, data) < 0)
            {
                std::cerr << "Error writing to CAN bus.\n";
            }
            //std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }

        std::cout << "Speed = " << std::dec << param32 << std::endl;
        //strstr << "Speed = " << std::dec << param32 << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    // speed down
    for (int i = 0; i < 500; i++)
    {
        param32 -= 10;
        param16 = 0;

        for (int j = 11; j <= 14; j++)
        {
            data[0] = 0xFF;
            data[1] = 0x86; // speed
            data[2] = (param32 >> 24) & 0xFF;
            data[3] = (param32 >> 16) & 0xFF;
            data[4] = (param32 >> 8) & 0xFF;
            data[5] = param32 & 0xFF;
            data[6] = (param16 >> 8) & 0xFF;
            data[7] = param16 & 0xFF;

            uint8_t id = j & 0xFF;

            if (device->writeCanData(id, 6, data) < 0)
            {
                std::cerr << "Error writing to CAN bus.\n";
            }
        }

        std::cout << "Speed = " << std::dec << param32 << std::endl;
        //strstr << "Speed = " << std::dec << param32 << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    //std::cout << strstr.str();

    //std::cout << strstr_rd.str();

    getchar();

    
    for (uint32_t j = 11; j <= 14; j++)
    {
        uint8_t id = j & 0xFF;
        if (device->writeCanData(id, 6, motor_off) < 0)
        {
            std::cerr << "Error writing to CAN bus.\n";
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    stopCan = true;
    readProc.join();

    device->Close();
    delete device;
}
#endif

*/






#include "can_api.h"
#include "sito_function.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <thread>
#include <chrono>
#include <atomic>

#ifndef PER
#define GREAT 1
#define PER GREAT
#endif

std::atomic<bool> stopCan = false;

void canReadProc(CanInterface* device)
{
    while (!stopCan)
    {
        CanFrame frame = device->readCanData(10);
        if (frame.can_dlc != 0)
            std::cout << frame;
    }
}

#if 0 

int main() // for steadywin motor
{
    // for windows you can specify the baudrate to the NewCanDevice function
    CanInterface* device = NewCanDevice();
    device->Open(0, 500000);

    // start the read thread
    std::thread readProc(canReadProc, device);

    std::cout << "Writing CAN data.\n";
    uint8_t data[8];
    int position = 0;
    int duration = 500;
    data[0] = 0xFF;
    data[1] = (position >> 24) & 0xFF;;
    data[2] = (position >> 16) & 0xFF;
    data[3] = (position >> 8) & 0xFF;
    data[4] = position & 0xFF;
    data[5] = (duration >> 16) & 0xFF;
    data[6] = (duration >> 8) & 0xFF;
    data[7] = duration & 0xFF;

    uint8_t motor_reset[8] = { 0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    uint8_t motor_ack_fault[8] = { 0xB3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    uint8_t motor_start[8] = { 0x91, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    uint8_t motor_stop[8] = { 0x92, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

    // ack fault
    if (device->writeCanData(0x01, 8, motor_ack_fault) < 0)
    {
        std::cerr << "Error writing to CAN bus.\n";
    }

    getchar();

    // start motor
    if (device->writeCanData(0x01, 8, motor_start) < 0)
    {
        std::cerr << "Error writing to CAN bus.\n";
    }

    getchar();

    // speed control
    data[0] = 0x94;
    data[1] = 0x00;
    data[2] = 0x00;
    data[3] = 0x4e;
    data[4] = 0x41;
    data[5] = 0x10;
    data[6] = 0x11;
    data[7] = 0x00;

    if (device->writeCanData(0x01, 8, data) < 0)
    {
        std::cerr << "Error writing to CAN bus.\n";
    }

    getchar();


    if (device->writeCanData(0x01, 8, motor_stop) < 0)
    {
        std::cerr << "Error writing to CAN bus.\n";
    }

    getchar();

    stopCan = true;
    readProc.join();

    device->Close();
    delete device;
}

#endif


#if 0 // for sito harmonic drive
int main()
{
    // for windows you can specify the baudrate to the NewCanDevice function


    std::stringstream strstr;


    CanInterface* device = NewCanDevice();
    device->Open(0, 1000000);

    // start the read thread
    std::thread readProc(canReadProc, device);

    std::cout << "Writing CAN data...\n";

    uint8_t data[8];

    uint8_t motor_open[6] = { 0xFF, 0x80, 0x00, 0x00, 0x00, 0x01 };
    uint8_t motor_off[6] = { 0xFF, 0x80, 0x00, 0x00, 0x00, 0x00 };

    int param32;
    int param16;

    // motor open
    for (uint32_t j = 10; j <= 14; j++)
    {
        uint8_t id = j & 0xFF;
        if (device->writeCanData(id, 6, motor_open) < 0)
        {
            std::cerr << "Error writing to CAN bus.\n";
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1));


    getchar();

    param32 = 1; // 
    param16 = 0; // 

    data[0] = 0xFF;
    data[1] = 0x89;
    data[2] = (param32 >> 24) & 0xFF;
    data[3] = (param32 >> 16) & 0xFF;
    data[4] = (param32 >> 8) & 0xFF;
    data[5] = param32 & 0xFF;
    data[6] = (param16 >> 8) & 0xFF;
    data[7] = param16 & 0xFF;
    
    // set motor origin
    for (uint32_t j = 10; j <= 14; j++)
    {

        uint8_t id = j & 0xFF;
        if (device->writeCanData(id, 8, data) < 0)
        {
            std::cerr << "Error writing to CAN bus.\n";
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    std::cout << "\nset motor origin current position...\n ";

    getchar();

    data[0] = 0xFF;
    data[1] = 0x95;
    data[2] = (param32 >> 24) & 0xFF;
    data[3] = (param32 >> 16) & 0xFF;
    data[4] = (param32 >> 8) & 0xFF;
    data[5] = param32 & 0xFF;
    data[6] = (param16 >> 8) & 0xFF;
    data[7] = param16 & 0xFF;


    // save setting
    for (uint32_t j = 10; j <= 14; j++)
    {

        uint8_t id = j & 0xFF;
        if (device->writeCanData(id, 8, data) < 0)
        {
            std::cerr << "Error writing to CAN bus.\n";
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    
    std::cout << "\nsetting saved\n.";

    param32 = 1000; // 
    param16 = 0; // 

    data[0] = 0xFF;
    data[1] = 0x8F;
    data[2] = (param32 >> 24) & 0xFF;
    data[3] = (param32 >> 16) & 0xFF;
    data[4] = (param32 >> 8) & 0xFF;
    data[5] = param32 & 0xFF;
    data[6] = (param16 >> 8) & 0xFF;
    data[7] = param16 & 0xFF;

    getchar();

    // set upper limit current
    for (uint32_t j = 10; j <= 14; j++)
    {

        uint8_t id = j & 0xFF;
        if (device->writeCanData(id, 8, data) < 0)
        {
            std::cerr << "Error writing to CAN bus.\n";
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    std::cout << "\n set current limit " << std::dec << param32 << "\n";

    getchar();


    param32 = 1; // 
    param16 = 0; // 

    data[0] = 0xFF;
    data[1] = 0x95;
    data[2] = (param32 >> 24) & 0xFF;
    data[3] = (param32 >> 16) & 0xFF;
    data[4] = (param32 >> 8) & 0xFF;
    data[5] = param32 & 0xFF;
    data[6] = (param16 >> 8) & 0xFF;
    data[7] = param16 & 0xFF;

    getchar();

    // save setting
    for (uint32_t j = 10; j <= 14; j++)
    {

        uint8_t id = j & 0xFF;
        if (device->writeCanData(id, 8, data) < 0)
        {
            std::cerr << "Error writing to CAN bus.\n";
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    std::cout << "setting saved ";

    getchar();
    getchar();


    param32 = 1000;
    param16 = 0; // 

    data[0] = 0xFF;
    data[1] = 0x86;
    data[2] = (param32 >> 24) & 0xFF;
    data[3] = (param32 >> 16) & 0xFF;
    data[4] = (param32 >> 8) & 0xFF;
    data[5] = param32 & 0xFF;
    data[6] = (param16 >> 8) & 0xFF;
    data[7] = param16 & 0xFF;


    // set motor speed control
    for (uint32_t j = 10; j <= 14; j++)
    {

        uint8_t id = j & 0xFF;
        if (device->writeCanData(id, 8, data) < 0)
        {
            std::cerr << "Error writing to CAN bus.\n";
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1));


    std::cout << "\nspeed control " << std::dec << param32/100 << " [rpm]\n";

    getchar();

    param32 = 0; // 
    param16 = 0; // 

    data[0] = 0xFF;
    data[1] = 0x04;
    data[2] = (param32 >> 24) & 0xFF;
    data[3] = (param32 >> 16) & 0xFF;
    data[4] = (param32 >> 8) & 0xFF;
    data[5] = param32 & 0xFF;
    data[6] = (param16 >> 8) & 0xFF;
    data[7] = param16 & 0xFF;

    getchar();

    // get motor speed
    for (uint32_t j = 10; j <= 14; j++)
    {

        uint8_t id = j & 0xFF;
        if (device->writeCanData(id, 2, data) < 0)
        {
            std::cerr << "Error writing to CAN bus.\n";
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    std::cout << "\nmotor speed\n";

    getchar();


    param32 = 1; // 
    param16 = 0; // 

    data[0] = 0xFF;
    data[1] = 0x87;
    data[2] = (param32 >> 24) & 0xFF;
    data[3] = (param32 >> 16) & 0xFF;
    data[4] = (param32 >> 8) & 0xFF;
    data[5] = param32 & 0xFF;
    data[6] = (param16 >> 8) & 0xFF;
    data[7] = param16 & 0xFF;

    // motor cease
    for (uint32_t j = 10; j <= 14; j++)
    {

        uint8_t id = j & 0xFF;
        if (device->writeCanData(id, 8, data) < 0)
        {
            std::cerr << "Error writing to CAN bus.\n";
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    std::cout << "\nmotor cease !!\n";

    getchar();


    param32 = 1000; // 
    param16 = 0; // 

    data[0] = 0xFF;
    data[1] = 0xA5;
    data[2] = (param32 >> 24) & 0xFF;
    data[3] = (param32 >> 16) & 0xFF;
    data[4] = (param32 >> 8) & 0xFF;
    data[5] = param32 & 0xFF;
    data[6] = (param16 >> 8) & 0xFF;
    data[7] = param16 & 0xFF;


    // set motor current control
    for (uint32_t j = 10; j <= 14; j++)
    {

        uint8_t id = j & 0xFF;
        if (device->writeCanData(id, 8, data) < 0)
        {
            std::cerr << "Error writing to CAN bus.\n";
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    std::cout << "\ncurrent control " << std::dec << param32 << "\n";

    getchar();

    param32 = 0; // 
    param16 = 0; // 

    data[0] = 0xFF;
    data[1] = 0x28;
    data[2] = (param32 >> 24) & 0xFF;
    data[3] = (param32 >> 16) & 0xFF;
    data[4] = (param32 >> 8) & 0xFF;
    data[5] = param32 & 0xFF;
    data[6] = (param16 >> 8) & 0xFF;
    data[7] = param16 & 0xFF;

    getchar();

    // get motor current
    for (uint32_t j = 10; j <= 14; j++)
    {

        uint8_t id = j & 0xFF;
        if (device->writeCanData(id, 2, data) < 0)
        {
            std::cerr << "Error writing to CAN bus.\n";
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    std::cout << "\nmotor current\n";

    getchar();


    param32 = 1; // 
    param16 = 0; // 

    data[0] = 0xFF;
    data[1] = 0x87;
    data[2] = (param32 >> 24) & 0xFF;
    data[3] = (param32 >> 16) & 0xFF;
    data[4] = (param32 >> 8) & 0xFF;
    data[5] = param32 & 0xFF;
    data[6] = (param16 >> 8) & 0xFF;
    data[7] = param16 & 0xFF;

    // motor cease
    for (uint32_t j = 10; j <= 14; j++)
    {

        uint8_t id = j & 0xFF;
        if (device->writeCanData(id, 8, data) < 0)
        {
            std::cerr << "Error writing to CAN bus.\n";
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    std::cout << "\nmotor cease !!\n";

    getchar();

    param32 = 0; // 
    param16 = 0; // 

    data[0] = 0xFF;
    data[1] = 0x03;
    data[2] = (param32 >> 24) & 0xFF;
    data[3] = (param32 >> 16) & 0xFF;
    data[4] = (param32 >> 8) & 0xFF;
    data[5] = param32 & 0xFF;
    data[6] = (param16 >> 8) & 0xFF;
    data[7] = param16 & 0xFF;

    getchar();

    // get motor position
    for (uint32_t j = 10; j <= 14; j++)
    {

        uint8_t id = j & 0xFF;
        if (device->writeCanData(id, 2, data) < 0)
        {
            std::cerr << "Error writing to CAN bus.\n";
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    std::cout << "\nmotor position\n";

    getchar();

    param32 = 0; // motor position
    param16 = 1000; // speed [rpm]

    data[0] = 0xFF;
    data[1] = 0x82;
    data[2] = (param32 >> 24) & 0xFF;
    data[3] = (param32 >> 16) & 0xFF;
    data[4] = (param32 >> 8) & 0xFF;
    data[5] = param32 & 0xFF;
    data[6] = (param16 >> 8) & 0xFF;
    data[7] = param16 & 0xFF;


    // run position
    for (uint32_t j = 10; j <= 14; j++)
    {

        uint8_t id = j & 0xFF;
        if (device->writeCanData(id, 8, data) < 0)
        {
            std::cerr << "Error writing to CAN bus.\n";
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    std::cout << "\n go to postion " << std::dec << param32 << " with " << std::dec << param16 << " [rpm] \n";

    getchar();


    param32 = 0; // 
    param16 = 0; // 

    data[0] = 0xFF;
    data[1] = 0x08;
    data[2] = (param32 >> 24) & 0xFF;
    data[3] = (param32 >> 16) & 0xFF;
    data[4] = (param32 >> 8) & 0xFF;
    data[5] = param32 & 0xFF;
    data[6] = (param16 >> 8) & 0xFF;
    data[7] = param16 & 0xFF;

    getchar();


    // get motor temperature
    for (uint32_t j = 10; j <= 14; j++)
    {

        uint8_t id = j & 0xFF;
        if (device->writeCanData(id, 2, data) < 0)
        {
            std::cerr << "Error writing to CAN bus.\n";
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    std::cout << "\nmotor temperature\n";

    getchar();
    getchar();

    // motor close
    for (uint32_t j = 10; j <= 14; j++)
    {
        uint8_t id = j & 0xFF;
        if (device->writeCanData(id, 6, motor_off) < 0)
        {
            std::cerr << "Error writing to CAN bus.\n";
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1));


    stopCan = true;
    readProc.join();

    device->Close();
    delete device;    
}
#endif

#if 1 // for sito harmonic drive
int main()
{
    // for windows you can specify the baudrate to the NewCanDevice function


    std::stringstream strstr;


    CanInterface* device = NewCanDevice();
    device->Open(0, 1000000);

    // start the read thread
    std::thread readProc(canReadProc, device);

    std::cout << "Writing CAN data...\n";

    int param32;
    int param16;

    getchar();

    sito_open(device); // 모터 오픈
    getchar();

    sito_set_origin(device); // 초기 위치(0) 기억
    getchar();

    sito_limit_current(device, 10); // 전류 제한
    getchar();

    sito_speed_control(device, 2000); // 속도 제어 
    getchar();

    sito_get_speed(device); // 모터 속도
    getchar();

    sito_cease(device); // 모터 중지
    getchar();

    sito_current_control(device, 100); // 전류제어
    getchar();

    sito_get_current(device); // 모터 전류
    getchar();

    sito_cease(device); // 모터 중지
    getchar();

    sito_position_control(device, 0, 2000);
    getchar();

    sito_get_position(device); // 모터 위치
    getchar();

    sito_get_temperature(device); // 모터 온도
    getchar();

    sito_close(device); // 모터 클로즈
    getchar();



    stopCan = true;
    readProc.join();

    device->Close();
    delete device;

}
#endif