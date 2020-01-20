/*
    File:        main.c
    Description: This is module created to check minimal configuration
                 of OLED display 128x32 based on ssd1306
    Assumptions: Module is created to work with at least
                 Raspberry Pi module Revision A
    Created:     Tueasday January 20, 2020, 10:19:14
    Author:      Michal Wolowik
    Email:       m.wolowik@emsyslabs.com
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include <termios.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <time.h>

#include <wiringPi.h>
#include <wiringSerial.h>

int fd_mdm;
int fd_gps;
int errno;

#define LED_RED         21
#define LED_GREEN       25

PI_THREAD(thread_led_green)
{
    while(1)
    {
        digitalWrite(LED_GREEN, HIGH);
        delay(500);
        digitalWrite(LED_GREEN, LOW);
        delay(500);
    }

    return 0;
}

PI_THREAD(thread_led_red)
{
    while(1)
    {
        digitalWrite(LED_RED, HIGH);
        delay(2500);
        digitalWrite(LED_RED, LOW);
        delay(2500);
    }

    return 0;
}

PI_THREAD(thread_ds18b20_temperature)
{
    #define  BUFSIZE  128
    #define TEMPSIZE 7
    char buf[BUFSIZE];
    char temp[TEMPSIZE];
    int fd;

    printf("Temperature thread started!!!\n");

    while(1){
        fd = open("/sys/bus/w1/devices/28-051700dadeff/w1_slave", O_RDONLY);
        if(fd == -1){
            perror("open device file error");
            break;
        }
        memset(buf, 0, BUFSIZE);
        read(fd, buf, BUFSIZE);
        close(fd);
        memset(temp, 0, TEMPSIZE);
        temp[0] = buf[67];
        temp[1] = buf[68];
        temp[2] = buf[69];
        temp[3] = buf[70];
        temp[4] = ',';
        temp[5] = buf[71];
        temp[6] = 'C';
        printf("Current temperature %s\n",temp);
        delay(100);
    }
    return 0;
}


int main(int argc, char **argv)
{
    int x;
    char buffer[100];

    struct tm *timeinfo ;
    time_t rawtime ;
    rawtime = time (NULL);
    timeinfo = localtime(&rawtime);

    wiringPiSetupGpio();

    pinMode(LED_RED, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);

    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, HIGH);

    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, LOW);


    /* Start modem thread */
    x = piThreadCreate(thread_led_red);

    if(!x)
        printf("Step 1. Led Red thread started\n");

    x = piThreadCreate(thread_led_green);

    if(!x)
        printf("Step 2. Led Green thread started\n");

    x = piThreadCreate(thread_ds18b20_temperature);

    if(!x)
        printf("Step 3. Temperature thread started\n");

    while(1){
        delay(500);
    }
}

