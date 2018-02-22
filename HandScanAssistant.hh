class MyMainFrame : public TGMainFrame 
{
    public:
        MyMainFrame(const TGWindow *p, std::string spectrogramFilename, UInt_t windowWidth, UInt_t windowHeight);
        virtual ~MyMainFrame();

        void WriteToYAML();
        void CloseWindow();

        void CreateLine();
        void CreateCurve();
        void CreateOther();

        void DrawAll();
        void PrintAllTracks();
        void DrawNextSpectrogram();

        void DrawCurrentSpectrogram();
        void DrawPreviousSpectrogram();
        void DoSlider();
        void SetupHistogramVector();

        void CurvedBoolButton();
        void SidebandBoolButton();

        void SetButtonStatus();

        void CreateDialog(const std::string &userPrompt);
        void OkayButton();
        void CancelButton();
        std::string GetUserInput();
    private:
        TRootEmbeddedCanvas *fEmbeddedCanvas;

        std::vector<BATrack> allTracks;
        std::vector<BACurve> allCurves;
        std::vector<BAOther> allOthers;

        TFile *spectrogramFile;
        TH2D *currentHistogram;

        TGDoubleHSlider *horizontalXSlider;
        TGDoubleHSlider *horizontalYSlider;

        TGHProgressBar *progressBar;

        TGHorizontalFrame *fHorizontalFrame;

        TGCheckButton *sidebandButton;
        TGCheckButton *curvedButton;

        int acquisitionIndex;

        std::vector<std::string> histogramNames;

        std::string userInput;
        TGTextEntry *fTE;
        TGTransientFrame *fDialog;


    ClassDef(MyMainFrame, 0)
};

