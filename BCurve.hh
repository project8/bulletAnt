/*
 * BCurve.hh
 *
 *  Created on: Feb 21, 2018
 *      Author: nbuzinsky
 */

#include <TLine.h>

class BCurve
{
    public:
        BCurve(const double &x1, const double &y1, const double &x2, const double &y2, const int &acqNumber);
        void Draw();
        int GetAcquisitionNumber();
        double GetTime();
        double GetStartFrequency();
        double GetEndFrequency();

    private:
        TLine curvedLine; //Straight Line: shows where beginning and end of curve is
        int acquisitionNumber;

};

BCurve::BCurve(const double &x1, const double &y1, const double &x2, const double &y2, const int &acqNumber): 
    curvedLine(TLine(x1,y1,x2,y2)), 
    acquisitionNumber(acqNumber)
{
   curvedLine.SetLineWidth(1);
   curvedLine.SetLineColor(kRed);

   return;
}

void BCurve::Draw()
{
    curvedLine.Draw();

}

int BCurve::GetAcquisitionNumber()
{
    return acquisitionNumber;
}
double BCurve::GetTime()
{
    return (curvedLine.GetX1() + curvedLine.GetX2()) / 2.;
}
double BCurve::GetStartFrequency()
{
    return curvedLine.GetY1();
}
double BCurve::GetEndFrequency()
{
    return curvedLine.GetY2();
}
