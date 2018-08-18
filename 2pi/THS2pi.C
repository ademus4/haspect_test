/**
 * \class THS2pi
 * 
 * Template final class .
 * 
 * Users should create their own analysis specific project classes.
 * 
 */



#include "TDatabasePDG.h"
#include "THS2pi.h"
#include <algorithm>


THS2pi::THS2pi(){
  SetVerbose(1);
  CheckCombi(); //comment out to remove messages
  
  //Set final state detected particles
  //AddParticle(particle,true/false you want to write in final vector, genID for linking to generated truth value)
  //Note if particle is added to final with a valid genID it will be used
  //to determine the correct permutation of the simulated event
  AddParticle(&fElectron,kTRUE,-1);
  AddParticle(&fProton,kTRUE,-1);
  AddParticle(&fPip,kTRUE,-1);
  AddParticle(&fPim,kTRUE,-1);

  //Set final state parents
  
  
  
  TString PID("NONE"); //set this to which particles you want to id via pdg code alone, or set it in individual AddTopology
  TString INCLUSIVE("");//set this to which particles you want reaction to be inclusive of, or set it in individual AddTopology "ALL"=> completely inclusive

  //include topology for analysis and get index
  AddTopology("e-:pi+:pi-:proton",
             bind(&THS2pi::Init_Iter0, this),
             bind(&THS2pi::Topo_0, this),
             PID,INCLUSIVE);

  AddTopology("e-:pi+:proton",
             bind(&THS2pi::Init_Iter1, this),
             bind(&THS2pi::Topo_1, this),
             PID,INCLUSIVE);

  AddTopology("e-:pi-:proton",
             bind(&THS2pi::Init_Iter2, this),
             bind(&THS2pi::Topo_2, this),
             PID,INCLUSIVE);

  AddTopology("e-:pi+:pi-",
             bind(&THS2pi::Init_Iter3, this),
             bind(&THS2pi::Topo_3, this),
             PID,INCLUSIVE);

  
  
  THSFinalState::InitFS();
}

void THS2pi::FileStart(){
  //May be called when a new file is opened
  fTrigger.SetParticles(frDetParts); //the detected particles
  fTrigger.SetEventInfo(fEventInfo);//once per event info
  fTrigger.SetRunInfo(fRunInfo);//once per run info

  //Configure the cuts (default are delta timing cuts in ns)
  fCuts.SetElCut(2,2); //Just FT,FD
  fCuts.SetPionPCut(0,2,0,2);//FT,FDTOF,CD,FDCAL
  fCuts.SetPionMCut(0,2,0,2);//FT,FDTOF,CD,FDCAL
  fCuts.SetProtCut(0,2,1,2);//FT,FDTOF,CD,FDCAL
  fCuts.SetGammaCut(2,2,0,2);//FT,FDTOF,CD,FDCAL

  if(THSFinalState::frGenParts) fTrigger.SetSim();//Should get this from RunInfo but not correct in EB at the moment
  
}

//Define topology Iterator functions
void THS2pi::Init_Iter0(){
  //THSParticle iterator initialisation
  //For topology e-:pi+:pi-:proton

   AutoIter(); //Let finalstate try and work out the iterattor for you, you can remove this if you want to do it yourself
}
void THS2pi::Init_Iter1(){
  //THSParticle iterator initialisation
  //For topology e-:pi+:proton

   AutoIter(); //Let finalstate try and work out the iterattor for you, you can remove this if you want to do it yourself
}
void THS2pi::Init_Iter2(){
  //THSParticle iterator initialisation
  //For topology e-:pi-:proton

   AutoIter(); //Let finalstate try and work out the iterattor for you, you can remove this if you want to do it yourself
}
void THS2pi::Init_Iter3(){
  //THSParticle iterator initialisation
  //For topology e-:pi+:pi-

   AutoIter(); //Let finalstate try and work out the iterattor for you, you can remove this if you want to do it yourself
}
// void THS2pi::Init_IterX(){
//  THSParticleIter* diter=CreateParticleIter(fTID_X,&fVecY,NPARTICLES);
//  ...
// }
//Define topology functions
void THS2pi::Topo_0(){
  //For topology e-:pi+:pi-:proton
  //Define starttime from electron candidate
  fTrigger.StartTime(&fElectron);
  //Subtract sarttime from all particles
  fTrigger.SubtractStartTime(&fElectron,&fProton,&fPip,&fPim);

  //Can apply some timing cuts now
  if(!fCuts.ElCut(&fElectron)){fGoodEvent=kFALSE;return;}
  if(!fCuts.ProtCut(&fProton)){fGoodEvent=kFALSE;return;}
  if(!fCuts.PionPCut(&fPip)){fGoodEvent=kFALSE;return;}
  if(!fCuts.PionMCut(&fPim)){fGoodEvent=kFALSE;return;}

  //Reconstruct missing or combined particles
  HSLorentzVector miss=fBeam+fTarget-fElectron.P4()-fProton.P4()-fPip.P4()-fPim.P4();
  fMissMass2=miss.M2();
  fMissMass=miss.M();
}
void THS2pi::Topo_1(){
  //For topology e-:pi+:proton
  //if(fElectron.Detector()>0) {fGoodEvent=kFALSE;return;} //Put some cuts on particle detectors
  //Define starttime from electron candidate
  fTrigger.StartTime(&fElectron);
  //Subtract sarttime from all particles
  fTrigger.SubtractStartTime(&fElectron,&fProton,&fPip);

  //Can apply some timing cuts now                                              
  if(!fCuts.ElCut(&fElectron)){fGoodEvent=kFALSE;return;}
  if(!fCuts.ProtCut(&fProton)){fGoodEvent=kFALSE;return;}
  if(!fCuts.PionPCut(&fPip)){fGoodEvent=kFALSE;return;}

  //Reconstruct missing or combined particles                                   
  HSLorentzVector miss=fBeam+fTarget-fElectron.P4()-fProton.P4()-fPip.P4();
  fMissMass2=miss.M2();
  fMissMass=miss.M();
  fPim.SetP4(miss);
  fPim.TakePDGMass();
}
void THS2pi::Topo_2(){
  //For topology e-:pi-:proton
  //if(fElectron.Detector()>0) {fGoodEvent=kFALSE;return;} //Put some cuts on particle detectors
  //Define starttime from electron candidate
  fTrigger.StartTime(&fElectron);
  //Subtract sarttime from all particles
  fTrigger.SubtractStartTime(&fElectron,&fProton,&fPim);

  //Can apply some timing cuts now                                                                                          
  if(!fCuts.ElCut(&fElectron)){fGoodEvent=kFALSE;return;}
  if(!fCuts.ProtCut(&fProton)){fGoodEvent=kFALSE;return;}
  if(!fCuts.PionMCut(&fPim)){fGoodEvent=kFALSE;return;}

  //Reconstruct missing or combined particles                                                                               
  HSLorentzVector miss=fBeam+fTarget-fElectron.P4()-fProton.P4()-fPim.P4();
  fMissMass2=miss.M2();
  fMissMass=miss.M();
  fPip.SetP4(miss);
  fPip.TakePDGMass();
}
void THS2pi::Topo_3(){
  //For topology e-:pi+:pi-
  //if(fElectron.Detector()>0) {fGoodEvent=kFALSE;return;} //Put some cuts on particle detectors
  //Define starttime from electron candidate
  fTrigger.StartTime(&fElectron);
  //Subtract starttime from all particles
  fTrigger.SubtractStartTime(&fElectron,&fPip,&fPim);

  //Can apply some timing cuts now                                                                                          
  if(!fCuts.ElCut(&fElectron)){fGoodEvent=kFALSE;return;}
  if(!fCuts.PionPCut(&fPip)){fGoodEvent=kFALSE;return;}
  if(!fCuts.PionMCut(&fPim)){fGoodEvent=kFALSE;return;}
  
  //Reconstruct missing or combined particles
  HSLorentzVector miss=fBeam+fTarget-fElectron.P4()-fPip.P4()-fPim.P4();
  fMissMass2=miss.M2();
  fMissMass=miss.M();
  fProton.SetP4(miss);
  fProton.TakePDGMass();

}
void THS2pi::Kinematics(){
  //configure trigger for this event
  fTrigger.ReadParticles();
  //Do calculations if Good Event
  HSLorentzVector pi2=fPip.P4()+fPim.P4();
  f2PiMass=pi2.M();
  
  HSLorentzVector Deltapp=fProton.P4()+fPip.P4();
  fDppMass=Deltapp.M();

  HSLorentzVector Delta0=fProton.P4()+fPim.P4();
  fD0Mass=Delta0.M();

  fKine.SetElecsTarget(fBeam,fElectron.P4(),fTarget);
  f_W=fKine.W();
  f_Q2=fKine.Q2();
  f_Pol=fKine.GammaPol();
  f_GammaE=fKine.GammaE();

  fKine.SetMesonBaryon(pi2,fProton.P4());
  fKine.ElectroCMDecay();
  fCMCosTh=fKine.CosTheta();
  fCMPhi=fKine.Phi();
  f_t=fKine.t();
  fKine.SetMesonDecay(fPip.P4(),fPim.P4());
  fKine.MesonDecayHelicity();
  fHelCosTh=fKine.CosTheta();
  fHelPhi=fKine.Phi();

  fPipP=fPip.P4().P();
  fPimP=fPim.P4().P();
  fPipTh=fPip.P4().Theta()*TMath::RadToDeg();
  fPimTh=fPim.P4().Theta()*TMath::RadToDeg();

}
//////////////////////////////////////////////////
/// Define conditions for track to be considered
/// good in event. Adding conditions on junk tracks
///  can greatly reduce combitorials etc.
/// kFALSE=> track ignored completely
Bool_t THS2pi::CheckParticle(THSParticle* part){
  if(part->Detector()<0)return kTRUE; //All FT tracks
  if(part->PDG()==45)return kFALSE; //? what are these?
  if(part->Time()==0)return kFALSE;   //Track needs time
  if(part->Charge()&&part->DeltaE()<2)return kFALSE; //Charged track needs deltaE>2

  return kTRUE;
}

void THS2pi::FinalStateOutTree(TTree* tree){
  THSFinalState::fFinalTree=tree;
  //tree->Branch("Final",&fFinal);//If you want to save the final THSParticles
  
  tree->Branch("MissMass",&fMissMass,"MissMass/D");
  tree->Branch("MissMass2",&fMissMass2,"MissMass2/D");
  tree->Branch("Topo",&fTopoID,"Topo/I");
  tree->Branch("NPerm",&fNPerm,"NPerm/I");
  tree->Branch("NDet",&fNDet,"NDet/I");
  tree->Branch("M2pi",&f2PiMass,"M2pi/D");
  tree->Branch("MDpp",&fDppMass,"MDpp/D");
  tree->Branch("MD0",&fD0Mass,"MD0/D");
  tree->Branch("Correct",&fCorrect,"Correct/I");
  tree->Branch("CMCosTh",&fCMCosTh,"CMCosTh/D");
  tree->Branch("CMPhi",&fCMPhi,"CMPhi/D");
  tree->Branch("HelCosTh",&fHelCosTh,"HelCosTh/D");
  tree->Branch("HelPhi",&fHelPhi,"HelPhi/D");
  tree->Branch("t",&f_t,"t/D");
  tree->Branch("GammaE",&f_GammaE,"GammaE/D");
  tree->Branch("Pol",&f_Pol,"Pol/D");
  tree->Branch("PipP",&fPipP,"PipP/D");
  tree->Branch("PimP",&fPimP,"PimP/D");
  tree->Branch("PipTh",&fPipTh,"PipTh/D");
  tree->Branch("PimTh",&fPimTh,"PimTh/D");
}
