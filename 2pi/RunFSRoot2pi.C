//root --hsdata --hsfinal=THS2pi RunFSRoot2pi.C
//You need to replace 2pi with your final state class name
{
  //Create FinalState
  THS2pi* fs=new THS2pi();
  // fs->SetGenerated(); //just analyse generated branch
   fs->SetMaxParticles(10);
  //create datamanager
  THSDataManager* dm=new THSDataManager();
  TChain chain("HSParticles");
  chain.Add("./out_clas_003432.evio.992.hipo.root");
  dm->InitChain(&chain);
  //connect Project to HSParticles
  fs->SetDataManager(dm);
  Int_t counter=0;
  
  //create ouput tree
  TFile* outfile=new TFile("OUTPUT.root","recreate");
  TTree* outtree=new TTree("FinalTree","output tree");
  fs->FinalStateOutTree(outtree); //connect ouput tree to project branches
  
  gBenchmark->Start("timer");
  
  while(dm->ReadEvent()){//loop over events
    fs->ProcessEvent();
  }
  gBenchmark->Stop("timer");
  gBenchmark->Print("timer");
  
  outfile->cd();
  outtree->Write();
  delete outfile;
}
