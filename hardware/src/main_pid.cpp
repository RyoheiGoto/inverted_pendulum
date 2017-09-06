#include "mbed.h"
#include "QEI.h"

Serial pc(USBTX, USBRX);
Ticker pen_control;
AnalogIn potentio(PA_0);
DigitalIn sw(USER_BUTTON);
DigitalOut led(LED1);
DigitalOut maxon_sd(PB_5);
PwmOut maxon_l(PB_3);
PwmOut maxon_r(PB_4);

const float pi = 3.1415926535f;
const float sampling_time = 0.01f; //10ms
const float adv_to_rad = 2.0f * pi * (333.3f / 360.0f) * (1.0f / 4096.0f);

float target_theta = 269.6f / 180.0f * pi;

float theta0 = 0.0f;
float thetai = 0.0f;

float theta_lpf = 0.0f;
float lpf_gain = 0.3f;

float kp = 0.0f;
float ki = 0.0f;
float kd = 0.0f;

float low_pass_filter(float val, float pre_val, float gamma)
{
    return gamma * pre_val + (1.0f - gamma) * val;
}

void pen_control_handler()
{
    int adv = potentio.read_u16() >> 4; //stm32f401 adv: 12bit
    float theta = low_pass_filter((float)(adv * adv_to_rad), theta_lpf, lpf_gain);
    theta_lpf = theta;
    
    theta = target_theta - theta;
    float dtheta = (theta - theta0) / sampling_time;
    theta0 = theta;
    thetai += theta * sampling_time;
    //printf("theta: %lf\t\t", theta * 180.0f / pi);
    
    if(thetai > 10000.0f){
        thetai = 10000.0f;
    }
    else if(thetai < -10000.0f){
        thetai = -10000.0f;
    }
    
    float duty_ratio = theta * kp + thetai * ki + dtheta * kd;
    //printf("dr: %3lf\t", duty_ratio);
    
    if(duty_ratio > 0.2f){
        duty_ratio = 0.2f;
    }
    else if(duty_ratio < -0.2f){
        duty_ratio = -0.2f;
    }
    
    //printf("(%lf)\r\n", duty_ratio);
    
    if(fabs(theta * 180.0f / pi) > 45){
        maxon_r = 0.0f;
        maxon_l = 0.0f;
        return;
    }
    
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
    
    maxon_sd = 1;
    maxon_r.period_us(100);
    maxon_l.period_us(100);
    maxon_r = 0.0f;
    maxon_l = 0.0f;

    printf("\r\nwaiting start signal...\r\n");

    while(sw){
        led = led ^ 1;
        wait(1.0f);
    }

#if 1
    target_theta = (potentio.read_u16() >> 4) * adv_to_rad;
    printf("target_theta: %lf\r\n", target_theta * 180.0f / pi);
#endif

    wait(1.0f);
    
    pen_control.attach(&pen_control_handler, sampling_time);
    
    while(1){
        while(pc.readable()){
            char buf = pc.getc();
            switch(buf){
                case 'q':
                    kp += 0.05f;
                    printf("kp: %lf\tki: %lf\tkd: %lf\r\n", kp, ki, kd);
                    break;
                case 'a':
                    kp -= 0.05f;
                    printf("kp: %lf\tki: %lf\tkd: %lf\r\n", kp, ki, kd);
                    break;
                case 'w':
                    ki += 0.0005f;
                    printf("kp: %lf\tki: %lf\tkd: %lf\r\n", kp, ki, kd);
                    break;
                case 's':
                    ki -= 0.0005f;
                    printf("kp: %lf\tki: %lf\tkd: %lf\r\n", kp, ki, kd);
                    break;
                case 'e':
                    kd += 0.0005f;
                    printf("kp: %lf\tki: %lf\tkd: %lf\r\n", kp, ki, kd);
                    break;
                case 'd':
                    kd -= 0.0005f;
                    printf("kp: %lf\tki: %lf\tkd: %lf\r\n", kp, ki, kd);
                    break;
                case 'r':
                    target_theta += 0.001f;
                    printf("target_theta: %lf\r\n", target_theta * 180.0f / pi);
                    break;
                case 'f':
                    target_theta -= 0.001f;
                    printf("target_theta: %lf\r\n", target_theta * 180.0f / pi);
                    break;
                case 't':
                    lpf_gain += 0.1f;
                    if(lpf_gain > 1){
                        lpf_gain = 1.0f;
                    }
                    printf("lpf_gain: %lf\r\n", lpf_gain);
                    break;
                case 'g':
                    lpf_gain -= 0.1f;
                    if(lpf_gain < 0){
                        lpf_gain = 0.0f;
                    }
                    printf("lpf_gain: %lf\r\n", lpf_gain);
                    break;
            }
        }
        wait(0.5f);
    }
}

