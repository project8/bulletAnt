/*
 * HandScanAssistant.C
 *
 *  Created on: Feb 21, 2018
 *      Author: nbuzinsky
 */

#include <TGClient.h>
#include <TGButton.h>
#include <TGLabel.h>
#include <TGFrame.h>
#include <TGLayout.h>
#include <TGSplitter.h>
#include <TRootEmbeddedCanvas.h>
#include <TH2D.h>
#include <TApplication.h>

#include "BTrack.hh"
#include "BCurve.hh"
#include "BOther.hh"
#include "HandScanAssistant.hh"



MyMainFrame::MyMainFrame(const TGWindow *p, std::string spectrogramFilename, UInt_t windowWidth, UInt_t windowHeight) : TGMainFrame(p, windowWidth, windowHeight), acquisitionIndex(0)
{

    spectrogramFile = new TFile(spectrogramFilename.c_str());
    SetupHistogramVector();

    // Create pair of vertical frames, in global frame object
    TGHorizontalFrame *fHorizontalFrame = new TGHorizontalFrame(this, windowWidth, windowHeight);

    const int buttonFrameWidth = 250;
    TGVerticalFrame *fVerticalFrame1 = new TGVerticalFrame(fHorizontalFrame, buttonFrameWidth, windowHeight, kFixedWidth);
    TGVerticalFrame *fVerticalFrame2 = new TGVerticalFrame(fHorizontalFrame, windowWidth, windowHeight);

    TGCompositeFrame *leftButtonFrame = new TGCompositeFrame(fVerticalFrame1, 400, windowHeight, kSunkenFrame); //Numbers dont matter here, fits full frame
    TGCompositeFrame *rightCanvasFrame = new TGCompositeFrame(fVerticalFrame2, 400, windowHeight, kSunkenFrame);

    fVerticalFrame1->AddFrame(leftButtonFrame, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 0, 0, 5, 10));
    fVerticalFrame2->AddFrame(rightCanvasFrame, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 0, 0, 5, 10));

    fHorizontalFrame->AddFrame(fVerticalFrame1, new TGLayoutHints(kLHintsLeft | kLHintsExpandY));

    // Create canvas widget
    fEmbeddedCanvas = new TRootEmbeddedCanvas("EmbeddedCanvas",rightCanvasFrame,windowWidth,windowHeight);
    int canvasPadding[4] = {10,10,10,1}; // left, right, top, bottom, in pixels
    rightCanvasFrame->AddFrame(fEmbeddedCanvas, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, canvasPadding[0], canvasPadding[1], canvasPadding[2], canvasPadding[3]));


    //Create Splitter
    TGVSplitter *splitter = new TGVSplitter(fHorizontalFrame,2,2);
    splitter->SetFrame(fVerticalFrame1, kTRUE);
    fHorizontalFrame->AddFrame(splitter, new TGLayoutHints(kLHintsLeft | kLHintsExpandY));

    fHorizontalFrame->AddFrame(fVerticalFrame2, new TGLayoutHints(kLHintsRight | kLHintsExpandX | kLHintsExpandY));
    AddFrame(fHorizontalFrame, new TGLayoutHints(kLHintsRight | kLHintsExpandX | kLHintsExpandY));

    ///// Lower Button Frame
    TGVerticalFrame *vframe = new TGVerticalFrame(this, 400, 400);
    TGCompositeFrame *cframe2 = new TGCompositeFrame(vframe, 200, 200, kHorizontalFrame | kFixedWidth );

    TGTextButton *previousSpectrogramButton = new TGTextButton(cframe2, "&Previous Spectrogram");
    cframe2->AddFrame(previousSpectrogramButton, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 3, 2, 2, 2));
    previousSpectrogramButton->Connect("Clicked()", "MyMainFrame", this, "DrawPreviousSpectrogram()");
    previousSpectrogramButton->SetToolTipText("Click on the button to save the application as C++ macro");

    TGTextButton *nextSpectrogramButton = new TGTextButton(cframe2, "&Next Spectrogram");
    cframe2->AddFrame(nextSpectrogramButton, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 3, 2, 2, 2));
    nextSpectrogramButton->Connect("Clicked()", "MyMainFrame", this, "DrawNextSpectrogram()");
    nextSpectrogramButton->SetToolTipText("Click on the button to save the application as C++ macro");

    TGTextButton *exit = new TGTextButton(cframe2, "&Exit ","gApplication->Terminate(0)");
    cframe2->AddFrame(exit, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 2, 0, 2, 2));
    vframe->AddFrame(cframe2, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));
    AddFrame(vframe, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));


    //Add buttons and sliders to left window
    
    //Add progress bar, for gratification
    progressBar = new TGHProgressBar(leftButtonFrame,TGProgressBar::kStandard,200);
    progressBar->ShowPosition();
    leftButtonFrame->AddFrame(progressBar, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY, 15,15,3,4));
    progressBar->SetBarColor("green");
    progressBar->SetRange(0,histogramNames.size()-1);

    TGTextButton *createTrackButton = new TGTextButton(leftButtonFrame,"&Create Track");
    createTrackButton->Connect("Clicked()","MyMainFrame",this,"CreateLine()");
    leftButtonFrame->AddFrame(createTrackButton, new TGLayoutHints(kLHintsCenterX | kLHintsTop, 5,5,3,4));
    createTrackButton->SetToolTipText("Create a new Line to fit to a track");

    //Change current track to be a sideband?
    sidebandButton = new TGCheckButton(leftButtonFrame, "Sideband",67);
    sidebandButton->Connect("Clicked()","MyMainFrame",this,"SidebandBoolButton()");
    leftButtonFrame->AddFrame(sidebandButton, new TGLayoutHints(kLHintsCenterX | kLHintsTop, 1,1,4,1));
    sidebandButton->SetState(kButtonDisabled);

    //Is part of a curve
    curvedButton = new TGCheckButton(leftButtonFrame, "Curved",67);
    curvedButton->Connect("Clicked()","MyMainFrame",this,"CurvedBoolButton()");
    leftButtonFrame->AddFrame(curvedButton, new TGLayoutHints(kLHintsCenterX | kLHintsTop, 1,1,1,1));
    curvedButton->SetState(kButtonDisabled);

    TGTextButton *createCurveButton = new TGTextButton(leftButtonFrame,"&Create Curve");
    createCurveButton->Connect("Clicked()","MyMainFrame",this,"CreateCurve()");
    leftButtonFrame->AddFrame(createCurveButton, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 5,5,3,4));
    createCurveButton->SetToolTipText("Create a line specifically for curved track");

    TGTextButton *createOtherButton = new TGTextButton(leftButtonFrame,"&Other Feature");
    createOtherButton->Connect("Clicked()","MyMainFrame",this,"CreateOther()");
    leftButtonFrame->AddFrame(createOtherButton, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 5,5,3,4));
    createOtherButton->SetToolTipText("Create a line specifically for curved track");


    TGTextButton *writeYAMLButton = new TGTextButton(leftButtonFrame,"&Write To YAML");
    writeYAMLButton->Connect("Clicked()","MyMainFrame",this,"WriteToYAML()");
    leftButtonFrame->AddFrame(writeYAMLButton, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 5,5,3,4));
    writeYAMLButton->SetToolTipText("Write all found tracks to file");

    TGTextButton *refreshPlotButton = new TGTextButton(leftButtonFrame,"&Refresh Plot");
    refreshPlotButton->Connect("Clicked()","MyMainFrame",this,"DrawCurrentSpectrogram()");
    leftButtonFrame->AddFrame(refreshPlotButton, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 5,5,3,4));
    refreshPlotButton->SetToolTipText("Messed up the zooming? Refresh");


    //Sliders
    TGLabel *xSliderLabel = new TGLabel(leftButtonFrame, "X Axis (Time)");
    leftButtonFrame->AddFrame(xSliderLabel, new TGLayoutHints(kLHintsExpandX | kLHintsTop, 5, 5, 2, 2));

    const int horizontalXSliderID = 45;
    horizontalXSlider = new TGDoubleHSlider(leftButtonFrame,150,kDoubleScaleBoth,horizontalXSliderID);
    horizontalXSlider->Connect("PositionChanged()", "MyMainFrame", this, "DoSlider()");
    horizontalXSlider->SetRange(0,0.01);
    horizontalXSlider->SetPosition(0,0.0025);
    leftButtonFrame->AddFrame(horizontalXSlider, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 15,15, 2, 20));

    TGLabel *ySliderLabel = new TGLabel(leftButtonFrame, "Y Axis (Frequency)");
    leftButtonFrame->AddFrame(ySliderLabel, new TGLayoutHints(kLHintsExpandX | kLHintsTop, 5, 5, 2, 2));

    const int horizontalYSliderID = 46;
    horizontalYSlider = new TGDoubleHSlider(leftButtonFrame,150,kDoubleScaleBoth,horizontalYSliderID);
    horizontalYSlider->Connect("PositionChanged()", "MyMainFrame", this, "DoSlider()");
    horizontalYSlider->SetRange(50e6,150e6);
    horizontalYSlider->SetPosition(50e6,65e6);
    leftButtonFrame->AddFrame(horizontalYSlider, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 15, 15, 2, 20));


    //Finishing Touches
    // What to clean up in destructor
    SetCleanup(kDeepCleanup);

    // Set a name to the main frame
    SetWindowName("Project 8 Handscanning Interface");
    MapSubwindows();
    MapWindow();

    //Draw first spectrogram automatically
    DrawCurrentSpectrogram();
}

void MyMainFrame::CurvedBoolButton()
{
    if(allTracks.empty()) return; //Do not want segfaults ... do nothing
    for(int i=allTracks.size() - 1; i >=0; --i)
    {
        if(allTracks[i].GetAcquisitionNumber() == acquisitionIndex)
        {
            allTracks[i].SetCurvedStatus();
            break;
        }
    }

}

void MyMainFrame::SidebandBoolButton()
{
    if(allTracks.empty()) return; //Do not want segfaults ... do nothing
    for(int i=allTracks.size() - 1; i >=0; --i)
    {
        if(allTracks[i].GetAcquisitionNumber() == acquisitionIndex)
        {
            allTracks[i].SetSidebandStatus();
            break;
        }
    }


}


void MyMainFrame::WriteToYAML()
{
    //WriteMetaData();
    //WriteTracks();
    //WriteCurves();
}


void MyMainFrame::SetupHistogramVector()
{
    std::string currentHistogramName;
    TList * histogramList = spectrogramFile->GetListOfKeys();

    int j = 0;
    while(histogramList->At(j))
    {
        currentHistogramName = histogramList->At(j)->GetName();
        if(histogramNames.empty() || histogramNames.back() != currentHistogramName)
        {
            histogramNames.push_back(currentHistogramName);
        }

        ++j;
    }

}

void MyMainFrame::DrawAll()
{
    for(int i=0;i<allTracks.size();++i)
    {
        if(allTracks[i].GetAcquisitionNumber() == acquisitionIndex)
        {
            allTracks[i].Draw();
        }
    }

    for(int i=0;i<allCurves.size();++i)
    {
        if(allCurves[i].GetAcquisitionNumber() == acquisitionIndex)
        {
            allCurves[i].Draw();
        }
    }

    for(int i=0;i<allOthers.size();++i)
    {
        if(allOthers[i].GetAcquisitionNumber() == acquisitionIndex)
        {
            allOthers[i].Draw();
        }
    }
}

void MyMainFrame::CreateOther()
{
    // Draws function graphics in randomly chosen interval
    double xAxisInterval[2] = {horizontalXSlider->GetMinPosition(), horizontalXSlider->GetMaxPosition()};
    double yAxisInterval[2] = {horizontalYSlider->GetMinPosition(), horizontalYSlider->GetMaxPosition()};
    double xAxisLength = xAxisInterval[1] - xAxisInterval[0];
    double yAxisLength = yAxisInterval[1] - yAxisInterval[0];
    double xLinePosition =  xAxisInterval[0] + 0.5 * xAxisLength;
    double yLinePosition =  yAxisInterval[0] + 0.5 * yAxisLength;

    allOthers.push_back(BOther(xLinePosition, yLinePosition,acquisitionIndex ));

    DrawAll();

    //Gets current canvas and updates after button press
    TCanvas *fCanvas = fEmbeddedCanvas->GetCanvas();
    fCanvas->cd();
    fCanvas->Update();
}


void MyMainFrame::CreateCurve()
{
    // Draws function graphics in randomly chosen interval
    double xAxisInterval[2] = {horizontalXSlider->GetMinPosition(), horizontalXSlider->GetMaxPosition()};
    double yAxisInterval[2] = {horizontalYSlider->GetMinPosition(), horizontalYSlider->GetMaxPosition()};
    double xAxisLength = xAxisInterval[1] - xAxisInterval[0];
    double yAxisLength = yAxisInterval[1] - yAxisInterval[0];
    double xLinePosition[2] = { xAxisInterval[0] + 0.25 * xAxisLength, xAxisInterval[0] + 0.75 * xAxisLength};
    double yLinePosition[2] = { yAxisInterval[0] + 0.25 * yAxisLength, yAxisInterval[0] + 0.75 * yAxisLength};

    allCurves.push_back(BCurve(xLinePosition[0], yLinePosition[0], xLinePosition[1], yLinePosition[1], acquisitionIndex ));

    DrawAll();

    //Gets current canvas and updates after button press
    TCanvas *fCanvas = fEmbeddedCanvas->GetCanvas();
    fCanvas->cd();
    fCanvas->Update();
}


void MyMainFrame::CreateLine()
{
    // Draws function graphics in randomly chosen interval
    double xAxisInterval[2] = {horizontalXSlider->GetMinPosition(), horizontalXSlider->GetMaxPosition()};
    double yAxisInterval[2] = {horizontalYSlider->GetMinPosition(), horizontalYSlider->GetMaxPosition()};
    double xAxisLength = xAxisInterval[1] - xAxisInterval[0];
    double yAxisLength = yAxisInterval[1] - yAxisInterval[0];
    double xLinePosition[2] = { xAxisInterval[0] + 0.25 * xAxisLength, xAxisInterval[0] + 0.75 * xAxisLength};
    double yLinePosition[2] = { yAxisInterval[0] + 0.25 * yAxisLength, yAxisInterval[0] + 0.75 * yAxisLength};

    allTracks.push_back(BTrack(xLinePosition[0], yLinePosition[0], xLinePosition[1], yLinePosition[1], acquisitionIndex ));

    DrawAll();

    sidebandButton->SetState(kButtonUp);
    curvedButton->SetState(kButtonUp);

    //Gets current canvas and updates after button press
    TCanvas *fCanvas = fEmbeddedCanvas->GetCanvas();
    fCanvas->cd();
    fCanvas->Update();
}


void MyMainFrame::DrawNextSpectrogram()
{
    if(acquisitionIndex < histogramNames.size() - 1) //If we are not out of range
    {
        ++acquisitionIndex;
        progressBar->Increment(1);
        DrawCurrentSpectrogram();
        SetButtonStatus();
    }
}

void MyMainFrame::DrawPreviousSpectrogram()
{
    if(acquisitionIndex!=0)
    {
        --acquisitionIndex;

        progressBar->Reset();
        progressBar->Increment(acquisitionIndex);
        DrawCurrentSpectrogram();
        SetButtonStatus();
    }

}

void MyMainFrame::SetButtonStatus()
{
    sidebandButton->SetState(kButtonUp);
    curvedButton->SetState(kButtonUp);

    sidebandButton->SetState(kButtonDisabled);
    curvedButton->SetState(kButtonDisabled);

    for(int i=allTracks.size() - 1; i >=0; --i)
    {
        if(allTracks[i].GetAcquisitionNumber() == acquisitionIndex)
        {
            (allTracks[i].GetSidebandStatus()) ? sidebandButton->SetState(kButtonDown) : sidebandButton->SetState(kButtonUp);
            (allTracks[i].GetCurvedStatus()) ? curvedButton->SetState(kButtonDown) : curvedButton->SetState(kButtonUp);
            break;
        }
    }
}

void MyMainFrame::DrawCurrentSpectrogram() //Draws spectrogram in righthand canvas depending on the acquisitionIndex
{
    std::string currentHistogramName = histogramNames[acquisitionIndex];
    TObject *fileObject  = fileObject = spectrogramFile->Get(currentHistogramName.c_str());
    currentHistogram = (TH2D*) fileObject;
    currentHistogram->Draw("colz");

    currentHistogram->SetTitle(histogramNames[acquisitionIndex].c_str());
    //currentHistogram->CenterTitle();

    DrawAll();

    //By default, have zoomed in view of spectrogram
    currentHistogram->GetXaxis()->SetRangeUser(0,0.0025);
    currentHistogram->GetYaxis()->SetRangeUser(50e6,65e6);

    horizontalXSlider->SetPosition(0,0.0025);
    horizontalYSlider->SetPosition(50e6,65e6);


    //Gets current canvas and updates after button press
    TCanvas *fCanvas = fEmbeddedCanvas->GetCanvas();
    fCanvas->cd();
    fCanvas->Update();
}

void MyMainFrame::DoSlider()
{
    currentHistogram->GetXaxis()->SetRangeUser(horizontalXSlider->GetMinPosition(),horizontalXSlider->GetMaxPosition());
    currentHistogram->GetYaxis()->SetRangeUser(horizontalYSlider->GetMinPosition(),horizontalYSlider->GetMaxPosition());
    currentHistogram->Draw("colz");

    currentHistogram->SetTitle(histogramNames[acquisitionIndex].c_str());
    //currentHistogram->CenterTitle();

    DrawAll();

    //Gets current canvas and updates after button press
    TCanvas *fCanvas = fEmbeddedCanvas->GetCanvas();
    fCanvas->cd();
    fCanvas->Update();
}

MyMainFrame::~MyMainFrame()
{
   // Clean up all widgets, frames and layouthints that were used
   Cleanup();
}

void MyMainFrame::CloseWindow()
{
   // Called when window is closed via the window manager.
   delete this; 
}

void HandScanAssistant(std::string inputFilename)
{
   // Popup the GUI...
   gStyle->SetOptStat(0);
   new MyMainFrame(gClient->GetRoot(), inputFilename , 1440, 900);
}

