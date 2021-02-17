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
#include "RadialParallelWorldConstructionScoring.hh"
//#include "ParallelWorldMessenger.hh"

#include "G4PSDoseDeposit.hh"
#include "TrackLengthEstimator.hh"


RadialParallelWorldConstructionScoring::RadialParallelWorldConstructionScoring(G4String& parallelWorldName, DetectorConstruction *det, G4int voxels)
  :G4VUserParallelWorld(parallelWorldName) 
  {
   nVoxels = voxels;
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

void RadialParallelWorldConstructionScoring::Construct() 
{
  //if (egsphant->getPhantomFilename().size() == 0) return;
    
  G4NistManager *man = G4NistManager::Instance();
  man->SetVerbose(2);
  G4Material* air = man->FindOrBuildMaterial("G4_AIR");
  G4Material* water = man->FindOrBuildMaterial("G4_WATER");
  G4Material* pb = man->FindOrBuildMaterial("G4_Pb");
  std::vector<G4String> materials;
  materials.push_back("air");
  materials.push_back("water");
  materials.push_back("pb");
    
  G4VPhysicalVolume* ghostWorldS = GetWorld();
  G4LogicalVolume* worldLogicalS = ghostWorldS->GetLogicalVolume();
  worldLogicalS->SetMaterial(0);

  //Open up a stringstream to append ints to strings

    
  G4Tubs* Scoring_Sphere = new G4Tubs("Scoring sphere", 0.*cm, 5.*cm, 2.5*cm, 0.*deg, 360.*deg);
  G4LogicalVolume* RadialScoring_log = new G4LogicalVolume(Scoring_Sphere, 0,"RadialScoring log", 0, 0, 0);



  /*
  G4int radiusincrements = 36;
  G4int zaxisincrements = 60;
  G4int numberofvoxels = radiusincrements*zaxisincrements;

  TG60Parameterisation* rhoparam = new TG60Parameterisation(0.9*mm, 0.1*mm, radiusincrements, 30*mm, 1*mm, zaxisincrements);
  
  //This sets values for the logical volume (don't need to create Scoring_Shell if I don't want to)
  G4PVParameterised* rhoscoring_shell = new G4PVParameterised("TG60Scoring shell", RadialScoring_log, worldLogicalS, kZAxis, numberofvoxels, rhoparam);

  G4cout << "The number of scoring bins is set to " << rhoscoring_shell->GetMultiplicity() << G4endl;
  */
  
  //
  //Radial-Rho Parameterisation
  //
  
  G4int rhoradiusincrements = 36;
  G4int rhorhoincrements = 180;
  G4int nrhovoxels = rhoradiusincrements*rhorhoincrements;
  //Set 5*mm so that it takes up 1/4 of the source train radially
  RadialParameterisation* rhoparam = new RadialParameterisation(0.95*mm, 0.1*mm, 36, 2*deg, 1.*mm);

  //This sets values for the logical volume (don't need to create Scoring_Shell if I don't want to)
  G4PVParameterised* rhoscoring_shell = new G4PVParameterised("RadialScoring shell", RadialScoring_log, worldLogicalS, kZAxis, nrhovoxels, rhoparam);
  
    
  G4SDManager* SDManager = G4SDManager::GetSDMpointer();
  G4MultiFunctionalDetector* RadialMultiFunctionalDetector = new G4MultiFunctionalDetector("det1");


  G4VPrimitiveScorer* radialscorer;
  radialscorer = new TrackLengthEstimator("charge",0);
  RadialMultiFunctionalDetector->RegisterPrimitive(radialscorer);
    
  SDManager->AddNewDetector(RadialMultiFunctionalDetector);
  //TG60Scoring_log->SetSensitiveDetector(pMultiFunctionalDetector);
  RadialScoring_log->SetSensitiveDetector(RadialMultiFunctionalDetector);


  G4VisAttributes* visAttributes = new G4VisAttributes();
  visAttributes->SetColor(1,0,0);
  visAttributes->SetVisibility(true);
  //TG60Scoring_log->SetVisAttributes(visAttributes);
  
    
}

/*void ParallelWorldConstructionScoring::Construct() 
{
  //if (egsphant->getPhantomFilename().size() == 0) return;
    
  G4NistManager *man = G4NistManager::Instance();
  man->SetVerbose(2);
  G4Material* air = man->FindOrBuildMaterial("G4_AIR");
  G4Material* water = man->FindOrBuildMaterial("G4_WATER");
  G4Material* pb = man->FindOrBuildMaterial("G4_Pb");
  std::vector<G4String> materials;
  materials.push_back("air");
  materials.push_back("water");
  materials.push_back("pb");
    
  G4VPhysicalVolume* ghostWorldS = GetWorld();
  G4LogicalVolume* worldLogicalS = ghostWorldS->GetLogicalVolume();
  worldLogicalS->SetMaterial(0);
    
  G4Tubs* Scoring_Sphere = new G4Tubs("Scoring sphere", 0.*cm, 20.*cm, 10.*cm, 0.*deg, 360.*deg);
  G4LogicalVolume* Scoring_log = new G4LogicalVolume(Scoring_Sphere, 0,"Scoring log", 0, 0, 0);
  
  G4int radiusincrements = 36;
  G4int zaxisincrements = 60;
  G4int numberofvoxels = radiusincrements*zaxisincrements;

  TG60Parameterisation* param = new TG60Parameterisation(0.9*mm, 0.1*mm, radiusincrements, 30*mm, 1*mm, zaxisincrements);
  
  //This sets values for the logical volume (don't need to create Scoring_Shell if I don't want to)
  G4PVParameterised* Scoring_shell = new G4PVParameterised("Scoring shell", Scoring_log, worldLogicalS, kZAxis, numberofvoxels, param);

  G4cout << "The number of scoring bins is set to " << Scoring_shell->GetMultiplicity() << G4endl;
    
  G4SDManager* pSDManager = G4SDManager::GetSDMpointer();
  G4MultiFunctionalDetector* pMultiFunctionalDetector = new G4MultiFunctionalDetector("det");


  G4VPrimitiveScorer* scorer;
  scorer = new TrackLengthEstimator("TotalDoseM",0);
  pMultiFunctionalDetector->RegisterPrimitive(scorer);
    
  pSDManager->AddNewDetector(pMultiFunctionalDetector);
  Scoring_log->SetSensitiveDetector(pMultiFunctionalDetector);

  G4VisAttributes* visAttributes = new G4VisAttributes();
  visAttributes->SetColor(1,0,0);
  visAttributes->SetVisibility(true);
  Scoring_log->SetVisAttributes(visAttributes);
  
    
}*/
