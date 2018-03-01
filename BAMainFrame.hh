/*
 * BAMainFrame.hh
 *
 *  Created on: Feb 23, 2018
 *      Author: nbuzinsky
 */

#ifndef BAMAINFRAME_HH_
#define BAMAINFRAME_HH_

#include <TGClient.h>
#include <TGButton.h>
#include <TGLabel.h>
#include <TGFrame.h>
#include <TGLayout.h>
#include <TGSplitter.h>
#include <TGFileDialog.h>
#include <TRootEmbeddedCanvas.h>
#include <TH2D.h>

#include "BAYAMLWriter.hh"
#include "BAYAMLReader.hh"

class BAMainFrame : public TGMainFrame 
{
    public:
        BAMainFrame(const TGWindow *p, std::string spectrogramFilename, UInt_t windowWidth, UInt_t windowHeight);
        virtual ~BAMainFrame();


        void CreateTrack();
        void CreateCurve();
        void CreateOther();

        void CurvedBoolButton();
        void SidebandBoolButton();

        void WriteToYAML();
        void LoadFromYAML();

        void DoSlider();

        void DrawCurrentSpectrogram();
        void DrawNextSpectrogram();
        void DrawPreviousSpectrogram();

        void OkayButton();
        void CancelButton();
        Bool_t HandleKey(Event_t *event);
        void SlideBar(TGDoubleHSlider &hSlider, bool isTimeAxis, bool positiveDirection);
        void Zoom(TGDoubleHSlider &hSlider, bool isTimeAxis, bool zoomIn);

    private:
        void CreateDialog(const std::string &userPrompt);
        const char *LoadFileDialog();

        void SetupHistogramVector();
        void DrawAll();
        void SetButtonStatus();


        void CloseWindow();

        TRootEmbeddedCanvas *fEmbeddedCanvas;

        std::vector<BATrack> allTracks;
        std::vector<BACurve> allCurves;
        std::vector<BAOther> allOthers;

        TFile *spectrogramFile;
        TH2D *currentHistogram;

        TGDoubleHSlider *horizontalXSlider;
        TGDoubleHSlider *horizontalYSlider;
        TGDoubleHSlider *horizontalZSlider;

        TGHProgressBar *progressBar;

        TGHorizontalFrame *fHorizontalFrame;

        TGCheckButton *sidebandButton;
        TGCheckButton *curvedButton;

        int acquisitionIndex;

        std::vector<std::string> histogramNames;

        std::string userInput;
        TGTextEntry *fTE;
        TGTransientFrame *fDialog;
        std::string spectrogramFilename;


    ClassDef(BAMainFrame, 0)
};

BAMainFrame::BAMainFrame(const TGWindow *p, std::string inputFilename, UInt_t windowWidth, UInt_t windowHeight) : 
    TGMainFrame(p, windowWidth, windowHeight), 
    acquisitionIndex(0), 
    spectrogramFilename(inputFilename)
{

    spectrogramFile = new TFile(spectrogramFilename.c_str());
    SetupHistogramVector();

    // Create pair of vertical frames, in global frame object
    //TGHorizontalFrame *fHorizontalFrame = new TGHorizontalFrame(this, windowWidth, windowHeight);
    fHorizontalFrame = new TGHorizontalFrame(this, windowWidth, windowHeight);

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
    previousSpectrogramButton->Connect("Clicked()", "BAMainFrame", this, "DrawPreviousSpectrogram()");
    previousSpectrogramButton->SetToolTipText("Click on the button to save the application as C++ macro");

    TGTextButton *nextSpectrogramButton = new TGTextButton(cframe2, "&Next Spectrogram");
    cframe2->AddFrame(nextSpectrogramButton, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 3, 2, 2, 2));
    nextSpectrogramButton->Connect("Clicked()", "BAMainFrame", this, "DrawNextSpectrogram()");
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
    createTrackButton->Connect("Clicked()","BAMainFrame",this,"CreateTrack()");
    leftButtonFrame->AddFrame(createTrackButton, new TGLayoutHints(kLHintsCenterX | kLHintsTop, 5,5,3,4));
    createTrackButton->SetToolTipText("Create a new Line to fit to a track");

    //Change current track to be a sideband?
    sidebandButton = new TGCheckButton(leftButtonFrame, "Sideband",67);
    sidebandButton->Connect("Clicked()","BAMainFrame",this,"SidebandBoolButton()");
    leftButtonFrame->AddFrame(sidebandButton, new TGLayoutHints(kLHintsCenterX | kLHintsTop, 1,1,4,1));
    sidebandButton->SetState(kButtonDisabled);

    //Is part of a curve
    curvedButton = new TGCheckButton(leftButtonFrame, "Curved",67);
    curvedButton->Connect("Clicked()","BAMainFrame",this,"CurvedBoolButton()");
    leftButtonFrame->AddFrame(curvedButton, new TGLayoutHints(kLHintsCenterX | kLHintsTop, 1,1,1,1));
    curvedButton->SetState(kButtonDisabled);

    TGTextButton *createCurveButton = new TGTextButton(leftButtonFrame,"&Create Curve");
    createCurveButton->Connect("Clicked()","BAMainFrame",this,"CreateCurve()");
    leftButtonFrame->AddFrame(createCurveButton, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 5,5,3,4));
    createCurveButton->SetToolTipText("Create a line specifically for curved track");

    TGTextButton *createOtherButton = new TGTextButton(leftButtonFrame,"&Other Feature");
    createOtherButton->Connect("Clicked()","BAMainFrame",this,"CreateOther()");
    leftButtonFrame->AddFrame(createOtherButton, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 5,5,3,4));
    createOtherButton->SetToolTipText("Create a line specifically for curved track");

    TGTextButton *writeYAMLButton = new TGTextButton(leftButtonFrame,"&Write To YAML");
    writeYAMLButton->Connect("Clicked()","BAMainFrame",this,"WriteToYAML()");
    leftButtonFrame->AddFrame(writeYAMLButton, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 5,5,3,4));
    writeYAMLButton->SetToolTipText("Write all found tracks to file");

    TGTextButton *loadYAMLButton = new TGTextButton(leftButtonFrame,"&Load from YAML");
    loadYAMLButton->Connect("Clicked()","BAMainFrame",this,"LoadFromYAML()");
    leftButtonFrame->AddFrame(loadYAMLButton, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 5,5,3,4));
    loadYAMLButton->SetToolTipText("Load the tracks from another YAML into bulletAnt");

    TGTextButton *refreshPlotButton = new TGTextButton(leftButtonFrame,"&Refresh Plot");
    refreshPlotButton->Connect("Clicked()","BAMainFrame",this,"DrawCurrentSpectrogram()");
    leftButtonFrame->AddFrame(refreshPlotButton, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 5,5,3,4));
    refreshPlotButton->SetToolTipText("Messed up the zooming? Refresh");


    //Sliders
    TGLabel *xSliderLabel = new TGLabel(leftButtonFrame, "X Axis (Time)");
    leftButtonFrame->AddFrame(xSliderLabel, new TGLayoutHints(kLHintsExpandX | kLHintsTop, 5, 5, 2, 2));

    const int horizontalXSliderID = 45;
    horizontalXSlider = new TGDoubleHSlider(leftButtonFrame,150,kDoubleScaleBoth,horizontalXSliderID);
    horizontalXSlider->Connect("PositionChanged()", "BAMainFrame", this, "DoSlider()");
    horizontalXSlider->SetRange(0,0.01);
    horizontalXSlider->SetPosition(0,0.0025);
    leftButtonFrame->AddFrame(horizontalXSlider, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 15,15, 2, 20));

    TGLabel *ySliderLabel = new TGLabel(leftButtonFrame, "Y Axis (Frequency)");
    leftButtonFrame->AddFrame(ySliderLabel, new TGLayoutHints(kLHintsExpandX | kLHintsTop, 5, 5, 2, 2));

    const int horizontalYSliderID = 46;
    horizontalYSlider = new TGDoubleHSlider(leftButtonFrame,150,kDoubleScaleBoth,horizontalYSliderID);
    horizontalYSlider->Connect("PositionChanged()", "BAMainFrame", this, "DoSlider()");
    horizontalYSlider->SetRange(50e6,150e6);
    horizontalYSlider->SetPosition(50e6,65e6);
    leftButtonFrame->AddFrame(horizontalYSlider, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 15, 15, 2, 20));

    TGLabel *zSliderLabel = new TGLabel(leftButtonFrame, "Z Axis (Power Cut)");
    leftButtonFrame->AddFrame(zSliderLabel, new TGLayoutHints(kLHintsExpandX | kLHintsTop, 5, 5, 2, 2));

    const int horizontalZSliderID = 47;
    horizontalZSlider = new TGDoubleHSlider(leftButtonFrame,150,kDoubleScaleBoth,horizontalZSliderID);
    horizontalZSlider->Connect("PositionChanged()", "BAMainFrame", this, "DoSlider()");
    horizontalZSlider->SetRange(0.,0.1);
    horizontalZSlider->SetPosition(0.06,0.1);
    leftButtonFrame->AddFrame(horizontalZSlider, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 15, 15, 2, 20));

    const TGWindow *main = gClient->GetRoot();
    BindKey(main, gVirtualX->KeysymToKeycode(kKey_a), 0);
    BindKey(main, gVirtualX->KeysymToKeycode(kKey_s), 0);
    BindKey(main, gVirtualX->KeysymToKeycode(kKey_w), 0);
    BindKey(main, gVirtualX->KeysymToKeycode(kKey_d), 0);
    BindKey(main, gVirtualX->KeysymToKeycode(kKey_r), 0);

    BindKey(main, gVirtualX->KeysymToKeycode(kKey_n), 0);
    BindKey(main, gVirtualX->KeysymToKeycode(kKey_p), 0);

    BindKey(main, gVirtualX->KeysymToKeycode(kKey_z), 0);
    BindKey(main, gVirtualX->KeysymToKeycode(kKey_c), 0);


    //Finishing Touches
    // What to clean up in destructor
    SetCleanup(kDeepCleanup);

    // Set a name to the main frame
    SetWindowName("bulletAnt- Project 8 Handscanning Interface");
    SetWMSizeHints(600, 400, windowWidth, windowHeight, 0, 0);
    MapSubwindows();
    MapWindow();

    //Draw first spectrogram automatically
    DrawCurrentSpectrogram();
}

Bool_t BAMainFrame::HandleKey(Event_t *event)
{
   // Handle arrow and spacebar keys
   char tmp[2];
   unsigned keySymbol;
   gVirtualX->LookupString(event, tmp, sizeof(tmp), keySymbol);

   if (event->fType == kGKeyPress) 
   {
      switch ((EKeySym)keySymbol) 
      {
          //Refresh
         case kKey_r:
             DrawCurrentSpectrogram();
             break;

         case kKey_n:
             DrawNextSpectrogram();
             break;

         case kKey_p:
             DrawPreviousSpectrogram();

         case kKey_d:
            SlideBar(*horizontalXSlider,true, true);
            break;

         case kKey_a:
            SlideBar(*horizontalXSlider,true, false);
            break;

         case kKey_w:
            SlideBar(*horizontalYSlider,false, true);
            break;

         case kKey_s:
            SlideBar(*horizontalYSlider,false, false);
            break;

         case kKey_z:
             Zoom(*horizontalXSlider,true, true);
             Zoom(*horizontalYSlider,false, true);
            break;

         case kKey_c:
             Zoom(*horizontalXSlider, true, false);
             Zoom(*horizontalYSlider, false, false);
            break;

         default:
            return kTRUE;
      }
   }
   return kTRUE;
}
void BAMainFrame::Zoom(TGDoubleHSlider &hSlider, bool isTimeAxis, bool zoomIn)
{
    double sliderBounds[2] = {hSlider.GetMinPosition(), hSlider.GetMaxPosition()};
    double axisBounds[2]; 
    double minLength[2] = {1e-5, 1e6};

    if(isTimeAxis)
    {
        axisBounds[0] = 0.; axisBounds[1] = 0.01;
    }
    else
    {
        axisBounds[0] = 50.e6; axisBounds[1] = 150.e6;
    }

    double sliderLength = sliderBounds[1] - sliderBounds[0];

    double sliderShift[2];
    const double slideFraction = 0.25;
    if(zoomIn)
    {
        sliderShift[0] =  slideFraction * sliderLength;
        sliderShift[1] = -slideFraction * sliderLength;
    }
    else
    {
        sliderShift[0] = TMath::Max(axisBounds[0] - sliderBounds[0], -slideFraction * sliderLength);
        sliderShift[1] = TMath::Min(axisBounds[1] - sliderBounds[1], slideFraction * sliderLength);
    }

    hSlider.SetPosition(sliderBounds[0] + sliderShift[0], sliderBounds[1] + sliderShift[1]);
    DoSlider();

}

void BAMainFrame::SlideBar(TGDoubleHSlider &hSlider, bool isTimeAxis, bool positiveDirection)
{
    double sliderBounds[2] = {hSlider.GetMinPosition(), hSlider.GetMaxPosition()};
    double axisBounds[2]; 

    if(isTimeAxis)
    {
        axisBounds[0] = 0.; axisBounds[1] = 0.01;
    }
    else
    {
        axisBounds[0] = 50.e6; axisBounds[1] = 150.e6;
    }

    double sliderLength = sliderBounds[1] - sliderBounds[0];

    double sliderShift;
    const double slideFraction = 0.5;
    if(positiveDirection)
        sliderShift = TMath::Min(axisBounds[1] - sliderBounds[1], slideFraction * sliderLength);
    else
        sliderShift = TMath::Max(axisBounds[0] - sliderBounds[0], -slideFraction * sliderLength);

    hSlider.SetPosition(sliderBounds[0] + sliderShift, sliderBounds[1] + sliderShift);
    DoSlider();

}

BAMainFrame::~BAMainFrame()
{
   // Clean up all widgets, frames and layouthints that were used
   Cleanup();
}


void BAMainFrame::CreateTrack()
{
    // Draws function graphics in randomly chosen interval
    double xAxisInterval[2] = {horizontalXSlider->GetMinPosition(), horizontalXSlider->GetMaxPosition()};
    double yAxisInterval[2] = {horizontalYSlider->GetMinPosition(), horizontalYSlider->GetMaxPosition()};
    double xAxisLength = xAxisInterval[1] - xAxisInterval[0];
    double yAxisLength = yAxisInterval[1] - yAxisInterval[0];
    double xLinePosition[2] = { xAxisInterval[0] + 0.25 * xAxisLength, xAxisInterval[0] + 0.75 * xAxisLength};
    double yLinePosition[2] = { yAxisInterval[0] + 0.25 * yAxisLength, yAxisInterval[0] + 0.75 * yAxisLength};

    allTracks.push_back(BATrack(xLinePosition[0], yLinePosition[0], xLinePosition[1], yLinePosition[1], acquisitionIndex ));

    DrawAll();

    sidebandButton->SetState(kButtonUp);
    curvedButton->SetState(kButtonUp);

    //Gets current canvas and updates after button press
    TCanvas *fCanvas = fEmbeddedCanvas->GetCanvas();
    fCanvas->cd();
    fCanvas->Update();
}

void BAMainFrame::CreateCurve()
{
    // Draws function graphics in randomly chosen interval
    double xAxisInterval[2] = {horizontalXSlider->GetMinPosition(), horizontalXSlider->GetMaxPosition()};
    double yAxisInterval[2] = {horizontalYSlider->GetMinPosition(), horizontalYSlider->GetMaxPosition()};
    double xAxisLength = xAxisInterval[1] - xAxisInterval[0];
    double yAxisLength = yAxisInterval[1] - yAxisInterval[0];
    double xLinePosition[2] = { xAxisInterval[0] + 0.25 * xAxisLength, xAxisInterval[0] + 0.75 * xAxisLength};
    double yLinePosition[2] = { yAxisInterval[0] + 0.25 * yAxisLength, yAxisInterval[0] + 0.75 * yAxisLength};

    allCurves.push_back(BACurve(xLinePosition[0], yLinePosition[0], xLinePosition[1], yLinePosition[1], acquisitionIndex ));

    DrawAll();

    //Gets current canvas and updates after button press
    TCanvas *fCanvas = fEmbeddedCanvas->GetCanvas();
    fCanvas->cd();
    fCanvas->Update();
}

void BAMainFrame::CreateOther()
{
    const std::string userPrompt = "Please enter a brief description of this feature";
    CreateDialog(userPrompt);
    std::string userDescription = userInput;
    userInput.clear();
    if(!userDescription.empty())
    {
        // Draws function graphics in randomly chosen interval
        double xAxisInterval[2] = {horizontalXSlider->GetMinPosition(), horizontalXSlider->GetMaxPosition()};
        double yAxisInterval[2] = {horizontalYSlider->GetMinPosition(), horizontalYSlider->GetMaxPosition()};
        double xAxisLength = xAxisInterval[1] - xAxisInterval[0];
        double yAxisLength = yAxisInterval[1] - yAxisInterval[0];
        double xLinePosition =  xAxisInterval[0] + 0.5 * xAxisLength;
        double yLinePosition =  yAxisInterval[0] + 0.5 * yAxisLength;

        allOthers.push_back(BAOther(xLinePosition, yLinePosition, acquisitionIndex, userDescription ));

        DrawAll();

        //Gets current canvas and updates after button press
        TCanvas *fCanvas = fEmbeddedCanvas->GetCanvas();
        fCanvas->cd();
        fCanvas->Update();
    }
}


void BAMainFrame::CurvedBoolButton()
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

void BAMainFrame::SidebandBoolButton()
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

void BAMainFrame::WriteToYAML()
{
    const std::string userPrompt = "Please enter your last name (no caps/ spaces)";
    CreateDialog(userPrompt);
    std::string scannerName = userInput;
    userInput.clear();
    if(!scannerName.empty())
    {
        BAYAMLWriter writeYAML(allTracks,allCurves,allOthers, spectrogramFilename, scannerName);
        writeYAML.Write();
    }

}

void BAMainFrame::LoadFromYAML()
{
    const char *fileCharacters = LoadFileDialog();
    if(fileCharacters)
    {
        std::string loadingFilename = fileCharacters;
        BAYAMLReader yamlReader(loadingFilename);
        yamlReader.Read();

        allTracks = yamlReader.GetTracks();
        allCurves = yamlReader.GetCurves(); 
        allOthers = yamlReader.GetOthers();

        DrawCurrentSpectrogram();
    }
}

void BAMainFrame::DoSlider()
{
    currentHistogram->GetXaxis()->SetRangeUser(horizontalXSlider->GetMinPosition(),horizontalXSlider->GetMaxPosition());
    currentHistogram->GetYaxis()->SetRangeUser(horizontalYSlider->GetMinPosition(),horizontalYSlider->GetMaxPosition());
    currentHistogram->GetZaxis()->SetRangeUser(horizontalZSlider->GetMinPosition(),horizontalZSlider->GetMaxPosition());
    currentHistogram->Draw("colz");

    currentHistogram->SetTitle(histogramNames[acquisitionIndex].c_str());
    //currentHistogram->CenterTitle();

    DrawAll();

    //Gets current canvas and updates after button press
    TCanvas *fCanvas = fEmbeddedCanvas->GetCanvas();
    fCanvas->cd();
    fCanvas->Update();

    //gPad->GetListOfPrimitives()->Print();
    TObject *obj = gPad->GetListOfPrimitives()->At(0);
    TFrame *ftemp = (TFrame *) obj;
    ftemp->SetBit(kCannotPick);
}

void BAMainFrame::DrawCurrentSpectrogram() //Draws spectrogram in righthand canvas depending on the acquisitionIndex
{
    std::string currentHistogramName = histogramNames[acquisitionIndex];
    TObject *fileObject  = fileObject = spectrogramFile->Get(currentHistogramName.c_str());
    currentHistogram = (TH2D*) fileObject;
    currentHistogram->Draw("colz");

    currentHistogram->SetTitle(histogramNames[acquisitionIndex].c_str());

    //By default, have zoomed in view of spectrogram
    currentHistogram->GetXaxis()->SetRangeUser(0,0.01);
    currentHistogram->GetYaxis()->SetRangeUser(50e6,150e6);
    currentHistogram->GetZaxis()->SetRangeUser(0.06,0.1);

    currentHistogram->SetBit(kCannotPick);


    horizontalXSlider->SetPosition(0,0.01);
    horizontalYSlider->SetPosition(50e6,150e6);
    horizontalZSlider->SetPosition(0.06,0.1);

    DrawAll();

    //Gets current canvas and updates after button press
    TCanvas *fCanvas = fEmbeddedCanvas->GetCanvas();
    fCanvas->cd();
    fCanvas->Update();

    TObject *obj = gPad->GetListOfPrimitives()->At(0);
    TFrame *ftemp = (TFrame *) obj;
    ftemp->SetBit(kCannotPick);

}

void BAMainFrame::DrawNextSpectrogram()
{
    if(acquisitionIndex < histogramNames.size() - 1) //If we are not out of range
    {
        ++acquisitionIndex;
        progressBar->Increment(1);
        DrawCurrentSpectrogram();
        SetButtonStatus();

    }
    else
    {
       int returnValue;

       new TGMsgBox(gClient->GetRoot(), fHorizontalFrame, "Warning", "There are no more spectrograms left. Press \"Write to YAML\" if done.", kMBIconStop, 0, &returnValue);

    }
}

void BAMainFrame::DrawPreviousSpectrogram()
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

void BAMainFrame::OkayButton()
{
    userInput = std::string(fTE->GetText());
    fDialog->CloseWindow();
}

void BAMainFrame::CancelButton()
{
    userInput.clear();
    fDialog->CloseWindow();
}



void BAMainFrame::CreateDialog(const std::string &userPrompt )
{
    const TGWindow *main = gClient->GetRoot();
    fDialog = new TGTransientFrame(main, main, 10, 10);
    //TGTransientFrame *fDialog = new TGTransientFrame(main, main, 10, 10);
    fDialog->SetCleanup(kDeepCleanup);
    TGLabel *label = new TGLabel(fDialog, userPrompt.c_str());
    TGTextBuffer *tbuf = new TGTextBuffer(256);  //will be deleted by TGtextEntry
    tbuf->AddText(0, "");

    //TGTextEntry *fTE = new TGTextEntry(fDialog, tbuf);
    fTE = new TGTextEntry(fDialog, tbuf);
    fTE->Resize(260, fTE->GetDefaultHeight());
    //fTE->SetCommand(cmd);
    TGLayoutHints *l1 = new TGLayoutHints(kLHintsTop | kLHintsLeft, 5, 5, 5, 0);
    TGLayoutHints *l2 = new TGLayoutHints(kLHintsTop | kLHintsLeft, 5, 5, 5, 5);

    fDialog->AddFrame(label, l1);
    fDialog->AddFrame(fTE, l2);

   // create frame and layout hints for Ok and Cancel buttons
   TGHorizontalFrame *hf = new TGHorizontalFrame(fDialog, 60, 20, kFixedWidth);
   TGLayoutHints *l3 = new TGLayoutHints(kLHintsCenterY | kLHintsExpandX, 5, 5, 0, 0);

   // create OK and Cancel buttons in their own frame (hf)
   int  nb = 0; int width = 0; int height = 0;
   TGTextButton *b;

   b = new TGTextButton(hf, "&Ok");
   b->Connect("Clicked()","BAMainFrame",this,"OkayButton()");
   b->Associate(fDialog);
   hf->AddFrame(b, l3);
   height = b->GetDefaultHeight();
   width = b->GetDefaultWidth();
   ++nb;

   b = new TGTextButton(hf, "&Cancel");
   b->Connect("Clicked()","BAMainFrame",this,"CancelButton()");
   b->Associate(fDialog);
   hf->AddFrame(b, l3);
   height = b->GetDefaultHeight();
   width = b->GetDefaultWidth();
   ++nb;

   // place button frame (hf) at the bottom
   TGLayoutHints *l4 = new TGLayoutHints(kLHintsBottom | kLHintsCenterX, 0, 0, 5, 5);

   fDialog->AddFrame(hf, l4);

   // keep buttons centered and with the same width
   hf->Resize((width + 20) * nb, height);

   // set dialog title
   fDialog->SetWindowName("User Action Required");

   // map all widgets and calculate size of dialog
   fDialog->MapSubwindows();

   width  = fDialog->GetDefaultWidth();
   height = fDialog->GetDefaultHeight();

   fDialog->Resize(width, height);

   // position relative to the parent window (which is the root window)
   Window_t wdum;
   int      ax, ay;

   gVirtualX->TranslateCoordinates(main->GetId(), main->GetId(),
                          (((TGFrame *) main)->GetWidth() - width) >> 1,
                          (((TGFrame *) main)->GetHeight() - height) >> 1,
                          ax, ay, wdum);
   fDialog->Move(ax, ay);
   fDialog->SetWMPosition(ax, ay);

   // make the message box non-resizable
   fDialog->SetWMSize(width, height);
   fDialog->SetWMSizeHints(width, height, width, height, 0, 0);

   fDialog->SetMWMHints(kMWMDecorAll | kMWMDecorResizeH  | kMWMDecorMaximize |
                                       kMWMDecorMinimize | kMWMDecorMenu,
                        kMWMFuncAll  | kMWMFuncResize    | kMWMFuncMaximize |
                                       kMWMFuncMinimize,
                        kMWMInputModeless);

   // popup dialog and wait till user replies
   fDialog->MapWindow();

   gClient->WaitFor(fDialog);
}

const char *BAMainFrame::LoadFileDialog()
{

   const char *gSaveAsTypes[] = { "YAML files","*.yaml", 0,0 };

   static TGFileInfo fi;
   fi.fFileTypes = gSaveAsTypes;
   new TGFileDialog(gClient->GetRoot(), gClient->GetRoot(), kFDOpen, &fi);

   return fi.fFilename;
}





void BAMainFrame::SetupHistogramVector()
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

void BAMainFrame::DrawAll()
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


void BAMainFrame::SetButtonStatus()
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


void BAMainFrame::CloseWindow()
{
   // Called when window is closed via the window manager.
   delete this; 
}

#endif /* BAMAINFRAME_HH_ */
