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


int sito_cease()
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
    data[1] = 0x87;
    data[2] = (param32 >> 24) & 0xFF;
    data[3] = (param32 >> 16) & 0xFF;
    data[4] = (param32 >> 8) & 0xFF;
    data[5] = param32 & 0xFF;
    data[6] = (param16 >> 8) & 0xFF;
    data[7] = param16 & 0xFF;

    getchar();

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

    std::cout << "\nmotor cease !! \n";

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

int sito_current_control()
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


    param32 = 10; // 
    param16 = 0; // 

    data[0] = 0xFF;
    data[1] = 0xA5;
    data[2] = (param32 >> 24) & 0xFF;
    data[3] = (param32 >> 16) & 0xFF;
    data[4] = (param32 >> 8) & 0xFF;
    data[5] = param32 & 0xFF;
    data[6] = (param16 >> 8) & 0xFF;
    data[7] = param16 & 0xFF;

    getchar();

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

    std::cout << "\n current control " << std::dec << param32 << "\n";

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

int sito_get_current()
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




int sito_get_postion()
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




int sito_get_speed()
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



int sito_get_temperature()
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




int sito_limit_current()
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


    param32 = 100; // 
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
    for (uint32_t j = 11; j <= 14; j++)
    {

        uint8_t id = j & 0xFF;
        if (device->writeCanData(id, 8, data) < 0)
        {
            std::cerr << "Error writing to CAN bus.\n";
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    std::cout << "setting saved ";

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



int sito_position_control()
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

    param32 = 500; // motor position
    param16 = 2000; // speed [rpm]

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


int sito_return_to_origin()
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
    data[1] = 0x88;
    data[2] = (param32 >> 24) & 0xFF;
    data[3] = (param32 >> 16) & 0xFF;
    data[4] = (param32 >> 8) & 0xFF;
    data[5] = param32 & 0xFF;
    data[6] = (param16 >> 8) & 0xFF;
    data[7] = param16 & 0xFF;

    getchar();

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

    std::cout << "return to origin ";

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