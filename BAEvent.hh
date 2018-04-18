/*
 * BAEvent.hh
 *
 *  Created on: Apr 18, 2018
 *      Author: nbuzinsky
 */

#ifndef BAEVENT_HH_
#define BAEVENT_HH_

#include <TLine.h>

class BAEvent
{
    public:
        BAEvent(const double &x1, const double &y1, const double &x2, const double &y2, const int &acqNumber);
        BAEvent() : BAEvent(0,0,0,0,0) {}
        void Draw();
        int GetAcquisitionNumber();
        void SetAcquisitionNumber(const int &acqNumber);

        double GetStartTime();
        double GetStartFrequency();
        double GetEndTime();
        double GetEndFrequency();

        void SetStartTime(const double &startTime);
        void SetStartFrequency(const double &startFrequency);
        void SetEndTime(const double &endTime);
        void SetEndFrequency(const double &endFrequency);

        void SetWriteStatus(const bool&);
        bool GetWriteStatus();

        void SetOpacity(const double &opacity);

    private:
        TLine eventRidge;
        bool writeStatus;
        int acquisitionNumber;
};

BAEvent::BAEvent(const double &x1, const double &y1, const double &x2, const double &y2, const int &acqNumber): 
    eventRidge(TLine(x1,y1,x2,y2)), 
    writeStatus(true),
    acquisitionNumber(acqNumber)
{
   eventRidge.SetLineWidth(1);
   eventRidge.SetLineColorAlpha(80,0.5);

   return;
}

void BAEvent::SetOpacity(const double &opacity)
{
   eventRidge.SetLineColorAlpha(80,opacity);
}

void BAEvent::Draw()
{
    eventRidge.Draw();

}

int BAEvent::GetAcquisitionNumber()
{
    return acquisitionNumber;
}

double BAEvent::GetStartTime()
{
    return eventRidge.GetX1();
}

double BAEvent::GetStartFrequency()
{
    return eventRidge.GetY1();
}

double BAEvent::GetEndTime()
{
    return eventRidge.GetX2();
}

double BAEvent::GetEndFrequency()
{
    return eventRidge.GetY2();
}

void BAEvent::SetStartTime(const double &startTime)
{
    eventRidge.SetX1(startTime);
}

void BAEvent::SetStartFrequency(const double &startFrequency)
{
    eventRidge.SetY1(startFrequency);
}

void BAEvent::SetEndTime(const double &endTime)
{
    eventRidge.SetX2(endTime);
}
void BAEvent::SetEndFrequency(const double &endFrequency)
{
    eventRidge.SetY2(endFrequency);
}

void BAEvent::SetAcquisitionNumber(const int &acqNumber)
{
    acquisitionNumber = acqNumber;
}

bool BAEvent::GetWriteStatus()
{
    return writeStatus;
}

void BAEvent::SetWriteStatus(const bool &s)
{
    writeStatus = s;
}


#endif /* BAEVENT_HH_ */
