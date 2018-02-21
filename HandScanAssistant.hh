class MyMainFrame : public TGMainFrame 
{
    public:
        MyMainFrame(const TGWindow *p, std::string spectrogramFilename, UInt_t windowWidth, UInt_t windowHeight);
        virtual ~MyMainFrame();
        void WriteToYAML();
        void CloseWindow();
        void CreateLine();
        void DrawAllLines();
        void PrintAllTracks();
        void DrawNextSpectrogram();
        void DrawCurrentSpectrogram();
        void DrawPreviousSpectrogram();
        void DoSlider();
        void SetupHistogramVector();
        void CurvedBoolButton();
        void SidebandBoolButton();
        void SetButtonStatus();
    private:
        TRootEmbeddedCanvas *fEmbeddedCanvas;
        std::vector<BTrack> allTracks;
        TFile *spectrogramFile;
        TH2D *currentHistogram;
        TGDoubleHSlider *horizontalXSlider;
        TGDoubleHSlider *horizontalYSlider;
        TGHProgressBar *progressBar;
        TGCheckButton *sidebandButton;
        TGCheckButton *curvedButton;
        int acquisitionIndex;
        std::vector<std::string> histogramNames;


    ClassDef(MyMainFrame, 0)
};

