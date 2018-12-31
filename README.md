

Glasgow College, UESTC
Project Report
Student Name：   Desire               Student No.：2015200201009
	
	Instructor:            Kathleen           
                                   
Project name：Opticcal pulse  sensor



Introduction 

The aim of this project is to design a pulse meter, which can be used to calculate the heart rate. This is done by measuring the reflection of light caused by the flow of blood. 
This report has the whole process of my design, block diagrams, circuit schematics, tested results, PCB layout, software code, etc.

The pulse meter is widely used in the medical field, this project is based on the principle of photoplethysmography (PPG) which is a non-invasive method of measuring the variation in blood volume in tissues using a light source and a detector. Since the change in blood volume is synchronous to the heart beat, this technique can be used to calculate the heart rate.
The use of TCRT100 simplifies the build process of the sensor part of the project as both the infrared light emitter diode and the detector are arranged side by side in a leaded package, thus blocking the surrounding ambient light, which could otherwise affect the sensor performance. This sensor is sensitive to the reflection of light caused by the flow of blood, then convert it to the digital signal. The output pulse can be fed to either an ADC channel or a digital input pin of a microcontroller for further processing and retrieving the heart rate in beats per minute (BPM). Finally, the number of pulses display on the LED screen.

Overall diagram

 
 
Fig.1. The block diagram for both analogue and digital parts.





        TCRT1000 used for sensing pulse from fingertip
  the signal from the sensor without using the filters.


As shown from the figure above TCRT1010 can detect the reflection of light caused by the flow of blood and output different voltage. According to the datasheet fig.4., I_F=20mA   I_C=0.5mA. Hence, in theory,  R_9=5V/20mA=150Ω R_10=5V/0.5mA=10kΩ .However, after testing, R_9=100Ω R_10=10kΩ are used in the circuit, because they more meet the requirements of currents. 
/////////////As we know, an ordinary person’s pulse is between 30/min and 180/min. So the frequency we should set is between 0.5Hz and 3Hz, but in order to reduce the noise and make the waveform more obvious, we will limit the frequency range of 0.1HZ to 10HZ. 

The output (VSENSOR) from the sensor is a periodic physiological waveform attributed to small variations in the reflected IR light which is caused by the pulsatile tissue blood volume inside the finger. The waveform is, therefore, synchronous with the heartbeat. The following circuit diagram describes the first stage of the signal conditioning which will suppress the large DC component and boost the weak pulsatile AC component, which carries the required information.
 
In the circuit shown above, the sensor output is first passed through a RC high-pass filter (HPF) to get rid of the DC component. The cut-off frequency of the HPF is set to 0.7 Hz. Next stage is an active low-pass filter (LPF) that is made of an Op-Amp circuit. The gain and the cut-off frequency of the LPF are set to 101 and 2.34 Hz, respectively. Thus the combination of the HPF and LPF helps to remove unwanted DC signal and high frequency noise including 60 Hz (50 Hz in some countries) mains interference, while amplifying the low amplitude pulse signal (AC component) 101 times.
 
The output from the first signal conditioning stage goes to a similar HPF/LPF combination for further filtering and amplification (shown below). So, the total voltage gain achieved from the two cascaded stages is 101*101 = 10201. The two stages of filtering and amplification converts the input PPG signals to near TTL pulses and they are synchronous with the heart beat. The frequency (f) of these pulses is related to the heart rate (BPM) as,
Beats per minute (BPM) = 60*f
Display

MAX7221 LED driver and 8×8 LED Display are chosen to display the rate of heart beats. The advantages of MAX7221 are given as follow. Firstly, the numbers displayed are clear and this chip’s principle is easy to understand. However, in my point of view, it is easier for me to use LCD displayer, because it is convenient to use the library of LCD, and for MAX7221, find which LEDs are used in different number and calculate them are very complex.
Between the mbed and MAX7221 LED driver, there is a 74HCT04  including 6 inverters. The reasons why it is used is that they can convert the voltage from 3.3V to 3.5V, then input to MAX7221.

REGULATOR 
According to the datasheet of sensor, the requirement voltage is 5V. In the laboratory, only 5V batteries can be found. Thus, the voltage regulator is used between the battery and sensor.


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






