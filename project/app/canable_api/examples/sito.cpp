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
int main()
{

    // for windows you can specify the baudrate to the NewCanDevice function
    CanInterface* device = NewCanDevice();
    device->Open(0, 1000000);

    // start the read thread
    std::thread readProc(canReadProc, device);

    std::cout << "Writing CAN data.\n";
    uint8_t data[8];
    int id_new = 11;
    data[0] = 0xFF;
    data[1] = 0x94;
    data[2] = 0;
    data[3] = 0;
    data[4] = 0;
    data[5] = id_new & 0xFF;
    data[6] = 0;
    data[7] = 0;
    uint8_t motor_open[6] = { 0xFF, 0x80, 0x00, 0x00, 0x00, 0x01 };
    uint8_t motor_off[6] = { 0xFF, 0x80, 0x00, 0x00, 0x00, 0x00 };


    if (device->writeCanData(0x00, 6, motor_open) < 0)
    {
        std::cerr << "Error writing to CAN bus.\n";
    }

    getchar();

    // set motor id
    if (device->writeCanData(0x00, 8, data) < 0)
    {
        std::cerr << "Error writing to CAN bus.\n";
    }

    data[0] = 0xFF;
    data[1] = 0x95;
    data[2] = 0;
    data[3] = 0;
    data[4] = 0;
    data[5] = 1 & 0xFF;
    data[6] = 0;
    data[7] = 0;

    // save motor parameters
    if (device->writeCanData(0x00, 8, data) < 0)
    {
        std::cerr << "Error writing to CAN bus.\n";
    }

    getchar();

    if (device->writeCanData(0x00, 6, motor_off) < 0)
    {
        std::cerr << "Error writing to CAN bus.\n";
    }

    stopCan = true;
    readProc.join();

    device->Close();
    delete device;
}
#endif