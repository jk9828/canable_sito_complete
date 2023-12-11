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
    CanInterface* device = NewCanDevice();
    device->Open(0, 1000000);

    // start the read thread
    std::thread readProc(canReadProc, device);

    std::cout << "Writing CAN data.\n";
    uint8_t data[8];
    int id_new = 11;
    data[0] = 0xFF;
    data[1] = 0x28;
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