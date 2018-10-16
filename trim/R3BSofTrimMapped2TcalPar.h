#ifndef __R3BSOFTRIMMAPPED2TCALPAR_H__
#define __R3BSOFTRIMMAPPED2TCALPAR_H__

#include "FairTask.h"
#include "TH1F.h"

class TClonesArray;
class R3BSofTcalPar;
class R3BEventHeader;

class R3BSofTrimMapped2TcalPar : public FairTask {
  
 public:
  
  /** Default constructor **/
  R3BSofTrimMapped2TcalPar();
  
  /** Standard constructor **/
  R3BSofTrimMapped2TcalPar(const char* name, Int_t iVerbose=1);
  
  /** Destructor **/
  virtual ~R3BSofTrimMapped2TcalPar();
  
  /** Virtual method Init **/
  virtual InitStatus Init();
  
  /** Virtual method Exec **/
  virtual void Exec(Option_t* opt);
  
  /** Virtual method FinishEvent **/
  virtual void FinishEvent();
  
  /** Virtual method FinishTask **/
  virtual void FinishTask(); 
  
  /** Virtual method Reset **/
  virtual void Reset();
  
  /** Virtual method ReInit **/
  virtual InitStatus ReInit();
  
  /** Virtual method calculate the Vftx Tcal Parameters **/
  virtual void CalculateVftxTcalParams();
  
  void SetOutputFile(const char *outFile);
  
  /** Accessor functions **/
  const Double_t GetNumDetectors() {return fNumDetectors;}
  const Double_t GetNumSections() {return fNumSections;}
  const Double_t GetNumChannels() {return fNumChannels;}
  const Double_t GetNumSignals() {return fNumSignals;}
  const Int_t GetMinStatistics(){return fMinStatistics;}

  void SetNumDetectors(Int_t NumberOfDetectors)   {fNumDetectors=NumberOfDetectors;}  
  void SetNumSections(Int_t NumberOfSections)     {fNumSections=NumberOfSections;}
  void SetNumChannels(Int_t NumberOfChannels)     {fNumChannels=NumberOfChannels;}
  void SetNumSignals(Int_t NumberOfDetectors, 
		     Int_t NumberOfSections, 
		     Int_t NumberOfChannels){

    fNumSignals=NumberOfDetectors*NumberOfSections*NumberOfChannels;
  }
  void SetNumTcalParsPerSignal(Int_t NumberOfTcalParsPerSignal) {
    fNumTcalParsPerSignal=NumberOfTcalParsPerSignal;
  }
  void SetMinStatistics(Int_t minstat){fMinStatistics=minstat;}



 protected:
  Int_t fNumDetectors;  // number of detectors (=2 for Sci, =28 for ToFW, =1 for Trim, =2 for Twim)
  Int_t fNumSections;	// number of sections  (=1 for Sci, =1 for ToFW, =3 for Trim, =2 for Twim)
  Int_t fNumChannels;   // number of channels  (=3 for Sci, =2 for ToFW, =6 for Twim, =16 for Twim)
  Int_t fNumSignals;
  Int_t fNumTcalParsPerSignal;
  Int_t fMinStatistics; // minimum statistics to proceed to the calibration

  // calibration parameters
  R3BSofTcalPar* fTcalPar;          // Tcal Parameters

  // input data
  TClonesArray* fMappedTrim;         // Array with mapped data from scintillator detectors - input data.
   
  // histograms
  TH1F** fh_TimeFineBin;
  TH1F** fh_TimeFineNs;
  char* fOutputFile;
  
 public:
  ClassDef(R3BSofTrimMapped2TcalPar,0);
};

#endif //__R3BSOFTRIMMAPPED2TCALPAR_H__
