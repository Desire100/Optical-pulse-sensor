                                     
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


