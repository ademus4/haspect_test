
#ifndef THS2PI_h
#define THS2PI_h

#include "THSFinalState.h"
#include "THSParticle.h"
#include "THSCLAS12Trigger.h"
#include "THSCLAS12DeltaTime.h"
#include <vector>

class THS2pi : public THSFinalState{

 public :
  THS2pi();
  ~THS2pi()=default;


  //Init functions
  void Init_Iter0();
  void Init_Iter1();
  void Init_Iter2();
  void Init_Iter3();
   //Topology action functions
  void Topo_0();
  void Topo_1();
  void Topo_2();
  void Topo_3();
  void Init_Generated();
  //void Init_IterX();
  //void Topo_X();
  virtual void FileStart();
  virtual Bool_t  CheckParticle(THSParticle* part);

  void Kinematics();
  protected :

  THSCLAS12Trigger fTrigger;//For CLAS12 trigger info
  THSCLAS12DeltaTime fCuts; //For particle cuts
  
  //Initial state
  HSLorentzVector fBeam=HSLorentzVector(0,0,10.6,10.6);
  HSLorentzVector fTarget=HSLorentzVector(0,0,0,0.938272);

  //Final Particles Detected
  THSParticle fElectron=THSParticle("e-");
  THSParticle fProton=THSParticle("proton");
  THSParticle fPip=THSParticle("pi+");
  THSParticle fPim=THSParticle("pi-");
 
  //Final Parents
 
  //Observables

  Double_t f_t;
  Double_t f_Q2;
  Double_t f_W;
  Double_t f_EGamma;
  Double_t f_Pol;
  Double_t fCMCosTh;
  Double_t fCMPhi;
  Double_t fPipP;
  Double_t fPimP;
  Double_t fPipTh;
  Double_t fPimTh;
  Double_t f_GammaE;
  Double_t fHelCosTh;
  Double_t fHelPhi;
  Double_t f2PiMass=0;
  Double_t fDppMass=0;
  Double_t fD0Mass=0;

  //Discriminators

  Double_t fMissMass=0;
  Double_t fMissMass2=0;
  Double_t fMissMass2Pi=0;
  Int_t fDetector=-1;

 public :
  virtual void FinalStateOutTree(TTree* tree);

 

};

#endif //ifdef THS2pi
