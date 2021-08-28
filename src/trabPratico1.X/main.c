
// CONFIG
#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#define _XTAL_FREQ 4000000 // 4MHz
unsigned int setPoint;
long int num_dac;
long int dutyCycle;
long int pi;
int dezena;
int unidade;


#include "lcd.h"
#include"serial.h"
#include "itoa.h"
#include "adc.h"
#include "eeprom.h"
#include "function.h"



void main(void) {
    unsigned int flag = -1;
    unsigned int setU = setPoint%10;
    unsigned int setD = (setPoint-setU)/10;
    
    
    //desabilitando comparadores para as portas RA0, RA1, RA2 e RA3 possam ser utilizadas como I/O digitais.    
    CMCONbits.CM0=1;
    CMCONbits.CM1=1;
    CMCONbits.CM2=1;

    TRISCbits.TRISC7 = 1; //RX
    TRISCbits.TRISC1 = 0; //RX
    TRISCbits.TRISC2 = 0; //RX
    TRISCbits.TRISC6 = 0; //TX
    
    /*Botões RB*/
    TRISBbits.TRISB1 = 1;
    TRISBbits.TRISB2 = 1;
    
    /*Entrada sinal analódico temp*/
    TRISAbits.TRISA1 = 0;
    TRISAbits.TRISA0 = 1;
    
    // LCD
    TRISD=0x00;
    TRISEbits.TRISE0=0;
    TRISEbits.TRISE1=0;
    
    // TRISx
    TRISAbits.TRISA1=1;     // RA0 INPUT
    
    PORTCbits.RC1 = 0; 
    PORTCbits.RC2 = 0; 

    PORTBbits.RB1 = 1; 
    PORTBbits.RB2 = 1;
    
    lcd_init();

    // ADC CONFIG
    ADCON1bits.ADCS2 = 1;
    ADCON0bits.ADCS1 = 1;
    ADCON0bits.ADCS0 = 0;// FOSC/64
    
    // Analog Channel Select bits. (AN1-RA0) = POT2, (AN2-RA2) - Usar Gerador
    ADCON0bits.CHS = 0b000; // Channel 1 (AN1) - RA0
    
    // A/D Result Format Select bit
    ADCON1bits.ADFM = 1; // Right justified. Six (6) Most Significant bits of ADRESH are read as ?0?.
    
    // A/D Port Configuration Control bits
    ADCON1bits.PCFG = 0b0010;
    
    // A/D On bit
    ADCON0bits.ADON = 1;
    
    /*pwm*/
    CCP1CON = 0x0F; // Select the PWM mode.
    PR2 = 0xF9; // PR2 = 249
    CCPR1L = 0; // By default set the dutyCycle to 0
    T2CON = 0x01; // prescaler 1:4
    TMR2ON = 1; //Start the Timer for PWM generation
    // PWM frequency = (FOSC/4)/PRESCALER/(PR2+1).
    // PWM period = 1/PWM frequency
    
    //gravar dados na EEPROM
    
       //eeprom_write(0x0A, (2+0x30));			// Write 0xFF-k at k address location
       //int a = eeprom_read(0x0A);
    //setU = lerEEPROM(0x1A);
    
    lcd_cmd(L_CLR);
    lcd_cmd(L_L1);
    lcd_str("Pressione Para");
    lcd_cmd(L_L2);
    lcd_str("Continuar...");
    //lcd_dat(setPoint + 0x30);
    __delay_ms(20);
    //lcd_cmd(L_CLR);
    //lcd_cmd(L_L1);
    //lcd_dat(a);
    //__delay_ms(1000);
    
    while (1) {
        while(RB1 == 1 && RB2 == 1);

       if(flag == -1){
            flag = telaGravarEEPROM();
        }       
        
        if(flag == 0){
            flag = telaInicio();
        }
        else if(flag == 2){
            telaSetPoint();
            flag = 0;
            armazenarSet();
        }
        else if(flag == 1){
            armazenarSet();
            flag = telaControlador();
        }
    }
}
