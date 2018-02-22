/*
 * HandScanAssistant.C
 *
 *  Created on: Feb 21, 2018
 *      Author: nbuzinsky
 */

#include "BAMainFrame.hh"

void HandScanAssistant(std::string inputFilename)
{
   // Popup the GUI...
   gStyle->SetOptStat(0);
   new BAMainFrame(gClient->GetRoot(), inputFilename , 1600, 1200);
}

