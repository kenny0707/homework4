#include "mbed.h"
#include "bbcar.h"
#include <cstdio>
BufferedSerial pc(USBTX, USBRX);

Ticker servo_ticker;
Ticker encoder_ticker;
PwmOut pin5(D5), pin6(D6);
DigitalIn encoder(D11);
DigitalInOut FR(D2), R(D3), L(D4), FL(D7);
volatile int steps;
volatile int last;

BBCar car(pin5, pin6, servo_ticker);
void encoder_control() {
   int value = encoder;
   if (!last && value) steps++;
   last = value;
}

int main() {
    float distance = 0;
   pc.set_baud(9600);
   steps = 0;
   last = 0;
   while (1) {
        FR.output();
        FL.output();
        R.output();
        L.output();
        FR = 1;
        R = 1;
        L = 1;
        FL = 1;
        wait_us(230);
        FR.input();
        FL.input();
        R.input();
        L.input();
        wait_us(230);
        int pattern = FL * 1000 + L * 100 + R * 10 + FR;
        if (pattern == 1000) {
            car.turn(200, 0.0000000001);
            pin5 = 0;
        }
        else if (pattern == 1100) {
            car.turn(100, 0.0001);
        }
        else if (pattern == 100) {
            car.turn(160, 0.7);
        }
        else if (pattern == 110) {
            car.goStraight(120);
        }
        else if (pattern == 10) {
            car.turn(160, -0.4);
        }
        else if (pattern == 11) {
            car.turn(200, -0.00000001);
        }
        else if (pattern == 1) {
            car.rotation1(200);
            pin6 = 0;
        } 
        else if (pattern == 1111){
            car.stop();
            break;
        }
        else car.goStraight(150);
    }
    encoder_ticker.attach(&encoder_control, 1ms);
    ThisThread::sleep_for(5s);
    car.goStraight(120);
    ThisThread::sleep_for(200ms);
    while (1) {
        FR.output();
        FL.output();
        R.output();
        L.output();
        FR = 1;
        R = 1;
        L = 1;
        FL = 1;
        wait_us(230);
        FR.input();
        FL.input();
        R.input();
        L.input();
        wait_us(230);
        int pattern = FL * 1000 + L * 100 + R * 10 + FR;
        if (pattern == 1000) {
            car.turn(200, 0.0000000001);
            pin5 = 0;
        }
        else if (pattern == 1100) {
            car.turn(100, 0.0001);
        }
        else if (pattern == 100) {
            car.turn(160, 0.7);
        }
        else if (pattern == 110) {
            car.goStraight(120);
        }
        else if (pattern == 10) {
            car.turn(160, -0.4);
        }
        else if (pattern == 11) {
            car.turn(200, -0.00000001);
        }
        else if (pattern == 1) {
            car.rotation1(200);
            pin6 = 0;
        } 
        else if (pattern == 1111){
            car.stop();
            distance = steps*6.5*3.14/32;
            break;
        }
        else car.goStraight(150);
    }
    printf("distance = %f\r\n",distance);
    while(1);
}

