#include "mbed.h"
#include "HEPTA_EPS.h"
#define MAG 0x13<<1 //addr_compus

RawSerial pc(USBTX,USBRX,9600);
HEPTA_EPS eps(p16,p26);
I2C i2c(p28,p27);

float magnet[3];
char cmd[2]={0};
char send[1], get[1];
int data[8]={0};
char temp; //temperature

int main() {
    i2c.frequency(100000);
    pc.printf("magnet setting\r\n");
    cmd[0]=0x4B; 
    cmd[1]=0x01;
    i2c.write(MAG,cmd,2);
    cmd[0]=0x4C; 
    cmd[1]=0x00;
    i2c.write(MAG,cmd,2);
    cmd[0]=0x4E; 
    cmd[1]=0x84;
    i2c.write(MAG,cmd,2);
    cmd[0]=0x51; 
    cmd[1]=0x04;
    i2c.write(MAG,cmd,2);
    cmd[0]=0x52; 
    cmd[1]=0x16;
    i2c.write(MAG,cmd,2);
    cmd[0]=0x00;
    i2c.write(MAG,cmd,1,1);
    i2c.read(MAG,cmd,1);
    while(1) {
        for(int i=0;i<8;i++){
        send[0]=(char)(0x42+i);
        i2c.write(MAG,send,1);
        i2c.read(MAG,get,1);
        temp=get[0];
        data[i]=temp;
        }
        for(int i=0;i<3;i++){
            if(i!=2)magnet[i]=(int16_t)(((int16_t)data[i*2+1]<<8) | data[i*2]) >> 3;
            else magnet[i]=(int16_t)(((int16_t)data[i*2+1]<<8) | data[i*2]) >> 1;
            if(i==2 && magnet[i]>16383)magnet[i]-=32768;
            else if(i!=2 && magnet[i]>4095)magnet[i]-=8092;
        }
        pc.printf("mx = %2.4f, my = %2.4f, mz = %2.4f\r\n\n",magnet[0],magnet[1],magnet[2]);
        wait(1);
    }
}