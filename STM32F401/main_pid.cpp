#include "mbed.h"
#include "QEI.h"

Ticker pen_control;
AnalogIn potentio(PA_0);
DigitalOut led(LED1);
PwmOut maxon_r(PB_3);
PwmOut maxon_l(PB_4);
PwmOut maxon_sd(PB_5);
QEI enc(PA_8, PA_9, NC, 1024 * 35, QEI::X4_ENCODING); //TODO fix
BusIn in(PA_8, PA_9);

//const float move_per_pulse = 0.f; //TODO
const float sampling_time = 0.01f; //10ms
const float target_theta = 0.f; //TODO
const float adv_to_rad = 0.f; //TODO
float x0 = 0.f;
float theta0 = 0.f;
float thetai = 0.f;

float kp = 0;
float ki = 0;
float kd = 0;

void pen_control_handler()
{
    /*
    float x = enc.getPulses() * move_per_pulse;
    float dx = (x - x0) / sampling_time;
    x0 = x;
    enc.reset();
    */
    
    float theta = target_theta - potentio.read() * adv_to_rad;
    float dtheta = (theta - theta0) / sampling_time;
    theta0 = theta;
    thetai += theta * sampling_time;
    
    if(thetai > 10000){
        thetai = 10000;
    }
    else if(thetai < -10000){
        thetai = -10000;
    }
    
    float duty_ratio = theta * kp + thetai * ki + dtheta * kd;
    
    if(duty_ratio > 1){
        duty_ratio = 1.f;
    }
    else if(duty_ratio < -1){
        duty_ratio = -1.f;
    }
    
    if(duty_ratio > 0){
        maxon_r = duty_ratio;
        maxon_l = 0.f;
    }
    else{
        maxon_r = 0.f;
        maxon_l = duty_ratio;
    }
}

int main()
{
    led = 1;
    in.mode(PullUp);
    maxon_r.period_ms(10);
    maxon_l.period_ms(10);
    
    pen_control.attach(&pen_control_handler, sampling_time);
    
    while(1){
        wait(0.1);
    }
}

