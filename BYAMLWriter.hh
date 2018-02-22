/*
 * BYAMLWriter.hh
 *
 *  Created on: Feb 21, 2018
 *      Author: nbuzinsky
 */

#include "BTrack.hh"
#include "BCurve.hh"
#include "BOther.hh"
#include <iostream>

class BYAMLWriter
{

    public:
        BYAMLWriter(const std::vector<BTrack> &t, const std::vector<BCurve> &c, const std::vector<BOther> &o, const std::string &filename);

        void Write();


    private:
        std::vector<BTrack> allTracks;
        std::vector<BCurve> allCurves;
        std::vector<BOther> allOthers;
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
BYAMLWriter::BYAMLWriter(const std::vector<BTrack> &t, const std::vector<BCurve> &c, const std::vector<BOther> &o, const std::string &filename): 
        allTracks(t), 
        allCurves(c), 
        allOthers(o), 
        outputFilename(filename) 
{
    todaysDate  = GetDate();

}

std::string BYAMLWriter::GetDate()
{
    TDatime dateTime;
    std::string sDate = std::string(dateTime.GetYear());
    sDate +="-";
    sDate +=std::string(dateTime.GetMonth());
    sDate +="-";
    sDate +=std::string(dateTime.GetDay());

    return sDate;
}


void BYAMLWriter::Write()
{
    WriteMetadata();
    WriteTracks();
    WriteCurves();
    WriteOthers();
}

void BYAMLWriter::WriteMetaData()
{
    std::ofstream outputFileStream;
    outputFileStream.open(outputFilename);
    outputFileStream << "metadata:" <<std::endl;
    outputFileStream << "\tscanner: \""<<scannerName<<"\""<<std::endl;
    outputFileStream << "\tdate: \""<<todaysDate<<"\""<<std::endl;
    outputFileStream << "\trun_id: \""<<runID<<"\""<<std::endl;
    outputFileStream << "\tspectrogram_file: \""<<spectrogramFile<<"\""<<std::endl;

    outputFileStream.close();

}

void BYAMLWriter::WriteTracks()
{
    std::ofstream outputFileStream;
    outputFileStream.open(outputFilename);
    outputFileStream << "tracks:" <<std::endl;
    
    for(int i=0;i<allTracks.size();++i)
    {
        outputFileStream << "\t- start_f: "<<allTracks[i].GetStartFrequency() <<std::endl;
        outputFileStream << "\t  start_t: "<<allTracks[i].GetStartTime() <<std::endl;
        outputFileStream << "\t  end_f: "<<allTracks[i].GetEndFrequency() <<std::endl;
        outputFileStream << "\t  end_t: "<<allTracks[i].GetEndTime() <<std::endl;
        outputFileStream << "\t  acquisition_number: "<<allTracks[i].GetAcquisitionNumber() <<std::endl;
        if(allTracks[i].GetSidebandStatus())
            outputFileStream << "\t  sideband: "<<allTracks[i].GetSidebandStatus() <<std::endl;
        if(allTracks[i].GetCurvedStatus())
            outputFileStream << "\t  curved: "<<allTracks[i].GetCurvedStatus() <<std::endl;  //may need quotes around true
    }

    outputFileStream.close();
}


void BYAMLWriter::WriteCurves()
{
    std::ofstream outputFileStream;
    outputFileStream.open(outputFilename);
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


void BYAMLWriter::WriteOthers()
{
    std::ofstream outputFileStream;
    outputFileStream.open(outputFilename);
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

