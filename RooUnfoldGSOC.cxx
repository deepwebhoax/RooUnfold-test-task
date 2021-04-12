#include <iostream>
using std::cout;
using std::endl;
#include "TRandom.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TCanvas.h"
#include <unistd.h>
// TASK 1: Make this code compile under CMake against some installation of ROOT

Double_t smear (Double_t xt){
    Double_t xsmear= gRandom->Gaus(-2.5,0.2); // bias and smear
    return xt+xsmear;
}

void RooUnfoldGSOC()
{
    TH2D response = TH2D("response","response",4, -10.0, 10.0,4, -10.0, 10.0);
    TH1D truth = TH1D("truth","truth",4,-10.,10.);
    TH1D reco = TH1D("reco","reco",4,-10.,10.);

// Train with a Breit-Wigner, mean 0.3 and width 2.5.
for (Int_t i= 0; i<10000000; i++) {
    Double_t xt= gRandom->BreitWigner (0.3, 2.5);
    truth.Fill(xt);
    Double_t x= smear (xt);
    response.Fill (x, xt);
    reco.Fill(x);
}

// We have three objects now. A distribution, an approximation of how that distribution
// response
// would look in our detector, and the response of how the detector affects it.
// now one would think that for given distribution we can use this response to
// predict how that distribution would look in our detector by multiplying through
TH1D prediction = TH1D("prediction","prediction",4,-10.,10.);
for (Int_t i= 0; i<reco.GetNbinsX(); i++){
    for (Int_t j= 0; j<truth.GetNbinsX(); j++){
        auto current = prediction.GetBinContent(i+1);
        auto contribution = truth.GetBinContent(j+1)*response.GetBinContent(i+1,j+1);
        prediction.SetBinContent(i+1,current+contribution);
    }
}
// in this case the prediction and reco should line up exactly.
cout<<"reco bins"<<endl;
for (Int_t i=0; i<reco.GetNbinsX(); i++){
    cout<<reco.GetBinContent(i+1)<<", ";
}
cout<<endl;

cout<<"prediciton"<<endl;
for (Int_t i=0; i<prediction.GetNbinsX(); i++){
    cout<<prediction.GetBinContent(i+1)<<", ";
}
// TASK 2 (Optional): the predictions don't line up. This is because the response needs to be normalised
// by the number of events measured in truth. Loop over the bins of the response to normalise
// each element to be the probability of reconstructing the event in each bin given the bin it
// truthfully came from. (divide each by truth). Ensure the prediction now lines up with reco.
// TCanvas *canv = new TCanvas("c1", "c1", 800, 800);
// c1->Divide(4,4); xd = x + gaus xdq = sum(xq*xdxq[i]) (t[i]*(x[j][i]))
// response.Draw("lego");
// canv->Print("picturecones.jpg");

cout<< endl<<"Task 2. Normalization"<<endl;
int n = 4;
TH1D predictionCorrected = TH1D("prediction_c","prediction_c",4,-10.,10.);
auto sum = truth.GetSumOfWeights();
for (Int_t i= 0; i<reco.GetNbinsX(); i++){
    for (Int_t j= 0; j<truth.GetNbinsX(); j++){
        auto current = predictionCorrected.GetBinContent(i+1);
        auto contribution = truth.GetBinContent(j+1)*response.GetBinContent(i+1,j+1)/sum;
        predictionCorrected.SetBinContent(i+1,current+contribution);
    }
}
cout<<"prediciton corrected"<<endl;
for (Int_t i=0; i<predictionCorrected.GetNbinsX(); i++){
    cout<<predictionCorrected.GetBinContent(i+1)<<", ";
}
// TASK 3 (Optional): here we perform the operation reco = Response.dot(truth). replace the loops with
// matrix operations using the built in ROOT TMatrix objects and operations. Test that it gives the same
// predictions as the loop method.


cout<< endl<<"Task 3. Matrix"<<endl;

TMatrix response_matrix(n,n);
TVector truth_vector(n);
for(int i=0; i<n; i++){
    for(int j=0; j<n; j++){
        response_matrix(i, j) = response.GetBinContent(i+1, j+1);
    }
    truth_vector[i] = truth.GetBinContent(i+1);
}

TVector prediction_vector(n);

truth_vector *= 1/sum;

prediction_vector = response_matrix * truth_vector;
prediction_vector.Print();

// TASK 4 (Optional): Setup a Series of CTests to verify that different operations give the same results.

// if (canv) { canv->Close(); gSystem->ProcessEvents(); delete canv; canv = 0; }
}

#ifndef __CINT__
int main () { RooUnfoldGSOC(); return 0; } // Main program when run stand-alone
#endif


