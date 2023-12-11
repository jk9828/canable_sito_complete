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

static std::atomic<bool> stopCan = false;



static void canReadProc(CanInterface* device)
{
    while (!stopCan)
    {
        CanFrame frame = device->readCanData(10);
        if (frame.can_dlc != 0)
            std::cout << frame;
    }
}



int sito_open(CanInterface* device) {

    uint8_t data[8];
    uint8_t motor_open[6] = { 0xFF, 0x80, 0x00, 0x00, 0x00, 0x01 };
    uint8_t motor_off[6] = { 0xFF, 0x80, 0x00, 0x00, 0x00, 0x00 };

    int param32;
    int param16;

    std::cout << "\nmotor open\n";

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
    return 0;
}




int sito_close(CanInterface* device) {

    uint8_t data[8];
    uint8_t motor_open[6] = { 0xFF, 0x80, 0x00, 0x00, 0x00, 0x01 };
    uint8_t motor_off[6] = { 0xFF, 0x80, 0x00, 0x00, 0x00, 0x00 };

    int param32;
    int param16;

    std::cout << "\nmotor out\n";

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

    return 0;
}




int sito_cease(CanInterface* device)
{
    uint8_t data[8];
    uint8_t motor_open[6] = { 0xFF, 0x80, 0x00, 0x00, 0x00, 0x01 };
    uint8_t motor_off[6] = { 0xFF, 0x80, 0x00, 0x00, 0x00, 0x00 };

    int param32;
    int param16;

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

    std::cout << "\nmotor cease !! \n";

    // motor cease
    for (uint32_t j = 11; j <= 14; j++)
    {

        uint8_t id = j & 0xFF;
        if (device->writeCanData(id, 8, data) < 0)
        {
            std::cerr << "Error writing to CAN bus.\n";
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    return 0;
}





int sito_current_control(CanInterface* device, int current)
{
    uint8_t data[8];
    uint8_t motor_open[6] = { 0xFF, 0x80, 0x00, 0x00, 0x00, 0x01 };
    uint8_t motor_off[6] = { 0xFF, 0x80, 0x00, 0x00, 0x00, 0x00 };

    int param32;
    int param16;

    param32 = current; // 
    param16 = 0; // 

    data[0] = 0xFF;
    data[1] = 0xA5;
    data[2] = (param32 >> 24) & 0xFF;
    data[3] = (param32 >> 16) & 0xFF;
    data[4] = (param32 >> 8) & 0xFF;
    data[5] = param32 & 0xFF;
    data[6] = (param16 >> 8) & 0xFF;
    data[7] = param16 & 0xFF;

    std::cout << "\ncurrent control : " << std::dec << param32 << "\n";

    // set motor current control
    for (uint32_t j = 11; j <= 14; j++)
    {

        uint8_t id = j & 0xFF;
        if (device->writeCanData(id, 8, data) < 0)
        {
            std::cerr << "Error writing to CAN bus.\n";
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    return 0;
}



int sito_get_current(CanInterface* device)
{

    uint8_t data[8];

    uint8_t motor_open[6] = { 0xFF, 0x80, 0x00, 0x00, 0x00, 0x01 };
    uint8_t motor_off[6] = { 0xFF, 0x80, 0x00, 0x00, 0x00, 0x00 };


    int param32;
    int param16;

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

    // get motor current
    for (uint32_t j = 11; j <= 14; j++)
    {

        uint8_t id = j & 0xFF;
        if (device->writeCanData(id, 2, data) < 0)
        {
            std::cerr << "Error writing to CAN bus.\n";
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    std::cout << "\nmotor current\n";

    return 0;
}







int sito_get_position(CanInterface* device)
{
    uint8_t data[8];

    uint8_t motor_open[6] = { 0xFF, 0x80, 0x00, 0x00, 0x00, 0x01 };
    uint8_t motor_off[6] = { 0xFF, 0x80, 0x00, 0x00, 0x00, 0x00 };

    int param32;
    int param16;

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

    // get motor position
    for (uint32_t j = 11; j <= 14; j++)
    {
        uint8_t id = j & 0xFF;
        if (device->writeCanData(id, 2, data) < 0)
        {
            std::cerr << "Error writing to CAN bus.\n";
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    std::cout << "\nmotor position\n";

    return 0;
}








int sito_get_speed(CanInterface* device)
{
    uint8_t data[8];

    uint8_t motor_open[6] = { 0xFF, 0x80, 0x00, 0x00, 0x00, 0x01 };
    uint8_t motor_off[6] = { 0xFF, 0x80, 0x00, 0x00, 0x00, 0x00 };

    int param32;
    int param16;

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
    for (uint32_t j = 11; j <= 14; j++)
    {

        uint8_t id = j & 0xFF;
        if (device->writeCanData(id, 2, data) < 0)
        {
            std::cerr << "Error writing to CAN bus.\n";
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    std::cout << "\nmotor speed\n";

    return 0;
}






int sito_get_temperature(CanInterface* device)
{
    uint8_t data[8];

    uint8_t motor_open[6] = { 0xFF, 0x80, 0x00, 0x00, 0x00, 0x01 };
    uint8_t motor_off[6] = { 0xFF, 0x80, 0x00, 0x00, 0x00, 0x00 };


    int param32;
    int param16;

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

    // get motor temperature
    for (uint32_t j = 11; j <= 14; j++)
    {

        uint8_t id = j & 0xFF;
        if (device->writeCanData(id, 2, data) < 0)
        {
            std::cerr << "Error writing to CAN bus.\n";
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    std::cout << "\nmotor temperature\n";
    return 0;
}








int sito_limit_current(CanInterface* device, int current)
{

    uint8_t data[8];

    uint8_t motor_open[6] = { 0xFF, 0x80, 0x00, 0x00, 0x00, 0x01 };
    uint8_t motor_off[6] = { 0xFF, 0x80, 0x00, 0x00, 0x00, 0x00 };

    int param32;
    int param16;

    param32 = current; // 
    param16 = 0; // 

    data[0] = 0xFF;
    data[1] = 0x8F;
    data[2] = (param32 >> 24) & 0xFF;
    data[3] = (param32 >> 16) & 0xFF;
    data[4] = (param32 >> 8) & 0xFF;
    data[5] = param32 & 0xFF;
    data[6] = (param16 >> 8) & 0xFF;
    data[7] = param16 & 0xFF;

    // set upper limit current
    for (uint32_t j = 11; j <= 14; j++)
    {

        uint8_t id = j & 0xFF;
        if (device->writeCanData(id, 8, data) < 0)
        {
            std::cerr << "Error writing to CAN bus.\n";
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    std::cout << "\n set current limit " << std::dec << param32 << "\n";

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

    // save setting
    for (uint32_t j = 11; j <= 14; j++)
    {

        uint8_t id = j & 0xFF;
        if (device->writeCanData(id, 8, data) < 0)
        {
            std::cerr << "Error writing to CAN bus.\n";
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    std::cout << "\nsetting saved\n ";
    return 0;
}






int sito_speed_control(CanInterface* device, int speed)
{

    uint8_t data[8];
    uint8_t motor_open[6] = { 0xFF, 0x80, 0x00, 0x00, 0x00, 0x01 };
    uint8_t motor_off[6] = { 0xFF, 0x80, 0x00, 0x00, 0x00, 0x00 };

    int param32;
    int param16;

    param32 = speed; // 
    param16 = 0; // 

    data[0] = 0xFF;
    data[1] = 0x86;
    data[2] = (param32 >> 24) & 0xFF;
    data[3] = (param32 >> 16) & 0xFF;
    data[4] = (param32 >> 8) & 0xFF;
    data[5] = param32 & 0xFF;
    data[6] = (param16 >> 8) & 0xFF;
    data[7] = param16 & 0xFF;

    // set motor current control
    for (uint32_t j = 11; j <= 14; j++)
    {

        uint8_t id = j & 0xFF;
        if (device->writeCanData(id, 8, data) < 0)
        {
            std::cerr << "Error writing to CAN bus.\n";
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    std::cout << "\nspeed control :" << std::dec << param32 << " [rpm]\n";
    return 0;
}






int sito_position_control(CanInterface* device, int position, int speed)
{

    uint8_t data[8];

    uint8_t motor_open[6] = { 0xFF, 0x80, 0x00, 0x00, 0x00, 0x01 };
    uint8_t motor_off[6] = { 0xFF, 0x80, 0x00, 0x00, 0x00, 0x00 };

    int param32;
    int param16;

    param32 = position; // motor position
    param16 = speed; // speed [rpm]

    data[0] = 0xFF;
    data[1] = 0x82;
    data[2] = (param32 >> 24) & 0xFF;
    data[3] = (param32 >> 16) & 0xFF;
    data[4] = (param32 >> 8) & 0xFF;
    data[5] = param32 & 0xFF;
    data[6] = (param16 >> 8) & 0xFF;
    data[7] = param16 & 0xFF;

    // run position
    for (uint32_t j = 11; j <= 14; j++)
    {

        uint8_t id = j & 0xFF;
        if (device->writeCanData(id, 8, data) < 0)
        {
            std::cerr << "Error writing to CAN bus.\n";
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    std::cout << "\n go to postion " << std::dec << param32 << " with " << std::dec << param16 << " [rpm] \n";

    return 0;
}






int sito_return_to_origin(CanInterface* device)
{
    uint8_t data[8];

    uint8_t motor_open[6] = { 0xFF, 0x80, 0x00, 0x00, 0x00, 0x01 };
    uint8_t motor_off[6] = { 0xFF, 0x80, 0x00, 0x00, 0x00, 0x00 };

    int param32;
    int param16;

    param32 = 1; // 
    param16 = 0; // 

    data[0] = 0xFF;
    data[1] = 0x88;
    data[2] = (param32 >> 24) & 0xFF;
    data[3] = (param32 >> 16) & 0xFF;
    data[4] = (param32 >> 8) & 0xFF;
    data[5] = param32 & 0xFF;
    data[6] = (param16 >> 8) & 0xFF;
    data[7] = param16 & 0xFF;

    // return to origin
    for (uint32_t j = 11; j <= 14; j++)
    {

        uint8_t id = j & 0xFF;
        if (device->writeCanData(id, 8, data) < 0)
        {
            std::cerr << "Error writing to CAN bus.\n";
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    std::cout << "\nreturn to origin\n ";

    return 0;
}






int sito_set_origin(CanInterface* device)
{

    uint8_t data[8];

    uint8_t motor_open[6] = { 0xFF, 0x80, 0x00, 0x00, 0x00, 0x01 };
    uint8_t motor_off[6] = { 0xFF, 0x80, 0x00, 0x00, 0x00, 0x00 };

    int param32;
    int param16;

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
    for (uint32_t j = 11; j <= 14; j++)
    {

        uint8_t id = j & 0xFF;
        if (device->writeCanData(id, 8, data) < 0)
        {
            std::cerr << "Error writing to CAN bus.\n";
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    std::cout << "\nset motor origin current position...\n\n";;

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
    }
    std::cout << "\nsetting saved.\n";

    return 0;
}