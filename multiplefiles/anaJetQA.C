#include "TBranch.h"
#include "TFile.h"
#include "TList.h"
#include "TString.h"
#include "TChain.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TProfile.h"
#include "THnSparse.h"
#include "Math/QuantFuncMathCore.h"
#include "Math/SpecFuncMathCore.h"
#include "Math/ProbFunc.h"
#include "TMath.h"
#include "TLorentzVector.h"
#include <vector>

using namespace std;

// Declaration of leaf types

//from HiEvtAnalyzer
Int_t           hiBin;
Float_t         phi0;
Float_t         vx;
Float_t         vy;
Float_t         vz;

//from skimanalysis
Int_t           pcollisionEventSelection;
Int_t           pHBHENoiseFilter;

//from jet analyzer
Int_t           nref;
Float_t         rawpt[500];  //[nref]
Float_t         jtpt[500];   //[nref]
Float_t         jteta[500];  //[nref]
Float_t         jty[500];    //[nref]
Float_t         jtphi[500];  //[nref]
Float_t         jtpu[500];   //[nref]
Float_t         jtm[500] ;   //[nref]
Float_t         refpt[500];  //[nref]
Float_t         refeta[500]; //[nref]
Float_t         refy[500];   //[nref]
Float_t         refphi[500]; //[nref]
Float_t         refm[500] ;  //[nref]
Float_t         refdrjt[500];//[nref]
Int_t           subid[500];  //[nref]
Float_t         pthat;
Int_t           ngen;
Float_t         genpt[500];   //[ngen]
Float_t         geneta[500];   //[ngen]
Float_t         geny[500];   //[ngen]
Float_t         genphi[500];   //[ngen]

// List of branches
TBranch        *b_hiBin;   //!
TBranch        *b_phi0;    //!
TBranch        *b_vx;   //!
TBranch        *b_vy;   //!
TBranch        *b_vz;   //!

TBranch        *b_pcollisionEventSelection;   //!
TBranch        *b_pHBHENoiseFilter;   //!

TBranch        *b_nref;    //!
TBranch        *b_rawpt;   //!
TBranch        *b_jtpt;    //!
TBranch        *b_jteta;   //!
TBranch        *b_jty;     //!
TBranch        *b_jtphi;   //!
TBranch        *b_jtpu;    //!
TBranch        *b_jtm;     //!
TBranch        *b_refpt;   //!
TBranch        *b_refeta;  //!
TBranch        *b_refy;    //!
TBranch        *b_refphi;  //!
TBranch        *b_refm;    //!
TBranch        *b_refdrjt; //!
TBranch        *b_subid;   //!
TBranch        *b_pthat;   //!
TBranch        *b_ngen;    //!
TBranch        *b_genpt;   //!
TBranch        *b_geneta;  //!
TBranch        *b_geny;    //!
TBranch        *b_genphi;  //!

Double_t deltaR(const Double_t phi1, const Double_t phi2, const Double_t eta1, const Double_t eta2);

void anaJetQA(std::vector<std::string> &infnames, TString jetTreeName = "akPu4PFJetAnalyzer", Int_t startEntry = 0, Int_t maxEvents = -1,  size_t startFile = 0, size_t nfiles = 0, Int_t jobNumber = 0, Int_t ieta = 0) {

  //const Int_t startEntry   = 0;

  const Int_t nCent = 3;
  Double_t centMin[nCent] = {0.,10.,40.};
  Double_t centMax[nCent] = {10.,40.,80.};

  const Int_t neta = 5;
  Double_t etaMin[neta] = {-1.5,-1.3,1.3,1.5,-2.};
  Double_t etaMax[neta] = {1.5,  1.3,1.5,2. , 2.};

  // Double_t centMin[nCent] = {0.,5.,10.};
  // Double_t centMax[nCent] = {5.,10.,20.};

  //Double_t centMin[nCent] = {0.,1.,3.};
  //Double_t centMax[nCent] = {1.,2.,5.};
  
  TChain *hiEvt = new TChain("hiEvtAnalyzer/HiTree");
  TString strjets = Form("%s/t",jetTreeName.Data());
  TChain *jetTree = new TChain(strjets.Data());
  TChain *skimTree = new TChain("skimanalysis/HltTree");
  
  if(nfiles>0) {
    nfiles = startFile + nfiles;
    if(nfiles>infnames.size()) nfiles = infnames.size();
  } else {
    startFile = 0;
    nfiles = infnames.size();
  }
  Printf("start file: %zu  nfiles: %zu",startFile,nfiles);
  for(size_t i=startFile; i<nfiles; i++) {
    TString tmp(infnames[i].c_str());
    if(tmp.IsNull()) continue;
    hiEvt->Add(infnames[i].c_str());
    jetTree->Add(infnames[i].c_str());
    skimTree->Add(infnames[i].c_str());
    Printf("i: %zu  %s",i,infnames[i].c_str());
  }
  
  hiEvt->AddFriend(jetTree);
  hiEvt->AddFriend(skimTree);

  //set branches
  hiEvt->SetBranchAddress("hiBin", &hiBin, &b_hiBin);
  //hiEvt->SetBranchAddress("phi0", &phi0, &b_phi0);
  hiEvt->SetBranchAddress("vx", &vx, &b_vx);
  hiEvt->SetBranchAddress("vy", &vy, &b_vy);
  hiEvt->SetBranchAddress("vz", &vz, &b_vz);

  hiEvt->SetBranchAddress("pcollisionEventSelection", &pcollisionEventSelection, &b_pcollisionEventSelection);
  hiEvt->SetBranchAddress("pHBHENoiseFilterResultProducer", &pHBHENoiseFilter, &b_pHBHENoiseFilter);

  hiEvt->SetBranchAddress("nref", &nref, &b_nref);
  hiEvt->SetBranchAddress("rawpt", rawpt, &b_rawpt);
  hiEvt->SetBranchAddress("jtpt", jtpt, &b_jtpt);
  hiEvt->SetBranchAddress("jteta", jteta, &b_jteta);
  hiEvt->SetBranchAddress("jty", jty, &b_jty);
  hiEvt->SetBranchAddress("jtphi", jtphi, &b_jtphi);
  hiEvt->SetBranchAddress("jtpu", jtpu, &b_jtpu);
  if(hiEvt->GetBranch("jtm"))
    hiEvt->SetBranchAddress("jtm", jtm, &b_jtm);
  if(hiEvt->GetBranch("refpt"))
    hiEvt->SetBranchAddress("refpt", refpt, &b_refpt);
  if(hiEvt->GetBranch("refeta"))
    hiEvt->SetBranchAddress("refeta", refeta, &b_refeta);
  if(hiEvt->GetBranch("refy"))
    hiEvt->SetBranchAddress("refy", refy, &b_refy);
  if(hiEvt->GetBranch("refphi"))
    hiEvt->SetBranchAddress("refphi", refphi, &b_refphi);
  if(hiEvt->GetBranch("refm"))
    hiEvt->SetBranchAddress("refm", refm, &b_refm);
  if(hiEvt->GetBranch("refdrjt"))
    hiEvt->SetBranchAddress("refdrjt", refdrjt, &b_refdrjt);
  if(hiEvt->GetBranch("subid"))
    hiEvt->SetBranchAddress("subid", subid, &b_subid);
  if(hiEvt->GetBranch("pthat"))
    hiEvt->SetBranchAddress("pthat", &pthat, &b_pthat);
  if(hiEvt->GetBranch("ngen"))
    hiEvt->SetBranchAddress("ngen", &ngen, &b_ngen);
  if(hiEvt->GetBranch("genpt"))
    hiEvt->SetBranchAddress("genpt", genpt, &b_genpt);
  if(hiEvt->GetBranch("geneta"))
    hiEvt->SetBranchAddress("geneta", geneta, &b_geneta);
  if(hiEvt->GetBranch("geny"))
    hiEvt->SetBranchAddress("geny", geny, &b_geny);
  if(hiEvt->GetBranch("genphi"))
    hiEvt->SetBranchAddress("genphi", genphi, &b_genphi);

  //////////////////////////////////////////////////////////////////////////////
  //Define output objects
  //////////////////////////////////////////////////////////////////////////////
  TList *fOutput =  new TList();
  TH1::SetDefaultSumw2();

  TString histName = "";
  TString histTitle = "";

  const Int_t nBinsPtDet  = 200;
  const Double_t minPtDet = -40.;
  const Double_t maxPtDet = 360.;

  const Int_t nBinsPtPart  = 200;
  const Double_t minPtPart = 0.;
  const Double_t maxPtPart = 400.;

  const Int_t nBinsMDet  = 100;
  const Double_t minMDet = -20.;
  const Double_t maxMDet =  80.;

  const Int_t nBinsMPart  = 100;
  const Double_t minMPart =   0.;
  const Double_t maxMPart = 100.;

  //Binning for THnSparse
  const Int_t nBinsSparse0 = 4;
  const Int_t nBins0[nBinsSparse0] = {nBinsMDet,nBinsMPart,nBinsPtDet,nBinsPtPart};
  const Double_t xmin0[nBinsSparse0]  = { minMDet, minMPart, minPtDet, minPtPart};
  const Double_t xmax0[nBinsSparse0]  = { maxMDet, maxMPart, maxPtDet, maxPtPart};

  const Int_t nBinsDM  = 100;
  const Double_t minDM = -25.;
  const Double_t maxDM = 25.;

  const Int_t nBinsDMRel  = 600;
  const Double_t minDMRel = -3.;
  const Double_t maxDMRel = 3.;

  const Int_t nBinsDPt  = 200;
  const Double_t minDPt = -100.;
  const Double_t maxDPt = 100.;

  const Int_t nBinsDPtRel  = 600;
  const Double_t minDPtRel = -3.;
  const Double_t maxDPtRel = 3.;

  const Int_t nBinsScalePt  = 300;
  const Double_t minScalePt = 0.;
  const Double_t maxScalePt = 3.;

  const Int_t nBinsScaleM  = 300;
  const Double_t minScaleM = 0.;
  const Double_t maxScaleM = 6.;


  THnSparse *hnResponse[nCent];
  THnSparse *hnResponseRaw[nCent];
  TH3F      *h3PtTrueMTrueDeltaM[nCent];
  TH3F      *h3PtTrueMTrueDeltaPt[nCent];
  TH3F      *h3PtTrueMTrueDeltaMRel[nCent];
  TH3F      *h3PtTrueMTrueDeltaPtRel[nCent];
  TH3F      *h3PtTrueMTrueScaleM[nCent];
  TH3F      *h3PtTrueMTrueScalePt[nCent];
  TH3F      *h3PtTrueDeltaRScalePt[nCent];
  TH3F      *h3PtTruePtSubScalePt[nCent];
  TH3F      *h3PtTrueMTrueScalePtRaw[nCent];

  //For efficiency calculation
  TH3F      *fh3PtEtaPhiGen[nCent];
  TH3F      *fh3PtEtaPhiGenRec[nCent];
  TH3F      *fh3PtEtaGenRecPtRec[nCent];

  //For fake rate calculation
  TH3F      *fh3PtRecEtaPhiTrue[nCent];
  TH3F      *fh3PtRecEtaPhiFake[nCent];
  TH3F      *fh3PtEtaPhiRecAll[nCent]; 
 
  //vs event plane
  TH3F      *h3PtTrueEPRelScalePt[nCent];
  TH3F      *h3PtTrueEPRelScaleM[nCent];

  TH1F *hEventSel = new TH1F("hEventSel","hEventSel",10,0,10);
  fOutput->Add(hEventSel);

  TH1F *hCentrality = new TH1F("hCentrality","hCentrality",100,0,100);
  fOutput->Add(hCentrality);

  //pt vs eta vs cent
  TH3F      *fh3PtEtaCent;
  histName = "fh3PtEtaCent";
  histTitle = Form("%s;#it{p}_{T};#eta;centrality",histName.Data());
  fh3PtEtaCent = new TH3F(histName.Data(),histTitle.Data(),nBinsPtPart,minPtPart,maxPtPart,100,-5.,5.,100,0,100.);
  fOutput->Add(fh3PtEtaCent);
  
  TH3F      *fh3PtRawEtaCent;
  histName = "fh3PtRawEtaCent";
  histTitle = Form("%s;#it{p}_{T};#eta;centrality",histName.Data());
  fh3PtRawEtaCent = new TH3F(histName.Data(),histTitle.Data(),nBinsPtPart,minPtPart,maxPtPart,100,-5.,5.,100,0,100.);
  fOutput->Add(fh3PtRawEtaCent);

  TH3F      *fh3PtPUEtaCent;
  histName = "fh3PtPUEtaCent";
  histTitle = Form("%s;#it{p}_{T};#eta;centrality",histName.Data());
  fh3PtPUEtaCent = new TH3F(histName.Data(),histTitle.Data(),nBinsPtPart,minPtPart,maxPtPart,100,-5.,5.,100,0,100.);
  fOutput->Add(fh3PtPUEtaCent);
  
  for(Int_t ic = 0; ic<nCent; ic++) {

    histName = Form("fh3PtEtaPhiGen_Cent%d",ic);
    histTitle = Form("%sp;#it{p}_{T,true}",histName.Data());
    fh3PtEtaPhiGen[ic] = new TH3F(histName.Data(),histTitle.Data(),nBinsPtPart,minPtPart,maxPtPart,100,-5.,5.,72,-TMath::Pi(),TMath::Pi());
    fOutput->Add(fh3PtEtaPhiGen[ic]);

    histName = Form("fh3PtEtaPhiGenRec_Cent%d",ic);
    histTitle = Form("%sp;#it{p}_{T,true}",histName.Data());
    fh3PtEtaPhiGenRec[ic] = new TH3F(histName.Data(),histTitle.Data(),nBinsPtPart,minPtPart,maxPtPart,100,-5.,5.,72,-TMath::Pi(),TMath::Pi());
    fOutput->Add(fh3PtEtaPhiGenRec[ic]);

    histName = Form("fh3PtEtaGenRecPtRec_Cent%d",ic);
    histTitle = Form("%sp;#it{p}_{T,true}",histName.Data());
    fh3PtEtaGenRecPtRec[ic] = new TH3F(histName.Data(),histTitle.Data(),nBinsPtPart,minPtPart,maxPtPart,100,-5.,5.,nBinsPtPart,minPtPart,maxPtPart);
    fOutput->Add(fh3PtEtaGenRecPtRec[ic]);
    
    histName = Form("fh3PtRecEtaPhiTrue_Cent%d",ic);
    histTitle = Form("%sp;#it{p}_{T,rec};#eta_{rec};#phi_{rec}",histName.Data());
    fh3PtRecEtaPhiTrue[ic] = new TH3F(histName.Data(),histTitle.Data(),nBinsPtPart,minPtPart,maxPtPart,100,-5.,5.,72,-TMath::Pi(),TMath::Pi());
    fOutput->Add(fh3PtRecEtaPhiTrue[ic]);

    histName = Form("fh3PtRecEtaPhiFake_Cent%d",ic);
    histTitle = Form("%sp;#it{p}_{T,rec};#eta_{rec};#phi_{rec}",histName.Data());
    fh3PtRecEtaPhiFake[ic] = new TH3F(histName.Data(),histTitle.Data(),nBinsPtPart,minPtPart,maxPtPart,100,-5.,5.,72,-TMath::Pi(),TMath::Pi());
    fOutput->Add(fh3PtRecEtaPhiFake[ic]);

    histName = Form("fh3PtEtaPhiRecAll_Cent%d",ic);
    histTitle = Form("%sp;#it{p}_{T,rec};#eta_{rec};#phi_{rec}",histName.Data());
    fh3PtEtaPhiRecAll[ic] = new TH3F(histName.Data(),histTitle.Data(),nBinsPtPart,minPtPart,maxPtPart,100,-5.,5.,72,-TMath::Pi(),TMath::Pi());
    fOutput->Add(fh3PtEtaPhiRecAll[ic]);   
 
    histName = Form("hnResponse_Cent%d",ic);
    histTitle = Form("%s;#it{M}_{sub};#it{M}_{true};#it{p}_{T,sub};#it{p}_{T,true}",histName.Data());
    hnResponse[ic] = new THnSparseF(histName.Data(),histTitle.Data(),nBinsSparse0,nBins0,xmin0,xmax0);
    fOutput->Add(hnResponse[ic]);

    histName = Form("hnResponseRaw_Cent%d",ic);
    histTitle = Form("%s;#it{M}_{sub};#it{M}_{true};#it{p}_{T,sub};#it{p}_{T,true}",histName.Data());
    hnResponseRaw[ic] = new THnSparseF(histName.Data(),histTitle.Data(),nBinsSparse0,nBins0,xmin0,xmax0);
    fOutput->Add(hnResponseRaw[ic]);

    histName = Form("h3PtTrueMTrueDeltaM_Cent%d",ic);
    histTitle = Form("%s;#it{p}_{T,true};#it{M}_{true};#it{M}_{sub}-#it{M}_{true}",histName.Data()); 
    h3PtTrueMTrueDeltaM[ic] = new TH3F(histName.Data(),histTitle.Data(),nBinsPtPart,minPtPart,maxPtPart,nBinsMPart,minMPart,maxMPart,nBinsDM,minDM,maxDM);
    fOutput->Add(h3PtTrueMTrueDeltaM[ic]);

    histName = Form("h3PtTrueMTrueDeltaPt_Cent%d",ic);
    histTitle = Form("%s;#it{p}_{T,true};#it{M}_{true};#it{p}_{T,sub}-#it{p}_{T,true}",histName.Data()); 
    h3PtTrueMTrueDeltaPt[ic] = new TH3F(histName.Data(),histTitle.Data(),nBinsPtPart,minPtPart,maxPtPart,nBinsMPart,minMPart,maxMPart,nBinsDPt,minDPt,maxDPt);
    fOutput->Add(h3PtTrueMTrueDeltaPt[ic]);

    histName = Form("h3PtTrueMTrueDeltaMRel_Cent%d",ic);
    histTitle = Form("%s;#it{p}_{T,true};#it{M}_{true};(#it{M}_{sub}-#it{M}_{true})/#it{M}_{true}",histName.Data()); 
    h3PtTrueMTrueDeltaMRel[ic] = new TH3F(histName.Data(),histTitle.Data(),nBinsPtPart,minPtPart,maxPtPart,nBinsMPart,minMPart,maxMPart,nBinsDMRel,minDMRel,maxDMRel);
    fOutput->Add(h3PtTrueMTrueDeltaMRel[ic]);

    histName = Form("h3PtTrueMTrueDeltaPtRel_Cent%d",ic);
    histTitle = Form("%s;#it{p}_{T,true};#it{M}_{true};(#it{p}_{T,sub}-#it{p}_{T,true})/#it{p}_{T,true}",histName.Data()); 
    h3PtTrueMTrueDeltaPtRel[ic] = new TH3F(histName.Data(),histTitle.Data(),nBinsPtPart,minPtPart,maxPtPart,nBinsMPart,minMPart,maxMPart,nBinsDPtRel,minDPtRel,maxDPtRel);
    fOutput->Add(h3PtTrueMTrueDeltaPtRel[ic]);

    histName = Form("h3PtTrueMTrueScaleM_Cent%d",ic);
    histTitle = Form("%s;#it{p}_{T,true};#it{M}_{true};#it{M}_{true}/#it{M}_{true}",histName.Data()); 
    h3PtTrueMTrueScaleM[ic] = new TH3F(histName.Data(),histTitle.Data(),nBinsPtPart,minPtPart,maxPtPart,nBinsMPart,minMPart,maxMPart,nBinsScaleM,minScaleM,maxScaleM);
    fOutput->Add(h3PtTrueMTrueScaleM[ic]);

    histName = Form("h3PtTrueMTrueScalePt_Cent%d",ic);
    histTitle = Form("%s;#it{p}_{T,true};#it{M}_{true};#it{p}_{T,sub}/#it{p}_{T,true}",histName.Data()); 
    h3PtTrueMTrueScalePt[ic] = new TH3F(histName.Data(),histTitle.Data(),nBinsPtPart,minPtPart,maxPtPart,nBinsMPart,minMPart,maxMPart,nBinsScalePt,minScalePt,maxScalePt);
    fOutput->Add(h3PtTrueMTrueScalePt[ic]);

    histName = Form("h3PtTrueDeltaRScalePt_Cent%d",ic);
    histTitle = Form("%s;#it{p}_{T,true};#Delta#it{r};#it{p}_{T,sub}/#it{p}_{T,true}",histName.Data()); 
    h3PtTrueDeltaRScalePt[ic] = new TH3F(histName.Data(),histTitle.Data(),nBinsPtPart,minPtPart,maxPtPart,100,0.,1.,nBinsScalePt,minScalePt,maxScalePt);
    fOutput->Add(h3PtTrueDeltaRScalePt[ic]);

    histName = Form("h3PtTruePtSubScalePt_Cent%d",ic);
    histTitle = Form("%s;#it{p}_{T,true};#it{p}_{T,sub};#it{p}_{T,sub}/#it{p}_{T,true}",histName.Data()); 
    h3PtTruePtSubScalePt[ic] = new TH3F(histName.Data(),histTitle.Data(),nBinsPtPart,minPtPart,maxPtPart,nBinsPtDet,minPtDet,maxPtDet,nBinsScalePt,minScalePt,maxScalePt);
    fOutput->Add(h3PtTruePtSubScalePt[ic]);

    histName = Form("h3PtTrueMTrueScalePtRaw_Cent%d",ic);
    histTitle = Form("%s;#it{p}_{T,true};#it{M}_{true};#it{p}_{T,sub}/#it{p}_{T,true}",histName.Data()); 
    h3PtTrueMTrueScalePtRaw[ic] = new TH3F(histName.Data(),histTitle.Data(),nBinsPtPart,minPtPart,maxPtPart,nBinsMPart,minMPart,maxMPart,nBinsScalePt,minScalePt,maxScalePt);
    fOutput->Add(h3PtTrueMTrueScalePtRaw[ic]);

    histName = Form("h3PtTrueEPRelScaleM_Cent%d",ic);
    histTitle = Form("%s;#it{p}_{T,true};#phi - #Psi;#it{M}_{sub}/#it{M}_{true}",histName.Data()); 
    h3PtTrueEPRelScaleM[ic] = new TH3F(histName.Data(),histTitle.Data(),nBinsPtPart,minPtPart,maxPtPart,100,0.,TMath::Pi(),nBinsScaleM,minScaleM,maxScaleM);
    fOutput->Add(h3PtTrueEPRelScaleM[ic]);

    histName = Form("h3PtTrueEPRelScalePt_Cent%d",ic);
    histTitle = Form("%s;#it{p}_{T,true};#phi - #Psi;#it{p}_{T,sub}/#it{p}_{T,true}",histName.Data()); 
    h3PtTrueEPRelScalePt[ic] = new TH3F(histName.Data(),histTitle.Data(),nBinsPtPart,minPtPart,maxPtPart,100,0.,TMath::Pi(),nBinsScalePt,minScalePt,maxScalePt);
    fOutput->Add(h3PtTrueEPRelScalePt[ic]);

  }

 
  //event loop
  Int_t nentries = hiEvt->GetEntries();
  Printf("nentries in chain %d maxEvents: %d",nentries,maxEvents);
  Int_t lastEntry = -1;
  if(maxEvents>0) lastEntry = startEntry + maxEvents;
  else            lastEntry = nentries;
  if(lastEntry>nentries) lastEntry = nentries;
  Printf("startEntry: %d  lastEntry: %d  nentries: %d",startEntry,lastEntry,nentries);
  //Int_t nEvt = 1000;
  for (int i=startEntry; i<lastEntry; i++) {
    hEventSel->Fill(0.5);
    if (i%1000==0) Printf("Processing event %d  %d",i, nentries);
    hiEvt->GetEntry(i);
    if(TMath::Abs(vz)>15.) continue;
    hEventSel->Fill(1.5);

    Double_t cent = (Double_t)hiBin/2.;
    hCentrality->Fill(cent);
    //    Printf("cent: %.1f  pthat: %.1f",cent,pthat);
    Int_t centBin = -1;
    for(Int_t ic = 0; ic<nCent; ic++)
      if(cent>=centMin[ic] && cent<centMax[ic]) centBin = ic;

    hEventSel->Fill(2.5);

    if(pcollisionEventSelection==0) continue;
    hEventSel->Fill(3.5);

    //Printf("pHBHENoiseFilter: %d",pHBHENoiseFilter);
    if(pHBHENoiseFilter==0) continue;
    hEventSel->Fill(4.5);
    
    //Loop over jets
    //Printf("njets reco: %d",nref);
    for(Int_t ij = 0; ij<nref; ij++) {

      // Double_t pt = jtpt[ij];
      // Double_t rawpt = rawpt[ij];
      // Double_t ptpu = jtpu[ij];
      // Double_t eta = jteta[ij];
      
      fh3PtEtaCent->Fill(jtpt[ij],jteta[ij],cent);
      fh3PtRawEtaCent->Fill(rawpt[ij],jteta[ij],cent);
      fh3PtPUEtaCent->Fill(jtpu[ij],jteta[ij],cent);
   
    }//jet loop

    
  }//event loop

  TFile *fout = new TFile(Form("jetPerformanceHistos%sEtaBin%d_%d.root",jetTreeName.Data(),ieta,jobNumber),"RECREATE");
  fOutput->Write();
  fout->Write();
  fout->Close();
}

Double_t deltaR(const Double_t phi1, const Double_t phi2, const Double_t eta1, const Double_t eta2) {
  //calculate distance  
  Double_t dPhi = phi1 - phi2;
  Double_t dEta = eta1 - eta2;
  dPhi = TVector2::Phi_mpi_pi(dPhi);
  return TMath::Sqrt(dPhi * dPhi + dEta * dEta);
}
