


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
//           MSP430FR2000
//         ---------------
//     /|\|               |
//      | |               |
//      --|RST            |
//        |           P1.7|--> UCA0TXD
//        |               |
//        |           P1.1|<-- Line A
//        |           P1.0|<-- Line B
//        |               |
//
//   Ryan Meredith
//   Texas Instruments Inc.
//   September 2017
//   Built with IAR Embedded Workbench v7.10.5 & Code Composer Studio v7.2
//******************************************************************************
#include <msp430.h>
#include <stdint.h>

#define LINE_A   BIT1
#define LINE_B   BIT0

#if defined(__TI_COMPILER_VERSION__)
uint32_t pos;
uint32_t pos_prev;
uint16_t time;
#elif defined(__IAR_SYSTEMS_ICC__)
__no_init uint8_t pos;
__no_init uint16_t time;
#else
#error Compiler not supported!
#endif

/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    // Initialize globals
    // 2147483648 = 2^32/2 = 2^31
    pos = 2147483648;
    pos_prev = 2147483648;
    time = 0;

    // Configure input for encoder pins
    P1OUT |= LINE_A | LINE_B;
    P1REN |= LINE_A | LINE_B;

    // Configure UART pins
    P1SEL0 |= BIT6 | BIT7;      // set 2-UART pin as second function

    // Unlock GPIO
    PM5CTL0 &= ~LOCKLPM5;

    // Configure one FRAM waitstate as required by the device datasheet for MCLK
    // operation beyond 8MHz _before_ configuring the clock system.
    FRCTL0 = FRCTLPW | NWAITS_1;

    __bis_SR_register(SCG0);    // disable FLL
    CSCTL3 |= SELREF__REFOCLK;  // Set REFO as FLL reference source
    CSCTL0 = 0;                 // clear DCO and MOD registers
    CSCTL1 &= ~(DCORSEL_7);     // Clear DCO frequency select bits first
    CSCTL1 |= DCORSEL_5;        // Set DCO = 16MHz
    CSCTL2 = FLLD_0 + 487;      // set to fDCOCLKDIV = (FLLN + 1)*(fFLLREFCLK/n)
                                //                   = (487 + 1)*(32.768 kHz/1)
                                //                   = 16 MHz
    __delay_cycles(3);
    __bic_SR_register(SCG0);                        // enable FLL
    while(CSCTL7 & (FLLUNLOCK0 | FLLUNLOCK1));      // FLL locked

    CSCTL4 = SELMS__DCOCLKDIV | SELA__REFOCLK;

    // Configure UART
    // From User's Guide Table of Baud Rates, 9600 baud at BRCLK = 16000000
    // UCBRx = 104;
    // UCBRSx = 0xD6
    // UCOS16 = 1
    // UCBRFx = 2
    // Fractional portion = 0.1667
    // User's Guide Table 21-5: UCBRSx = 0xD6
    // UCBRFx = 2

    // 9600 baud
//    UCA0CTLW0 |= UCSWRST;
//    UCA0CTLW0 |= UCSSEL__SMCLK;
//    UCA0BRW = 104;
//    UCA0MCTLW = 0xD600 | UCOS16_1 | UCBRF_2;
    // 115200 baud
    UCA0CTLW0 |= UCSWRST;
    UCA0CTLW0 |= UCSSEL__SMCLK;
    UCA0BRW = 8;
    UCA0MCTLW = 0xD600 | UCOS16_1 | UCBRF_10;
    UCA0CTLW0 &= ~UCSWRST;                  // Initialize eUSCI

    // Source = ACLK = 32768 Hz, divided by 16 (2048 Hz)
    SYSCFG2 |= RTCCKSEL;                    // Select ACLK as RTC clock
    RTCCTL = RTCSS_1 | RTCSR | RTCPS__16;

    // set interrupts and enable
    P1IES = P1IN & (LINE_A | LINE_B);
    P1IFG = 0;                              // Clear all P1 interrupt flags
    P1IE = LINE_A | LINE_B;

    while (1){
         __bis_SR_register(LPM3_bits | GIE);    // Enter LPM3, enable interrupts
         __no_operation();

         if (pos != pos_prev)
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

             pos_prev = pos;
         }

//         while(!(UCA0IFG&UCTXIFG));
//         UCA0TXBUF = time >> 8;
//         __no_operation();
//
//         while(!(UCA0IFG&UCTXIFG));
//         UCA0TXBUF = time;
//         __no_operation();
    }
}

void __error_tx(void)
{
    while(!(UCA0IFG&UCTXIFG));
    UCA0TXBUF = 0xFF;       // For 32-bit count send a FFFFFFFF - which should never happen, interpreted as error by host.
    __no_operation();
}




#pragma vector=PORT1_VECTOR
__interrupt void port1_ISR(void){

    time = RTCCNT;
    uint8_t state = P1IN & (LINE_A | LINE_B);

    switch(__even_in_range(P1IV, P1IV__P1IFG7)) {
        case P1IV__NONE:   break;
        case P1IV__P1IFG0:
            if (P1IES & LINE_B) {       // if falling edge on Line B
                if (state & LINE_A) {   // Transitioned from State 3 to State 2
                    //pos += 1;
                }
                else {
                    pos -= 1;           // Transitioned from State 1 to State 0
                }
            }
            else {                      // else rising edge on Line B
                if (state & LINE_A) {   // Transitioned from State 2 to State 3
                    //pos -= 1;
                }
                else {                  // Transitioned from State 0 to State 1
                    pos += 1;
                }
            }

            P1IES ^= LINE_B;
            break;
        case P1IV__P1IFG1:
            if (P1IES & LINE_A) {       // if falling edge on Line A
                if (state & LINE_B) {   // Transitioned from State 3 to State 1
                    //pos -= 1;
                }
                else {
                    //pos += 1;           // Transitioned from State 2 to State 0
                }
            }
            else {                      // else rising edge on Line A
                if (state & LINE_B) {   // Transitioned from State 1 to State 3
                    //pos += 1;
                }
                else {                  // Transitioned from State 0 to State 2
                    //pos -= 1;
                }
            }

            P1IES ^= LINE_A;
            break;
        case P1IV__P1IFG2: break;
        case P1IV__P1IFG3: break;
        case P1IV__P1IFG4: break;
        case P1IV__P1IFG5: break;
        case P1IV__P1IFG6: break;
        case P1IV__P1IFG7: break;
        default:   break;
    }

    __bic_SR_register_on_exit(LPM3_bits); // Exit LPM3 on return
}


