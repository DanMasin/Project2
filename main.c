#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <sys/attribs.h>
#include "config.h"

#include "lcd.h"
#include "ssd.h"
#include "btn.h"
#include "led.h"
#include "swt.h"

#pragma config JTAGEN = OFF     
#pragma config FWDTEN = OFF      


/* ------------------------------------------------------------ */
/*						Configuration Bits		                */
/* ------------------------------------------------------------ */


// Device Config Bits in  DEVCFG1:	
#pragma config FNOSC =	PRIPLL
#pragma config FSOSCEN =	OFF
#pragma config POSCMOD =	XT
#pragma config OSCIOFNC =	ON

// The follwing configuration sets the Peripheral Bus frequency (I/O Devices frequency)
// to be: (CPU_FRQ/FPBDIV). In this case 80MHz/2 = 40MHz.
// Therefore we define PB_FRQ to 40MHz (in config.h file)
#pragma config FPBDIV =     DIV_2

// Device Config Bits in  DEVCFG2:	
// original PLL frequency = 8MHz
// The following configuration sets the CPU_FRQ frequency to ((8/2)*20)/1 = 80MHz
#pragma config FPLLIDIV =	DIV_2
#pragma config FPLLMUL =	MUL_20
#pragma config FPLLODIV =	DIV_1
#define TMR_TIME    0.002 

//void SSD_Timer2Setup()
//{
//  // The following configurations set the PR2 value and the PreScaling factor 
//  PR2 = (int)(((float)(TMR_TIME * PB_FRQ) / 64)); //set period register, generates one interrupt every 2 ms
//  TMR2 = 0;                           //    initialize count to 0
//  T2CONbits.TCKPS = 2;                //    1:64 prescale value
//  
//  T2CONbits.TGATE = 0;                //    not gated input (the default)
//  T2CONbits.TCS = 0;                  //    PCBLK input (the default)
//  T2CONbits.ON = 1;                   //    turn on Timer2
//  IPC1bits.T1IP = 7;                  //    priority
//  IPC1bits.T1IS = 3;                  //    subpriority
//  IFS0bits.T1IF = 0;                  //    clear interrupt flag
//  IEC0bits.T1IE = 1;                  //    enable interrupt
//  macro_enable_interrupts();          //    enable interrupts at CPU
//}
//
// void __ISR(_TIMER_2_VECTOR, ipl2auto) Timer2SR(void)
//{
//   IFS0bits.T1IF = 0; 
//  
//}
void SSD();
// all the Init function we need
void Init(){
    LCD_Init(); 
    SSD_Init();
    BTN_Init();
    SWT_Init();
    LED_Init();
    
    
}
int main(int argc, char** argv) 
{
    Init();
    led_check();     //suppose to get IOreg array and lit the right led, for now checks only at the start but should check every cycle 
    switches_config();
    SSD(); 
    return (1);
}

void SSD()
{  
    unsigned int wCnt = 0, baseCnt = 0, option = 0;
//    while(1){
//    if(baseCnt++ == 1000000)
//        {
//        //display the counter values on the 4 digits available
//            if (option == 0){ // the counter is going
//                SSD_WriteDigitsGrouped(wCnt++, 0); //printing the seconds going up
//            }
//            if (option == 1){ //the counter is holding
//                SSD_WriteDigitsGrouped(wCnt, 0); 
//                if (BTN_GetValue(3)){ //if BTNR is pressed
//                    wCnt++; //going one up
//                }
//            }
//            if (BTN_GetValue(1)){ //if BTNL is pressed
//                if (option == 0){ //if im counting
//                    option = 1;   // put on hold
//                }
//                else{            //if im on hold we continue counting
//                    option = 0;
//                }
//            }    
//            baseCnt = 0;       //reseting 
//        
//      if (BTN_GetValue(2))
//         {
//             wCnt=0;
//             option = 0;
//         }
//  }
//  }
}
    

//*******need to be modified - not working with the timer of the ssd
/* function that checks the switches, works in the interaption method when the  
 */
void switches_config(){
    unsigned int baseCnt = 0;
     SWT_Init();
     //**********interaption method 
     while(1){
         if(++baseCnt == 100000)
         {
             if ((SWT_GetValue(0)==0) && (SWT_GetValue(1)==0)){
                 clear();
                 LCD_WriteStringAtPos("Instruction",0,0);
                 LCD_WriteStringAtPos("PC number",1,0);
             }
             if ((SWT_GetValue(0)==1) && (SWT_GetValue(1)==0)){
                 clear();
                 LCD_WriteStringAtPos("RXX=YYYYYYY",0,0);
                 if (BTN_GetValue(0)){
                    LCD_WriteStringAtPos("BTNU was pressed",1,0); //checking if its pressed
                    //if (atoi(XX)<15){
                        //make stuff
                    //} 
                    //else(){
                        // XX = 0 something like that
                    }
                 }
             if ((SWT_GetValue(0)==0) && (SWT_GetValue(1)==1)){
                 clear();
                 LCD_WriteStringAtPos("MAA=DDDDDDDD",0,0);
                 // checking the SW5 and SW6 to know the value
                 if ((SWT_GetValue(5)==0) && (SWT_GetValue(6)==0)){
                     LCD_WriteStringAtPos("RSP=000",1,0);
                 }
                 if ((SWT_GetValue(5)==1) && (SWT_GetValue(6)==0)){
                     LCD_WriteStringAtPos("RSP=100",1,0);
                 }
                 if ((SWT_GetValue(6)==1)){
                     LCD_WriteStringAtPos("RSP=1FF",1,0);
                 }
                 //if the BTNR pressed need to do +1 to the RSP
                  if (BTN_GetValue(0)){
                     LCD_WriteStringAtPos("BTNU was pressed",1,0);
                     //make something like RSP++ dont forget the cycle!!
                  }
             }
             if ((SWT_GetValue(0)==1) && (SWT_GetValue(1)==1)){
                 clear();
                 LCD_WriteStringAtPos("Both on",0,0);
             }
             baseCnt = 0;
         }
     }
}
//cleaning the LCD screen - not a good one 
void clear(){
     LCD_DisplayClear();
}

void led_check(){             //Checks which led should be on
    char test[7]="0111000";   //a test for now until the IOregisters array will be ready to read from
    int i=0, cur;
    for (i=0; i<7; i++){
        if (test[i]=='1')
            cur=1;
        else cur=0;
        LED_SetValue(i, cur);
    }
}