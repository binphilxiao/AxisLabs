/* --COPYRIGHT--,BSD
 * Copyright (c) 2017, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
//******************************************************************************
//  MSP430FR2000 Quadrature Encoder Demo
//
//  Description: Quadrature encoder device position and time tracker. P1.1 and
//  P1.0 act as input pins for Line A and Line B of a quadrature encoder device.
//  Each time an edge occurs on Line A or Line B, the position counter is
//  updated according to the following state diagram and an 8-bit integer
//  denoting the position is transmitted through UART followed immediately by a
//  16-bit integer timestamp.
//
//
//        Quadrature Encoder State Machine
//
//       ---------     pos++      ---------
//      | State 0 |------------->| State 1 |
//      |         |              |         |
//      |   A B   |    pos--     |   A B   |
//      |   0 0   |<-------------|   0 1   |
//       ---------                ---------
//        ^     |                  ^     |
//        |     |                  |     |
//  pos++ |     | pos--      pos-- |     | pos++
//        |     |                  |     |
//        |     V                  |     V
//       ---------     pos--      ---------
//      | State 2 |------------->| State 3 |
//      |         |              |         |
//      |   A B   |    pos++     |   A B   |
//      |   1 0   |<-------------|   1 1   |
//       ---------                ---------
//
//  XT1 is considered to be absent in this example.
//  ACLK = default REFO ~32768Hz, MCLK = SMCLK = ~16MHz.
//
//           MSP430FR2111 --> modified by Nabeel to Bing's schematic
//         ---------------
//     /|\|               |
//      | |               |
//      --|RST        P1.2|--> UCA0RXD
//        |           P1.3|--> UCA0TXD
//        |               |
//        |           P2.0|<-- Line B
//        |           P2.1|<-- Line A
//        |               |
//
//   Ryan Meredith
//   Texas Instruments Inc.
//   September 2017
//   Built with IAR Embedded Workbench v7.10.5 & Code Composer Studio v7.2
//******************************************************************************
#include <msp430.h>
#include <stdint.h>

#define LINE_A          BIT1            // P2.1
#define LINE_B          BIT0            // P2.0

#define DEF_UART 1
//#define SEC_UART 1
#ifdef DEF_UART
    #define LINE_TXD        BIT7        // P1.7
    #define LINE_RXD        BIT6        // P1.6
#elif SEC_UART
    #define LINE_TXD        BIT3        // P1.3
    #define LINE_RXD        BIT2        // P1.2
#endif

#define DCO_FREQ_MHZ    16              // 8Mhz or 16Mhz
#define MCLK_FREQ_MHZ   DCO_FREQ_MHZ    // MCLK = 16MHz
#define BAUD_RATE       115200          // 115200 or 9600

#if defined(__TI_COMPILER_VERSION__)
uint32_t pos;
uint32_t pos_prev;
uint16_t time;
uint8_t stat_pre;
uint8_t err_cnt;        // err_cnt indicates either a missing pulse or a glitch
/*------------------------Format of  err-------------------------------------
 * |  bit7  |  bit6  |  bit5  |  bit4  |  bit3  |  bit2  |  bit1  |  bit0  |
 * |     Reserved    |       edge      |   post-state    |  current-state  |
 */
/*------------------------Format of  edge in err-----------------------------
 *    00                01                10                 11
 *    Line_A rising     Line_A falling    Line_B rising      Line_B falling
 */
uint8_t err;            // err indicates the latest status of error
uint8_t err_flg;
#elif defined(__IAR_SYSTEMS_ICC__)
__no_init uint8_t pos;
__no_init uint16_t time;
#else
#error Compiler not supported!
#endif

/**
 * Initial UART
 */
void Init_Port(void)
{
    //----------Configure input for encoder pins----------//
    P2OUT |= LINE_B | LINE_A;       //Pull-up internally(when set as inputs).
    P2REN |= LINE_B | LINE_A;       //Pull up enable.

    //----------Configure UART pins----------//
    P1SEL0 |= LINE_RXD | LINE_TXD;      //set 2-UART pin as second function

    PM5CTL0 &= ~LOCKLPM5;       // Disable high-impedance mode to activate port settings
}


/**
 * Software_Trim
 */
void Software_Trim()
{
    unsigned int oldDcoTap = 0xffff;
    unsigned int newDcoTap = 0xffff;
    unsigned int newDcoDelta = 0xffff;
    unsigned int bestDcoDelta = 0xffff;
    unsigned int csCtl0Copy = 0;
    unsigned int csCtl1Copy = 0;
    unsigned int csCtl0Read = 0;
    unsigned int csCtl1Read = 0;
    unsigned int dcoFreqTrim = 3;
    unsigned char endLoop = 0;

    do
    {
        CSCTL0 = 0x100;                         // DCO Tap = 256
        do
        {
            CSCTL7 &= ~DCOFFG;                  // Clear DCO fault flag
        }while (CSCTL7 & DCOFFG);               // Test DCO fault flag

        __delay_cycles((unsigned int)3000 * MCLK_FREQ_MHZ);// Wait FLL lock status (FLLUNLOCK) to be stable
                                                           // Suggest to wait 24 cycles of divided FLL reference clock
        while((CSCTL7 & (FLLUNLOCK0 | FLLUNLOCK1)) && ((CSCTL7 & DCOFFG) == 0));

        csCtl0Read = CSCTL0;                   // Read CSCTL0
        csCtl1Read = CSCTL1;                   // Read CSCTL1

        oldDcoTap = newDcoTap;                 // Record DCOTAP value of last time
        newDcoTap = csCtl0Read & 0x01ff;       // Get DCOTAP value of this time
        dcoFreqTrim = (csCtl1Read & 0x0070)>>4;// Get DCOFTRIM value

        if(newDcoTap < 256)                    // DCOTAP < 256
        {
            newDcoDelta = 256 - newDcoTap;     // Delta value between DCPTAP and 256
            if((oldDcoTap != 0xffff) && (oldDcoTap >= 256)) // DCOTAP cross 256
                endLoop = 1;                   // Stop while loop
            else
            {
                dcoFreqTrim--;
                CSCTL1 = (csCtl1Read & (~DCOFTRIM)) | (dcoFreqTrim<<4);
            }
        }
        else                                   // DCOTAP >= 256
        {
            newDcoDelta = newDcoTap - 256;     // Delta value between DCPTAP and 256
            if(oldDcoTap < 256)                // DCOTAP cross 256
                endLoop = 1;                   // Stop while loop
            else
            {
                dcoFreqTrim++;
                CSCTL1 = (csCtl1Read & (~DCOFTRIM)) | (dcoFreqTrim<<4);
            }
        }

        if(newDcoDelta < bestDcoDelta)         // Record DCOTAP closest to 256
        {
            csCtl0Copy = csCtl0Read;
            csCtl1Copy = csCtl1Read;
            bestDcoDelta = newDcoDelta;
        }

    }while(endLoop == 0);                      // Poll until endLoop == 1

    CSCTL0 = csCtl0Copy;                       // Reload locked DCOTAP
    CSCTL1 = csCtl1Copy;                       // Reload locked DCOFTRIM
    while(CSCTL7 & (FLLUNLOCK0 | FLLUNLOCK1)); // Poll until FLL is locked
}

/**
 * Initialize Clock
 */
void Init_Clock(void)
{
        FRCTL0 = FRCTLPW | NWAITS_1;                //Increase waitstate before speed up

        __bis_SR_register(SCG0);                    //disable FLL
        CSCTL3 |= SELREF__REFOCLK;                  //Set REFO as FLL reference source
#if DCO_FREQ_MHZ==16
        CSCTL1 = DCOFTRIMEN_1 | DCOFTRIM0 | DCOFTRIM1 | DCORSEL_5;// DCOFTRIM=5, DCO Range = 16MHz
        CSCTL2 = FLLD_0 + 487;                      // DCODIV = 16MHz
#endif
#if DCO_FREQ_MHZ==8
        CSCTL1 = DCOFTRIMEN_1 | DCOFTRIM0 | DCOFTRIM1 | DCORSEL_3;// DCOFTRIM=3, DCO Range = 8MHz
        CSCTL2 = FLLD_0 + 243;                      // DCODIV = 16MHz
#endif
        __delay_cycles(3);
        __bic_SR_register(SCG0);                    //Enable FLL
        Software_Trim();
        CSCTL4 = SELMS__DCOCLKDIV | SELA__REFOCLK;  // set default REFO(~32768Hz) as ACLK source, ACLK = 32768Hz

        SYSCFG2 |= RTCCKSEL;                        //Select ACLK as RTC clock
        RTCCTL = RTCSS_1 | RTCSR | RTCPS__16;
}

/**
 * Initialize Interrupt
 */
void Init_Interrupt(void)
{
    P2IES = P2IN & (LINE_A | LINE_B);
    P2IE = LINE_A | LINE_B;                 //Enable interrupts
    P2IFG &= (~LINE_A & ~LINE_B);           // Clear all P1 interrupt flags
    __bis_SR_register(GIE);
}

/**
 * Initialize UART
 */
void Init_Uart(void)
{
    UCA0CTLW0 |= UCSWRST;       //Hold eUSCI in reset
    UCA0CTLW0 |= UCSSEL__SMCLK; //SMCLK, Asynchronous, 1 stop, 8 bits, LSB, No Parity

    //User's guide 22.3.10 or Table 22-5
#if (BAUD_RATE == 115200 && DCO_FREQ_MHZ == 16)
    UCA0BRW = 8;                //N = 16Mhz / 115200, INT(N/16) =8
    UCA0MCTLW |= UCOS16_1;      //Oversampling mode enable
    UCA0MCTLW |= UCBRF_10;      //UCBRFx = INT([(N/16) – INT(N/16)] × 16) = 10
    UCA0MCTLW |= 0xF700;        //N-INT(N) = 0.89, User's Guide Table 22-4: UCBRSx = 0xF7
#elif (BAUD_RATE == 9600 && DCO_FREQ_MHZ == 16)
    UCA0BRW = 104;              //N = 16M / 9600, INT(N/16) =104
    UCA0MCTLW |= UCOS16_1;      //Oversampling mode enable
    UCA0MCTLW |= UCBRF_2;       //UCBRFx = INT([(N/16) – INT(N/16)] × 16) = 2
    UCA0MCTLW |= 0xD600;        //N-INT(N) = 0.67, User's Guide Table 22-4: UCBRSx = 0xD6
#elif (BAUD_RATE == 9600 && DCO_FREQ_MHZ == 8)
    UCA0BRW = 52;                //N = 8Mhz / 9600, INT(N/16) =52
    UCA0MCTLW |= UCOS16_1;      //Oversampling mode enable
    UCA0MCTLW |= UCBRF_1;      //UCBRFx = INT([(N/16) – INT(N/16)] × 16) = 1
    UCA0MCTLW |= 0x4900;        //N-INT(N) = 0.33, User's Guide Table 22-4: UCBRSx = 0x49
#endif

    UCA0CTLW0 &= ~UCSWRST;      //Initialize eUSCI
    UCA0IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt

    //__bis_SR_register(LPM3_bits | GIE);    // Enter LPM3, enable interrupts
    __no_operation();
}

void TestUart()
{
    uint8_t i;
    i = 'a';
    while (1){
         while(!(UCA0IFG & UCTXIFG));
         UCA0TXBUF = i;                   // Load data onto buffer
         __no_operation();                     // For debugger
         i++;
    }
}

/*void __error_tx(void)
{
    while(!(UCA0IFG&UCTXIFG));
    UCA0TXBUF = 0xFF;       // For 32-bit count send a FFFFFFFF - which should never happen, interpreted as error by host.
    __no_operation();
}*/


/**
 * Uart ISR
 */
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCI_A0_VECTOR))) USCI_A0_ISR (void)
#else
#error Compiler not supported!
#endif
{
  switch(__even_in_range(UCA0IV,USCI_UART_UCTXCPTIFG))
  {
    case USCI_NONE: break;
    case USCI_UART_UCRXIFG:
      while(!(UCA0IFG&UCTXIFG));
      UCA0TXBUF = UCA0RXBUF;
      __no_operation();
      break;
    case USCI_UART_UCTXIFG: break;
    case USCI_UART_UCSTTIFG: break;
    case USCI_UART_UCTXCPTIFG: break;
    default: break;   }
}

/**
 * PrintLog
 * ------------------------------Format of frame-------------------------------------
 * |  byte7  |  byte6  |  byte5  |  byte4  |  byte3  |  byte2  |  byteit1  |  byte0  |
 * |     0xffff        | err_cnt |   err   |                  position               |
 */
void PrintLog()
{
    while (1){
         if ((pos != pos_prev) && (pos % 1024==0) || err_flg==1)
         {
             while(!(UCA0IFG&UCTXIFG));
             UCA0TXBUF = pos >> 24;
             __no_operation();

             while(!(UCA0IFG&UCTXIFG));
             UCA0TXBUF = pos >> 16;
             __no_operation();

             while(!(UCA0IFG&UCTXIFG));
             UCA0TXBUF = pos >> 8;
             __no_operation();

             while(!(UCA0IFG&UCTXIFG));
             UCA0TXBUF = pos;
             __no_operation();

             while(!(UCA0IFG&UCTXIFG));
             UCA0TXBUF = err;
             __no_operation();

             while(!(UCA0IFG&UCTXIFG));
             UCA0TXBUF = err_cnt;
             __no_operation();

             while(!(UCA0IFG&UCTXIFG));
             UCA0TXBUF = 255;
             __no_operation();

             while(!(UCA0IFG&UCTXIFG));
             UCA0TXBUF = 255;
             __no_operation();

             pos_prev = pos;
             err_flg=0;
         }
    }
}

/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer

    pos = 2147483648;           // Initialize globals
    pos_prev = 2147483648;      // 2147483648 = 2^32/2 = 2^31
    time = 0;
    err=0;
    err_flg=0;

    Init_Clock();
    Init_Port();
    stat_pre = P2IN & (LINE_A | LINE_B);
    Init_Uart();
    Init_Interrupt();

    //TestUart();
    PrintLog();
}

/**
 * Port 2 ISR
 */
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=PORT2_VECTOR
__interrupt void Port_2_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(PORT2_VECTOR))) Port_2 (void)
#else
#error Compiler not supported!
#endif
{
    time = RTCCNT;
    uint8_t stat = P2IN & (LINE_A | LINE_B);

    switch(__even_in_range(P2IV, P2IV__P2IFG7)) {
        case P2IV__NONE:   break;
        case P2IV__P2IFG0:
            if (P2IES & LINE_B) {       // if falling edge on Line B
                if (stat & LINE_A) {    // Transitioned from State 3 to State 2
                    pos += 1;
                    if (stat_pre != 3) {
                        err_flg=1;
                        err |= 0x30;
                    }
                }
                else {
                    pos -= 1;           // Transitioned from State 1 to State 0
                    if (stat_pre != 1) {
                        err_flg=1;
                        err |= 0x30;
                    }
                }
            }
            else {                      // else rising edge on Line B
                if (stat & LINE_A) {    // Transitioned from State 2 to State 3
                    pos -= 1;
                    if (stat_pre != 2) {
                        err_flg=1;
                        err |= 0x20;
                    }
                }
                else {                  // Transitioned from State 0 to State 1
                    pos += 1;
                    if (stat_pre != 0) {
                        err_flg=1;
                        err |= 0x20;
                    }
                }
            }

            P2IES ^= LINE_B;
            break;
        case P2IV__P2IFG1:
            if (P2IES & LINE_A) {       // if falling edge on Line A
                if (stat & LINE_B) {    // Transitioned from State 3 to State 1
                    pos -= 1;
                    if (stat_pre != 3) {
                        err_flg=1;
                        err |= 0x10;
                    }
                }
                else {
                    pos += 1;           // Transitioned from State 2 to State 0
                    if (stat_pre != 2) {
                        err_flg=1;
                        err |= 0x10;
                    }
                }
            }
            else {                      // else rising edge on Line A
                if (stat & LINE_B) {    // Transitioned from State 1 to State 3
                    pos += 1;
                    if (stat_pre != 1) {
                        err_flg=1;
                        err |= 0x00;
                    }
                }
                else {                  // Transitioned from State 0 to State 2
                    pos -= 1;
                    if (stat_pre != 0) {
                        err_flg=1;
                        err |= 0x00;
                    }
                }
            }

            P2IES ^= LINE_A;
            break;
        case P2IV__P2IFG2: break;
        case P2IV__P2IFG3: break;
        case P2IV__P2IFG4: break;
        case P2IV__P2IFG5: break;
        case P2IV__P2IFG6: break;
        case P2IV__P2IFG7: break;
        default:   break;
    }
    if (err_flg){
        err_cnt++;
        err |= stat;
        err |= ((stat_pre<<2) & 0x0c);
    }
    stat_pre = stat;
    //__bic_SR_register_on_exit(LPM3_bits); // Exit LPM3 on return
}

