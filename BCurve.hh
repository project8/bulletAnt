/*
 * BCurve.hh
 *
 *  Created on: Feb 21, 2018
 *      Author: nbuzinsky
 */

#include <TLine.h>

class BCurve
{
    public:

        void UpdateParameters();

    private:
        TLine curvedLine; //Straight Line: shows where beginning and end of curve is
        double startFrequency;
        double endFrequency;
        double centerTime;
        int acquisitionNumber;

};

void BCurve::UpdateParameters()
{
    startFrequency = curvedLine.GetY1();
    endFrequency = curvedLine.GetY2();

    centerTime = (curvedLine.GetX1() curvedLine.GetX2()) / 2.; //average
    return;
}
