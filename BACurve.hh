/*
 * BACurve.hh
 *
 *  Created on: Feb 21, 2018
 *      Author: nbuzinsky
 */

#include <TLine.h>

class BACurve
{
    public:
        BACurve(const double &x1, const double &y1, const double &x2, const double &y2, const int &acqNumber);
        void Draw();
        int GetAcquisitionNumber();
        double GetTime();
        double GetStartFrequency();
        double GetEndFrequency();

    private:
        TLine curvedLine; //Straight Line: shows where beginning and end of curve is
        int acquisitionNumber;

};

BACurve::BACurve(const double &x1, const double &y1, const double &x2, const double &y2, const int &acqNumber): 
    curvedLine(TLine(x1,y1,x2,y2)), 
    acquisitionNumber(acqNumber)
{
   curvedLine.SetLineWidth(1);
   curvedLine.SetLineColor(kRed);

   return;
}

void BACurve::Draw()
{
    curvedLine.Draw();

}

int BACurve::GetAcquisitionNumber()
{
    return acquisitionNumber;
}
double BACurve::GetTime()
{
    return (curvedLine.GetX1() + curvedLine.GetX2()) / 2.;
}
double BACurve::GetStartFrequency()
{
    return curvedLine.GetY1();
}
double BACurve::GetEndFrequency()
{
    return curvedLine.GetY2();
}
