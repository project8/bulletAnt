#include <TGClient.h>
#include <TGButton.h>
#include <TGLabel.h>
#include <TGFrame.h>
#include <TGLayout.h>
#include <TGSplitter.h>
#include <TRootEmbeddedCanvas.h>
#include <TH2D.h>


class MyMainFrame : public TGMainFrame 
{
    public:
        MyMainFrame(const TGWindow *p, std::string spectrogramFilename, UInt_t windowWidth, UInt_t windowHeight);
        virtual ~MyMainFrame();
        void WriteToYAML();
        void CloseWindow();
        void DrawLine();
        void PrintAllTracks();
        void DrawNextSpectrogram();
        void DrawCurrentSpectrogram();
        void DrawPreviousSpectrogram();
        void DoSlider();
    private:
        TRootEmbeddedCanvas *fEmbeddedCanvas;
        std::vector<TLine> allLines;
        TFile *spectrogramFile;
        TList * histogramList;
        TObject *fileObject; //generic TObject to retrieve the histograms from the root file
        TH2D *currentHistogram;
        int histogramListIndex;
        TGDoubleHSlider *horizontalXSlider;
        TGDoubleHSlider *horizontalYSlider;

    std::string currentHistogramName, previousHistogramName;

    ClassDef(MyMainFrame, 0)
};


void MyMainFrame::WriteToYAML()
{
    Printf("beep boop...");
}

MyMainFrame::MyMainFrame(const TGWindow *p, std::string spectrogramFilename, UInt_t windowWidth, UInt_t windowHeight) : TGMainFrame(p, windowWidth, windowHeight), histogramListIndex(0)
{

    spectrogramFile = new TFile(spectrogramFilename.c_str());
    histogramList = spectrogramFile->GetListOfKeys();

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
   TGTextButton *createTrackButton = new TGTextButton(leftButtonFrame,"&Create Track");
   createTrackButton->Connect("Clicked()","MyMainFrame",this,"DrawLine()");
   leftButtonFrame->AddFrame(createTrackButton, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 5,5,3,4));
   createTrackButton->SetToolTipText("Create a new Line to fit to a track");

   TGTextButton *printTracksButton = new TGTextButton(leftButtonFrame,"&Print All Tracks");
   printTracksButton->Connect("Clicked()","MyMainFrame",this,"PrintAllTracks()");
   leftButtonFrame->AddFrame(printTracksButton, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 5,5,3,4));
   printTracksButton->SetToolTipText("Write all found tracks to file");

   //Sliders
    TGLabel *xSliderLabel = new TGLabel(leftButtonFrame, "X Axis (Time)");
    leftButtonFrame->AddFrame(xSliderLabel, new TGLayoutHints(kLHintsExpandX, 3, 2, 2, 2));

    const int horizontalXSliderID = 45;
    horizontalXSlider = new TGDoubleHSlider(leftButtonFrame,150,kDoubleScaleBoth,horizontalXSliderID);
    horizontalXSlider->Connect("PositionChanged()", "MyMainFrame", this, "DoSlider()");
    horizontalXSlider->SetRange(0,0.01);
    horizontalXSlider->SetPosition(0,0.0025);
    leftButtonFrame->AddFrame(horizontalXSlider, new TGLayoutHints(kLHintsCenterY | kLHintsExpandX, 3, 2, 2, 2));

    TGLabel *ySliderLabel = new TGLabel(leftButtonFrame, "Y Axis (Frequency)");
    leftButtonFrame->AddFrame(ySliderLabel, new TGLayoutHints(kLHintsExpandX, 3, 2, 2, 2));

    const int horizontalYSliderID = 46;
    horizontalYSlider = new TGDoubleHSlider(leftButtonFrame,150,kDoubleScaleBoth,horizontalYSliderID);
    horizontalYSlider->Connect("PositionChanged()", "MyMainFrame", this, "DoSlider()");
    horizontalYSlider->SetRange(50e6,150e6);
    horizontalYSlider->SetPosition(50e6,65e6);
    leftButtonFrame->AddFrame(horizontalYSlider, new TGLayoutHints(kLHintsCenterY | kLHintsExpandX, 3, 2, 2, 2));

    //Finishing Touches
    // What to clean up in destructor
    SetCleanup(kDeepCleanup);

    // Set a name to the main frame
    SetWindowName("Project 8 Handscanning Interface");
    //SetWMSizeHints(350, 200, 600, 400, 0, 0);
    MapSubwindows();
    //Resize(GetDefaultSize());
    MapWindow();

    //Draw first spectrogram automatically
    DrawCurrentSpectrogram();
}

void MyMainFrame::DrawLine()
{
   // Draws function graphics in randomly chosen interval
   double xAxisInterval[2] = {horizontalXSlider->GetMinPosition(), horizontalXSlider->GetMaxPosition()};
   double yAxisInterval[2] = {horizontalYSlider->GetMinPosition(), horizontalYSlider->GetMaxPosition()};
   double xAxisLength = xAxisInterval[1] - xAxisInterval[0];
   double yAxisLength = yAxisInterval[1] - yAxisInterval[0];
   double xLinePosition[2] = { xAxisInterval[0] + 0.25 * xAxisLength, xAxisInterval[0] + 0.75 * xAxisLength};
   double yLinePosition[2] = { yAxisInterval[0] + 0.25 * yAxisLength, yAxisInterval[0] + 0.75 * yAxisLength};

   allLines.push_back(TLine(xLinePosition[0], yLinePosition[0], xLinePosition[1], yLinePosition[1] ));
   allLines.back().SetLineWidth(1);
   allLines.back().SetLineColor(kRed);
   allLines.back().Draw();

   //Gets current canvas and updates after button press
   TCanvas *fCanvas = fEmbeddedCanvas->GetCanvas();
   fCanvas->cd();
   fCanvas->Update();
}


void MyMainFrame::DrawNextSpectrogram()
{
    ++histogramListIndex;

    for(int i=0;i<3;++i)
    {
        if(!histogramList->At(histogramListIndex))
        {
            //Warn user they are out of index, can just write and exit if done
            histogramListIndex-=2; //Assumes that we always have this x2 repitition in the filenames
            break;
        }

        previousHistogramName = currentHistogramName;
        currentHistogramName = histogramList->At(histogramListIndex)->GetName();

        if(previousHistogramName != currentHistogramName) break;

        ++histogramListIndex;
    }

    DrawCurrentSpectrogram();
}

void MyMainFrame::DrawPreviousSpectrogram()
{
    --histogramListIndex;

    for(int i=0;i<3;++i)
    {
        if(histogramListIndex < 0)
        {
            //Warn user they are out of index, you cant get away that easy
            ++histogramListIndex;
            break;
        }

        previousHistogramName = currentHistogramName;
        currentHistogramName = histogramList->At(histogramListIndex)->GetName();

        if(previousHistogramName == currentHistogramName) break;

        --histogramListIndex;
    }

    DrawCurrentSpectrogram();
}

void MyMainFrame::DrawCurrentSpectrogram() //Draws spectrogram in righthand canvas depending on the histogramListIndex
{
    currentHistogramName = histogramList->At(histogramListIndex)->GetName();
    fileObject = spectrogramFile->Get(currentHistogramName.c_str());
    currentHistogram = (TH2D*) fileObject;
    currentHistogram->Draw("colz");

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
    for(int i=0;i<allLines.size();++i)
    {
    allLines[i].Draw();
    }
    //Gets current canvas and updates after button press
    TCanvas *fCanvas = fEmbeddedCanvas->GetCanvas();
    fCanvas->cd();
    fCanvas->Update();
}


void MyMainFrame::PrintAllTracks()
{
    for(int i=0;i<allLines.size();++i)
    {
        printf("Line %d starts at (%e, %e)\n",i, allLines[i].GetX1(),allLines[i].GetY1());
        printf("Line %d ends at (%e, %e)\n",i, allLines[i].GetX2(),allLines[i].GetY2());
        printf("---------------------------\n");
    }
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
