/*
 * BATreeReader.hh
 *
 *  Created on: Mar 20, 2018
 *      Author: nbuzinsky
 */

// Create specialized class to parse ROOT Trees containing tracks

#ifndef BATREEREADER_HH_
#define BATREEREADER_HH_

#include "BATrack.hh"

class BATreeReader
{
    public:
        BATreeReader(const std::string &inputFile): fileName(inputFile) {}
        void Read();

        std::vector<BATrack> GetTracks();

    private:
        std::vector<BATrack> allTracks;
        std::string fileName;

        void ReadTracks();

};

std::vector<BATrack> BATreeReader::GetTracks()
{
    return allTracks;
}

void BATreeReader::Read()
{
    ReadTracks();
}

void BATreeReader::ReadTracks()
{
    
    TFile *f = new TFile(fileName.c_str());
    TTree *t1 = (TTree*)f->Get("multiTrackEvents");
    if(!t1)
    {
        cout<<"ROOT File does not have correct type of tree. Exiting application..."<<endl;
        exit(1);
    }

    double fStartFrequency;
    double fEndFrequency;
    double fStartTime;
    double fEndTime;

    const int nTracks = t1->GetEntries(); 

    for(int i=0;i<nTracks;++i)
    {
        t1->GetEntry(i);
        allTracks.push_back(BATrack());

        fStartFrequency = t1->GetLeaf("fStartFrequency")->GetValue();
        fEndFrequency = t1->GetLeaf("fEndFrequency")->GetValue();
        fStartTime = t1->GetLeaf("fStartTimeInRunC")->GetValue();
        fEndTime = t1->GetLeaf("fEndTimeInRunC")->GetValue();

        allTracks.back().SetStartFrequency(fStartFrequency); 
        allTracks.back().SetEndFrequency(fEndFrequency); 
        allTracks.back().SetStartTime(fStartTime); 
        allTracks.back().SetEndTime(fEndTime); 
    }
}

#endif /* BATREEREADER_HH_ */
