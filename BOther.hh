/*
 * BOther.hh
 *
 *  Created on: Feb 21, 2018
 *      Author: nbuzinsky
 */

#include <string>

class BOther
{

    public:
        BOther(const double &x, const double &y,const int &acqNumber);
        //void SetComment();
        void Draw();
        int GetAcquisitionNumber();
        double GetTime();
        double GetFrequency();

    private:
        TMarker featurePosition;
        std::string userComment;
        int acquisitionNumber;
};

BOther::BOther(const double &x, const double &y,const int &acqNumber): 
    featurePosition(TMarker(x,y,8)), 
    acquisitionNumber(acqNumber)
{
    featurePosition.SetMarkerColor(kWhite);

}

void BOther::Draw()
{
    featurePosition.Draw();
}

int BOther::GetAcquisitionNumber()
{
    return acquisitionNumber;
}

double BOther::GetTime()
{
    return featurePosition.GetX();
}

double BOther::GetFrequency()
{
    return featurePosition.GetY();
}
