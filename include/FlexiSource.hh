#ifndef FlexiSource_h
#define FlexiSource_h 1

#include <vector>
#include "SourceMaterial.hh"

#include "G4VUserDetectorConstruction.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Sphere.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4SubtractionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "globals.hh"
#include "G4VisAttributes.hh"

#include "SourceModel.hh"

class G4LogicalVolume;
class G4VPhysicalVolume;
class SourceModel;

class FlexiSource : public SourceModel {
  public:
  FlexiSource();
  explicit FlexiSource(G4String, G4String);
  ~FlexiSource();
  void CreateSource(G4VPhysicalVolume*);
  void CleanSource();
  void SetupDimensions();
  void SetPositionFilename(G4String);
    //void ReadFile();
  std::vector<G4ThreeVector> Positions;
  std::vector<G4double> TimeArray;
  G4double TotalTime;
  G4double x, y, z, theta, phi, px, py, pz, E;

  private:
  G4Tubs* source_vol;
  G4LogicalVolume* source_log;
  G4VisAttributes* source_att;
  G4Tubs* capsule_vol;
  G4LogicalVolume* capsule_log;
  G4VisAttributes* capsule_att;
  G4Cons* LCap_vol;
  G4LogicalVolume* LCap_log;
  G4VisAttributes* LCap_att;
  G4Tubs* RCapCyl;
  G4LogicalVolume* RCapCyl_log;
  G4VisAttributes* RCapCyl_att;
  G4Tubs* cable_vol;
  G4LogicalVolume* cable_log;
  G4VisAttributes* cable_att;
  G4Sphere* halfsph;
  G4Box* box;
  G4SubtractionSolid* RCapSph;
  G4LogicalVolume*  RCapSph_log;
  G4VisAttributes* RCapSph_att;

  G4double innerSourceRadius;
  G4double outerSourceRadius;
  G4double heightSourceHalf;
  G4double innerCapsuleRadius;
  G4double outerCapsuleRadius;
  G4double heightCapsuleHalf;
  G4double innerCableRadius;
  G4double outerCableRadius;
  G4double heightCableHalf;
  G4double innerLCapRadius1;
  G4double outerLCapRadius1;
  G4double innerLCapRadius2;
  G4double outerLCapRadius2;
  G4double heightLCapHalf;
  G4double  heightRCap;
  G4double innerRCapRadius;
  G4double outerRCapRadius;
  G4double CourbureRadius;

  SourceMaterial* pMaterial;
  G4int currentPosition;
  G4ThreeVector ParticlePosition;
  G4String fPositionFilename;
  G4String fCore;
};

#endif

