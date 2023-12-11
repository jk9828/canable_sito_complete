#include "can_api.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <thread>
#include <chrono>
#include <atomic>

#include "sito.h"

#ifndef PER
#define GREAT 1
#define PER GREAT
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
    int param16 = 500;
    data[0] = 0xFF;
    data[1] = 0x82;
    data[2] = (param32 >> 24) & 0xFF;
    data[3] = (param32 >> 16) & 0xFF;
    data[4] = (param32 >> 8) & 0xFF;
    data[5] = param32 & 0xFF;
    data[6] = (param16 >> 8) & 0xFF;
    data[7] = param16 & 0xFF;
    uint8_t motor_open[6] = { 0xFF, 0x80, 0x00, 0x00, 0x00, 0x01 };
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