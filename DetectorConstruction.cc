/*




THIS IS THE DETECTOR CONSTRUCTION CROSS SECTION FOR VISUALIZATION PURPOSES ONLY





*/
#include "DetectorConstruction.hh"

#include "SourceMaterial.hh"

#include "FlexiSource.hh"
#include "MicroSelectronV2.hh"

#include "DetectorMessenger.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"
#include "G4Material.hh"
#include "G4Orb.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4Sphere.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"
#include "G4UnitsTable.hh"
#include "G4VisAttributes.hh"
#include "G4SDManager.hh"
#include "G4MultiFunctionalDetector.hh"
#include "PSDoseDeposit.hh"
#include "PSDoseAirDeposit.hh"
#include "SphericalParameterisation.hh"
#include "TG60Parameterisation.hh"
#include "RadialParameterisation.hh"
#include "TrackLengthEstimator.hh"
#include "G4NistManager.hh"
#include "G4UserLimits.hh"
#include "NovosteBetaCath.hh"
#include "GuidantGallileo.hh"

DetectorConstruction::DetectorConstruction(G4int NumberVoxels)
 :  World_log(0), World_phys(0) {
  WorldSize=40.*cm;
  //ScoringSphereRadius=1.*cm;
  Nvoxels=NumberVoxels;

  // FlexiSource is the default source in the geometry.
  fBrachySource = "None";
  fCore = "Water";

  detectorMessenger = new DetectorMessenger(this);

  pMaterial = new SourceMaterial();
  ArteryShift = 0*mm;
}

DetectorConstruction::~DetectorConstruction() {}

void DetectorConstruction::SelectBrachySource(G4String val) 
 {
  fBrachySource = val;
  G4cout << "The source is set to " << val << G4endl;
 }

void DetectorConstruction::SelectBrachyCore(G4String val) 
 {
  fCore = val;
  G4cout << "The phantom is set to " << val << G4endl;
 }

G4VPhysicalVolume* DetectorConstruction::Construct() {
  
  
  if (fBrachySource == "NovosteBetaCath40mmguidewire") 
  {
    Source = new NovosteBetaCath(16, true);
    ArteryShift = 0.3775*mm;
  } 
  if (fBrachySource == "NovosteBetaCath40mm") 
  {
    Source = new NovosteBetaCath(16, false);
    ArteryShift = 0.3775*mm;
  } 
  if (fBrachySource == "GuidantGalileoGuidewire") 
  {
    Source = new GuidantGallileo(true);
    ArteryShift = 0.4275*mm;
  } 
  if (fBrachySource == "GuidantGalileo") 
  {
    Source = new GuidantGallileo(false);
    ArteryShift = 0.4275*mm;
  } 
  

  ConstructWorld();
  // Model the source in the world
  //if (fBrachySource != "") { Source->CreateSource(World_phys); }
  if (Source) { Source->CreateSource(World_phys); }
  

  return World_phys;
}

void DetectorConstruction::ConstructWorld() 
{


    G4NistManager* man = G4NistManager::Instance();

  G4Element*  H  = man->FindOrBuildElement("H");
  G4Element*  C = man->FindOrBuildElement("C");
  G4Element*  N =man->FindOrBuildElement("N");
  G4Element*  O = man->FindOrBuildElement("O");
  G4Element*  Na = man->FindOrBuildElement("Na");
  G4Element*  Mg = man->FindOrBuildElement("Mg");
  G4Element*  P  = man->FindOrBuildElement("P");
  G4Element*  S  = man->FindOrBuildElement("S");
  G4Element*  Cl  = man->FindOrBuildElement("Cl");
  G4Element*  K = man->FindOrBuildElement("K");
  G4Element*  Ca = man->FindOrBuildElement("Ca");
  G4Element*  Fe = man->FindOrBuildElement("Fe");
  G4Element*  Zn  = man->FindOrBuildElement("Zn");
  G4Element*  Cr =man->FindOrBuildElement("Cr");
  G4Element*  Ni = man->FindOrBuildElement("Ni");
  G4Element*  Mn = man->FindOrBuildElement("Mn");
  G4Element*  Si = man->FindOrBuildElement("Si");


  G4Material* water = man->FindOrBuildMaterial("G4_WATER");
  G4Material* adipose = man->FindOrBuildMaterial("G4_ADIPOSE_TISSUE_ICRP");


  //
// User Defined Materials
//

  // Muscle Tissue (will play role of Vascular Smooth Muscle)
  G4Material* Muscle = new G4Material( "Muscle", 1.06*CLHEP::g/CLHEP::cm3, 13 );
  Muscle->AddElement( H, 0.119477 );
  Muscle->AddElement( C, 0.63724 );
  Muscle->AddElement( N, 0.00797 );
  Muscle->AddElement( O, 0.232333 );
  Muscle->AddElement( Na, 0.0005 );
  Muscle->AddElement( Mg , 2e-05 );
  Muscle->AddElement( P, 0.00016 );
  Muscle->AddElement( S, 0.00073 );
  Muscle->AddElement( Cl, 0.00119 );
  Muscle->AddElement( K, 0.00032 );
  Muscle->AddElement( Ca, 2e-05 );
  Muscle->AddElement( Fe, 2e-05 );
  Muscle->AddElement( Zn, 2e-05 );

  G4Material* CalcifiedPlaque = new G4Material( "CalcifiedPlaque", 1.45*CLHEP::g/CLHEP::cm3, 3 );
  CalcifiedPlaque->AddElement( Ca, 0.20039);
  CalcifiedPlaque->AddElement( P, 0.06195);
  CalcifiedPlaque->AddElement( C, 0.73766);

  G4Material* steel = new G4Material( "Steel", 8.0*CLHEP::g/CLHEP::cm3, 6 );
  steel->AddElement( Fe, 0.6792 );
  steel->AddElement( Cr, 0.1900 );
  steel->AddElement( Ni, 0.1000 );
  steel->AddElement( Mn, 0.0200 );
  steel->AddElement( Si, 0.0100 );
  steel->AddElement( C , 0.0008 );


//World Volume
G4Sphere* World = new G4Sphere("World", 0.*CLHEP::cm, 15.*CLHEP::cm, 0.*CLHEP::deg, 360.*CLHEP::deg, 0.*CLHEP::deg, 180.*CLHEP::deg);
World_log = new G4LogicalVolume(World, water, "World log", 0, 0, 0);   //G4LogicalVolume* (Solid, Material, Shape)

G4Sphere* MiniWorld = new G4Sphere("World", 0.*CLHEP::cm, 10.*CLHEP::cm, 0.*CLHEP::deg, 360.*CLHEP::deg, 0.*CLHEP::deg, 180.*CLHEP::deg);

//Artery Walls (Media, assuming Media Thickness of 1.1mm, 1.4mm in stenoted area)

G4Tubs* TunicaMedia = new G4Tubs("TunicaMedia",1.5*CLHEP::mm,2.6*CLHEP::mm,30*CLHEP::mm,270.*CLHEP::deg,180.*CLHEP::deg);
G4Tubs* TunicaRemodelling = new G4Tubs("TunicaRemodelling", 1.35*mm,1.5*mm,10*mm,270.*CLHEP::deg,180.*CLHEP::deg);
G4Tubs* TunicaAdventitia = new G4Tubs("TunicaAdventitia",2.6*CLHEP::mm,5*CLHEP::mm,30*CLHEP::mm,270.*CLHEP::deg,180.*CLHEP::deg);

G4LogicalVolume* TunicaMedia_log = new G4LogicalVolume(TunicaMedia,Muscle,"TunicaMedia_log");
G4LogicalVolume* TunicaRemodelling_log = new G4LogicalVolume(TunicaRemodelling,Muscle,"TunicaRemodelling_log");
G4LogicalVolume* TunicaAdventitia_log = new G4LogicalVolume(TunicaAdventitia,adipose,"TunicaAdventitia_log");

//G4Tubs* Stent1 = new G4Tubs("Stent1",1.36*CLHEP::mm,1.5*CLHEP::mm,10*CLHEP::mm,87.*CLHEP::deg,3.*CLHEP::deg);
G4Tubs* Stent2 = new G4Tubs("Stent2",1.36*CLHEP::mm,1.5*CLHEP::mm,10*CLHEP::mm,42*CLHEP::deg,6.*CLHEP::deg);
G4Tubs* Stent3 = new G4Tubs("Stent3",1.36*CLHEP::mm,1.5*CLHEP::mm,10*CLHEP::mm,357.*CLHEP::deg,6.*CLHEP::deg);
G4Tubs* Stent4 = new G4Tubs("Stent4",1.36*CLHEP::mm,1.5*CLHEP::mm,10*CLHEP::mm,312.*CLHEP::deg,6.*CLHEP::deg);
//G4Tubs* Stent5 = new G4Tubs("Stent5",1.36*CLHEP::mm,1.5*CLHEP::mm,10*CLHEP::mm,270.*CLHEP::deg,3.*CLHEP::deg);
//G4Tubs* Stent6 = new G4Tubs("Stent6",1.36*CLHEP::mm,1.5*CLHEP::mm,10*CLHEP::mm,222.*CLHEP::deg,6.*CLHEP::deg);
//G4Tubs* Stent7 = new G4Tubs("Stent7",1.36*CLHEP::mm,1.5*CLHEP::mm,10*CLHEP::mm,177.*CLHEP::deg,6.*CLHEP::deg);
//G4Tubs* Stent8 = new G4Tubs("Stent8",1.36*CLHEP::mm,1.5*CLHEP::mm,10*CLHEP::mm,132.*CLHEP::deg,6.*CLHEP::deg);

//G4LogicalVolume* Stent1_log = new G4LogicalVolume(Stent1,steel,"Stent1_log");
G4LogicalVolume* Stent2_log = new G4LogicalVolume(Stent2,steel,"Stent2_log");
G4LogicalVolume* Stent3_log = new G4LogicalVolume(Stent3,steel,"Stent3_log");
G4LogicalVolume* Stent4_log = new G4LogicalVolume(Stent4,steel,"Stent4_log");
//G4LogicalVolume* Stent5_log = new G4LogicalVolume(Stent5,steel,"Stent5_log");
//G4LogicalVolume* Stent6_log = new G4LogicalVolume(Stent6,steel,"Stent6_log");
//G4LogicalVolume* Stent7_log = new G4LogicalVolume(Stent7,steel,"Stent7_log");
//G4LogicalVolume* Stent8_log = new G4LogicalVolume(Stent8,steel,"Stent8_log");



//Plaque (0.5mm thick in area with remodelling)
G4Tubs* Plaque = new G4Tubs("Plaque",0.85*CLHEP::mm,1.35*CLHEP::mm,5*CLHEP::mm,0.*CLHEP::deg,90.*CLHEP::deg);
G4LogicalVolume* Plaque_log = new G4LogicalVolume(Plaque,CalcifiedPlaque,"Plaque_log");

//
// Visualization Attributes
//
  
  //World
  G4VisAttributes* w_vis = new G4VisAttributes();
  w_vis->SetColor(1, 0, 0);
  w_vis->SetVisibility(false);
  w_vis->SetForceSolid(false);
  World_log->SetVisAttributes(w_vis);

  G4VisAttributes* media_vis = new G4VisAttributes();
  media_vis->SetColor(1, 0, 0);
  media_vis->SetVisibility(true);
  media_vis->SetForceSolid(false);
  TunicaMedia_log->SetVisAttributes(media_vis);

  G4VisAttributes* plaque_vis = new G4VisAttributes();
  plaque_vis->SetColor(1, 1, 0);
  plaque_vis->SetVisibility(true);
  plaque_vis->SetForceSolid(false);
  Plaque_log->SetVisAttributes(plaque_vis);

//
// Placing Physical Volumes
//
  
  //World
  World_phys = new G4PVPlacement(0, G4ThreeVector(0,0,0), World_log, "World_phys", 0, false, 0, false);

  if (fCore == "Artery" || fCore == "Artery_Stent") 
  { 
  new G4PVPlacement(0, G4ThreeVector(0,ArteryShift,0), TunicaMedia_log, "TunicaMedia_phys", World_log, false, 0, false);
  new G4PVPlacement(0, G4ThreeVector(0,ArteryShift,0), TunicaRemodelling_log, "TunicaRemodelling_phys", World_log, false, 0, false);
  new G4PVPlacement(0, G4ThreeVector(0,ArteryShift,0), Plaque_log, "Plaque_phys", World_log, false, 0, false);
  new G4PVPlacement(0, G4ThreeVector(0,ArteryShift,0), TunicaAdventitia_log, "TunicaAdventitia_phys", World_log, false, 0, false);
  }
  if (fCore == "Artery_Stent") 
  {
  //new G4PVPlacement(0, G4ThreeVector(0,0,0), Stent1_log, "Stent1_phys", TunicaRemodelling_log, false, 0, false);
  new G4PVPlacement(0, G4ThreeVector(0,0,0), Stent2_log, "Stent2_phys", TunicaRemodelling_log, false, 0, false);
  new G4PVPlacement(0, G4ThreeVector(0,0,0), Stent3_log, "Stent3_phys", TunicaRemodelling_log, false, 0, false);
  new G4PVPlacement(0, G4ThreeVector(0,0,0), Stent4_log, "Stent4_phys", TunicaRemodelling_log, false, 0, false);
  //new G4PVPlacement(0, G4ThreeVector(0,0,0), Stent5_log, "Stent5_phys", TunicaRemodelling_log, false, 0, false);
  ///new G4PVPlacement(0, G4ThreeVector(0,0,0), Stent6_log, "Stent6_phys", TunicaRemodelling_log, false, 0, false);
  //new G4PVPlacement(0, G4ThreeVector(0,0,0), Stent7_log, "Stent7_phys", TunicaRemodelling_log, false, 0, false);
  //new G4PVPlacement(0, G4ThreeVector(0,0,0), Stent8_log, "Stent8_phys", TunicaRemodelling_log, false, 0, false);
  }




  //This if for if you want to visualize the parallel scoring world
  /*
  G4LogicalVolume* MiniWorld_log = new G4LogicalVolume(MiniWorld, water, "World log", 0, 0, 0);
  G4PVPlacement* MiniWorld_phys = new G4PVPlacement(0,G4ThreeVector(0,0,0),"MiniWorld_phys",MiniWorld_log,World_phys,false,0,false);
  
  G4Tubs* Scoring_Sphere = new G4Tubs("Scoring sphere", 0.*cm, 20.*cm, 10.*cm, 0.*deg, 360.*deg);
  G4LogicalVolume* Scoring_log = new G4LogicalVolume(Scoring_Sphere, water,"Scoring log", 0, 0, 0);
  
  /*  TG 60 Param 
  G4int radiusincrements = 36;
  G4int zaxisincrements = 60;
  G4int numberofvoxels = radiusincrements*zaxisincrements;

  TG60Parameterisation* param = new TG60Parameterisation(0.9*mm, 0.1*mm, radiusincrements, 30*mm, 1*mm, zaxisincrements);
  // 
  
  //Radial-Rho Parameterisation
  G4int radiusincrements = 36;
  G4int rhoincrements = 18;
  G4int numberofvoxels = radiusincrements*rhoincrements;
  //Set 5*mm so that it takes up 1/4 of the source train radially
  RadialParameterisation* param = new RadialParameterisation(0.9*mm, 0.1*mm, 36, 20.*deg, 5*mm);

  //This sets values for the logical volume (don't need to create Scoring_Shell if I don't want to)
  G4PVParameterised* Scoring_shell = new G4PVParameterised("Scoring shell", Scoring_log, MiniWorld_log, kZAxis, numberofvoxels, param);
  


  G4VisAttributes* World_visAtt = new G4VisAttributes;
  World_visAtt->SetVisibility(true);
  World_visAtt->SetForceWireframe(true);
  World_visAtt->SetColor(1, 0., 0);
  Scoring_log->SetVisAttributes(World_visAtt);
  */
  

}
