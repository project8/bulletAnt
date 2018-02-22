/*
 * BAYAMLWriter.hh
 *
 *  Created on: Feb 21, 2018
 *      Author: nbuzinsky
 */

#include "BATrack.hh"
#include "BACurve.hh"
#include "BAOther.hh"
#include <iostream>

class BAYAMLWriter
{

    public:
        BAYAMLWriter(const std::vector<BATrack> &t, const std::vector<BACurve> &c, const std::vector<BAOther> &o, const std::string &filename, const std::string &sName);

        void Write();


    private:
        std::vector<BATrack> allTracks;
        std::vector<BACurve> allCurves;
        std::vector<BAOther> allOthers;
        std::string outputFilename;
        std::string scannerName;
        std::string todaysDate;
        std::string runID;
        std::string spectrogramFile; //path in DIRAC

        void WriteMetadata();
        void WriteTracks();
        void WriteCurves();
        void WriteOthers();

        std::string GetDate();
};
BAYAMLWriter::BAYAMLWriter(const std::vector<BATrack> &t, const std::vector<BACurve> &c, const std::vector<BAOther> &o, const std::string &filename, const std::string &sName): 
        allTracks(t), 
        allCurves(c), 
        allOthers(o), 
        outputFilename(filename) ,
        scannerName(sName) 
{
    todaysDate  = GetDate();

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
    WriteMetadata();
    WriteTracks();
    WriteCurves();
    WriteOthers();
}

void BAYAMLWriter::WriteMetadata()
{
    std::ofstream outputFileStream;
    outputFileStream.open(outputFilename, std::ofstream::trunc);
    outputFileStream << "metadata:" <<std::endl;
    outputFileStream << "\tscanner: \""<<scannerName<<"\""<<std::endl;
    outputFileStream << "\tdate: \""<<todaysDate<<"\""<<std::endl;
    outputFileStream << "\trun_id: \""<<runID<<"\""<<std::endl;
    outputFileStream << "\tspectrogram_file: \""<<spectrogramFile<<"\""<<std::endl;

    outputFileStream.close();

}

void BAYAMLWriter::WriteTracks()
{
    if(!allTracks.empty())
    {
        std::ofstream outputFileStream;
        outputFileStream.open(outputFilename, std::ofstream::app);
        outputFileStream << "tracks:" <<std::endl;
        
        for(int i=0;i<allTracks.size();++i)
        {
            outputFileStream << "\t- start_f: "<<allTracks[i].GetStartFrequency() <<std::endl;
            outputFileStream << "\t  start_t: "<<allTracks[i].GetStartTime() <<std::endl;
            outputFileStream << "\t  end_f: "<<allTracks[i].GetEndFrequency() <<std::endl;
            outputFileStream << "\t  end_t: "<<allTracks[i].GetEndTime() <<std::endl;
            outputFileStream << "\t  acquisition_number: "<<allTracks[i].GetAcquisitionNumber() <<std::endl;
            if(allTracks[i].GetSidebandStatus())
                outputFileStream << "\t  sideband: "<< std::boolalpha <<allTracks[i].GetSidebandStatus() <<std::endl; //Print as true/ false, not 1 or 0
            if(allTracks[i].GetCurvedStatus())
                outputFileStream << "\t  curved: "<< std::boolalpha << allTracks[i].GetCurvedStatus() <<std::endl;  
        }
        outputFileStream.close();
    }
}


void BAYAMLWriter::WriteCurves()
{
    if(!allCurves.empty())
    {
        std::ofstream outputFileStream;
        outputFileStream.open(outputFilename, std::ofstream::app);

        outputFileStream << "curves:" <<std::endl;
        
        for(int i=0;i<allCurves.size();++i)
        {
            outputFileStream << "\t- time: "<<allCurves[i].GetTime() <<std::endl;
            outputFileStream << "\t  start_f: "<<allCurves[i].GetStartFrequency() <<std::endl;
            outputFileStream << "\t  end_f: "<<allCurves[i].GetEndFrequency() <<std::endl;
            outputFileStream << "\t  acquisition_number: "<<allCurves[i].GetAcquisitionNumber() <<std::endl;
        }
        outputFileStream.close();
    }
}


void BAYAMLWriter::WriteOthers()
{
    if(!allOthers.empty())
    {
        std::ofstream outputFileStream;
        outputFileStream.open(outputFilename, std::ofstream::app);
        outputFileStream << "other_features:" <<std::endl;
        
        for(int i=0;i<allOthers.size();++i)
        {
            outputFileStream << "\t- time: "<<allOthers[i].GetTime() <<std::endl;
            outputFileStream << "\t  frequency: "<<allOthers[i].GetFrequency() <<std::endl;
            outputFileStream << "\t  comment: "<<allOthers[i].GetComment() <<std::endl;
            outputFileStream << "\t  acquisition_number: "<<allOthers[i].GetAcquisitionNumber() <<std::endl;
        }
        outputFileStream.close();
    }
}

