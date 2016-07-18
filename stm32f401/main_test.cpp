#include "mbed.h"
#include "QEI.h"

DigitalOut led(LED1);
PwmOut maxon_r(PB_3);
PwmOut maxon_l(PB_4);
PwmOut maxon_sd(PB_5);
AnalogIn potentio(PA_0);
QEI enc(PA_9, PA_8, NC, 1024 * 35, QEI::X4_ENCODING);

int main()
{
    printf("test start\r\n");
    led = 1;
    
#if 0
    enc.reset();
    
    while(1){
        printf("potentio: %lf\r\n", potentio.read());
        printf("enc current state: %d\r\n", enc.getCurrentState());
        printf("enc pulses: %d\r\n", enc.getPulses());
        printf("enc revolutions: %d\r\n", enc.getRevolutions());
        wait(0.5);
    }
#else
    maxon_r.period_us(100);
    maxon_l.period_us(100);
    maxon_sd.write(1.0);
    
    while(1){
        //maxon_r.write(potentio.read());
        maxon_r.write(0.3);
        maxon_l.write(0.0);
        wait(3.0);
        maxon_r.write(0.0);
        maxon_l.write(0.3);
        wait(3.0);
    }
#endif
}

