
#include "can_api.h"
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

#if 1 // for sito harmonic drive
int sito_set_origin()
{

    // for windows you can specify the baudrate to the NewCanDevice function
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

    getchar();

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

    getchar();

    // set motor origin
    for (uint32_t j = 11; j <= 14; j++)
    {

        uint8_t id = j & 0xFF;
        if (device->writeCanData(id, 8, data) < 0)
        {
            std::cerr << "Error writing to CAN bus.\n";
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    std::cout << "set motor origin current position... ";

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
    for (uint32_t j = 11; j <= 14; j++)
    {

        uint8_t id = j & 0xFF;
        if (device->writeCanData(id, 8, data) < 0)
        {
            std::cerr << "Error writing to CAN bus.\n";
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        std::cout << "setting saved.";

        getchar();

        // motor close
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
}
#endif

