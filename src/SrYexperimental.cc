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
// Class that defines the Novoste Beta Cath 3.5 French model 
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
#include "SrYexperimental.hh"
#include "SourceModel.hh"
#include <sstream>

Sryexperimental::Sryexperimental(G4int numsources, G4bool Guidewire)
{
 numberofsources = numsources;
 guidewire = Guidewire;
 SetupDimensions();
}
void Sryexperimental::SetupDimensions()
{
 sourceradius = 0.16*mm;
 sourcetrainshift = 0.05*mm;
 shellradius = 0.19*mm;
 jacketradius = 0.2225*mm;
 catheterouterradius = 0.45*mm;
 guidewireradius = 0.1775*mm;

 catheterthickness = 0.0875*mm;
 

 sourcelength = 2.44*mm;
 shelllength = 2.5*mm;
 sourcetrainlength = numberofsources*shelllength;
 //jacketlength runs the length of the sources + an extra 2.5mm on each size for radiopaque marker
 jacketlength = sourcetrainlength+(5*mm);
 catheterlength = sourcetrainlength*3;
}

void Sryexperimental::CleanSource()
{
	G4cout << "No Source Cleaning Implemented for this Model" << G4endl;
}

void Sryexperimental::CreateSource(G4VPhysicalVolume* mothervolume)
{

	std::ostringstream oss;

 //
 //Elements and Materials Definitions
 //
  G4NistManager* man = G4NistManager::Instance();
  G4Element*  Fe = man->FindOrBuildElement("Fe");
  G4Element*  Cr =man->FindOrBuildElement("Cr");
  G4Element*  Ni = man->FindOrBuildElement("Ni");
  G4Element*  Mn = man->FindOrBuildElement("Mn");
  G4Element*  Si = man->FindOrBuildElement("Si");
  G4Element*  C  = man->FindOrBuildElement("C");
  G4Element*  O  = man->FindOrBuildElement("O");
  G4Element*  Al  = man->FindOrBuildElement("Al");
  
  //Should consider looking up specific materials for their aluminum
  G4Material* Aluminum = man->FindOrBuildMaterial("G4_Al");
  G4Material* Platinum = man->FindOrBuildMaterial("G4_Pt");
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

  //Sintered Aluminum (Appears granular microscopically, variant of aluminum, up to 20% Aluminum Oxide)
  //Below I took 90% pure aluminum, and 10% Aluminum Oxide to generate the densities and compositions
  //As taken from the Geant materials database
  G4Material* SinteredAluminum = new G4Material("SinteredAluminum",2.826*CLHEP::g/CLHEP::cm3,2);
  SinteredAluminum->AddElement(Al , 0.9529251);
  SinteredAluminum->AddElement(O , 0.0470749);

  //Assuming 30 degrees celcius as internal body temp
  //G4double temperature = 303.15*kelvin;
  //Pressure below 760 mmHg (atmospheric) + 120 mmHg (typical pressure in blood vessel)
  //G4double p_Body = 117323*(CLHEP::pascal);  

 //
	//Shapes and Logical Volumes
 //
   /*
	 G4Tubs* ShieldInner = new G4Tubs("ShieldInner",0*mm,sourceradius,(shelllength-sourcelength)/2,0*deg,360*deg);
	 G4Tubs* ShieldOuter = new G4Tubs("ShieldOuter",sourceradius,shellradius,(numberofsources/2)*2.5*mm,0*deg,360*deg);
	 G4Tubs* Jacket = new G4Tubs("Jacket",shellradius,jacketradius,jacketlength/2,0*deg,360*deg);
	 G4Tubs* Source = new G4Tubs("Source",0*mm,sourceradius,(sourcelength/2),0*deg,360*deg);
	 G4Tubs* InnerCatheter = new G4Tubs("InnerCatheter",jacketradius+sourcetrainshift,jacketradius+sourcetrainshift+catheterthickness,catheterlength/2,0*deg,360*deg);
	 //The algebra below seems a little messy at first glance
	 //The inner radius is shifted by, twice the offset because the source is shifted up and the cathere is shifted down
	 //Shifted up by the thickness + jacketradius, so the inside just touches the inner catheter
	 G4Tubs* GuideCatheter = new G4Tubs("GuideCatheter",catheterthickness+(2*sourcetrainshift)+jacketradius, catheterouterradius+sourcetrainshift,catheterlength/2,0*deg,360*deg);
	 /*G4Sphere(const G4String& pName,
             G4double  pRmin,
             G4double  pRmax,
             G4double  pSPhi,
             G4double  pDPhi,
             G4double  pSTheta,
             G4double  pDTheta )*/
  G4Sphere* CatheterEnd = new G4Sphere("CatheterEnd",(0.55-0.0875)*mm, 0.55*mm,0*deg,360*deg,90*deg,90*deg);
  //Innermost catheter that surrounds guidewire. Made to same thickness as other catheters.
  G4Tubs* InnerCatheter = new G4Tubs("InnerCatheter",guidewireradius,guidewireradius+(catheterthickness/2),catheterlength/2,0*deg,360*deg);
  //Middle catheter to allow 0.1 square mm of water
  G4Tubs* MiddleCatheter = new G4Tubs("MiddleCatheter",0.2669*mm,0.3544*mm,catheterlength/2,0*deg,360*deg);
  //Outermost Catheter, constrained to same size as Novoste Beta Cath system
  G4Tubs* GuideCatheter = new G4Tubs("GuideCatheter",(0.55-0.0875)*mm, 0.55*mm,catheterlength/2,0*deg,360*deg);
  //Guidewire in centre
	G4Tubs* GuideWire = new G4Tubs("GuideWire",0*mm,guidewireradius, catheterlength/2, 0*deg, 360*deg);

  G4Tubs* ShieldBetween = new G4Tubs("ShieldBetween",0.3640*mm,(0.55-0.0875)*mm,(shelllength-sourcelength)/2,0*deg,360*deg);
  G4Tubs* ShieldOuter = new G4Tubs("ShieldOuter",(0.55-0.0875-0.03)*mm,(0.55-0.0875)*mm,(numberofsources/2)*2.5*mm,0*deg,360*deg);
  G4Tubs* ShieldInner = new G4Tubs("ShieldInner",0.3640*mm*mm,0.3940*mm*mm,(numberofsources/2)*2.5*mm,0*deg,360*deg);

  G4Tubs* Source = new G4Tubs("Source",0.3940*mm,0.4325*mm,(sourcelength/2),0*deg,360*deg);


	 //Logical Volumes
	 G4LogicalVolume* ShieldInner_log = new G4LogicalVolume(ShieldInner,steel,"ShieldInner_log",0,0,0);
	 G4LogicalVolume* ShieldOuter_log = new G4LogicalVolume(ShieldOuter,steel,"ShieldOuter_log",0,0,0);
   G4LogicalVolume* ShieldBetween_log = new G4LogicalVolume(ShieldBetween,steel,"ShieldBetween_log",0,0,0);
	 //G4LogicalVolume* Jacket_log = new G4LogicalVolume(Jacket,steel,"Jacket_log",0,0,0);
	 G4LogicalVolume* Source_log = new G4LogicalVolume(Source,SinteredAluminum,"Source_log",0,0,0);
	 G4LogicalVolume* InnerCatheter_log = new G4LogicalVolume(InnerCatheter,PVC,"InnerCatheter_log",0,0,0);
   G4LogicalVolume* MiddleCatheter_log = new G4LogicalVolume(MiddleCatheter,PVC,"MiddleCatheter_log",0,0,0);
	 G4LogicalVolume* GuideCatheter_log = new G4LogicalVolume(GuideCatheter,PVC,"GuideCatheter_log",0,0,0);
	 G4LogicalVolume* CatheterEnd_log = new G4LogicalVolume(CatheterEnd,PVC,"CatheterEnd_log",0,0,0);
	 G4LogicalVolume* GuideWire_log = new G4LogicalVolume(GuideWire,steel,"GuideWire_log",0,0,0);
	 

 //
 // Visualization Attributes
 //

  //Source
  G4VisAttributes* s_vis = new G4VisAttributes();
  s_vis->SetColor(1,1,0);
  //s_vis->SetColor((153/255), 0, (76/255));
  s_vis->SetVisibility(true);
  s_vis->SetForceSolid(false);
  Source_log->SetVisAttributes(s_vis);

  G4VisAttributes* shield_vis = new G4VisAttributes();
  shield_vis->SetColor(0, 0, 1);
  shield_vis->SetVisibility(true);
  shield_vis->SetForceSolid(false);
  ShieldInner_log->SetVisAttributes(shield_vis);
  ShieldOuter_log->SetVisAttributes(shield_vis);


  G4VisAttributes* jacket_vis = new G4VisAttributes();
  jacket_vis->SetColor(1, 0, 0);
  jacket_vis->SetVisibility(true);
  jacket_vis->SetForceSolid(false);
  ShieldBetween_log->SetVisAttributes(jacket_vis);

  G4VisAttributes* innercath_vis = new G4VisAttributes();
  innercath_vis->SetColor(1,1, 0);
  innercath_vis->SetVisibility(true);
  innercath_vis->SetForceSolid(false);
  InnerCatheter_log->SetVisAttributes(innercath_vis);
  MiddleCatheter_log->SetVisAttributes(innercath_vis);
   GuideCatheter_log->SetVisAttributes(innercath_vis);

  G4VisAttributes* guidecath_vis = new G4VisAttributes();
  guidecath_vis->SetColor(1,1, 1);
  guidecath_vis->SetVisibility(false);
  guidecath_vis->SetForceSolid(false);
  CatheterEnd_log->SetVisAttributes(guidecath_vis);


 //
 //Physical Placements 
 //

	 //Placement of Aluminum cores and steel inner shells
  
	 for(int i = 0;i<(numberofsources/2);i++)
	 {
	 	
      G4double Shift = shelllength/2 + i*shelllength;
      oss << "Source_phys" << i;
      G4String name  = oss.str();
      G4cout << name << G4endl; 
      oss.str(std::string());
      oss << "Source_phys-" << i;
      G4String negname  = oss.str();
      G4cout << negname << G4endl; 
      oss.str(std::string());
	  new G4PVPlacement(0,G4ThreeVector(0,0,Shift),name,Source_log,mothervolume,true,i,false);
	  new G4PVPlacement(0,G4ThreeVector(0,0,-Shift),negname,Source_log,mothervolume,true,i+1,false);

	  G4double ShieldShift = i*shelllength;
	  //For first iteration place centre and end shields, for last iteration place symmetric shields between each source
	  //Shield refers to stainless steel piece between the individual sources
	  if (ShieldShift != 0 ) 
	  {
	   new G4PVPlacement(0,G4ThreeVector(0,0,ShieldShift),"ShieldBetween_phys"+i,ShieldBetween_log,mothervolume,false,0,false);
	   new G4PVPlacement(0,G4ThreeVector(0,0,-ShieldShift),"ShieldBetween_phys-"+i,ShieldBetween_log,mothervolume,false,0,false);
	  }
	  else
	  {
	  	new G4PVPlacement(0,G4ThreeVector(0,0,0),"ShieldBetween_phys"+i,ShieldBetween_log,mothervolume,false,0,false);
	  	new G4PVPlacement(0,G4ThreeVector(0,0,(numberofsources/2)*shelllength),"ShieldBetween_phys"+i,ShieldBetween_log,mothervolume,false,0,false);
	  	new G4PVPlacement(0,G4ThreeVector(0,0,-(numberofsources/2)*shelllength),"ShieldBetween_phys"+i,ShieldBetween_log,mothervolume,false,0,false);
	  }
	 }
  // Placement of outer shield and steel jacket that run along the source train
	 //new G4PVPlacement(0,G4ThreeVector(0,sourcetrainshift,0),"ShieldOuter_phys",ShieldOuter_log,mothervolume,false,0,false);
	 //new G4PVPlacement(0,G4ThreeVector(0,sourcetrainshift,0),"Jacket_phys",Jacket_log,mothervolume,false,0,false);
	 // Placement of inner and outer catheter, see D.E Roa dosimetry paper for design specifications of catheter
	 //new G4PVPlacement(0,G4ThreeVector(0,0,(catheterlength-jacketlength)/2),"InnerCatheter_phys",InnerCatheter_log,mothervolume,false,0,false);
	 //new G4PVPlacement(0,G4ThreeVector(0,-sourcetrainshift,(catheterlength-jacketlength)/2),"GuideCatheter_phys",GuideCatheter_log,mothervolume,false,0,false);
	 //new G4PVPlacement(0,G4ThreeVector(0,-sourcetrainshift,-jacketlength/2),"CatheterEnd_phys",CatheterEnd_log,mothervolume,false,0,false);

   
	 new G4PVPlacement(0,G4ThreeVector(0, 0, ((catheterlength-jacketlength)/2)), "GuideWire_phys",GuideWire_log,mothervolume,false,0,false);
   new G4PVPlacement(0,G4ThreeVector(0,0,(catheterlength-jacketlength)/2),"InnerCatheter_phys",InnerCatheter_log,mothervolume,false,0,false);
   new G4PVPlacement(0,G4ThreeVector(0,0,(catheterlength-jacketlength)/2),"MiddleCatheter_phys",MiddleCatheter_log,mothervolume,false,0,false);
   new G4PVPlacement(0,G4ThreeVector(0,0,(catheterlength-jacketlength)/2),"GuideCatheter_phys",GuideCatheter_log,mothervolume,false,0,false);
   new G4PVPlacement(0,G4ThreeVector(0,0,0),"ShieldOuter_phys",ShieldOuter_log,mothervolume,false,0,false);
   new G4PVPlacement(0,G4ThreeVector(0,0,0),"ShieldInner_phys",ShieldInner_log,mothervolume,false,0,false);
   new G4PVPlacement(0,G4ThreeVector(0,0,-jacketlength/2),"CatheterEnd_phys",CatheterEnd_log,mothervolume,false,0,false);
   

 G4cout << "Experimental Strontium-Yttrium Device constructed, " << sourcetrainlength << "mm source train in use." << G4endl; 

}