/*
 * BACurve.hh
 *
 *  Created on: Feb 21, 2018
 *      Author: nbuzinsky
 */

#ifndef BACURVE_HH_
#define BACURVE_HH_

#include <TLine.h>

class BACurve
{
    public:
        BACurve(const double &x1, const double &y1, const double &x2, const double &y2, const int &acqNumber);
        BACurve(): BACurve(0,0,0,0,0) {}
        void Draw();
        int GetAcquisitionNumber();
        double GetTime();
        double GetStartFrequency();
        double GetEndFrequency();

        void SetAcquisitionNumber(const int &acqNumber);
        void SetTime(const double &time);
        void SetStartFrequency(const double &startFrequency);
        void SetEndFrequency(const double &endFrequency);

        bool IsValid();

    private:
        TLine curvedLine; //Straight Line: shows where beginning and end of curve is
        int acquisitionNumber;

};

BACurve::BACurve(const double &x1, const double &y1, const double &x2, const double &y2, const int &acqNumber): 
    curvedLine(TLine(x1,y1,x2,y2)), 
    acquisitionNumber(acqNumber)
{
   curvedLine.SetLineWidth(1);
   curvedLine.SetLineColor(6);

   return;
}

void BACurve::SetAcquisitionNumber(const int &acqNumber)
{
    acquisitionNumber = acqNumber;
}

void BACurve::SetTime(const double &time)
{
    const double timeLength = 0.001; //Default length of a curve for parsing. Doesnt really matter
    curvedLine.SetX1(time - timeLength/2.);
    curvedLine.SetX2(time + timeLength/2.);
}

void BACurve::SetStartFrequency(const double &startFrequency)
{
    curvedLine.SetY1(startFrequency);
}

void BACurve::SetEndFrequency(const double &endFrequency)
{
    curvedLine.SetY2(endFrequency);
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

bool BACurve::IsValid()
{
    return ((curvedLine.GetX1() > 0) || (curvedLine.GetX2()>0));
}

#endif /* BACURVE_HH_ */
