/*




THIS IS THE DETECTOR CONSTRUCTION CROSS SECTION FOR VISUALIZATION PURPOSES ONLY








*/
#include "DetectorConstruction.hh"

#include "SourceMaterial.hh"


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
#include "SrYexperimental.hh"

//DetectorConstruction::DetectorConstruction(G4int NumberVoxels)
DetectorConstruction::DetectorConstruction()
 :  World_log(0), World_phys(0) {


  // FlexiSource is the default source in the geometry.
  // Vessel thickness [mm] represents the tunica media only.
  fBrachySource = "NovosteBetaCath40mmguidewire";
  fCore = "Water";
  fStentOverlapping == "Overlapped";
  fStentNumber = "One";
  detectorMessenger = new DetectorMessenger(this);

  pMaterial = new SourceMaterial();
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

 void DetectorConstruction::SelectStentOverlapping(G4String val)
  {
   fStentOverlapping = val;
   G4cout << "The position of three stents is set to " << val << G4endl;
  }
void DetectorConstruction::SelectStentNumber(G4String val)
 {
  fStentNumber = val;
  G4cout << "The number of stents is set to " << val << G4endl;
 }

G4VPhysicalVolume* DetectorConstruction::Construct() {
  if (fBrachySource == "GuidantGalileoGuidewire")
  {
    Source = new GuidantGallileo(true);
  }
  if (fBrachySource == "NovosteBetaCath40mmguidewire")
  {
    Source = new NovosteBetaCath(16, true);
  }

  if (fBrachySource == "NovosteBetaCath40mm")
  {
    Source = new NovosteBetaCath(16, false);
  }

  if (fBrachySource == "GuidantGalileo")
  {
    Source = new GuidantGallileo(false);

  }
   if (fBrachySource == "sryexperimental")
  {
    Source = new Sryexperimental(16, true);
  }

  if (fStentNumber == "One")
 {
   StentNumber = 1;
 }
 if (fStentNumber == "Three")
 {
   StentNumber = 3;
 }

  RVD = 5*mm;
  mediaThickness = 0.5*mm;
  adventitiaThickness = 0.7*mm;

  RVR = 0.5 * RVD;
  CalcifiedThickness = 0.5*mm;

  pMaterial->DefineMaterials();
  ConstructWorld();
  //Source->WriteSource();
  // Model the source in the phantom
  Source->CreateSource(World_phys);
  // Model the source in the world
  //if (fBrachySource != "") { Source->CreateSource(World_phys); }
  //if (Source) { Source->CreateSource(World_phys); }
  return World_phys;
}
void DetectorConstruction::ConstructWorld()
{

  G4NistManager* man = G4NistManager::Instance();

  G4Material* vacuum = man->FindOrBuildMaterial("G4_Galactic");
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
  G4Element*  Mo = man->FindOrBuildElement("Mo");
  G4Element*  Co = man->FindOrBuildElement("Co");
  G4Element*  W = man->FindOrBuildElement("W");
  G4Element*  Br = man->FindOrBuildElement("Br");


  G4Material* water = man->FindOrBuildMaterial("G4_WATER");


G4Material* Media = new G4Material( "Media", 1.07*CLHEP::g/CLHEP::cm3, 12, kStateSolid, Temperature);
Media->AddElement( C, 0.3215 );
Media->AddElement( H, 0.0821 );
Media->AddElement( N, 0.1140 );
Media->AddElement( O, 0.423733 );
Media->AddElement( Mg , 0.0024 );
Media->AddElement( K, 0.0399 );
Media->AddElement( Na, 0.0054 );
Media->AddElement( Cl, 0.0068 );
Media->AddElement( Fe, 0.000104 );
Media->AddElement( Zn, 0.000158 );
Media->AddElement( Br, 0.000190 );
Media->AddElement( Ca, 0.003715 );


G4Material* Adventitia = new G4Material( "Adventitia", 1.07*CLHEP::g/CLHEP::cm3, 4, kStateSolid, Temperature);
Adventitia->AddElement( C, 0.5280 );
Adventitia->AddElement( H, 0.0698 );
Adventitia->AddElement( N, 0.1724 );
Adventitia->AddElement( O, 0.2298 );

G4Material* FibroticPlaque = new G4Material( "FibroticPlaque", 1.22*CLHEP::g/CLHEP::cm3, 9, kStateSolid, Temperature);
FibroticPlaque->AddElement( Ca, 0.223);
FibroticPlaque->AddElement( P, 0.034);
FibroticPlaque->AddElement( Na, 0.063);
FibroticPlaque->AddElement( K, 0.028);
FibroticPlaque->AddElement( Mg, 0.034);
FibroticPlaque->AddElement( C, 0.256);
FibroticPlaque->AddElement( H, 0.047);
FibroticPlaque->AddElement( N, 0.179);
FibroticPlaque->AddElement( O, 0.136);


G4Material* CalcifiedPlaque = new G4Material( "CalcifiedPlaqueMaterial", 1.45*CLHEP::g/CLHEP::cm3, 5, kStateSolid, Temperature);
CalcifiedPlaque->AddElement( Ca, 0.7029);
CalcifiedPlaque->AddElement( P, 0.2733);
CalcifiedPlaque->AddElement( Na, 0.0048);
CalcifiedPlaque->AddElement( K, 0.0097);
CalcifiedPlaque->AddElement( Mg, 0.0093);

WorldSize = 40.*cm;
// ScoringSphereRadius=1.*cm;
// maximum strut thickness common among all the stents.
Stent_thickness = 0.08*mm;
Ring_Longitudinal_length = 0.25*mm;
Ring_spacing = 1.75*mm;
Rod_number = 8;
Ring_number = 16; //obtained from pictures of the manufacturer
// adventitia thickness remains uncahnged for all vessels diameters
// the lowest value among the maximum lesion size [mm] coverred
// by all the stents
LesionLength = 32*mm;
ArteryLength = 3*LesionLength;
//Nvoxels = NumberVoxels;


// setting the kStateSolid, Temperature at which tissues and plaques are constructed
Temperature = 310.5*kelvin;
Neointima = 0.3*mm;

// World Volume
G4Sphere* World = new G4Sphere("World", 0.*CLHEP::cm, 100.*CLHEP::cm, 0*CLHEP::deg, 360*CLHEP::deg, 0*CLHEP::deg, 180*CLHEP::deg);
World_log = new G4LogicalVolume(World, water, "World log", 0, 0, 0);   //G4LogicalVolume* (Solid, Material, Shape)

G4Sphere* MiniWorld = new G4Sphere("World", 0.*CLHEP::cm, 10.*CLHEP::cm, 0*CLHEP::deg, 360*CLHEP::deg, 0*CLHEP::deg, 180*CLHEP::deg);

G4Tubs* TunicaMedia = new G4Tubs("TunicaMedia",RVR,(RVR+mediaThickness),0.5*ArteryLength,0*CLHEP::deg,360*CLHEP::deg);
G4Tubs* TunicaAdventitia = new G4Tubs("TunicaAdventitia",(RVR+mediaThickness),(RVR+mediaThickness+adventitiaThickness),0.5*ArteryLength,0*CLHEP::deg,360*CLHEP::deg);
G4LogicalVolume* TunicaMedia_log = new G4LogicalVolume(TunicaMedia,Media,"TunicaMedia_log");
G4LogicalVolume* TunicaAdventitia_log = new G4LogicalVolume(TunicaAdventitia,Adventitia,"TunicaAdventitia_log");
// Visualization Attributes
//World
G4VisAttributes* w_vis = new G4VisAttributes();
w_vis->SetColor(1, 0, 0);
w_vis->SetVisibility(false);
w_vis->SetForceSolid(false);
World_log->SetVisAttributes(w_vis);

G4VisAttributes* stent_vis = new G4VisAttributes();
stent_vis->SetColor((1), (1), (1));
stent_vis->SetVisibility(true);
stent_vis->SetForceSolid(false);

G4VisAttributes* media_vis = new G4VisAttributes();
media_vis->SetColor(1, (152/255), (127/255));
media_vis->SetVisibility(true);
media_vis->SetForceSolid(true);
TunicaMedia_log->SetVisAttributes(media_vis);

G4VisAttributes* adventitia_vis = new G4VisAttributes();
//adventitia_vis->SetColor(1, (178/255), (145/255));
// adventitia_vis->SetColor(0, 0, 1);
adventitia_vis->SetColor(1, 1, 0);
adventitia_vis->SetVisibility(true);
adventitia_vis->SetForceSolid(true);
TunicaAdventitia_log->SetVisAttributes(adventitia_vis);

G4VisAttributes* fibrotic_vis = new G4VisAttributes();
fibrotic_vis->SetColor(1, 1, (146/255));
fibrotic_vis->SetVisibility(true);
fibrotic_vis->SetForceSolid(true);

G4VisAttributes* calcified_vis = new G4VisAttributes();
//calcified_vis->SetColor(1, 1, (146/255));
calcified_vis->SetColor(0.5, 0.5, 0.5);
calcified_vis->SetVisibility(true);
calcified_vis->SetForceSolid(true);

// Placing Physical Volumes

//World
World_phys = new G4PVPlacement(0, G4ThreeVector(0,0,0), World_log, "World_phys", 0, false, 0, false);
if (fCore == "Artery_Stent")
{
  new G4PVPlacement(0, G4ThreeVector(0,0,0), TunicaMedia_log, "TunicaMedia_phys", World_log, false, 0, false);
  new G4PVPlacement(0, G4ThreeVector(0,0,0), TunicaAdventitia_log, "TunicaAdventitia_phys", World_log, false, 0, false);

  G4Tubs* TunicaRemodelling = new G4Tubs("TunicaRemodelling", (RVR-Neointima)*mm,RVR*mm,0.5*LesionLength*mm,0*CLHEP::deg,360*CLHEP::deg);
  G4LogicalVolume* TunicaRemodelling_log_1 = new G4LogicalVolume(TunicaRemodelling,FibroticPlaque,"TunicaRemodelling_log_1");
  TunicaRemodelling_log_1->SetVisAttributes(fibrotic_vis);
  new G4PVPlacement(0, G4ThreeVector(0,0,0), TunicaRemodelling_log_1, "TunicaRemodelling_phys_1", World_log, false, 0, false);

  G4LogicalVolume* TunicaRemodelling_log_2 = new G4LogicalVolume(TunicaRemodelling,vacuum,"TunicaRemodelling_log_2");
  TunicaRemodelling_log_2->SetVisAttributes(fibrotic_vis);
  new G4PVPlacement(0, G4ThreeVector(0,0,0), TunicaRemodelling_log_2, "TunicaRemodelling_phys_2", World_log, false, 0, false);

  G4LogicalVolume* TunicaRemodelling_log_3 = new G4LogicalVolume(TunicaRemodelling,vacuum,"TunicaRemodelling_log_3");
  TunicaRemodelling_log_3->SetVisAttributes(fibrotic_vis);
  new G4PVPlacement(0, G4ThreeVector(0,0,0), TunicaRemodelling_log_3, "TunicaRemodelling_phys_3", World_log, false, 0, false);

  // constructing ringReplica_MotherVolume for stent rings replication.
  G4Tubs* ringReplica_MotherVolume = new G4Tubs("ringReplica_MotherVolume", RVR-3*Stent_thickness,RVR*mm,0.5*LesionLength*mm,0*CLHEP::deg,360*CLHEP::deg);
  G4LogicalVolume* ringReplica_MotherVolume_log_1 = new G4LogicalVolume(ringReplica_MotherVolume,vacuum,"ringReplica_MotherVolume_log_1");
  ringReplica_MotherVolume_log_1->SetVisAttributes(fibrotic_vis);
  new G4PVPlacement(0, G4ThreeVector(0,0,0), ringReplica_MotherVolume_log_1, "ringReplica_MotherVolume_phys_1", World_log, false, 0, false);

  G4LogicalVolume* ringReplica_MotherVolume_log_2 = new G4LogicalVolume(ringReplica_MotherVolume,vacuum,"ringReplica_MotherVolume_log_2");
  ringReplica_MotherVolume_log_2->SetVisAttributes(fibrotic_vis);
  new G4PVPlacement(0, G4ThreeVector(0,0,0), ringReplica_MotherVolume_log_2, "ringReplica_MotherVolume_phys_2", World_log, false, 0, false);

  G4LogicalVolume* ringReplica_MotherVolume_log_3 = new G4LogicalVolume(ringReplica_MotherVolume,vacuum,"ringReplica_MotherVolume_log_3");
  ringReplica_MotherVolume_log_3->SetVisAttributes(fibrotic_vis);
  new G4PVPlacement(0, G4ThreeVector(0,0,0), ringReplica_MotherVolume_log_3, "ringReplica_MotherVolume_phys_3", World_log, false, 0, false);

  G4Tubs* plaque = new G4Tubs("CalcifiedPlaque", (RVR-Neointima-CalcifiedThickness)*mm,(RVR-Neointima)*mm,0.5*LesionLength*mm,0*CLHEP::deg,360*CLHEP::deg);
  G4LogicalVolume* plaque_log = new G4LogicalVolume(plaque,CalcifiedPlaque,"plaque_log");
  plaque_log->SetVisAttributes(calcified_vis);
  new G4PVPlacement(0, G4ThreeVector(0,0,0), plaque_log, "plaque_phys", World_log, false, 0, false);

  // Creating Boston Scientific: Synergy
  G4Material* stent = new G4Material( fStentModel, 8.00*CLHEP::g/CLHEP::cm3, 5, kStateSolid, Temperature);
  stent->AddElement( Fe, 0.63 );
  stent->AddElement( Cr, 0.18 );
  stent->AddElement( Ni, 0.14 );
  stent->AddElement( Mo, 0.026 );
  stent->AddElement( Mn, 0.024 );

  if (fStentOverlapping == "Overlapped")
  {
    G4Tubs* stent_rod_1 = new G4Tubs(fStentModel,RVR-StentNumber*Stent_thickness, RVR,0.5*LesionLength,45*CLHEP::deg,(9)*CLHEP::deg);
    G4LogicalVolume* stent_rod_1_log = new G4LogicalVolume(stent_rod_1,stent,"stent_rod_1_log");
    stent_rod_1_log->SetVisAttributes(stent_vis);
    G4VPhysicalVolume* divided_rod_phys_1 = new G4PVReplica("divided_rod_phys_1", stent_rod_1_log, TunicaRemodelling_log_1,kPhi, Rod_number, 45*CLHEP::deg, 0*CLHEP::deg);
    G4Tubs* stent_ring_1 = new G4Tubs(fStentModel,RVR-StentNumber*Stent_thickness, RVR,0.5*Ring_Longitudinal_length,0*CLHEP::deg,360*CLHEP::deg);
    G4LogicalVolume* stent_ring_1_log = new G4LogicalVolume(stent_ring_1,stent,"stent_ring_1_log");
    stent_ring_1_log->SetVisAttributes(stent_vis);
    G4VPhysicalVolume* divided_ring_phys_1 = new G4PVReplica("divided_ring_phys_1", stent_ring_1_log, ringReplica_MotherVolume_log_1, kZAxis, Ring_number, Ring_Longitudinal_length+Ring_spacing);
  }
  if (fStentOverlapping == "NotOverlapped")
  {
    G4Tubs* stent_rod_1 = new G4Tubs(fStentModel,RVR-3*Stent_thickness, RVR-2*Stent_thickness,0.5*LesionLength,45*CLHEP::deg,(9)*CLHEP::deg);
    G4LogicalVolume* stent_rod_1_log = new G4LogicalVolume(stent_rod_1,stent,"stent_rod_1_log");
    G4VPhysicalVolume* divided_rod_phys_1 = new G4PVReplica("divided_rod_phys_1", stent_rod_1_log, TunicaRemodelling_log_1,kPhi, Rod_number, 45*CLHEP::deg, 4.5*CLHEP::deg);
    G4Tubs* stent_ring_1 = new G4Tubs(fStentModel,RVR-3*Stent_thickness, RVR-2*Stent_thickness,0.5*Ring_Longitudinal_length,0*CLHEP::deg,360*CLHEP::deg);
    G4LogicalVolume* stent_ring_1_log = new G4LogicalVolume(stent_ring_1,stent,"stent_ring_1_log");
    G4VPhysicalVolume* divided_ring_phys_1 = new G4PVReplica("divided_ring_phys_1", stent_ring_1_log, ringReplica_MotherVolume_log_1, kZAxis, Ring_number, Ring_Longitudinal_length+Ring_spacing);

    G4Tubs* stent_rod_2 = new G4Tubs(fStentModel,RVR-2*Stent_thickness, RVR-Stent_thickness,0.5*LesionLength,45*CLHEP::deg,(9)*CLHEP::deg);
    G4LogicalVolume* stent_rod_2_log = new G4LogicalVolume(stent_rod_2,stent,"stent_rod_2_log");
    G4VPhysicalVolume* divided_rod_phys_2 = new G4PVReplica("divided_rod_phys_2", stent_rod_2_log, TunicaRemodelling_log_2,kPhi, Rod_number, 45*CLHEP::deg, 22.5*CLHEP::deg);
    G4Tubs* stent_ring_2 = new G4Tubs(fStentModel,RVR-2*Stent_thickness, RVR-Stent_thickness,0.5*Ring_Longitudinal_length,0*CLHEP::deg,360*CLHEP::deg);
    G4LogicalVolume* stent_ring_2_log = new G4LogicalVolume(stent_ring_2,stent,"stent_ring_2_log");
    G4VPhysicalVolume* divided_ring_phys_2 = new G4PVReplica("divided_ring_phys_2", stent_ring_2_log, ringReplica_MotherVolume_log_2, kZAxis, Ring_number, Ring_Longitudinal_length+Ring_spacing);

    G4Tubs* stent_rod_3 = new G4Tubs(fStentModel,RVR-Stent_thickness, RVR,0.5*LesionLength,45*CLHEP::deg,(9)*CLHEP::deg);
    G4LogicalVolume* stent_rod_3_log = new G4LogicalVolume(stent_rod_3,stent,"stent_rod_3_log");
    G4VPhysicalVolume* divided_rod_phys_3 = new G4PVReplica("divided_rod_phys_3", stent_rod_3_log, TunicaRemodelling_log_3,kPhi, Rod_number, 45*CLHEP::deg, 40.5*CLHEP::deg);
    G4Tubs* stent_ring_3 = new G4Tubs(fStentModel,RVR-Stent_thickness, RVR,0.5*Ring_Longitudinal_length,0*CLHEP::deg,360*CLHEP::deg);
    G4LogicalVolume* stent_ring_3_log = new G4LogicalVolume(stent_ring_3,stent,"stent_ring_3_log");
    G4VPhysicalVolume* divided_ring_phys_3 = new G4PVReplica("divided_ring_phys_3", stent_ring_3_log, ringReplica_MotherVolume_log_3, kZAxis, Ring_number, Ring_Longitudinal_length+Ring_spacing);
  }
}


/*
//This if for if you want to visualize the parallel scoring world
 G4LogicalVolume* MiniWorld_log = new G4LogicalVolume(MiniWorld, water, "World log", 0, 0, 0);
 G4PVPlacement* MiniWorld_phys = new G4PVPlacement(0,G4ThreeVector(0,0,0),"MiniWorld_phys",MiniWorld_log,World_phys,false,0,false);

 G4LogicalVolume* MiniWorld_log1 = new G4LogicalVolume(MiniWorld, water, "World log", 0, 0, 0);
 G4PVPlacement* MiniWorld_phys1 = new G4PVPlacement(0,G4ThreeVector(0,0,0),"MiniWorld_phys1",MiniWorld_log1,World_phys,false,0,false);

 G4Tubs* Scoring_Sphere = new G4Tubs("Scoring sphere", 0.*cm, 20.*cm, 10.*cm, 0.*deg, 360.*deg);
 G4LogicalVolume* Scoring_log1 = new G4LogicalVolume(Scoring_Sphere, water,"Scoring log", 0, 0, 0);
//using G4Replica from Jonathan's code.
 G4Material* voxelMat = 0;
 G4int radiusincrements = 28;
 G4double fSpatialResolution = 0.1*mm;
 G4double fRhoMin = 0.9*mm;
 G4double fAngularResolution = 9*CLHEP::deg;
 G4double angularincrements = 40;

 G4Tubs* Scoring_Cyl = new G4Tubs("Scoring cylinder", fRhoMin, fRhoMin + fSpatialResolution*radiusincrements, 0.5*LesionLength, 0*CLHEP::deg, 360*CLHEP::deg);
 G4LogicalVolume* Scoring_Cyl_Log = new G4LogicalVolume(Scoring_Cyl, vacuum, "Scoring log", 0, 0, 0);
 new G4PVPlacement(0,G4ThreeVector(),Scoring_Cyl_Log,"Scoring_Phys",MiniWorld_log,false,0);

 G4Tubs* ThetaVoxel = new G4Tubs("Angular_slice",fRhoMin, fRhoMin + fSpatialResolution*radiusincrements,0.5*LesionLength,0*CLHEP::deg,(9)*CLHEP::deg);
 G4LogicalVolume* logThetaRep = new G4LogicalVolume(ThetaVoxel,vacuum,"Logical Angular_slice");
 new G4PVReplica("Theta_replica", logThetaRep, Scoring_Cyl_Log,kPhi, angularincrements, fAngularResolution, 4.5*CLHEP::deg);

 G4Tubs* RadialVoxel = new G4Tubs("Radial_slice",fRhoMin, fRhoMin + fSpatialResolution,0.5*LesionLength,0*CLHEP::deg,(9)*CLHEP::deg);
 Scoring_log = new G4LogicalVolume(RadialVoxel,vacuum,"Logical voxel");
 new G4PVReplica("Rho_replica",Scoring_log,logThetaRep,kRho,radiusincrements,fSpatialResolution,fRhoMin);
*/
/*

 }

/*
 //  TG 60 Param
 G4int radiusincrements = 26;
 G4int zaxisincrements = 1;
 G4int numberofvoxels = radiusincrements*zaxisincrements;

 TG60Parameterisation* param = new TG60Parameterisation(0.9*mm, 0.1*mm, radiusincrements, 7.5*mm, 0.5*LesionLength, zaxisincrements);

*/
 //Radial-Rho Parameterisation
 /*
 G4int radiusincrements1 = 26;
 G4int rhoincrements = 40;
 G4int numberofvoxels = radiusincrements1*rhoincrements;
 //Set 5*mm so that it takes up 1/4 of the source train radially
 RadialParameterisation* param = new RadialParameterisation(0.9*mm, 0.1*mm, radiusincrements1, 9.*deg, 10*mm);

 //This sets values for the logical volume (don't need to create Scoring_Shell if I don't want to)
 G4PVParameterised* Scoring_shell = new G4PVParameterised("Scoring shell", Scoring_log1, MiniWorld_log1, kZAxis, numberofvoxels, param);



 G4VisAttributes* World_visAtt = new G4VisAttributes;
 World_visAtt->SetVisibility(true);
 World_visAtt->SetForceWireframe(true);
 World_visAtt->SetColor(1, 0., 0);
 Scoring_log1->SetVisAttributes(World_visAtt);
*/
}
