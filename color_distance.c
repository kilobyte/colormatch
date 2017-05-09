#include <stdio.h>
#include <math.h>
#include "color_distance.h"

#define pi 3.141592653589793238462643383279

/// Computes the CIEDE2000 color-difference between two Lab colors
/// Based on the article:
/// The CIEDE2000 Color-Difference Formula: Implementation Notes,
/// Supplementary Test Data, and Mathematical Observations,", G. Sharma,
/// W. Wu, E. N. Dalal, submitted to Color Research and Application,
/// January 2004.
/// Available at http://www.ece.rochester.edu/~/gsharma/ciede2000/
/// Based on the C++ implementation by Ofir Pele, The Hebrew University of Jerusalem 2010.
//
double deltaE2000(double *lab1, double *lab2)
{
    double Lstd = *lab1;
    double astd = *(lab1+1);
    double bstd = *(lab1+2);

    double Lsample = *lab2;
    double asample = *(lab2+1);
    double bsample = *(lab2+2);

    double Cabstd= sqrt(astd*astd+bstd*bstd);
    double Cabsample= sqrt(asample*asample+bsample*bsample);

    double Cabarithmean= (Cabstd + Cabsample)/2.0;

    double G= 0.5*( 1.0 - sqrt( pow(Cabarithmean,7.0)/(pow(Cabarithmean,7.0) + pow(25.0,7.0))));

    double apstd= (1.0+G)*astd; // aprime in paper
    double apsample= (1.0+G)*asample; // aprime in paper
    double Cpsample= sqrt(apsample*apsample+bsample*bsample);

    double Cpstd= sqrt(apstd*apstd+bstd*bstd);
    // Compute product of chromas
    double Cpprod= (Cpsample*Cpstd);


    // Ensure hue is between 0 and 2pi
    double hpstd= atan2(bstd,apstd);
    if (hpstd<0) hpstd+= 2.0*pi;  // rollover ones that come -ve

    double hpsample= atan2(bsample,apsample);
    if (hpsample<0) hpsample+= 2.0*pi;
    if ( (fabs(apsample)+fabs(bsample))==0.0)  hpsample= 0.0;

    double dL= (Lsample-Lstd);
    double dC= (Cpsample-Cpstd);

    // Computation of hue difference
    double dhp= (hpsample-hpstd);
    if (dhp>pi)  dhp-= 2.0*pi;
    if (dhp<-pi) dhp+= 2.0*pi;
    // set chroma difference to zero if the product of chromas is zero
    if (Cpprod == 0.0) dhp= 0.0;

    // Note that the defining equations actually need
    // signed Hue and chroma differences which is different
    // from prior color difference formulae

    double dH= 2.0*sqrt(Cpprod)*sin(dhp/2.0);
    //%dH2 = 4*Cpprod.*(sin(dhp/2)).^2;

    // weighting functions
    double Lp= (Lsample+Lstd)/2.0;
    double Cp= (Cpstd+Cpsample)/2.0;

    // Average Hue Computation
    // This is equivalent to that in the paper but simpler programmatically.
    // Note average hue is computed in radians and converted to degrees only
    // where needed
    double hp= (hpstd+hpsample)/2.0;
    // Identify positions for which abs hue diff exceeds 180 degrees
    if ( fabs(hpstd-hpsample)  > pi ) hp-= pi;
    // rollover ones that come -ve
    if (hp<0) hp+= 2.0*pi;

    // Check if one of the chroma values is zero, in which case set
    // mean hue to the sum which is equivalent to other value
    if (Cpprod==0.0) hp= hpsample+hpstd;

    double Lpm502= (Lp-50.0)*(Lp-50.0);;
    double Sl= 1.0+0.015*Lpm502/sqrt(20.0+Lpm502);
    double Sc= 1.0+0.045*Cp;
    double T= 1.0 - 0.17*cos(hp - pi/6.0) + 0.24*cos(2.0*hp) + 0.32*cos(3.0*hp+pi/30.0) - 0.20*cos(4.0*hp-63.0*pi/180.0);
    double Sh= 1.0 + 0.015*Cp*T;
    double delthetarad= (30.0*pi/180.0)*exp(- pow(( (180.0/pi*hp-275.0)/25.0),2.0));
    double Rc=  2.0*sqrt(pow(Cp,7.0)/(pow(Cp,7.0) + pow(25.0,7.0)));
    double RT= -sin(2.0*delthetarad)*Rc;

    // The CIE 00 color difference
    return sqrt( pow((dL/Sl),2.0) + pow((dC/Sc),2.0) + pow((dH/Sh),2.0) + RT*(dC/Sc)*(dH/Sh) );
}

void rgb2lab(int rgb, double lab[3])
{
    double r = ((rgb>>16)&0xff)/255.0;
    double g = ((rgb>> 8)&0xff)/255.0;
    double b = ((rgb    )&0xff)/255.0;
    double x,y,z;

    r = (r > 0.04045) ? pow((r + 0.055) / 1.055, 2.4) : r / 12.92;
    g = (g > 0.04045) ? pow((g + 0.055) / 1.055, 2.4) : g / 12.92;
    b = (b > 0.04045) ? pow((b + 0.055) / 1.055, 2.4) : b / 12.92;

    // at D65
    x = (r * 0.4124 + g * 0.3576 + b * 0.1805) / 0.95047;
    y = (r * 0.2126 + g * 0.7152 + b * 0.0722) / 1.00000;
    z = (r * 0.0193 + g * 0.1192 + b * 0.9505) / 1.08883;

    x = (x > 0.008856) ? pow(x, 1.0/3) : (7.787 * x) + 16.0/116;
    y = (y > 0.008856) ? pow(y, 1.0/3) : (7.787 * y) + 16.0/116;
    z = (z > 0.008856) ? pow(z, 1.0/3) : (7.787 * z) + 16.0/116;

    lab[0] = 116 * y - 16;
    lab[1] = 500 * (x - y);
    lab[2] = 200 * (y - z);
}

int color_diff(int rgb1, int rgb2)
{
    double lab1[3], lab2[3];
    rgb2lab(rgb1, lab1);
    rgb2lab(rgb2, lab2);
    // scale is 0..100, min dist is 0.42 -- a single byte would be ok,
    // but extra accuracy costs us nothing.
    return deltaE2000(lab1, lab2)*16777216;
}
