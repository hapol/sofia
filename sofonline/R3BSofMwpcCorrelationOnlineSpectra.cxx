// ------------------------------------------------------------
// -----        R3BSofMwpcCorrelationOnlineSpectra        -----
// -----    Created 02/12/19  by J.L. Rodriguez-Sanchez   -----
// -----           Fill SOFIA online histograms           -----
// ------------------------------------------------------------

/*
 * This task should fill histograms with SOFIA online data
 */

#include "R3BSofMwpcCorrelationOnlineSpectra.h"
#include "R3BEventHeader.h"
#include "R3BSofMwpcCalData.h"
#include "R3BSofMwpcHitData.h"
#include "THttpServer.h"

#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRunOnline.h"
#include "FairRuntimeDb.h"
#include "TCanvas.h"
#include "TFolder.h"
#include "TH1F.h"
#include "TH2F.h"

#include "TClonesArray.h"
#include "TMath.h"
#include "TRandom.h"
#include <array>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdlib.h>

using namespace std;

R3BSofMwpcCorrelationOnlineSpectra::R3BSofMwpcCorrelationOnlineSpectra()
    : FairTask("SofMwpcCorrelationOnlineSpectra", 1)
    , fCalItemsMwpc1(NULL)
    , fHitItemsMwpc1(NULL)
    , fCalItemsMwpc2(NULL)
    , fHitItemsMwpc2(NULL)
    , fNameDet1("Mwpc1")
    , fNameDet2("Mwpc2")
    , fNEvents(0)
{
}

R3BSofMwpcCorrelationOnlineSpectra::R3BSofMwpcCorrelationOnlineSpectra(const TString& name,
                                                                       Int_t iVerbose,
                                                                       const TString& namedet1,
                                                                       const TString& namedet2)
    : FairTask(name, iVerbose)
    , fCalItemsMwpc1(NULL)
    , fHitItemsMwpc1(NULL)
    , fCalItemsMwpc2(NULL)
    , fHitItemsMwpc2(NULL)
    , fNameDet1(namedet1)
    , fNameDet2(namedet2)
    , fNEvents(0)
{
}

R3BSofMwpcCorrelationOnlineSpectra::~R3BSofMwpcCorrelationOnlineSpectra()
{
    LOG(INFO) << "R3BSofMWPC" + fNameDet1 + "vs" + fNameDet2 + "CorrelationOnlineSpectra::Delete instance";
    if (fCalItemsMwpc1)
        delete fCalItemsMwpc1;
    if (fHitItemsMwpc1)
        delete fHitItemsMwpc1;
    if (fCalItemsMwpc2)
        delete fCalItemsMwpc2;
    if (fHitItemsMwpc2)
        delete fHitItemsMwpc2;
}

InitStatus R3BSofMwpcCorrelationOnlineSpectra::Init()
{

    LOG(INFO) << "R3BSof" + fNameDet1 + "vs" + fNameDet2 + "CorrelationOnlineSpectra::Init ";

    // try to get a handle on the EventHeader. EventHeader may not be
    // present though and hence may be null. Take care when using.

    FairRootManager* mgr = FairRootManager::Instance();
    if (NULL == mgr)
        LOG(FATAL) << "R3BSof" + fNameDet1 + "vs" + fNameDet2 +
                          "CorrelationOnlineSpectra::Init FairRootManager not found";
    // header = (R3BEventHeader*)mgr->GetObject("R3BEventHeader");

    FairRunOnline* run = FairRunOnline::Instance();
    run->GetHttpServer()->Register("", this);

    // get access to mapped data of the active target
    fCalItemsMwpc1 = (TClonesArray*)mgr->GetObject(fNameDet1 + "CalData");
    if (!fCalItemsMwpc1)
    {
        return kFATAL;
    }
    fCalItemsMwpc2 = (TClonesArray*)mgr->GetObject(fNameDet2 + "CalData");
    if (!fCalItemsMwpc2)
    {
        return kFATAL;
    }

    // Create histograms for detectors
    TString Name1;
    TString Name2;

    cMWPCCal2D =
        new TCanvas(fNameDet1 + "-" + fNameDet2 + "_cal", fNameDet1 + "-" + fNameDet2 + " cal info", 10, 10, 800, 700);
    cMWPCCal2D->Divide(2, 1);

    // MWPC: Cal data
    Name1 = "fh2_" + fNameDet1 + "-" + fNameDet2 + "_calx";
    Name2 = "Position-X: " + fNameDet1 + " vs " + fNameDet2;
    if (fNameDet2 != "Mwpc3")
        fh2_mwpc_calx = new TH2F(Name1, Name2, 240, 0.5, 64.5, 240, 0.5, 64.5);
    else
        fh2_mwpc_calx = new TH2F(Name1, Name2, 240, 0.5, 64.5, 1152, 0.5, 288.5);
    fh2_mwpc_calx->GetXaxis()->SetTitle(fNameDet1 + "-X [pads]");
    fh2_mwpc_calx->GetYaxis()->SetTitle(fNameDet2 + "-X [pads]");
    fh2_mwpc_calx->GetYaxis()->SetTitleOffset(1.1);
    fh2_mwpc_calx->GetXaxis()->CenterTitle(true);
    fh2_mwpc_calx->GetYaxis()->CenterTitle(true);
    fh2_mwpc_calx->GetXaxis()->SetLabelSize(0.045);
    fh2_mwpc_calx->GetXaxis()->SetTitleSize(0.045);
    fh2_mwpc_calx->GetYaxis()->SetLabelSize(0.045);
    fh2_mwpc_calx->GetYaxis()->SetTitleSize(0.045);
    cMWPCCal2D->cd(1);
    fh2_mwpc_calx->Draw("col");

    Name1 = "fh2_" + fNameDet1 + "-" + fNameDet2 + "_caly";
    Name2 = "Position-Y: " + fNameDet1 + " vs " + fNameDet2;
    if (fNameDet2 != "Mwpc3")
        fh2_mwpc_caly = new TH2F(Name1, Name2, 240, 0.5, 64.5, 240, 0.5, 64.5);
    else
        fh2_mwpc_caly = new TH2F(Name1, Name2, 240, 0.5, 64.5, 480, 0.5, 120.5);
    fh2_mwpc_caly->GetXaxis()->SetTitle(fNameDet1 + "-Y [pads]");
    fh2_mwpc_caly->GetYaxis()->SetTitle(fNameDet2 + "-Y [pads]");
    fh2_mwpc_caly->GetYaxis()->SetTitleOffset(1.1);
    fh2_mwpc_caly->GetXaxis()->CenterTitle(true);
    fh2_mwpc_caly->GetYaxis()->CenterTitle(true);
    fh2_mwpc_caly->GetXaxis()->SetLabelSize(0.045);
    fh2_mwpc_caly->GetXaxis()->SetTitleSize(0.045);
    fh2_mwpc_caly->GetYaxis()->SetLabelSize(0.045);
    fh2_mwpc_caly->GetYaxis()->SetTitleSize(0.045);
    cMWPCCal2D->cd(2);
    fh2_mwpc_caly->Draw("col");

    // MAIN FOLDER
    TFolder* mainfolMW = new TFolder(fNameDet1 + "-" + fNameDet2, fNameDet1 + "-" + fNameDet2 + " info");
    mainfolMW->Add(cMWPCCal2D);
    // mainfolMW->Add(cMWPCHit2D);
    run->AddObject(mainfolMW);

    // Register command to reset histograms
    run->GetHttpServer()->RegisterCommand("Reset_" + fNameDet1 + "-" + fNameDet2 + "_HIST",
                                          Form("/Objects/%s/->Reset_Histo()", GetName()));

    return kSUCCESS;
}

void R3BSofMwpcCorrelationOnlineSpectra::Reset_Histo()
{
    LOG(INFO) << "R3BSof" + fNameDet1 + "vs" + fNameDet2 + "CorrelationOnlineSpectra::Reset_Histo";
    // Cal data
    fh2_mwpc_calx->Reset();
    fh2_mwpc_caly->Reset();
    // Hit data
    fh2_mwpc_hitx->Reset();
    fh2_mwpc_hity->Reset();
}

void R3BSofMwpcCorrelationOnlineSpectra::Exec(Option_t* option)
{
    FairRootManager* mgr = FairRootManager::Instance();
    if (NULL == mgr)
        LOG(FATAL) << "R3BSof" + fNameDet1 + "vs" + fNameDet2 +
                          "CorrelationOnlineSpectra::Exec FairRootManager not found";

    // Fill Cal data
    if (fCalItemsMwpc1 && fCalItemsMwpc1->GetEntriesFast() > 0 && fCalItemsMwpc2 &&
        fCalItemsMwpc2->GetEntriesFast() > 0)
    {
        // First MWPC
        Int_t nHits = fCalItemsMwpc1->GetEntriesFast();
        Int_t maxpadx1 = -1, maxpady1 = -1, maxqx1 = 0, maxqy1 = 0;
        for (Int_t ihit = 0; ihit < nHits; ihit++)
        {
            R3BSofMwpcCalData* hit = (R3BSofMwpcCalData*)fCalItemsMwpc1->At(ihit);
            if (!hit)
                continue;
            if (hit->GetPlane() == 1 || hit->GetPlane() == 2)
            {
                if (hit->GetQ() > maxqx1)
                {
                    maxpadx1 = hit->GetPad();
                    maxqx1 = hit->GetQ();
                }
            }
            if (hit->GetPlane() == 3)
            {
                if (hit->GetQ() > maxqy1)
                {
                    maxpady1 = hit->GetPad();
                    maxqy1 = hit->GetQ();
                }
            }
        }
        // Second MWPC
        nHits = fCalItemsMwpc2->GetEntriesFast();
        Int_t maxpadx2 = -1, maxpady2 = -1, maxqx2 = 0, maxqy2 = 0;
        for (Int_t ihit = 0; ihit < nHits; ihit++)
        {
            R3BSofMwpcCalData* hit = (R3BSofMwpcCalData*)fCalItemsMwpc2->At(ihit);
            if (!hit)
                continue;
            if (hit->GetPlane() == 1 || hit->GetPlane() == 2)
            {
                if (hit->GetQ() > maxqx2)
                {
                    maxpadx2 = hit->GetPad();
                    maxqx2 = hit->GetQ();
                }
            }
            if (hit->GetPlane() == 3)
            {
                if (hit->GetQ() > maxqy2)
                {
                    maxpady2 = hit->GetPad();
                    maxqy2 = hit->GetQ();
                }
            }
        }
        if (maxpadx1 > -1 && maxpadx2 > -1)
            fh2_mwpc_calx->Fill(maxpadx1 + gRandom->Uniform(-0.5, 0.5), maxpadx2 + gRandom->Uniform(-0.5, 0.5));
        if (maxpady1 > -1 && maxpady2 > -1)
            fh2_mwpc_caly->Fill(maxpady1 + gRandom->Uniform(-0.5, 0.5), maxpady2 + gRandom->Uniform(-0.5, 0.5));
    }

    fNEvents += 1;
}

void R3BSofMwpcCorrelationOnlineSpectra::FinishEvent()
{
    if (fCalItemsMwpc1)
    {
        fCalItemsMwpc1->Clear();
    }
    if (fHitItemsMwpc1)
    {
        fHitItemsMwpc1->Clear();
    }
    if (fCalItemsMwpc2)
    {
        fCalItemsMwpc2->Clear();
    }
    if (fHitItemsMwpc2)
    {
        fHitItemsMwpc2->Clear();
    }
}

void R3BSofMwpcCorrelationOnlineSpectra::FinishTask()
{

    if (fCalItemsMwpc1 && fCalItemsMwpc2)
    {
        cMWPCCal2D->Write();
    }
    if (fHitItemsMwpc1 && fHitItemsMwpc2)
    {
        cMWPCHit2D->Write();
    }
}

ClassImp(R3BSofMwpcCorrelationOnlineSpectra)
