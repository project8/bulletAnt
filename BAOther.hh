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
        void Draw();
        int GetAcquisitionNumber();
        double GetTime();
        double GetFrequency();
        std::string GetComment();

    private:
        TMarker featurePosition;
        std::string userComment;
        int acquisitionNumber;
};

BAOther::BAOther(const double &x, const double &y,const int &acqNumber, const std::string &sInput): 
    featurePosition(TMarker(x,y,8)), 
    acquisitionNumber(acqNumber),
    userComment(sInput)
{
    featurePosition.SetMarkerColor(kWhite);

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

#endif /* BAOTHER_HH_ */
