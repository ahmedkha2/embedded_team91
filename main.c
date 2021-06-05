#include "stdint.h"
#include "math.h"
#include "C:/Keil/Labware/inc/tm4c123gh6pm.h"

double rad(double x)
{
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
void discheck(double latitude1, double longitude1, double latitude2, double longitude2) {

    double distance;
    distance = calcdist(latitude1, longitude1, latitude2, longitude2);
    if (distance > 100) {
        GPIO_PORTF_DATA_R |= 0x08;
    }
}

void systeminit() {}
void init() {
    //Global ports initialization
        uint32_t delay;
    SYSCTL_RCGCGPIO_R |= 0x23;
    delay = 1;

        //port A initialization
    GPIO_PORTA_LOCK_R = 0x4C4F434B;
    GPIO_PORTA_CR_R = 0XFF;
    GPIO_PORTA_DIR_R = 0xE0;
    GPIO_PORTA_AFSEL_R &= 0x1F;
    GPIO_PORTA_PCTL_R = 0x0;
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
int main() {
    init();
    while (1)
    {


    }
}
