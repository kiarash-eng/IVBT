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

#include <algorithm>
#include <iomanip>
#include <cstdlib>
#include <sstream>
#include <iostream>
#include <string>
// USER //
#include "MicroSelectronV2.hh"
#include "SourceMaterial.hh"
#include "DetectorConstruction.hh"
#include "PositionList.hh"
// GEANT4 //
#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "G4Sphere.hh"
#include "G4RunManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4UnionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"
#include "G4TransportationManager.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"

using namespace std;

MicroSelectronV2::MicroSelectronV2(G4String fileName, G4String core) {
  pMaterial = new SourceMaterial();
  SetupDimensions();
  fPositionFilename = fileName;
  fCore = core;
}

MicroSelectronV2::MicroSelectronV2() {
  pMaterial = new SourceMaterial();
  SetupDimensions();
  fPositionFilename = "";
  fCore = "G4_Ir";
}

void MicroSelectronV2::SetupDimensions() {
  innerCylSourceRadius = 0.0*mm;
  outerCylSourceRadius = 0.325*mm;
  heightCylSourceHalf = 1.74*mm;

  innerMinConSourceRadius = 0.0*mm;
  outerMinConSourceRadius = 0.265*mm;
  innerMaxConSourceRadius = 0.0*mm;
  outerMaxConSourceRadius = 0.325*mm;
  heightConeSourceHalf = 0.03*mm;

  innerCyl1CapsuleRadius = 0.325*mm;
  outerCyl1CapsuleRadius = 0.45*mm;
  heightCyl1CapsuleHalf = 1.8*mm;

  innerCyl2CapsuleRadius = 0.0*mm;
  outerCyl2CapsuleRadius = 0.45*mm;
  heightCyl2CapsuleHalf = 0.275*mm;

  innerMinConCapsuleRadius = 0.0*mm;
  outerMinConCapsuleRadius = 0.35*mm;
  innerMaxConCapsuleRadius = 0.0*mm;
  outerMaxConCapsuleRadius = 0.45*mm;
  heightConCapsuleHalf = 0.075*mm;

  innerCableRadius = 0.*mm;
  outerCableRadius = 0.35*mm;
  heightCableHalf = 1.0*mm;

  startPhi = 0.;
  spanPhi = 2. * CLHEP::pi;
  spanTheta = CLHEP::pi/2.;
}

MicroSelectronV2::~MicroSelectronV2() {
  delete pMaterial;
}

void MicroSelectronV2::SetPositionFilename(G4String fileName) {
  fPositionFilename = fileName;
}

void MicroSelectronV2::CreateSource(G4VPhysicalVolume* mother) {
  G4Material* steel = pMaterial->GetMat("Steel");
  G4Material* coreMaterial = pMaterial->GetMat(fCore);

  //Core
  G4Tubs* source_vol1 = new G4Tubs("Source_vol1", innerCylSourceRadius, outerCylSourceRadius,heightCylSourceHalf, startPhi, spanPhi);
  G4Cons* source_vol2 = new G4Cons("source_vol2", innerMaxConSourceRadius, outerMaxConSourceRadius,
				   innerMinConSourceRadius,outerMinConSourceRadius, heightConeSourceHalf,startPhi, spanPhi);

  G4Cons* source_vol3 = new G4Cons("source_vol3",innerMinConSourceRadius,outerMinConSourceRadius,
				   innerMaxConSourceRadius,outerMaxConSourceRadius,heightConeSourceHalf,startPhi, spanPhi);

  G4VSolid* source1 = new G4UnionSolid("source1", source_vol1, source_vol2, 0, G4ThreeVector(0.0, 0.0, heightCylSourceHalf + heightConeSourceHalf));
  G4VSolid* source = new G4UnionSolid("source", source1, source_vol3, 0, G4ThreeVector(0.0, 0.0, -(heightCylSourceHalf + heightConeSourceHalf)));

  G4LogicalVolume* source_log = new G4LogicalVolume(source, coreMaterial, "source_log");
  G4VisAttributes* source_att = new G4VisAttributes();
  source_att->SetColor(1, 0, 0);
  source_att->SetVisibility(true);
  source_att->SetForceSolid(false);
  source_log->SetVisAttributes(source_att);

  // Steel Capsule
  G4Tubs* capsuleCyl1 = new G4Tubs("capsuleCyl1",innerCyl1CapsuleRadius ,outerCyl1CapsuleRadius,heightCyl1CapsuleHalf,startPhi, spanPhi);
  G4Tubs* capsuleCyl2 = new G4Tubs("capsuleCyl2",innerCyl2CapsuleRadius ,outerCyl2CapsuleRadius,heightCyl2CapsuleHalf,startPhi, spanPhi);

  G4Cons* capsuleCone  = new G4Cons("capsuleCone",innerMinConCapsuleRadius,outerMinConCapsuleRadius,
				    innerMaxConCapsuleRadius,outerMaxConCapsuleRadius,heightConCapsuleHalf,startPhi, spanPhi);
  G4Sphere* capsSphere = new G4Sphere("capsSphere",innerCylSourceRadius,outerCyl1CapsuleRadius,startPhi,spanPhi,startPhi,spanTheta);

  G4VSolid* capsuleHalf1 = new G4UnionSolid("capsuleHalf1",capsuleCyl1,capsuleCyl2, 0, G4ThreeVector(0.0, 0.0, -(heightCyl1CapsuleHalf + heightCyl2CapsuleHalf)));
  G4VSolid* capsuleHalf2 = new G4UnionSolid("capsuleHalf2",capsuleHalf1,capsuleCone, 0, G4ThreeVector(0.0, 0.0,
												      -(heightCyl1CapsuleHalf + 2*heightCyl2CapsuleHalf + heightConCapsuleHalf)));

  G4VSolid* capsule = new G4UnionSolid("capsule",capsuleHalf2,capsSphere, 0, G4ThreeVector(0.0, 0.0, heightCyl1CapsuleHalf));

  G4LogicalVolume* capsule_log = new G4LogicalVolume(capsule, steel, "capsule_log");
  G4VisAttributes* capsule_att = new G4VisAttributes();
  capsule_att->SetColor(0.5, 0.5, 0.5);
  capsule_att->SetVisibility(true);
  capsule_att->SetForceSolid(false);
  capsule_log->SetVisAttributes(capsule_att);

  // Steel Cable
  G4Tubs* cable_vol = new G4Tubs("cable_vol",innerCableRadius ,outerCableRadius,heightCableHalf , startPhi, spanPhi);
  G4LogicalVolume* cable_log = new G4LogicalVolume(cable_vol, steel, "cable_log");
  G4VisAttributes* cable_att = new G4VisAttributes();
  cable_att->SetColor(0, 0, 1);
  cable_att->SetVisibility(true);
  cable_att->SetForceSolid(false);
  cable_log->SetVisAttributes(cable_att);

  G4double cableOffset = -heightCylSourceHalf - 2 * heightConeSourceHalf - 2. * heightCyl2CapsuleHalf - 2 * heightConCapsuleHalf - heightCableHalf;
  if (fPositionFilename != "") {
    PositionList readPositionList;
    readPositionList.ReadDataFile(fPositionFilename);
    G4int numberOfSources = readPositionList.GetNumberOfSources();
    G4cout << "Total number of sources: " << numberOfSources << G4endl;
    for(G4int SourceNumber = 1; SourceNumber <= numberOfSources; SourceNumber++) {
      G4ThreeVector shift = readPositionList.GetPosition(SourceNumber-1);
      G4cout << "Source position: " << SourceNumber<<"  Position: "<<shift<<G4endl;
      new G4PVPlacement(0, shift+G4ThreeVector(0,0,0), "source", source_log, mother, false, SourceNumber);
      new G4PVPlacement(0, shift+G4ThreeVector(0,0,0), "capsule", capsule_log, mother, false, SourceNumber);
      new G4PVPlacement(0, shift+G4ThreeVector(0,0,cableOffset), "cable", cable_log, mother,  false, SourceNumber);
    }
  }
  else {
    G4int SourceNumber = 1;
    new G4PVPlacement(0, G4ThreeVector(0,0,0) , "source", source_log, mother, false, SourceNumber);
    new G4PVPlacement(0, G4ThreeVector(0,0,0) , "capsule", capsule_log, mother, false, SourceNumber);
    new G4PVPlacement(0, G4ThreeVector(0,0,cableOffset), "cable", cable_log, mother, false, SourceNumber);
  }

}

void MicroSelectronV2::CleanSource() {
  G4cout << " MicroSelectronV2 is cleaned" << G4endl;
  G4RunManager::GetRunManager() -> GeometryHasBeenModified();
}
