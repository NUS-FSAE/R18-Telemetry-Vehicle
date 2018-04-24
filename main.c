#include <pic18f45k80.h>
#include <stdlib.h>
#include <string.h>
#include "mcc_generated_files/mcc.h"

#define SOF_BYTE0 0xFE
#define SOF_BYTE1 0xFF
#define EOF_BYTE0 0X7E

uCAN_MSG canMessage;
bool send = false;
int data = 0;
uint8_t data1[9],data2[9],data3[9],data4[9],data5[9];

void sendPacket_id(uint8_t id);
void sendPacket(uint8_t* bytes);

void updateData() {
    INTERRUPT_GlobalInterruptDisable();
    INTERRUPT_PeripheralInterruptDisable();
    data = (data+1)%127;
    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();
}

void time_to_send() {
    TMR1_StopTimer();
    INTERRUPT_GlobalInterruptDisable();
    INTERRUPT_PeripheralInterruptDisable();
    send = true;
}

void main(void) {
    // Initialize the device
    SYSTEM_Initialize();
    
    //CAN Configuration
    CIOCONbits.CLKSEL = 1;
    CIOCONbits.ENDRHI = 1;

    RXB0CONbits.RXM0 = 1;
    RXB0CONbits.RXM1 = 0;
    RXB0CONbits.RB0DBEN = 0;
    RXB0CONbits.FILHIT0 = 0;

    EUSART2_Initialize();
    
//    TMR0_SetInterruptHandler(&updateData);
    TMR1_SetInterruptHandler(&time_to_send);

    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();

    while (1) {
        if (CAN_receive(&canMessage)) {
            LED_SetHigh();
            uint8_t id = canMessage.frame.id;
            switch(id) {
                case 0x40:
                    data1[0] = id;
                    memcpy(data1+1,&(canMessage.array[6]),8);
                    break;
                case 0x41:
                    data2[0] = id;
                    memcpy(data2+1,&(canMessage.array[6]),8);
                    break;
                case 0x42:
                    data3[0] = id;
                    memcpy(data3+1,&(canMessage.array[6]),8);
                    break;
                case 0x43:
                    data4[0] = id;
                    memcpy(data4+1,&(canMessage.array[6]),8);
                    break;
                case 0x44:
                    data5[0] = id;
                    memcpy(data5+1,&(canMessage.array[6]),8);
                    break;
            }
        }
        
        if(send) {
            INTERRUPT_GlobalInterruptDisable();
            INTERRUPT_PeripheralInterruptDisable();
            sendPacket(data1);
            sendPacket(data2);
            sendPacket(data3);
            sendPacket(data4);
            sendPacket(data5);
            send = false;
            TMR1_StartTimer();
            INTERRUPT_GlobalInterruptEnable();
            INTERRUPT_PeripheralInterruptEnable();
        }
    }
}

void sendPacket(uint8_t* bytes) {
    EUSART2_Write(SOF_BYTE0); //start of frame byte
    for (int i = 0; i < 9; ++i) {
        EUSART2_Write(*(bytes+i));
    }
    EUSART2_Write(EOF_BYTE0); // end of Frame
}

void sendPacket_id(uint8_t id) {
    LED_SetHigh();
    EUSART2_Write(SOF_BYTE0);  //start of frame byte1
    EUSART2_Write(id);
    for(int i=0; i<8; ++i) {
        EUSART2_Write(data);
    }
    EUSART2_Write(EOF_BYTE0); // end of Frame
}
/**
 End of File
*/