/*
 * BAYAMLWriter.hh
 *
 *  Created on: Feb 21, 2018
 *      Author: nbuzinsky
 */

#ifndef BAYAMLWRITER_HH_
#define BAYAMLWRITER_HH_

#include "BATrack.hh"
#include "BAEvent.hh"
#include "BACurve.hh"
#include "BAOther.hh"
#include <iostream>
#include <regex>
#include <string>

class BAYAMLWriter
{

    public:
        BAYAMLWriter(const std::vector<BATrack> &t, const std::vector<BAEvent> &e, const std::vector<BACurve> &c, const std::vector<BAOther> &o, const std::string &inFilename, const std::string &sName);

        void Write();


    private:
        std::vector<BATrack> allTracks;
        std::vector<BAEvent> allEvents;
        std::vector<BACurve> allCurves;
        std::vector<BAOther> allOthers;
        std::string inputFilename;
        std::string outputFilename;
        std::string scannerName;
        std::string todaysDate;
        std::string runID;
        std::string spectrogramFile; //path in DIRAC

        void WriteMetadata();
        void WriteTracks();
        void WriteEvents();
        void WriteCurves();
        void WriteOthers();

        std::string GetDate();
        void GetOutputFilename();
};
BAYAMLWriter::BAYAMLWriter(const std::vector<BATrack> &t, const std::vector<BAEvent> &e, const std::vector<BACurve> &c, const std::vector<BAOther> &o, const std::string &inFilename, const std::string &sName): 
        allTracks(t), 
        allEvents(e), 
        allCurves(c), 
        allOthers(o), 
        inputFilename(inFilename),
        scannerName(sName)
{
    todaysDate  = GetDate();

}

void BAYAMLWriter::GetOutputFilename()
{
    //Change extension
    outputFilename = inputFilename;

    std::cout<<outputFilename<<std::endl;
    std::regex spectrogramPrefix("rid00000([0-9]{4})(_[0-9]+)?(_|\\.)");
    std::smatch m;
    regex_search(outputFilename, m, spectrogramPrefix);

    if(m.size()!=4)
    {
        //Print default name
        outputFilename = "handscan.yaml";
    }
    else
    {
        //Set RunID too
        runID = m[1].str();
        std::string fileIndex = m[2].str();
        outputFilename = "handscan";
        outputFilename += fileIndex + ".yaml";
    }
}

std::string BAYAMLWriter::GetDate()
{
    TDatime dateTime;
    std::string sDate = std::to_string(dateTime.GetYear());
    sDate +="-";
    sDate +=std::to_string(dateTime.GetMonth());
    sDate +="-";
    sDate +=std::to_string(dateTime.GetDay());

    return sDate;
}


void BAYAMLWriter::Write()
{
    GetOutputFilename();

    WriteMetadata();
    WriteTracks();
    WriteEvents();
    WriteCurves();
    WriteOthers();
}

void BAYAMLWriter::WriteMetadata()
{
    std::ofstream outputFileStream;
    outputFileStream.open(outputFilename, std::ofstream::trunc);
    outputFileStream << "metadata:" <<std::endl;
    outputFileStream << "    scanner: \""<<scannerName<<"\""<<std::endl;
    outputFileStream << "    date: \""<<todaysDate<<"\""<<std::endl;
    outputFileStream << "    run_id: \""<<runID<<"\""<<std::endl;
    outputFileStream << "    spectrogram_file: \""<<spectrogramFile<<"\""<<std::endl;

    outputFileStream.close();

}

void BAYAMLWriter::WriteTracks()
{
    if(!allTracks.empty())
    {
        std::ofstream outputFileStream;
        outputFileStream << fixed << std::setprecision(11);
        outputFileStream.open(outputFilename, std::ofstream::app);
        outputFileStream << "tracks:" <<std::endl;
        
        for(int i=0;i<allTracks.size();++i)
        {
            if(allTracks[i].GetWriteStatus())
            {
                outputFileStream << "    - start_f: "<<allTracks[i].GetStartFrequency() <<std::endl;
                outputFileStream << "      start_t: "<<allTracks[i].GetStartTime() <<std::endl;
                outputFileStream << "      end_f: "<<allTracks[i].GetEndFrequency() <<std::endl;
                outputFileStream << "      end_t: "<<allTracks[i].GetEndTime() <<std::endl;
                if(allTracks[i].GetSidebandStatus())
                    outputFileStream << "      sideband: "<< std::boolalpha <<allTracks[i].GetSidebandStatus() <<std::endl; //Print as true/ false, not 1 or 0
                if(allTracks[i].GetCurvedStatus())
                    outputFileStream << "      curved: "<< std::boolalpha << allTracks[i].GetCurvedStatus() <<std::endl;  
                outputFileStream << "      acquisition_number: "<<allTracks[i].GetAcquisitionNumber() <<std::endl<<std::endl;
            }
        }
        outputFileStream.close();
    }
}

void BAYAMLWriter::WriteEvents()
{
    if(!allEvents.empty())
    {
        std::ofstream outputFileStream;
        outputFileStream << fixed << std::setprecision(11);
        outputFileStream.open(outputFilename, std::ofstream::app);
        outputFileStream << "events:" <<std::endl;
        
        for(int i=0;i<allEvents.size();++i)
        {
            if(allEvents[i].GetWriteStatus())
            {
                outputFileStream << "    - start_f: "<<allEvents[i].GetStartFrequency() <<std::endl;
                outputFileStream << "      start_t: "<<allEvents[i].GetStartTime() <<std::endl;
                outputFileStream << "      end_f: "<<allEvents[i].GetEndFrequency() <<std::endl;
                outputFileStream << "      end_t: "<<allEvents[i].GetEndTime() <<std::endl;
                outputFileStream << "      acquisition_number: "<<allEvents[i].GetAcquisitionNumber() <<std::endl<<std::endl;
            }
        }
        outputFileStream.close();
    }
}


void BAYAMLWriter::WriteCurves()
{
    if(!allCurves.empty())
    {
        std::ofstream outputFileStream;
        outputFileStream << fixed << std::setprecision(11);
        outputFileStream.open(outputFilename, std::ofstream::app);

        outputFileStream << "curves:" <<std::endl;
        
        for(int i=0;i<allCurves.size();++i)
        {
            if(allCurves[i].GetWriteStatus())
            {
                outputFileStream << "    - time: "<<allCurves[i].GetTime() <<std::endl;
                outputFileStream << "      start_f: "<<allCurves[i].GetStartFrequency() <<std::endl;
                outputFileStream << "      end_f: "<<allCurves[i].GetEndFrequency() <<std::endl;
                outputFileStream << "      acquisition_number: "<<allCurves[i].GetAcquisitionNumber() <<std::endl<<std::endl;
            }
        }
        outputFileStream.close();
    }
}


void BAYAMLWriter::WriteOthers()
{
    if(!allOthers.empty())
    {
        std::ofstream outputFileStream;
        outputFileStream << fixed << std::setprecision(11);
        outputFileStream.open(outputFilename, std::ofstream::app);
        outputFileStream << "other_features:" <<std::endl;
        
        for(int i=0;i<allOthers.size();++i)
        {
            if(allOthers[i].GetWriteStatus())
            {
                outputFileStream << "    - time: "<<allOthers[i].GetTime() <<std::endl;
                outputFileStream << "      frequency: "<<allOthers[i].GetFrequency() <<std::endl;
                outputFileStream << "      comment: "<<allOthers[i].GetComment() <<std::endl;
                outputFileStream << "      acquisition_number: "<<allOthers[i].GetAcquisitionNumber() <<std::endl<<std::endl;
            }
        }
        outputFileStream.close();
    }
}

#endif /* BAYAMLWRITER_HH_ */
