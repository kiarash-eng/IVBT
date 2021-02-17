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
#include "FlexiSource.hh"
#include "PositionList.hh"
// GEANT4 //
#include "globals.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4NistManager.hh"
#include "G4LogicalVolume.hh"
#include "G4UserLimits.hh"
#include "G4UIcommand.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4Sphere.hh"
#include "G4RunManager.hh"
#include "G4Region.hh"
#include "G4ProductionCuts.hh"
#include "G4SolidStore.hh"
#include "G4UnitsTable.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

using namespace std;

FlexiSource::FlexiSource(G4String fileName, G4String core)
:source_vol(0), source_log(0), source_att(0), capsule_vol(0),
capsule_log(0), capsule_att(0), LCap_vol(0), LCap_log(0),
LCap_att(0), RCapCyl(0), RCapCyl_log(0), RCapCyl_att(0),
cable_vol(0), cable_log(0), cable_att(0), halfsph(0), box(0),
RCapSph(0), RCapSph_log(0), RCapSph_att(0) {
  pMaterial = new SourceMaterial();
  fPositionFilename = fileName;
  fCore = core;
  SetupDimensions();
  G4cout << " The Source is FlexiSource" << G4endl;
}

FlexiSource::FlexiSource()
:source_vol(0), source_log(0), source_att(0), capsule_vol(0),
capsule_log(0), capsule_att(0), LCap_vol(0), LCap_log(0),
LCap_att(0), RCapCyl(0), RCapCyl_log(0), RCapCyl_att(0),
cable_vol(0), cable_log(0), cable_att(0), halfsph(0), box(0),
RCapSph(0), RCapSph_log(0), RCapSph_att(0) {
  pMaterial = new SourceMaterial();
  fPositionFilename = "";
  fCore = "G4_Ir";
  SetupDimensions();
  G4cout << " The Source is FlexiSource" << G4endl;
}

FlexiSource:: ~FlexiSource() {
  delete pMaterial;
}

void FlexiSource::SetupDimensions() {
  innerSourceRadius = 0.*mm;
  outerSourceRadius = 0.3*mm;
  heightSourceHalf = 1.75*mm;
  innerCapsuleRadius = 0.3*mm;
  outerCapsuleRadius = 0.425*mm;
  heightCapsuleHalf = 1.75*mm;
  innerCableRadius = 0.*mm;
  outerCableRadius = 0.25*mm;
  heightCableHalf = 2.5*mm;
  innerLCapRadius1 = 0.*mm;
  outerLCapRadius1 = 0.25*mm;
  innerLCapRadius2 = 0.*mm;
  outerLCapRadius2 = 0.425*mm;
  heightLCapHalf = 0.225*mm;
  heightRCap = 0.65*mm;
  innerRCapRadius = 0.*mm;
  outerRCapRadius = 0.425*mm;
  CourbureRadius = 2.2 * outerRCapRadius*mm;
}

void FlexiSource::SetPositionFilename(G4String fileName) {
  fPositionFilename = fileName;
}

void FlexiSource::CreateSource(G4VPhysicalVolume* mother) {
  G4Material* steel = pMaterial->GetMat("Steel");
  G4Material* coreMaterial = pMaterial->GetMat(fCore);


  G4double boxz = sqrt((CourbureRadius * CourbureRadius) - (outerRCapRadius * outerRCapRadius))*mm;
  G4double heightRCapCylHalf = (heightRCap -(CourbureRadius-boxz))*mm;

  source_vol = new G4Tubs("source", innerSourceRadius, outerSourceRadius, heightSourceHalf, 0, 2*CLHEP::pi);
  source_log = new G4LogicalVolume(source_vol, coreMaterial, "source");
  source_att = new G4VisAttributes();
  source_att->SetColor(0, 1, 0, 0);
  source_att->SetVisibility(true);
  source_att->SetForceSolid(true);
  source_log->SetVisAttributes(source_att);

  capsule_vol = new G4Tubs("capsule", innerCapsuleRadius, outerCapsuleRadius, heightCapsuleHalf, 0, 2*CLHEP::pi);
  capsule_log = new G4LogicalVolume(capsule_vol,steel, "capsule_log");
  capsule_att = new G4VisAttributes();
  capsule_att->SetColor(0,0.5,0.5,0);
  capsule_att->SetVisibility(true);
  capsule_att->SetForceSolid(true);
  capsule_log->SetVisAttributes(capsule_att);

  LCap_vol = new G4Cons("LCap", innerLCapRadius1, outerLCapRadius1,innerLCapRadius2, outerLCapRadius2, heightLCapHalf, 0, 2*CLHEP::pi);
  LCap_log = new G4LogicalVolume(LCap_vol, steel, "LCap_log");
  LCap_att = new G4VisAttributes();
  LCap_att->SetColor(0, 1, 0, 0);
  LCap_att->SetVisibility(true);
  LCap_att->SetForceSolid(true);
  LCap_log->SetVisAttributes(LCap_att);

  RCapCyl = new G4Tubs ("RCapCyl",innerRCapRadius, outerRCapRadius,heightRCapCylHalf, 0, 2*CLHEP::pi);
  RCapCyl_log = new G4LogicalVolume(RCapCyl, steel, "RCapCyl_log");
  RCapCyl_att = new G4VisAttributes();
  RCapCyl_att->SetColor(0, 0, 1, 0);
  RCapCyl_att->SetVisibility(true);
  RCapCyl_att->SetForceSolid(true);
  RCapCyl_log->SetVisAttributes(RCapCyl_att);

  cable_vol = new G4Tubs("cable", innerCableRadius, outerCableRadius, heightCableHalf, 0, 2*CLHEP::pi);
  cable_log = new G4LogicalVolume(cable_vol, steel, "cable_log");
  cable_att = new G4VisAttributes();
  cable_att->SetColor(0.3, 0.3, 0.3, 0);
  cable_att->SetVisibility(true);
  cable_att->SetForceSolid(true);
  cable_log->SetVisAttributes(cable_att);

  halfsph = new G4Sphere("halfsph", innerRCapRadius, CourbureRadius, 0, 2*CLHEP::pi, 0, CLHEP::pi/2);
  box = new G4Box("box", CourbureRadius, CourbureRadius, boxz);
  RCapSph = new G4SubtractionSolid("RCapSph", halfsph, box);
  RCapSph_log = new G4LogicalVolume(RCapSph, steel, "RCapSph_log");
  RCapSph_att = new G4VisAttributes();
  RCapSph_att->SetColor(1, 1, 0, 0);
  RCapSph_att->SetVisibility(true);
  RCapSph_att->SetForceSolid(true);
  RCapSph_log->SetVisAttributes(RCapSph_att);

  if (fPositionFilename != "") {
    PositionList readPositionList;
    readPositionList.ReadDataFile(fPositionFilename);
    G4int numberOfSources = readPositionList.GetNumberOfSources();
    G4cout << "Total number of sources: " << numberOfSources << G4endl;
    for (G4int SourceNumber = 1; SourceNumber <= numberOfSources; SourceNumber++) {
      G4ThreeVector shift = readPositionList.GetPosition(SourceNumber-1);
      G4cout << "Source position: " << SourceNumber <<"  Position: " << shift << G4endl;

      new G4PVPlacement(0, shift+G4ThreeVector(0,0,0), "source", source_log, mother, false , SourceNumber);
      new G4PVPlacement(0, shift+G4ThreeVector(0,0,-heightSourceHalf - 2*heightLCapHalf-heightCableHalf), "cable", cable_log, mother, false, SourceNumber);
      new G4PVPlacement(0, shift+G4ThreeVector(0,0,0), "capsule", capsule_log, mother , false, SourceNumber);
      new G4PVPlacement(0, shift+G4ThreeVector(0,0,-heightSourceHalf-heightLCapHalf),"LCap", LCap_log, mother, false, SourceNumber);
      new G4PVPlacement(0, shift+G4ThreeVector(0,0,heightSourceHalf+heightRCapCylHalf), "RCapCyl", RCapCyl_log, mother, false, SourceNumber);
      new G4PVPlacement(0, shift+G4ThreeVector(0,0,heightSourceHalf+2*heightRCapCylHalf-boxz), "RCapSph", RCapSph_log, mother, false, SourceNumber);

      G4cout << " The Source is placed" << G4endl;
    }
  }
  else {
    G4int SourceNumber = 1;
    new G4PVPlacement(0, G4ThreeVector(0,0,0), "source", source_log, mother, false, SourceNumber);
    new G4PVPlacement(0, G4ThreeVector(0,0,-heightSourceHalf - 2*heightLCapHalf-heightCableHalf), "cable", cable_log, mother, false, SourceNumber);
    new G4PVPlacement(0, G4ThreeVector(0,0,0), "capsule", capsule_log, mother, false, SourceNumber);
    new G4PVPlacement(0, G4ThreeVector(0,0,-heightSourceHalf-heightLCapHalf),"LCap", LCap_log,mother, false, SourceNumber);
    new G4PVPlacement(0, G4ThreeVector(0,0,heightSourceHalf+heightRCapCylHalf), "RCapCyl", RCapCyl_log, mother, false, SourceNumber);
    new G4PVPlacement(0, G4ThreeVector(0,0,heightSourceHalf+2*heightRCapCylHalf-boxz), "RCapSph", RCapSph_log, mother, false, SourceNumber);

    G4cout << " The Source is placed" << G4endl;
  }

}



void FlexiSource::CleanSource() {
  /*
  delete source_vol;
  delete source_log;
  delete source_att;
  delete capsule_vol;
  delete capsule_log;
  delete capsule_att;
  delete LCap_vol;
  delete LCap_log;
  delete LCap_att;
  delete RCapCyl;
  delete RCapCyl_log;
  delete RCapCyl_att;
  delete cable_vol;
  delete cable_log;
  delete cable_att;
  delete halfsph;
  delete box;
  delete RCapSph;
  delete RCapSph_log;
  delete RCapSph_att;


   source_vol=0;
   source_log=0;
   source_att=0;
   capsule_vol=0;
   capsule_log=0;
   capsule_att=0;
   LCap_vol=0;
   LCap_log=0;
   LCap_att=0;
   RCapCyl=0;
   RCapCyl_log=0;
   RCapCyl_att=0;
   cable_vol=0;
   cable_log=0;
   cable_att=0;
   halfsph=0;
   box=0;
   RCapSph=0;
   RCapSph_log=0;
   RCapSph_att=0;
  */
}
