#include "mbed.h"
#include "QEI.h"

Ticker pen_control;
AnalogIn potentio(PA_0);
DigitalIn sw(USER_BUTTON);
DigitalOut led(LED1);
PwmOut maxon_r(PB_3);
PwmOut maxon_l(PB_4);
PwmOut maxon_sd(PB_5);

const float pi = 3.1415926535f;
const float sampling_time = 0.01f; //10ms
const float adv_to_rad = 2.0f * pi * (333.0f / 360.0f) * (1.0f / 4096.0f);

float target_theta = pi;

float x0 = 0.f;
float theta0 = 0.f;
float thetai = 0.f;

float kp = 30.0;
float ki = 0.5;
float kd = 0.5;

void pen_control_handler()
{
    int adv = potentio.read_u16() >> 4; //stm32f401 adv: 12bit
    float theta = target_theta - adv * adv_to_rad;
    float dtheta = (theta - theta0) / sampling_time;
    theta0 = theta;
    thetai += theta * sampling_time;
    printf("theta: %lf\t\t", theta * 180.0f / pi);
    
    if(thetai > 10000.0f){
        thetai = 10000.0f;
    }
    else if(thetai < -10000.0f){
        thetai = -10000.0f;
    }
    
    float duty_ratio = theta * kp + thetai * ki + dtheta * kd;
    printf("dr: %3lf\t", duty_ratio);
    
    if(duty_ratio > 0.3f){
        duty_ratio = 0.3f;
    }
    else if(duty_ratio < -0.3f){
        duty_ratio = -0.3f;
    }
    
    printf("(%lf)\r\n", duty_ratio);
    
    if(duty_ratio >= 0.0f){
        maxon_r = duty_ratio;
        maxon_l = 0.0f;
    }
    else{
        maxon_r = 0.0f;
        maxon_l = -duty_ratio;
    }
}

int main()
{
    led = 1;
    
    sw.mode(PullUp);
    
    maxon_r.period_us(100);
    maxon_l.period_us(100);
    maxon_sd = 1.0f;
    maxon_r = 0.0f;
    maxon_l = 0.0f;

    printf("\r\nwaiting start signal...\r\n");

    while(sw){
        led = led ^ 1;
        wait(1.0f);
    }

    target_theta = (potentio.read_u16() >> 4) * adv_to_rad;
    printf("target_theta: %lf\r\n", target_theta * 180.0f / pi);
    
    wait(1.0f);
    
    pen_control.attach(&pen_control_handler, sampling_time);
    
    while(1){
        wait(1.0f);
    }
}

