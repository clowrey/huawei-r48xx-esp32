#include <Arduino.h>
#include <CAN.h>


#include "huawei.h"
#include "commands.h"
#include "main.h"




unsigned long g_Time1000;
unsigned long g_Time5000;


void onCANReceive(int packetSize)
{
    if(!CAN.packetExtended())
        return;
    if(CAN.packetRtr())
        return;

    uint32_t msgid = CAN.packetId();

    uint8_t data[packetSize];
    CAN.readBytes(data, sizeof(data));

    Huawei::onRecvCAN(msgid, data, packetSize);
}

void setup()
{
    Serial.begin(921600);
    while(!Serial);
    Serial.println("BOOTED!");

    // PSU enable pin
    //pinMode(POWER_EN_GPIO, OUTPUT_OPEN_DRAIN);
    //digitalWrite(POWER_EN_GPIO, 0); // Default = ON


    CAN.setPins(26, 27); // CAN.setPins(rx, tx)
    if(!CAN.begin(250E3)) {
        Serial.println("Starting CAN failed!");
        while(1);
    }
    Serial.println("Starting CAN succeeded!");


    // crashes when calling some functions inside interrupt
    //CAN.onReceive(onCANReceive);
}

void loop()
{
    int packetSize = CAN.parsePacket();
    if(packetSize)
        onCANReceive(packetSize);

    if((millis() - g_Time1000) > 1000)
    {
       // Huawei::every1000ms();
        g_Time1000 = millis();

        //Huawei::sendGetData(0x01); 
    }

    if((millis() - g_Time5000) > 5000)
    {
        g_Time5000 = millis();
        
        //Huawei::setVoltage(45, 0x01, 0);  //set address to 0x01 for first supply
        //Huawei::setVoltage(46, 0x02, 0);  //set address to 0x02 for second supply
        //Huawei::setVoltage(47, 0x00, 0);  //set address to 0x00 to send to both supplies

        //Huawei::setCurrent(7,0);  //set current to 5 not permanent

        //Huawei::sendCAN(msgid, data, len, rtr);

        //Huawei::sendGetInfo();
        //Huawei::sendGetDescription();

        
        //Huawei::sendGetData(0x00);
        //Commands::parseLine("status");
        //Huawei::HuaweiInfo &info = Huawei::g_PSU;
        //Serial.printf("Output Voltage: %.2f V\n", info.output_voltage);
    }
}

//}

