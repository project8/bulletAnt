/*
 * BAOther.hh
 *
 *  Created on: Feb 21, 2018
 *      Author: nbuzinsky
 */

#include <string>

class BAOther
{

    public:
        BAOther(const double &x, const double &y,const int &acqNumber);
        void SetComment(const std::string &sInput);
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

BAOther::BAOther(const double &x, const double &y,const int &acqNumber): 
    featurePosition(TMarker(x,y,8)), 
    acquisitionNumber(acqNumber)
{
    featurePosition.SetMarkerColor(kWhite);

}

void BAOther::SetComment(const std::string &sInput)
{
    userComment = sInput;
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
