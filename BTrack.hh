#include <TLine.h>
class BTrack
{
    public:
        BTrack(const double &x1, const double &y1, const double &x2, const double &y2, const double &acqNumber);
        void Draw();
    private:
        TLine trackRidge;
        bool isSideband;
        bool isCurved;
        int acquisitionNumber;
};

BTrack::BTrack(const double &x1, const double &y1, const double &x2, const double &y2, const double &acqNumber): 
    trackRidge(TLine(x1,y1,x2,y2)), 
    isSideband(false), 
    isCurved(false), 
    acquisitionNumber(acqNumber)
{
   trackRidge.SetLineWidth(1);
   trackRidge.SetLineColor(kRed);

   return;
}

void BTrack::Draw()
{
    trackRidge.Draw();

};
