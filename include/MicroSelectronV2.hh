//////////////////////////////////////////////////////////////////////////
//
// Author: Shirin A. Enger <shirin@enger.se>
//
// License & Copyright
// ===================
//
// Copyright 2015 Shirin A Enger <shirin@enger.se>
//
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//////////////////////////////////////////////////////////////////////////

// USER //
#ifndef MicroSelectronV2_H
#define MicroSelectronV2_H 1
#include "SourceMaterial.hh"
// GEANT4 //
#include "G4Cons.hh"
#include "G4UnionSolid.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"
#include "G4TransportationManager.hh"
#include "G4VisAttributes.hh"
#include "SourceModel.hh"

class G4LogicalVolume;
class G4Tubs;
class G4Box;
class G4Sphere;
class G4VPhysicalVolume;
class G4VisAttributes;
class SourceModel;

class MicroSelectronV2: public SourceModel {
  public:
    explicit MicroSelectronV2(G4String, G4String);
    MicroSelectronV2();
    ~MicroSelectronV2();

    void CreateSource(G4VPhysicalVolume*);
    void CleanSource();

    void SetPositionFilename(G4String fileName);
    void SetupDimensions();

  private:
    G4double innerCylSourceRadius, outerCylSourceRadius,heightCylSourceHalf,innerMinConSourceRadius;
    G4double outerMinConSourceRadius, innerMaxConSourceRadius, outerMaxConSourceRadius,heightConeSourceHalf;
    G4double innerCyl1CapsuleRadius, outerCyl1CapsuleRadius, heightCyl1CapsuleHalf, innerCyl2CapsuleRadius;
    G4double outerCyl2CapsuleRadius, heightCyl2CapsuleHalf, innerMinConCapsuleRadius, outerMinConCapsuleRadius;
    G4double innerMaxConCapsuleRadius, outerMaxConCapsuleRadius, heightConCapsuleHalf, innerCableRadius;
    G4double outerCableRadius, heightCableHalf, startPhi, spanPhi, spanTheta ;
    SourceMaterial* pMaterial;
    G4String fPositionFilename;
    G4String fCore;
};
#endif








