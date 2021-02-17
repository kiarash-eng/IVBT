//////////////////////////////////////////////////////////////////////////
//
// Author: Joseph M. Decunha <joseph.decunha@mail.mcgill.ca>
//
// License & Copyright
// ===================
//
// Copyright 2016 Joseph M. Decunha <joseph.decunha@mail.mcgill.ca>
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
//
//
//-----------------------------------------------------------------------
// Class that defines the Guidant Galileo 20mm model 
// for Intravascular Brachytherapy
//////////////////////////////////////////////////////////////////////////


// Geant4
#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "G4UImanager.hh"
#include "G4Box.hh"
#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4Sphere.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "Randomize.hh"
#include "G4RandomDirection.hh"
#include "G4NistManager.hh"
#include "G4Tubs.hh"
#include "G4RotationMatrix.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4UserLimits.hh"
// User
#include "GuidantGallileo.hh"
#include "SourceModel.hh"

GuidantGallileo::GuidantGallileo(G4bool Guidewire)
{
 guidewire = Guidewire;
 SetupDimensions();
}
void GuidantGallileo::SetupDimensions()
{
 sourceradius = 0.12*mm;
 tungstenradius = 0.135*mm;
 shellradius = 0.225*mm;
 

 sourcelength = 20*mm;
 tungstenlength = 1*mm;
 shelllength = 40*mm;
}

void GuidantGallileo::CleanSource()
{
	G4cout << "No Source Cleaning Implemented for this Model" << G4endl;
}

void GuidantGallileo::CreateSource(G4VPhysicalVolume* mothervolume)
{
 //
 //Elements and Materials Definitions
 //
  G4NistManager* man = G4NistManager::Instance();
  G4Element*  Ni = man->FindOrBuildElement("Ni");
  G4Element*  Ti = man->FindOrBuildElement("Ti");
  G4Element*  Mn = man->FindOrBuildElement("Mn");
  G4Element*  Si = man->FindOrBuildElement("Si");
  G4Element*  C  = man->FindOrBuildElement("C");
  G4Element*  O  = man->FindOrBuildElement("O");
  G4Element*  Al  = man->FindOrBuildElement("Al");
  G4Element*  Fe = man->FindOrBuildElement("Fe");
  G4Element*  Cr =man->FindOrBuildElement("Cr");
  
  //Should consider looking up specific materials for their aluminum
  G4Material* Tungsten = man->FindOrBuildMaterial("G4_W");
  G4Material* PVC = man->FindOrBuildMaterial("G4_POLYVINYL_CHLORIDE");

 //
 // User Defined Materials
 //

  // Stainless steel
  G4Material* steel = new G4Material( "Steel", 8.0*CLHEP::g/CLHEP::cm3, 6 );
  steel->AddElement( Fe, 0.6792 );
  steel->AddElement( Cr, 0.1900 );
  steel->AddElement( Ni, 0.1000 );
  steel->AddElement( Mn, 0.0200 );
  steel->AddElement( Si, 0.0100 );
  steel->AddElement( C , 0.0008 );

  // Nickel Titanium, alloy of the former elements in approx. equal parts
  // super elastic. Density taken from Wikipedia.
  G4Material* NickelTitanium = new G4Material("Nickel_Titanium", 6.45*CLHEP::g/CLHEP::cm3, 2);
  NickelTitanium->AddElement(Ni, 0.5);
  NickelTitanium->AddElement(Ti, 0.5);

 //
 //Shapes and Logical Volumes
 //

  G4Tubs* SourceWire = new G4Tubs("SourceWire",0*mm,sourceradius,sourcelength/2,0*deg,360*deg);
  G4Tubs* TungstenMarker = new G4Tubs("TungstenMarker", 0*mm, tungstenradius, tungstenlength/2, 0*deg, 360*deg);
  G4Tubs* InnerShell = new G4Tubs("InnerShell",0*mm,tungstenradius, (18*mm)/2 , 0*deg,360*deg);
  G4Tubs* OuterShell = new G4Tubs("OuterShell",tungstenradius,shellradius,shelllength/2, 0*deg,360*deg);
  //Throwing this in here at the last minute, sorry for the magic numbers
  //Trying to approximate the same guidewire as in the Novoste Beta Cath (same distance from source as though there was a catheter there)
  G4Tubs* GuideWire = new G4Tubs("GuideWire",0*mm,0.1775*mm, 30*mm, 0*deg, 360*deg);
  G4Sphere* ShellEnd = new G4Sphere("ShellEnd", 0*mm,shellradius,0*deg,360*deg,90*deg,90*deg);

  G4LogicalVolume* SourceWire_log = new G4LogicalVolume(SourceWire,PVC,"SourceWire_log",0,0,0);
  G4LogicalVolume* TungstenMarker_log = new G4LogicalVolume(TungstenMarker,Tungsten,"TungstenMarker_log",0,0,0);
  G4LogicalVolume* InnerShell_log = new G4LogicalVolume(InnerShell,NickelTitanium,"InnerShell_log",0,0,0);
  G4LogicalVolume* OuterShell_log = new G4LogicalVolume(OuterShell,NickelTitanium,"OuterShell_log",0,0,0);
  G4LogicalVolume* ShellEnd_log = new G4LogicalVolume(ShellEnd,NickelTitanium,"ShellEnd_log",0,0,0);
  G4LogicalVolume* GuideWire_log = new G4LogicalVolume(GuideWire,steel,"GuideWire_log",0,0,0);

 //
 // Visualization Attributes
 //

  //Source
  G4VisAttributes* s_vis = new G4VisAttributes();
  s_vis->SetColor(0,1,1);
  s_vis->SetVisibility(true);
  s_vis->SetForceSolid(false);
  SourceWire_log->SetVisAttributes(s_vis);

  G4VisAttributes* marker_vis = new G4VisAttributes();
  marker_vis->SetColor(1, 0, 1);
  marker_vis->SetVisibility(true);
  marker_vis->SetForceSolid(false);
  TungstenMarker_log->SetVisAttributes(marker_vis);


  G4VisAttributes* shield_vis = new G4VisAttributes();
  shield_vis->SetColor(0, 0, 1);
  shield_vis->SetVisibility(true);
  shield_vis->SetForceSolid(false);
  InnerShell_log->SetVisAttributes(shield_vis);
  OuterShell_log->SetVisAttributes(shield_vis);
  ShellEnd_log->SetVisAttributes(shield_vis);

  G4VisAttributes* guidewire_vis = new G4VisAttributes();
  guidewire_vis->SetColor(1,0, 0);
  guidewire_vis->SetVisibility(true);
  guidewire_vis->SetForceSolid(false);
  GuideWire_log->SetVisAttributes(guidewire_vis);
  
 //
 //Physical Placements 
 //

  new G4PVPlacement(0,G4ThreeVector(0,0,0),"SourceWire_Phys",SourceWire_log,mothervolume,false,0,false);
  new G4PVPlacement(0,G4ThreeVector(0,0,((sourcelength/2)+tungstenlength/2)),"TungstenMarker_phys",TungstenMarker_log,mothervolume,false,0,false);
  new G4PVPlacement(0,G4ThreeVector(0,0,-((sourcelength/2)+tungstenlength/2)),"TungstenMarker_phys",TungstenMarker_log,mothervolume,false,0,false);

  new G4PVPlacement(0,G4ThreeVector(0,0,20*mm),"InnerShell_phys",InnerShell_log,mothervolume,false,0,false);
  new G4PVPlacement(0,G4ThreeVector(0,0,((shelllength/2)-(sourcelength/2)-tungstenlength)),"OutersShell_phys",OuterShell_log,mothervolume,false,0,false);
  new G4PVPlacement(0,G4ThreeVector(0,0,-(sourcelength/2+tungstenlength)),"ShellEnd_phys",ShellEnd_log,mothervolume,false,0,false);

   if (guidewire == true) {
   new G4PVPlacement(0,G4ThreeVector(0, shellradius+0.1775*mm, 18*mm), "GuideWire_phys",GuideWire_log,mothervolume,false,0,false);
   }

 G4cout << "Guidant Galileo 20mm model succesfully constructed" << G4endl; 

}