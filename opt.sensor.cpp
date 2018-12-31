

SOFTWARE DESIGN
Pulse measuring algorithm
In the program, the high levels of signal are detected to count the number of heart beats. 
When one rising edge of signal appear, start timing. vol=int(i/t. read()*60 is used to calculate the pulse rate, and i means the number of beats in 5s(t .read()<=5). The reason why 5s is chosen as a loop is that this period can make the measure more accurate, and the victims also do not need long time to get the result. 
In addition, the unhealthy rate of heart beats can also be reminded in our design. If the rate of pulse is less than 60, LED1 will be turned on. If the rate of pulse is larger than 100, LED3 will be turned on. The relative code is given as follow.


#include "mbed.h"

#define driver_no_op 0x00;
#define driver_digit0 0x01
#define driver_digit1 0x02
#define driver_digit2 0x03
#define driver_digit3 0x04
#define driver_digit4 0x05
#define driver_digit5 0x06
#define driver_digit6 0x07
#define driver_digit7 0x08
#define driver_decodemode 0x09
#define driver_intensity 0x0A
#define driver_scanLimit 0x0B
#define driver_shutdown 0x0C
#define driver_displaytest 0x0F

SPI max7221(p11,p12,p13);
DigitalOut load(p14);
DigitalOut hundred1(LED1);
DigitalOut hundred2(LED2);
DigitalIn sign(p15);

void Display(int reg,int dat){
    load = 0;
    max7221.write(reg);   //choose registers
    max7221.write(dat);   //put registers
    load = 1 ;
    }

void print(int pulse){
    
    int hundred,ten,unit;
    hundred=pulse/100;
    ten=(pulse%100)/10;
    unit=pulse%10;   //calculate hundred,ten and unit
                    
    if (hundred==1){
        hundred1=1;
        }
        else{
            hundred1=0;
            }    //the LED1 on when the pulse more than 100
            
    if (hundred==2){
        hundred2=1;
        }
        else{
            hundred2=0;
            }    //the LED2 on when the pulse more than 200
    
    switch(ten){    //define ten number on 8x8 LED
            case 0:
                Display (5,0); 
                Display (6,0);
                Display (7,0);
                Display (8,0);break;
            case 1:
                Display (5,0); 
                Display (6,0);
                Display (7,255);
                Display (8,0);break;
            case 2:
                Display (5,241); 
                Display (6,145);
                Display (7,145);
                Display (8,159);break;
            case 3:
                Display (5,255); 
                Display (6,145);
                Display (7,145);
                Display (8,145);break;
            case 4:
                Display (5,16); 
                Display (6,255);
                Display (7,16);
                Display (8,240);break;
            case 5:
                Display(8,241); 
                Display(7,145);
                Display(6,145);
                Display(5,159);break;
            case 6:
                Display(5,143);
                Display(6,137);
                Display(7,137);
                Display(8,255);
            case 7:
                Display(5,255); 
                Display(6,128);
                Display(7,128);
                Display(8,128);break;
            case 8:
                Display(5,255); 
                Display(6,145);
                Display(7,145);
                Display(8,255);break;
            case 9:
                Display(5,255); 
                Display(6,145);
                Display(7,145);
                Display(8,241);break;
           default:break;}
        switch(unit){    //define unit number on 8x8 LED
           case 0:
                Display (1,255); 
                Display (2,129);
                Display (3,129);
                Display (4,255);break;
            case 1:
                Display (1,0); 
                Display (2,0);
                Display (3,255);
                Display (4,0);break;
            case 2:
                Display (1,241); 
                Display (2,145);
                Display (3,145);
                Display (4,159);break;
            case 3:
                Display (1,255); 
                Display (2,145);
                Display (3,145);
                Display (4,145);break;
            case 4:
                Display (1,16); 
                Display (2,255);
                Display (3,16);
                Display (4,240);break;
            case 5:
                Display(1,241); 
                Display(2,145);
                Display(3,145);
                Display(4,159);break;
            case 6:
                Display(1,255); 
                Display(2,137); 
                Display(3,137);
                Display(4,143);break;
            case 7:
                Display(1,255); 
                Display(2,128);
                Display(3,128);
                Display(4,128);break;
            case 8:
                Display(1,255); 
                Display(2,145);
                Display(3,145);
                Display(4,255);break;
            case 9:
                Display(1,255); 
                Display(2,145);
                Display(3,145);
                Display(4,241);break;
           default:break;}
    }
    

int main() {
    
    max7221.format(8,0);      // SPI setup: 8 bits, mode 0
    max7221.frequency(100000);
    
    Display(driver_scanLimit, 0x07);    //initiation of the max 7219
    Display(driver_decodemode,0x00);    
    Display(driver_intensity, 0x0F);
    Display(driver_shutdown, 0x01);    
    Display(driver_displaytest, 0x00);
    
    int i=0,point=0,pulse=0;    
    Timer  t;    //initiation of the Timer t
    t.start();    //start timing
    
    while(1){      
        while (t.read()<=5){ 
            while(1){
                if (sign<0.5)
                t.reset();
                if (sign>0.5)    //start count when a high level sign appearing 
                i=0,point=0;
                break;
                    }
            while(t.read()<=5){   //if the time <= 5s
                    if ((sign>0.5)&&(point==0))
                    point=1;
                    if ((sign<0.5)&&(point==1))
                    i++;
                    point=0;    //count 1 when a voltage becomes high from low
                    }
            
            pulse=int(i/t.read()*60);    //calculate the heart rate
            print(pulse);    //use the print function shows the pulse on 8x8 LED
            t.reset();     //again the timing
            }  
        }          
    }
