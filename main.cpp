#include "mbed.h"
#include "MQ9.h"
#include "DS1820.h"

Serial pc(USBTX, USBRX);

DigitalOut green(D6);
DigitalOut red(D4);
AnalogIn fire5(A5);
AnalogIn fire4(A4);
AnalogIn fire3(A3);
AnalogIn fire2(A2);
AnalogIn fire1(A1);

DigitalOut gasHeater(D5);
AnalogIn gas(A0);

MQ9 gasSensor(A0);

DS1820 tempSensor(D8);

//MQ9 gasSensor(##);
//DS1820 ds1820(##);

//Тревога = мигает красный светодиод
void alarm(int fire){
    if(fire){
        for (int i=0; i<5;++i){
            green = 0;
            red = 1;
            wait_ms(50);
            red = 0;
            wait_ms(50);
        }
        pc.printf("\n===============================\n"
                  "    Attention!!! FIRE ATTACK !!!"
                  "\n===============================\n");
    }
    
}
//Все хорощо = мигает зеленый светодиод
void success(int fire){
    if(!fire){
        red = 0;
        green = 1;
        wait_ms(500);
        green = 0;
        wait_ms(500);
    }
}

int main (){
    pc.baud(115200);
    pc.printf("\nHello! This is Fire Safety Sensor by Vlad Abramov!\n");
    float temp = 0;
    int result = 0;
    int fire = 0;
    int gasHeat = 0;
    tempSensor.begin();
    while(1){
        // Устраняем перегрев датчика дыма
        if (gasHeat/2 == 0) {gasHeater = true;}
            else {gasHeater = false;}
            
        // Собираем показания с датчика пламени    
        pc.printf("\nKY-026 sensor:");
        pc.printf("\n Fire sensor #1: %0.2f"
                  "\n Fire sensor #2: %0.2f"
                  "\n Fire sensor #3: %0.2f"
                  "\n Fire sensor #4: %0.2f"
                  "\n Fire sensor #5: %0.2f \n",fire1.read()*5000,fire2.read()*5000,fire3.read()*5000,fire4.read()*5000,fire5.read()*5000);
        // Проверка есть ли огонь          
        if ((fire5*5000 > 2000.0f) || (fire4*5000 > 2000.0f) || 
            (fire3*5000 > 4000.0f) || (fire2*5000 > 2000.0f) ||
            (fire1*5000 > 2000.0f)){fire = 1;}
        success(fire);
        alarm(fire);
        
        //Собираем показания датчика дыма
        gasSensor.read();
        pc.printf("\nMQ9 sensor:");
        pc.printf("\n CO ppm:%3.10f"
                  "\n LPG ppm:%3.10f"
                  "\n Methane ppm:%3.10f \n",gasSensor.getCO_ppm(), gasSensor.getLPG_ppm(), gasSensor.getMethane_ppm());
        
        //Проверка есть ли дым
        
        success(fire);
        alarm(fire);
    
        pc.printf("\nDS1820 sensor:");
            tempSensor.startConversion();   // start temperature conversion from analog to digital
            wait(1.0);                  // let DS1820 complete the temperature conversion
            result = tempSensor.read(temp); // read temperature from DS1820 and perform cyclic redundancy check (CRC)
            switch (result) {
                  case 0:                 // no errors -> 'temp' contains the value of measured temperature
                      pc.printf("\n temp = %3.1f%cC\r\n", temp, 176);
                      break;
  
                  case 1:                 // no sensor present -> 'temp' is not updated
                      pc.printf("\n no sensor present\n\r");
                      break;
  
                  case 2:                 // CRC error -> 'temp' is not updated
                      pc.printf("\n CRC error\r\n");
              }
        
        success(fire);
        alarm(fire);
    }
}
