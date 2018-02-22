/*
 * HandScanAssistant.C
 *
 *  Created on: Feb 21, 2018
 *      Author: nbuzinsky
 */

#include "BAMainFrame.hh"

void HandScanAssistant(std::string inputFilename)
{
   if(!inputFilename.empty() && inputFilename[0] == std::string("~"))
   {
       std::cerr<<"Please specify the absolute path!"<<std::endl;
       gApplication->Terminate(1);

   }

   // Popup the GUI...
   gStyle->SetOptStat(0);
   new BAMainFrame(gClient->GetRoot(), inputFilename , 1600, 1200);
}

