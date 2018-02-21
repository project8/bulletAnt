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
        //BOther();
        void SetComment();

    private:
        TPoint featurePosition;
        std::string userComment;
        int acquisitionNumber;
};
