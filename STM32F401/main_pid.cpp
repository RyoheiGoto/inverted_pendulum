#include "mbed.h"
#include "QEI.h"

#define PI 3.1415926535

Ticker pen_control;
AnalogIn potentio(PA_0);
DigitalOut led(LED1);
PwmOut maxon_r(PB_3);
PwmOut maxon_l(PB_4);
PwmOut maxon_sd(PB_5);
QEI enc(PA_8, PA_9, NC, 1024 * 35, QEI::X4_ENCODING); //TODO fix
BusIn in(PA_8, PA_9);

//const float move_per_pulse = 0.00000961666996597224f; //TODO
const float sampling_time = 0.01f; //10ms
const float target_theta = 180.0f / 180.0f * (float)PI; //TODO
const float adv_to_rad = 2.0f * (float)PI * (333.0f / 360.0f) * (1.0f / 4096.0f);
float x0 = 0.f;
float theta0 = 0.f;
float thetai = 0.f;

float kp = 30.0;
float ki = 0.5;
float kd = 0.5;

void pen_control_handler()
{
    //float x = enc.getPulses() * move_per_pulse;
    //float dx = (x - x0) / sampling_time;
    //x0 = x;
    //enc.reset();
    //printf("x: %lf\t", x);
    
    int adv = potentio.read_u16() >> 4;
    float theta = target_theta - adv * adv_to_rad;
    float dtheta = (theta - theta0) / sampling_time;
    theta0 = theta;
    thetai += theta * sampling_time;
    printf("theta: %5lf\t\t\t", theta * 180.0f / (float)PI);
    
    if(thetai > 10000){
        thetai = 10000;
    }
    else if(thetai < -10000){
        thetai = -10000;
    }
    
    float duty_ratio = theta * kp + thetai * ki + dtheta * kd;
    printf("dr: %3lf\t", duty_ratio);
    
    if(duty_ratio > 0.3f){
        duty_ratio = 0.3f;
    }
    else if(duty_ratio < -0.3f){
        duty_ratio = -0.3f;
    }
    
    printf("(%3lf)\r\n", duty_ratio);
    
    if(duty_ratio > 0){
        maxon_r = duty_ratio;
        maxon_l = 0.f;
    }
    else{
        maxon_r = 0.f;
        maxon_l = -duty_ratio;
    }
}

int main()
{
    led = 1;
    in.mode(PullUp);
    maxon_r.period_ms(100);
    maxon_l.period_ms(100);
    maxon_sd = 1.f;
    
    maxon_r = 0.f;
    maxon_l = 0.f;
    
    pen_control.attach(&pen_control_handler, sampling_time);
    
    while(1){
        wait(0.1);
    }
}

