//
// Created by heihei on 2020/5/19.
//
#include "utils.h"

uchar getMaxrgb(uchar blue, uchar green, uchar red)
{
    if (blue >= green and blue >= red)
    {
        return blue;
    }
    else if (green >= blue and green >= red)
    {
        return green;
    }
    else if (red >= green and red >= blue)
    {
        return red;
    }
}


uchar getMinrgb(uchar blue, uchar green, uchar red)
{
    if (blue <= green and blue <= red)
    {
        return blue;
    }
    else if (green <= blue and green <= red)
    {
        return green;
    }
    else if (red <= green and red <= blue)
    {
        return red;
    }
}


uchar getMaxPixRgb(uchar blue, uchar green, uchar red)
{
    if (blue >= green and blue >= red)
    {
        return blue;
    }
    else if (green >= blue and green >= red)
    {
        return green;
    }
    else if (red >= green and red >= blue)
    {
        return red;
    }
}


uchar getMinPixRgb(uchar blue, uchar green, uchar red)
{
    if (blue <= green and blue <= red)
    {
        return blue;
    }
    else if (green <= blue and green <= red)
    {
        return green;
    }
    else if (red <= green and red <= blue)
    {
        return red;
    }
}

cv::Vec3b solveOutBounds(cv::Vec3b &pix)
{
    if(pix[0]<0)
    {
        pix[0]=0;
    }
    else if (pix[0] > 255)
    {
        pix[0]=255;
    }

    if(pix[1]<0)
    {
        pix[1]=0;
    }
    else if (pix[1] > 255)
    {
        pix[1]=255;
    }

    if(pix[2]<0)
    {
        pix[2]=0;
    }
    else if (pix[2] > 255)
    {
        pix[2]=255;
    }

    return pix;
}