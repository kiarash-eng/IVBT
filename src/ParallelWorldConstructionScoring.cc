#include <string>
#include <vector>

#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4Sphere.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"

#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"
#include "G4VPVParameterisation.hh"
#include "G4PhantomParameterisation.hh"

#include "G4VisAttributes.hh"

#include "G4SDManager.hh"
#include "G4MultiFunctionalDetector.hh"

#include "G4SystemOfUnits.hh"
#include "G4VUserParallelWorld.hh"
#include "G4VPrimitiveScorer.hh"
#include "SphericalParameterisation.hh"
#include "TG60Parameterisation.hh"
#include "RadialParameterisation.hh"
#include "DetectorConstruction.hh"
#include "ParallelWorldConstructionScoring.hh"
//#include "ParallelWorldMessenger.hh"

#include "G4PSDoseDeposit.hh"
#include "TrackLengthEstimator.hh"


//ParallelWorldConstructionScoring::ParallelWorldConstructionScoring(G4String& parallelWorldName, DetectorConstruction *det, G4int voxels)
ParallelWorldConstructionScoring::ParallelWorldConstructionScoring(G4String& parallelWorldName)
  :G4VUserParallelWorld(parallelWorldName)
  {
    AngularResolution = 9*deg;
    RadialResolution = 0.1*mm;
    RhoMin = 0.9*mm;
    RhoMax = 3.7*mm;
    ThetaMin = 0*deg;
    ThetaMax = 359.9*deg;
    G4cout << ThetaMax/deg << " " << ThetaMin/deg << " " << AngularResolution << G4endl;
    thetaInc = (ThetaMax-ThetaMin)/AngularResolution;
    rhoInc = (RhoMax - RhoMin)/RadialResolution;
  
  }

//void ParallelWorldConstructionScoring::setPhantFilename(G4String filename) {
//    phantomFilename = filename;
//}

//void ParallelWorldConstructionScoring::setAkPerHistory(G4double akPerHistory) {
//    fAkPerHistory = akPerHistory;
//}

//void ParallelWorldConstructionScoring::setRefAk(G4double refAk) {
//    fRefAk = refAk;
//}

//void ParallelWorldConstructionScoring::setTotalTime(G4double totalTime) {
//    fTotalTime = totalTime;
//}

//void ParallelWorldConstructionScoring::setTrackLength(G4bool trackStatus) {
//    fUseTrackLength = trackStatus;
//}

G4int ParallelWorldConstructionScoring::GetNumVoxels()
{
    G4int Ntotal = rhoInc*thetaInc;
  return Ntotal;
//  return 0;
}
G4int ParallelWorldConstructionScoring::GetNumVoxelsRow()
{
    G4int NTheta = thetaInc; //angular increments
  return NTheta;
//  return 0;
}

G4int ParallelWorldConstructionScoring::GetNumVoxelsCol()
{
    G4int Nrho = rhoInc; //radius increments
  return Nrho;
//  return 0;
}

std::vector<G4String> ParallelWorldConstructionScoring::GetColumnLabels()  {

  std::vector<G4String> colLabels;
    colLabels.push_back("theta (deg)/rho (mm)");
    G4double label = RhoMin;
    while(label<=RhoMax) {
      colLabels.push_back(std::to_string(label/mm));
      label += RadialResolution;
    }
 return colLabels;
}

std::vector<G4String> ParallelWorldConstructionScoring::GetRowLabels()  {

  std::vector<G4String> rowLabels;
  G4double label = ThetaMin;
  while(label<=ThetaMax) {
    rowLabels.push_back(std::to_string(label/CLHEP::deg));
    label += AngularResolution;
    }
  return rowLabels;

}

void ParallelWorldConstructionScoring::Construct()
{
  //if (egsphant->getPhantomFilename().size() == 0) return;

  G4NistManager *man = G4NistManager::Instance();
  man->SetVerbose(2);
  G4Material* air = man->FindOrBuildMaterial("G4_AIR");
  G4Material* water = man->FindOrBuildMaterial("G4_WATER");
  G4Material* pb = man->FindOrBuildMaterial("G4_Pb");
  /*std::vector<G4String> materials;
  materials.push_back("air");
  materials.push_back("water");
  materials.push_back("pb");

*/
  G4VPhysicalVolume* ghostWorldS = GetWorld();
  G4LogicalVolume* worldLogicalS = ghostWorldS->GetLogicalVolume();
  worldLogicalS->SetMaterial(NULL);

  //Open up a stringstream to append ints to strings
  //std::ostringstream namestream;


  //4.58s for 0 > 360deg
  //13.63s for 0 >180deg

  //namestream << "Scoring_Sphere" << nVoxels;
  //G4Tubs* Scoring_Sphere = new G4Tubs(namestream.str(), 0.*cm, 5.*cm, 5.*cm, 0.*deg, 360.*deg);

    //G4double startdist = 0.0*mm;
  //if (nVoxels == 1) {  startdist = 0.9*mm; Scoring_Sphere->SetInnerRadius(0.9*mm); Scoring_Sphere->SetOuterRadius(1.0*mm);}
  //else {  startdist = 1.0*mm; Scoring_Sphere->SetInnerRadius(1.0*mm); Scoring_Sphere->SetOuterRadius(1.1*mm);}

   //Empty the stringstream from the next operation
   /*
  namestream.str("");
  namestream.clear();
  namestream << "TG60Scoring_log" << nVoxels;
  G4LogicalVolume* TG60Scoring_log = new G4LogicalVolume(Scoring_Sphere, 0,namestream.str(), 0, 0, 0);
*/
  //Empty the stringstream from the next operation
//  namestream.str("");
//  namestream.clear();

/*

  G4int Nrho = 26;
  G4int zaxisincrements = 1;
  G4int numberofvoxels = Nrho*zaxisincrements;
  TG60Parameterisation* tg60param = new TG60Parameterisation(0.9*mm, 0.1*mm, Nrho, 7.5*mm, 16*mm, zaxisincrements);
  //RadialParameterisation* tg60param = new RadialParameterisation(0.9*mm, 0.1*mm, Nrho, 180.*deg, 0.05*mm);


  //This sets values for the logical volume (don't need to create Scoring_Shell if I don't want to)
  G4PVParameterised* TG60scoring_shell = new G4PVParameterised("TG60Scoring_shell", TG60Scoring_log, worldLogicalS, kZAxis, numberofvoxels, tg60param);
*/

G4Material* voxelMat = 0;
voxelMat = 0;


G4Tubs* Scoring_Cyl = new G4Tubs("Scoring cylinder",RhoMin, RhoMax, 16, 0*CLHEP::deg, 360*CLHEP::deg);
G4LogicalVolume* Scoring_Cyl_Log = new G4LogicalVolume(Scoring_Cyl, voxelMat, "Scoring log", 0, 0, 0);
new G4PVPlacement(0,G4ThreeVector(),Scoring_Cyl_Log,"Scoring_Phys",worldLogicalS,false,0);

G4Tubs* ThetaVoxel = new G4Tubs("Angular_slice",RhoMin, RhoMax,16,0*CLHEP::deg,(9)*CLHEP::deg);
G4LogicalVolume* logThetaRep = new G4LogicalVolume(ThetaVoxel,voxelMat,"Logical Angular_slice");
new G4PVReplica("Theta_replica", logThetaRep, Scoring_Cyl_Log,kPhi, thetaInc, AngularResolution, 4.5*CLHEP::deg);

G4Tubs* RadialVoxel = new G4Tubs("Radial_slice",RhoMin, RhoMin + RadialResolution,16,0*CLHEP::deg,(9)*CLHEP::deg);
Scoring_log = new G4LogicalVolume(RadialVoxel,voxelMat,"Logical voxel");
new G4PVReplica("Rho_replica",Scoring_log,logThetaRep,kRho,rhoInc,RadialResolution,RhoMin);


/*
  G4int rhoNrho = 26; //the radius increases up to a bit further than the outermost layer.
  G4int rhorhoincrements = 40;
  G4int nrhovoxels = rhoNrho*rhorhoincrements;
  //Set 5*mm so that it takes up 1/4 of the source train radially
  RadialParameterisation* rhoparam = new RadialParameterisation(0.9*mm, 0.1*mm, rhoNrho, 9.*deg, 10*mm);

  //This sets values for the logical volume (don't need to create Scoring_Shell if I don't want to)
  G4PVParameterised* TG60scoring_shell = new G4PVParameterised("RadialScoring shell", TG60Scoring_log, worldLogicalS, kZAxis, nrhovoxels, rhoparam);

  G4cout << "The number of scoring bins is set to " << TG60scoring_shell->GetMultiplicity() << G4endl;
*/
  //namestream << "det" << nVoxels;
}
  void ParallelWorldConstructionScoring::ConstructSD() {
  G4SDManager* pSDManager = G4SDManager::GetSDMpointer();
  //G4MultiFunctionalDetector* pMultiFunctionalDetector = new G4MultiFunctionalDetector(namestream.str());
  //G4MultiFunctionalDetector* pMultiFunctionalDetector = new G4MultiFunctionalDetector(namestream.str());
  G4MultiFunctionalDetector* pMultiFunctionalDetector = new G4MultiFunctionalDetector("det");
  //G4cout << "TEST: " <<namestream.str() << G4endl;
  //Empty the stringstream from the next operation
  //namestream.str("");
  //namestream.clear();


  G4VPrimitiveScorer* scorer;
  //namestream << "TotalDoseM" << nVoxels;
  //scorer = new TrackLengthEstimator(namestream.str(),0);
  //scorer = new TrackLengthEstimator(namestream.str(),1);
  scorer = new TrackLengthEstimator("TotalDoseM",1);
  ((TrackLengthEstimator*) scorer)->SetNumberCol(this->GetNumVoxelsCol());
  ((TrackLengthEstimator*) scorer)->SetNumberRow(this->GetNumVoxelsRow());
  //((G4PSDoseDeposit* ) scorer)->SetEnergyCut(0);
  //((G4PSDoseDeposit* ) scorer)->SetPSS(0);



  pMultiFunctionalDetector->RegisterPrimitive(scorer);
  //  G4cout << "test" << namestream.str() << G4endl;

  pSDManager->AddNewDetector(pMultiFunctionalDetector);
  SetSensitiveDetector(Scoring_log,pMultiFunctionalDetector);
  //TG60Scoring_log->SetSensitiveDetector(pMultiFunctionalDetector);
/*
  G4VisAttributes* visAttributes = new G4VisAttributes();
  visAttributes->SetVisibility(true);
  visAttributes->SetForceWireframe(true);
  visAttributes->SetColor(1,0,0);
  TG60Scoring_log->SetVisAttributes(visAttributes);
*/

}
