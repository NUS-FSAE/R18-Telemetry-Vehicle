/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs 

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs  - 1.45
        Device            :  PIC18F46K80
        Driver Version    :  2.00
    The generated drivers are tested against the following:
        Compiler          :  XC8 1.35
        MPLAB             :  MPLAB X 3.40
*/

/*
    (c) 2016 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

#include <pic18f45k80.h>

#include "mcc_generated_files/mcc.h"

/*
                         Main application
 */
void sendPacket(uint8_t id, uint8_t* data);

void main(void)
{
    uCAN_MSG canMessage;
    // Initialize the device
    SYSTEM_Initialize();

    PORTCbits.RC6 = 0;
    LATC6 = 0;
    TRISC6 = 0;
    
    //CAN Configuration
    CIOCONbits.CLKSEL = 1;
    CIOCONbits.ENDRHI = 1;
    CIOCONbits.TX2SRC = 0;
    CIOCONbits.TX2EN = 0;
    RXB0CONbits.RXM0 = 1;
    RXB0CONbits.RXM1 = 0;
    RXB0CONbits.RB0DBEN = 0;
    RXB0CONbits.FILHIT0 = 0;

    EUSART2_Initialize();

    while (1) {
        canMessage.frame.data0 = 0x05;
        canMessage.frame.data1 = 0x05;
        canMessage.frame.data2 = 0x05;
        canMessage.frame.data3 = 0x05;
        canMessage.frame.data4 = 0x05;
        canMessage.frame.data5 = 0x05;
        canMessage.frame.data6 = 0x05;
        canMessage.frame.data7 = 0x05;
        canMessage.frame.id = 0x643;
        canMessage.frame.dlc = 8;
        canMessage.frame.idType = dSTANDARD_CAN_MSG_ID_2_0B;
        CAN_transmit(&canMessage);
        
        
        LED_SetHigh();
        __delay_ms(1000);
        LED_SetLow();
        __delay_ms(100);
        if (CAN_receive(&canMessage)) {
            LED_SetHigh();
            if (canMessage.frame.id == 0x640) {
                sendPacket(1, &(canMessage.frame.data0));
            } else if (canMessage.frame.id == 0x641) {
                sendPacket(2, &(canMessage.frame.data0));
            } else if (canMessage.frame.id == 0x642) {
                sendPacket(3, &(canMessage.frame.data0));
            }
        }
    }
}

void sendPacket(uint8_t id, uint8_t* data) {
    EUSART2_Write(0x7C);
    EUSART2_Write(id);
    for(int i=0; i<8; ++i) {
        EUSART2_Write(*(data+i));
    }
    EUSART2_Write(0x7E);
}
/**
 End of File
*/