/*
 * BATrack.hh
 *
 *  Created on: Feb 21, 2018
 *      Author: nbuzinsky
 */

#ifndef BATRACK_HH_
#define BATRACK_HH_

#include <TLine.h>

class BATrack
{
    public:
        BATrack(const double &x1, const double &y1, const double &x2, const double &y2, const int &acqNumber);
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

BATrack::BATrack(const double &x1, const double &y1, const double &x2, const double &y2, const int &acqNumber): 
    trackRidge(TLine(x1,y1,x2,y2)), 
    isSideband(false), 
    isCurved(false), 
    acquisitionNumber(acqNumber)
{
   trackRidge.SetLineWidth(1);
   trackRidge.SetLineColor(kWhite);

   return;
}

void BATrack::Draw()
{
    trackRidge.Draw();

}

int BATrack::GetAcquisitionNumber()
{
    return acquisitionNumber;
}

double BATrack::GetStartTime()
{
    return trackRidge.GetX1();
}

double BATrack::GetStartFrequency()
{
    return trackRidge.GetY1();
}

double BATrack::GetEndTime()
{
    return trackRidge.GetX2();
}

double BATrack::GetEndFrequency()
{
    return trackRidge.GetY2();
}

void BATrack::SetSidebandStatus()
{
    isSideband = !isSideband;
}

void BATrack::SetCurvedStatus()
{
    isCurved = !isCurved;
}

bool BATrack::GetSidebandStatus()
{
    return isSideband;
}

bool BATrack::GetCurvedStatus()
{
    return isCurved;
}

#endif /* BATRACK_HH_ */
