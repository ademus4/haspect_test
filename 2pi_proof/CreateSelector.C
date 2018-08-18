/** \file CreateSelector.C
 * 
 * Macro to generate a TSelector
 * using the THSSkeleton class
 * 
 * Usage:
 * root --hssel CreateSelector.C
 * 
 */

void CreateSelector(){

  THSSkeleton* sk = new THSSkeleton();
  sk->SetHisto();   //Going to make histograms
  sk->SetNewTree();   //Going to make new tree
  sk->SetFinalState("THS2pi"); //Give final state class name
  sk->CreateSelector("process","/home/adamt/work/haspect_test/2pi/out_clas_003432.evio.992.hipo.root","HSParticles"); //create selector with all specifications
};
