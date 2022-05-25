#include "mbed.h"
#include "bbcar.h"
#include <cstdio>

DigitalOut led1(LED1);
Ticker servo_ticker;
PwmOut pin5(D5), pin6(D6);
DigitalInOut pin10(D10);
Thread turner;
BBCar car(pin5, pin6, servo_ticker);
float data[120];

void turn()
{
    while(1)
    {
        car.rotation1(20);
        ThisThread::sleep_for(3s);
        car.rotation1(-20);
        ThisThread::sleep_for(3500ms);
    }
}

int main() 
{
    ThisThread::sleep_for(3000ms);
    parallax_ping  ping1(pin10);
    car.rotation1(-20);
    ThisThread::sleep_for(2000ms);
    turner.start(turn);
    int flag = 0;
    float edge = 5.0;
    while(1) 
    {
        for (int i = 0; i < 100; i++)
        {
            ThisThread::sleep_for(50ms);
            data[i] = float(ping1) - 7.0;
        }
        if (flag == 1)
        {
            float min = data[0];
            int minidx = 0;
            for (int i = 1; i < 60; i++)
            {
                if (data[i] < min)
                {
                    min = data[i];
                    minidx = i;
                }
            }
            for (int i = minidx; i < minidx + 20; i++)
            {
                if (data[i+1] - data[i] > 2)
                {
                    edge = data[i];
                    break;
                }
            }
            float width;
            width = sqrt(abs(edge*edge-min*min))  / 2;
            printf("%lf\r\n",width);
            flag = 0;
        }
        flag++;
    }
}

