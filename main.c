#include "stdint.h"
#include "math.h"
#include "C:/Keil/Labware/inc/tm4c123gh6pm.h"

double rad(double x)
{// converting degrees into radians
    return x * 3.14159265359 / 180;
}
double calcdist(double latitude1, double longitude1, double latitude2, double longitude2)
{
    double Y;
    double sdlongitude;
    double cdlongitude;
    double slatitude1;
    double clatitude1;
    double slatitude2;
    double clatitude2;
    double Z;

    Y = rad(longitude1 - longitude2);
    sdlongitude = sin(Y);
    cdlongitude = cos(Y);
    latitude1 = rad(latitude1);
    latitude2 = rad(latitude2);
    slatitude1 = sin(latitude1);
    clatitude1 = cos(latitude1);
    slatitude2 = sin(latitude2);
    clatitude2 = cos(latitude2);
    Y = (clatitude1 * slatitude2) - (slatitude1 * clatitude2 * cdlongitude);
    Y = (Y * Y);
    Y += (clatitude2 * sdlongitude) * (clatitude2 * sdlongitude);
    Y = sqrt(Y);
    Z = (slatitude1 * slatitude2) + (clatitude1 * clatitude2 * cdlongitude);
    Y = atan2(Y, Z);
    return Y * 6372795;
}
void systeminit() {}
void init() {
    //Global ports initialization   
    SYSCTL_RCGCGPIO_R |= 0x23; //Activate port A,B,F
    while(!(SYSCTL_PRGPIO_R&0x23)){};


        //port A initialization
    GPIO_PORTA_LOCK_R = 0x4C4F434B;
    GPIO_PORTA_CR_R = 0XFF;
    GPIO_PORTA_DIR_R = 0xE0;
    GPIO_PORTA_AFSEL_R &= 0x1F;
    GPIO_PORTA_PCTL_R &= ~0xFFF00000;
    GPIO_PORTA_AMSEL_R = 0x0;
    GPIO_PORTA_DEN_R = 0XFF;
    GPIO_PORTA_PUR_R = 0X0;

        //port B initialization
    GPIO_PORTB_LOCK_R = 0x4C4F434B;
    GPIO_PORTB_CR_R = 0XFF;
    GPIO_PORTB_DIR_R = 0xFF;
    GPIO_PORTB_AFSEL_R = 0x0;
    GPIO_PORTB_PCTL_R = 0x0;
    GPIO_PORTB_AMSEL_R = 0x0;
    GPIO_PORTB_DEN_R = 0XFF;
    GPIO_PORTB_PUR_R = 0X0;

        //port F initialization
    GPIO_PORTF_LOCK_R = 0x4C4F434B;
    GPIO_PORTF_CR_R = 0X1F;
    GPIO_PORTF_DIR_R = 0x0E;
    GPIO_PORTF_AFSEL_R = 0x0;
    GPIO_PORTF_PCTL_R = 0x0;
    GPIO_PORTF_AMSEL_R = 0x0;
    GPIO_PORTF_DEN_R = 0X1F;
    GPIO_PORTF_PUR_R = 0X11;

}
void UART0_Init(void){
    SYSCTL_RCGCUART_R |= SYSCTL_RCGCUART_R0;
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R0;

    UART0_CTL_R &= ~UART_CTL_UARTEN;
    //set buad rate devider
    UART0_IBRD_R = 104;
    UART0_FBRD_R = 11;
        UART0_LCRH_R = (UART_LCRH_WLEN_8 | UART_LCRH_FEN);
    UART0_CTL_R |= (UART_CTL_UARTEN | UART_CTL_RXE | UART_CTL_TXE);

    GPIO_PORTA_AFSEL_R |= 0x03;
    GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R & ~0xFF) | (GPIO_PCTL_PA0_U0RX | GPIO_PCTL_PA1_U0TX);
    GPIO_PORTA_DEN_R |= 0x03;
}

char UART0_read(void){
    while((UART0_FR_R&0x10) == 0x10);
    return UART0_DR_R & 0xFF;
}
void UART0_write(char c){
    while((UART0_FR_R & UART_FR_TXFF) != 0);
    UART0_DR_R = c;
}
void UART1_Init(void){
    SYSCTL_RCGCUART_R |= 0x02;
    while((SYSCTL_PRUART_R & 0x02) == 0);
    SYSCTL_RCGCGPIO_R |= 0x02;
        while((SYSCTL_PRGPIO_R & 0x02) == 0);

      GPIO_PORTB_CR_R |= 0x1F;
    GPIO_PORTB_AMSEL_R &= ~0x1F;
    GPIO_PORTB_AFSEL_R |= 0x03;
    GPIO_PORTB_PCTL_R = (GPIO_PORTB_PCTL_R & ~0xFF) | (GPIO_PCTL_PB0_U1RX | GPIO_PCTL_PB1_U1TX);
    GPIO_PORTB_DEN_R |= 0x1F;
    GPIO_PORTB_DIR_R |= 0x1E;
    GPIO_PORTB_DIR_R &= ~0x01;

    UART1_CTL_R &= ~UART_CTL_UARTEN;
    //set buad rate devider
    UART1_IBRD_R = 104;
    UART1_FBRD_R = 11;
      UART1_LCRH_R = (UART_LCRH_WLEN_8 | UART_LCRH_FEN);
    UART1_CTL_R |= (UART_CTL_UARTEN | UART_CTL_RXE | UART_CTL_TXE);
}

char UART1_read(void){
    while((UART1_FR_R & 0x10) == 0x10);
    return UART1_DR_R & 0xFF;
}
void delay(int d){//d=delay in milliseconds
    int d1,d2;
    for(d1=0; d1<d; d1++){
        for(d2=0; d2<3180; d2++){}
    }
}
double getLongitude(char* str) {
    int i;
    double deg = 0, degm = 10, min = 0, minm = 10, sec = 0, secm = 10000, seconds;
    for (i = 0; i < 10;i++) {
        if (i < 2) {
            deg += (str[i] - '0') * degm;
            degm = degm / 10;
        }
        else if (i >= 2 && i < 4) {
            min += (str[i] - '0') * minm;
            minm = minm / 10;
        }
        else if (i >= 5 && i < 10) {
            sec += (str[i] - '0') * secm;
            secm = secm / 10;
        }
    }
    seconds = (double)sec / 1000;
    return  deg + (min / 60) + (seconds / 3600);

}

double getlatitude(char* str) {
    int i;
    double deg2 = 0, degm2 = 10, min2 = 0, minm2 = 10, sec2 = 0, secm2 = 10000, seconds2;
    for (i = 0; i < 10;i++) {
        if (i < 2)  {
            deg2 += (str[i] - '0') * degm2;
            degm2 = degm2 / 10;
        }
        else if (i >= 2 && i < 4) {
            min2 += (str[i] - '0') * minm2;
            minm2 = minm2 / 10;
        }
        else if (i >= 5 && i < 10) {
            sec2 += (str[i] - '0') * secm2;
            secm2 = secm2 / 10;
        }

    }
    seconds2 = (double)sec2 / 1000;
    return deg2 + (min2 / 60) + (seconds2 / 3600);

}

void discheck(int dist) {

    if (dist > 100) {
        GPIO_PORTF_DATA_R |= 0x08;
    }
    else{
               GPIO_PORTF_DATA_R |= 0x00;
    }
}
void delay(int d){//d=delay in milliseconds
	int d1,d2;
	for(d1=0; d1<d; d1++){
		for(d2=0; d2<3180; d2++){}
	}
}

void SEG_command(unsigned char comm){
	GPIO_PORTA_DATA_R&= 0x1F;
	GPIO_PORTA_DATA_R|= 0x80;
	GPIO_PORTB_DATA_R = comm;
	
	delay(0);
	
	GPIO_PORTA_DATA_R &= 0x1F;
	
	delay(2);
}

void SEG_data(unsigned char dat){
	GPIO_PORTA_DATA_R &= 0x3F;
	GPIO_PORTA_DATA_R |= 0x20;
	GPIO_PORTB_DATA_R = dat;
	GPIO_PORTA_DATA_R |= 0x80;
	
	delay(0);
	
	GPIO_PORTA_DATA_R &= 0x1F;
	
	delay(2);
}
void SEG_distance_display(int dist){ 
    char char1,char2,char3;
    char word[11] = {'D','i','s','t','a','n','c','e',' ','=',' '};
    int j ;
    unsigned int k;


    //transforming the number distance into 3 characters
    j = dist/100;
    char1 = 48 + j;
    dist = dist - j * 100;
    j = dist / 10; 
    char2 = 48 + j;
    dist = dist - j*10;
    char3 = 48 + dist;

    SEG_command(0x30);
    SEG_command(0x38);
    SEG_command(0x01);
    SEG_command(0x0F);
    SEG_command(0x80);

    delay(500);

    for (k = 0; k < 10; k++){
        SEG_data(word[k]);
        SEG_command(0x06);
    }

    SEG_data(char1);
    SEG_command(0x06);
    SEG_data(char2);
    SEG_command(0x06);
    SEG_data(char3);
    SEG_command(0x06);

    delay(500);
}
int main() {
   UART0_Init();
   UART1_Init();
		
   while(1){
	UART0_write(UART1_read());	
   }
}
