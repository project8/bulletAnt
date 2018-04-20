/*
 * BAYAMLReader.hh
 *
 *  Created on: Feb 23, 2018
 *      Author: nbuzinsky
 */

// No good way to parse YAML files in ROOT in interpreted mode, like we need for application
// Create specialized class to parse file

#ifndef BAYAMLREADER_HH_
#define BAYAMLREADER_HH_

#include "BATrack.hh"
#include "BACurve.hh"
#include "BAEvent.hh"
#include "BAOther.hh"

class BAYAMLReader
{
    public:
        BAYAMLReader(const std::string &inputFile): fileName(inputFile) {}
        void Read();

        std::vector<BATrack> GetTracks();
        std::vector<BAEvent> GetEvents();
        std::vector<BACurve> GetCurves();
        std::vector<BAOther> GetOthers();

    private:
        std::vector<BATrack> allTracks;
        std::vector<BAEvent> allEvents;
        std::vector<BACurve> allCurves;
        std::vector<BAOther> allOthers;
        std::string fileName;
        std::string GetKey(const std::string &sLine);

        void ReadTracks();
        void ReadEvents();
        void ReadCurves();
        void ReadOthers();

};

std::vector<BATrack> BAYAMLReader::GetTracks()
{
    return allTracks;
}

std::vector<BAEvent> BAYAMLReader::GetEvents()
{
    return allEvents;
}


std::vector<BACurve> BAYAMLReader::GetCurves()
{
    return allCurves;
}

std::vector<BAOther> BAYAMLReader::GetOthers()
{
    return allOthers;
}

std::string BAYAMLReader::GetKey(const std::string &sLine)
{
    std::size_t found = sLine.find(":") + 1;
    std::size_t newStringLength = sLine.size() - found;
    return sLine.substr(found,newStringLength);
}

void BAYAMLReader::Read()
{
    ReadTracks();
    ReadEvents();
    ReadCurves();
    ReadOthers();

}

void BAYAMLReader::ReadTracks()
{
    ifstream fileStream(fileName.c_str());
    std::string currentLine;
    const std::string tracksHead = "tracks:";
    const std::string eventsHead = "events:";
    const std::string curvesHead = "curves:";
    const std::string othersHead = "other_features:";

    //Loop over lines, doing nothing, until found beginning of tracks section
    while (std::getline(fileStream, currentLine) && currentLine.find(tracksHead) == std::string::npos);  //Do nothing!! (intentional)

    while (std::getline(fileStream, currentLine)) 
    {
       if (currentLine.find("   - ") != std::string::npos)
       {
           allTracks.push_back(BATrack());
       }

       if (currentLine.find("start_t:") != std::string::npos)
       {
           allTracks.back().SetStartTime(std::stod(GetKey(currentLine)));
       }
       else if (currentLine.find("end_t:") != std::string::npos)
       {
           allTracks.back().SetEndTime(std::stod(GetKey(currentLine)));
       }
       else if (currentLine.find("start_f:") != std::string::npos)
       {
           allTracks.back().SetStartFrequency(std::stod(GetKey(currentLine)));
       }
       else if (currentLine.find("end_f:") != std::string::npos)
       {
           allTracks.back().SetEndFrequency(std::stod(GetKey(currentLine)));
       }
       else if (currentLine.find("sideband:") != std::string::npos)
       {
           allTracks.back().SetSidebandStatus(); // fix me
       }
       else if (currentLine.find("curved:") != std::string::npos)
       {
           allTracks.back().SetCurvedStatus(); // fix me
       }
       else if (currentLine.find("acquisition_number:") != std::string::npos)
       {
           allTracks.back().SetAcquisitionNumber(std::stoi(GetKey(currentLine)));
       }
       else if ((currentLine.find(eventsHead) != std::string::npos) || (currentLine.find(curvesHead) != std::string::npos) || (currentLine.find(othersHead) != std::string::npos))
       {
           break;
       }
    }
}

void BAYAMLReader::ReadEvents()
{
    ifstream fileStream(fileName.c_str());
    std::string currentLine;
    const std::string tracksHead = "tracks:";
    const std::string eventsHead = "events:";
    const std::string curvesHead = "curves:";
    const std::string othersHead = "other_features:";

    //Loop over lines, doing nothing, until found beginning of tracks section
    while (std::getline(fileStream, currentLine) && currentLine.find(eventsHead) == std::string::npos);  //Do nothing!! (intentional)

    while (std::getline(fileStream, currentLine)) 
    {
       if (currentLine.find("   - ") != std::string::npos)
       {
           allEvents.push_back(BAEvent());
       }

       if (currentLine.find("start_t:") != std::string::npos)
       {
           allEvents.back().SetStartTime(std::stod(GetKey(currentLine)));
       }
       else if (currentLine.find("end_t:") != std::string::npos)
       {
           allEvents.back().SetEndTime(std::stod(GetKey(currentLine)));
       }
       else if (currentLine.find("start_f:") != std::string::npos)
       {
           allEvents.back().SetStartFrequency(std::stod(GetKey(currentLine)));
       }
       else if (currentLine.find("end_f:") != std::string::npos)
       {
           allEvents.back().SetEndFrequency(std::stod(GetKey(currentLine)));
       }
       else if (currentLine.find("acquisition_number:") != std::string::npos)
       {
           allEvents.back().SetAcquisitionNumber(std::stoi(GetKey(currentLine)));
       }
       else if ((currentLine.find(tracksHead) != std::string::npos) || (currentLine.find(curvesHead) != std::string::npos) || (currentLine.find(othersHead) != std::string::npos))
       {
           break;
       }
    }
}


void BAYAMLReader::ReadCurves()
{
    ifstream fileStream(fileName.c_str());
    std::string currentLine;
    const std::string tracksHead = "tracks:";
    const std::string eventsHead = "events:";
    const std::string curvesHead = "curves:";
    const std::string othersHead = "other_features:";

    //Loop over lines, doing nothing, until found beginning of tracks section
    while (std::getline(fileStream, currentLine) && currentLine.find(curvesHead) == std::string::npos);  //Do nothing!! (intentional)

    while (std::getline(fileStream, currentLine)) 
    {
       if (currentLine.find("-") != std::string::npos)
       {
           allCurves.push_back(BACurve());
       }

       if (currentLine.find("time:") != std::string::npos)
       {
           allCurves.back().SetTime(std::stod(GetKey(currentLine)));
       }
       else if (currentLine.find("start_f:") != std::string::npos)
       {
           allCurves.back().SetStartFrequency(std::stod(GetKey(currentLine)));
       }
       else if (currentLine.find("end_f:") != std::string::npos)
       {
           allCurves.back().SetEndFrequency(std::stod(GetKey(currentLine)));
       }
       else if (currentLine.find("acquisition_number:") != std::string::npos)
       {
           allCurves.back().SetAcquisitionNumber(std::stoi(GetKey(currentLine)));
       }
       else if ((currentLine.find(tracksHead) != std::string::npos) || (currentLine.find(eventsHead) != std::string::npos) || (currentLine.find(othersHead) != std::string::npos))
       {
           break;
       }
    }
}



void BAYAMLReader::ReadOthers()
{
    ifstream fileStream(fileName.c_str());
    std::string currentLine;
    const std::string tracksHead = "tracks:";
    const std::string eventsHead = "events:";
    const std::string curvesHead = "curves:";
    const std::string othersHead = "other_features:";

    //Loop over lines, doing nothing, until found beginning of tracks section
    while (std::getline(fileStream, currentLine) && currentLine.find(othersHead) == std::string::npos);  //Do nothing!! (intentional)

    while (std::getline(fileStream, currentLine)) 
    {
       if (currentLine.find("-") != std::string::npos)
       {
           allOthers.push_back(BAOther());
       }

       if (currentLine.find("time:") != std::string::npos)
       {
           allOthers.back().SetTime(std::stod(GetKey(currentLine)));
       }
       else if (currentLine.find("frequency:") != std::string::npos)
       {
           allOthers.back().SetFrequency(std::stod(GetKey(currentLine)));
       }
       else if (currentLine.find("comment:") != std::string::npos)
       {
           allOthers.back().SetComment(GetKey(currentLine));
       }
       else if (currentLine.find("acquisition_number:") != std::string::npos)
       {
           allOthers.back().SetAcquisitionNumber(std::stoi(GetKey(currentLine)));
       }
       else if ((currentLine.find(tracksHead) != std::string::npos) || (currentLine.find(eventsHead) != std::string::npos) || (currentLine.find(curvesHead) != std::string::npos))
       {
           break;
       }
    }
}


#endif /* BAYAMLREADER_HH_ */
