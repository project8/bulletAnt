/*
 * BAOther.hh
 *
 *  Created on: Feb 21, 2018
 *      Author: nbuzinsky
 */

#ifndef BAOTHER_HH_
#define BAOTHER_HH_

#include <string>

class BAOther
{

    public:
        BAOther(const double &x, const double &y,const int &acqNumber, const std::string &sInput);
        BAOther(): BAOther(0,0,0,"") {}
        void Draw();
        int GetAcquisitionNumber();
        double GetTime();
        double GetFrequency();
        std::string GetComment();

        void SetAcquisitionNumber(const int &acqNumber);
        void SetTime(const double &time);
        void SetFrequency(const double &frequency);
        void SetComment(const std::string &comment);

        void SetWriteStatus(const bool&);
        bool GetWriteStatus();
        void SetOpacity(const double &opacity);

    private:
        TMarker featurePosition;
        std::string userComment;
        int acquisitionNumber;
        bool writeStatus;
};

BAOther::BAOther(const double &x, const double &y,const int &acqNumber, const std::string &sInput): 
    featurePosition(TMarker(x,y,8)), 
    acquisitionNumber(acqNumber),
    writeStatus(true),
    userComment(sInput)
{
    featurePosition.SetMarkerColorAlpha(2,0.5);

}

void BAOther::SetOpacity(const double &opacity)
{
    featurePosition.SetMarkerColorAlpha(2,opacity);
}

std::string BAOther::GetComment()
{
    return userComment;
}

void BAOther::Draw()
{
    featurePosition.Draw();
}

int BAOther::GetAcquisitionNumber()
{
    return acquisitionNumber;
}

double BAOther::GetTime()
{
    return featurePosition.GetX();
}

double BAOther::GetFrequency()
{
    return featurePosition.GetY();
}

void BAOther::SetAcquisitionNumber(const int& acqNumber)
{
    acquisitionNumber = acqNumber;
}

void BAOther::SetTime(const double &time)
{
    featurePosition.SetX(time);
}
void BAOther::SetFrequency(const double &frequency)
{
    featurePosition.SetY(frequency);
}
void BAOther::SetComment(const std::string &comment)
{
    userComment = comment;
}

bool BAOther::GetWriteStatus()
{
    return writeStatus;
}

void BAOther::SetWriteStatus(const bool &s)
{
    writeStatus = s;
}



#endif /* BAOTHER_HH_ */
