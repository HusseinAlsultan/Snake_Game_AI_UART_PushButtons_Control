/*
C code for playing the snake game that built with Processing IDE
using push buttons on the MSP430FR2355 via UART serial communication
 */

#include <msp430.h>

void UART_Setup(void);              // set-up UART A0 and A1 ports
void P4_Setup(void);                // set-up port 4 pins

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;       // stop watchdog timer

    UART_Setup();
    P4_Setup();

    PM5CTL0 &= ~LOCKLPM5;           //turn on GPIO system

    __enable_interrupt();           //global Enable for maskable interrupts...(GIE bits in SR)

    while(1) {}                     //do nothing

    return 0;
}


//----------------------------------------------------------------
//---ISRs---------------------------------------------------------

#pragma vector = PORT4_VECTOR
__interrupt void ISR_P4(void)
{
    switch(P4IV)
    {
        case P4IV_P4IFG0:
            // Pin 4.0 pressed
            UCA0TXBUF = 'L'; // Print 'U' through UART
            break;

        case P4IV_P4IFG4:
            // Pin 4.4 pressed
            UCA0TXBUF = 'D'; // Print 'D' through UART
            break;

        case P4IV_P4IFG6:
            // Pin 4.6 pressed
            UCA0TXBUF = 'R'; // Print 'R' through UART
            break;

        case P4IV_P4IFG7:
            // Pin 4.7 pressed
            UCA0TXBUF = 'U'; // Print 'L' through UART
            break;

        default:
            break;
    }

    P4IFG &= ~(BIT0 | BIT4 | BIT6 | BIT7); // Clear interrupt flags
}

void UART_Setup(void)
{
    //I used default UART framing; 8-bit, LSB first, No Parity, No address, No extra Stop Bit

    //-----------set up UART A0
    UCA0CTLW0 |= UCSWRST;              //put UART A0 in to SW reset

    UCA0CTLW0 |= UCSSEL__SMCLK;        //choose SMCLK for UART A0(at 115200 BR)
    UCA0BRW = 8;                       //set Prescalar to 8
    UCA0MCTLW = 0xD600;                //setup modulation settings + low frequency

    //--configure pins for UART A0 RX
    P1SEL1 &= ~BIT6;                   //setup P1.6 to use UART A0 RX instead of digital I/O
    P1SEL0 |= BIT6;                    //P1SEL1.6 : P1SEL0.6 = 01

    //--configure pins for UART A0 TX
    P1SEL1 &= ~BIT7;                   //setup P1.7 to use UART A0 TX instead of digital I/O
    P1SEL0 |= BIT7;                    //P1SEL1.7 : P1SEL0.7 = 01

    UCA0CTLW0 &= ~UCSWRST;             //take UART A0 out of SW reset
}

void P4_Setup(void)
{
    // Configure pins P4.0, P4.4, P4.6, P4.7 as inputs
    P4DIR &= ~(BIT0 | BIT4 | BIT6 | BIT7);

    //enable the built in resistors of pins P4.0, P4.4, P4.6, P4.7
    P4REN |= BIT0 | BIT4 | BIT6 | BIT7;

    //make the resistors of pins P4.0, P4.4, P4.6, P4.7 as Pull-up
    P4OUT |= BIT0 | BIT4 | BIT6 | BIT7;

    //make P4.0, P4.4, P4.6, P4.7 IRQs sensitivity High-to-Low
    P4IES |= BIT0 | BIT4 | BIT6 | BIT7;

    //--enable IRQs for pins P4.0, P4.4, P4.6, P4.7
    P4IE |= BIT0 | BIT4 | BIT6 | BIT7;

    //clear flags of P4.0, P4.4, P4.6, P4.7
    P4IFG &= ~(BIT0 | BIT4 | BIT6 | BIT7);
}
