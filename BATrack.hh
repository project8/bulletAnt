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
        BATrack() : BATrack(0,0,0,0,0) {}
        void Draw();
        int GetAcquisitionNumber();
        void SetAcquisitionNumber(const int &acqNumber);

        void SetSidebandStatus();
        bool GetSidebandStatus();

        void SetCurvedStatus();
        bool GetCurvedStatus();

        double GetStartTime();
        double GetStartFrequency();
        double GetEndTime();
        double GetEndFrequency();

        void SetStartTime(const double &startTime);
        void SetStartFrequency(const double &startFrequency);
        void SetEndTime(const double &endTime);
        void SetEndFrequency(const double &endFrequency);

        bool IsValid();

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
   trackRidge.SetLineColorAlpha(2,0.5);

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

void BATrack::SetStartTime(const double &startTime)
{
    trackRidge.SetX1(startTime);
}

void BATrack::SetStartFrequency(const double &startFrequency)
{
    trackRidge.SetY1(startFrequency);
}

void BATrack::SetEndTime(const double &endTime)
{
    trackRidge.SetX2(endTime);
}
void BATrack::SetEndFrequency(const double &endFrequency)
{
    trackRidge.SetY2(endFrequency);
}

void BATrack::SetAcquisitionNumber(const int &acqNumber)
{
    acquisitionNumber = acqNumber;
}

bool BATrack::IsValid()
{
    return ((GetStartTime() > 0) || (GetEndTime() > 0));
}

#endif /* BATRACK_HH_ */
