/*
 * BASpectrogram.hh
 *
 *  Created on: Mar 6, 2018
 *      Author: nbuzinsky
 */

#ifndef BASPECTROGRAM_HH_
#define BASPECTROGRAM_HH_


class BASpectrogram
{
    public:
        BASpectrogram();
        ~BASpectrogram();
        void SetData(const TObject* inputData);
        void SetUserSparsity(const bool &status);
        void SetDataSparsity(const bool &status);

        bool GetSparsity();

        void SparsifyRawHistogram();
        void Draw();
        void SetTitle(const char *spectrogramTitle);

        TAxis* GetXaxis();
        TAxis* GetYaxis();
        TAxis* GetZaxis();

        double GetMinimum();
        double GetMaximum();

    private:
        bool userSparse; //true if user wants to show sparse data
        bool dataSparse; //true if data read in is already sparse
        bool drawSparse; //true if either of two is true

        void SetSparsity();

        TGraph *currentGraph;
        TH2D *currentHistogram;
};

BASpectrogram::BASpectrogram()
{
}

BASpectrogram::~BASpectrogram()
{
}

TAxis* BASpectrogram::GetXaxis()
{
    if(drawSparse)
        return currentGraph->GetXaxis();
    else
        return currentHistogram->GetXaxis();
}

TAxis* BASpectrogram::GetYaxis()
{
    if(drawSparse)
        return currentGraph->GetYaxis();
    else
        return currentHistogram->GetYaxis();
}

TAxis* BASpectrogram::GetZaxis()
{
    return currentHistogram->GetZaxis();
}


double BASpectrogram::GetMinimum()
{
    if(drawSparse)
        return 0;
    else
        return currentHistogram->GetMinimum();
}

double BASpectrogram::GetMaximum()
{
    if(drawSparse)
        return 1;
    else
        return currentHistogram->GetMaximum();
}

void BASpectrogram::SetTitle(const char *spectrogramTitle)
{
    if(drawSparse)
        currentGraph->SetTitle(spectrogramTitle);
    else
        currentHistogram->SetTitle(spectrogramTitle);
}

void BASpectrogram::Draw()
{
    if(drawSparse)
    {
        currentGraph->Draw("Ap");
        currentGraph->SetBit(kCannotPick);
    }
    else
    {
        currentHistogram->Draw("col");
        currentHistogram->SetBit(kCannotPick);
    }
}

void BASpectrogram::SetUserSparsity(const bool &status)
{
    userSparse = status;
    SetSparsity();
}

void BASpectrogram::SetDataSparsity(const bool &status)
{
    dataSparse = status;
    SetSparsity();
}

void BASpectrogram::SetSparsity()
{
    drawSparse = userSparse || dataSparse;
}

bool BASpectrogram::GetSparsity()
{
    return drawSparse;
}

void BASpectrogram::SetData(const TObject* inputData)
{
    if(dataSparse)
        currentGraph = (TGraph*) inputData;
    else
        currentHistogram = (TH2D*) inputData;
}

//Converts histogram ro one where only sufficiently high power bins are plotted as points on graph
void BASpectrogram::SparsifyRawHistogram()
{
    std::vector<double> xVector, yVector;
    const double cutFraction = 0.75; //If bin is below cutFraction * max of histogram, then it is cut from sparse plot

    for(int i=0; i < currentHistogram->GetXaxis()->GetNbins(); ++i)
    {
        for(int j=0; j < currentHistogram->GetYaxis()->GetNbins(); ++j)
        {
            if(currentHistogram->GetBinContent(i,j) > cutFraction * currentHistogram->GetMaximum() ) 
            {
                xVector.push_back(currentHistogram->GetXaxis()->GetBinCenter(i));
                yVector.push_back(currentHistogram->GetYaxis()->GetBinCenter(j));
            }
        }
    }

    currentGraph = new TGraph(xVector.size(), xVector.data(), yVector.data());
}


#endif /* BASPECTROGRAM_HH_ */
