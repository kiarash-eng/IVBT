#include "G4VPVParameterisation.hh"
#include "G4ThreeVector.hh"
#include "globals.hh"
#include <vector>

class G4LogicalVolume;

class RadialParameterisation : public G4VPVParameterisation {

public:
  
  // Constructor
  RadialParameterisation(const G4double startRadius, const G4double stepRadius, const G4double incrementRadius, const G4double deltaAngle, const G4double zhalfLength);
  
  // Destructor
  virtual ~RadialParameterisation();
  
  // Method
  virtual void ComputeDimensions(G4Tubs& cylinder, const G4int copyNo, const G4VPhysicalVolume*) const;
  virtual void ComputeTransformation(const G4int, G4VPhysicalVolume*) const;

  /*
  private:
    
    void ComputeTransformation(const G4int, G4VPhysicalVolume*) const {};
 */

private:
  
  G4double startradius;
  G4double stepradius, deltaangle;
  G4double zhalflength;
  G4double incrementradius;

};