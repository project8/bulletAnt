/*
 * BTrack.hh
 *
 *  Created on: Feb 21, 2018
 *      Author: nbuzinsky
 */

#include <TLine.h>
class BTrack
{
    public:
        BTrack(const double &x1, const double &y1, const double &x2, const double &y2, const int &acqNumber);
        void Draw();
        int GetAcquisitionNumber();

        void SetSidebandStatus();
        bool GetSidebandStatus();

        void SetCurvedStatus();
        bool GetCurvedStatus();

        double GetStartTime();
        double GetStartFrequency();
        double GetEndTime();
        double GetEndFrequency();

    private:
        TLine trackRidge;
        bool isSideband;
        bool isCurved;
        int acquisitionNumber;
};

BTrack::BTrack(const double &x1, const double &y1, const double &x2, const double &y2, const int &acqNumber): 
    trackRidge(TLine(x1,y1,x2,y2)), 
    isSideband(false), 
    isCurved(false), 
    acquisitionNumber(acqNumber)
{
   trackRidge.SetLineWidth(1);
   trackRidge.SetLineColor(kWhite);

   return;
}

void BTrack::Draw()
{
    trackRidge.Draw();

}

int BTrack::GetAcquisitionNumber()
{
    return acquisitionNumber;
}

double BTrack::GetStartTime()
{
    return trackRidge.GetX1();
}

double BTrack::GetStartFrequency()
{
    return trackRidge.GetY1();
}

double BTrack::GetEndTime()
{
    return trackRidge.GetX2();
}

double BTrack::GetEndFrequency()
{
    return trackRidge.GetY2();
}

void BTrack::SetSidebandStatus()
{
    isSideband = !isSideband;
}

void BTrack::SetCurvedStatus()
{
    isCurved = !isCurved;
}

bool BTrack::GetSidebandStatus()
{
    return isSideband;
}

bool BTrack::GetCurvedStatus()
{
    return isCurved;
}


