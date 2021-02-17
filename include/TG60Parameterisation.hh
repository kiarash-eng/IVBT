#include "G4VPVParameterisation.hh"
#include "G4ThreeVector.hh"
#include "globals.hh"
#include <vector>

class G4LogicalVolume;

class TG60Parameterisation : public G4VPVParameterisation {

public:
  
  // Constructor
  TG60Parameterisation(const G4double startRadius, const G4double stepRadius, const G4double incrementRadius, const G4double startZ, const G4double stepZ, const G4double incrementZ);
  
  // Destructor
  virtual ~TG60Parameterisation();
  
  // Method
  virtual void ComputeDimensions(G4Tubs& cylinder, const G4int copyNo, const G4VPhysicalVolume*) const;
  virtual void ComputeTransformation(const G4int, G4VPhysicalVolume*) const;
    
private:
  
  G4double startradius;
  G4double stepradius;
  G4double incrementradius;
  G4double startz, stepz, incrementz;
  // Mutable keyword is used for internal counters
  // The const methods above can only modify parameters marked with mutable
  mutable G4int radiusNumber, zNumber;

};