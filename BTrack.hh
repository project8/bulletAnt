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
        BTrack(const double &x1, const double &y1, const double &x2, const double &y2, const double &acqNumber);
        void Draw();
        int GetAcquisitionNumber();

        void SetSidebandStatus();
        bool GetSidebandStatus();

        void SetCurvedStatus();
        bool GetCurvedStatus();

        double GetX1();
        double GetY1();
        double GetX2();
        double GetY2();
    private:
        TLine trackRidge;
        bool isSideband;
        bool isCurved;
        int acquisitionNumber;
};

BTrack::BTrack(const double &x1, const double &y1, const double &x2, const double &y2, const double &acqNumber): 
    trackRidge(TLine(x1,y1,x2,y2)), 
    isSideband(false), 
    isCurved(false), 
    acquisitionNumber(acqNumber)
{
   trackRidge.SetLineWidth(1);
   trackRidge.SetLineColor(kRed);

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

double BTrack::GetX1()
{
    return trackRidge.GetX1();
}

double BTrack::GetY1()
{
    return trackRidge.GetY1();
}

double BTrack::GetX2()
{
    return trackRidge.GetX2();
}

double BTrack::GetY2()
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


