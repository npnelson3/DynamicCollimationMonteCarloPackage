// Component for Aperture
//
// ********************************************************************
// *                                                                  *
// *                                                                  *
// * This file was obtained from Topas MC Inc under the license       *
// * agreement set forth at http://www.topasmc.org/registration       *
// * Any use of this file constitutes full acceptance of              *
// * this TOPAS MC license agreement.                                 *
// *                                                                  *
// ********************************************************************
//

#include "Aperture.hh"

#include "G4BooleanSolid.hh"

#include "TsParameterManager.hh"

#include "G4Box.hh"


#include "G4SubtractionSolid.hh"

#include "G4LogicalVolume.hh"

#include "G4PVPlacement.hh"

#include "G4UIcommand.hh"

#include "G4SystemOfUnits.hh"

Aperture::Aperture(TsParameterManager* pM, TsExtensionManager* eM, TsMaterialManager* mM, TsGeometryManager* gM,
			 TsVGeometryComponent* parentComponent, G4VPhysicalVolume* parentVolume, G4String& name) :
TsVGeometryComponent(pM, eM, mM, gM, parentComponent, parentVolume, name)
{
}


Aperture::~Aperture()
{
}


G4VPhysicalVolume* Aperture::Construct()
{
	BeginConstruction();

	//Call dimensions of outerplate
	G4double HalfLengthZ = fPm->GetDoubleParameter(GetFullParmName("HLZ"), "Length");
	G4double HalfLengthX = fPm->GetDoubleParameter(GetFullParmName("HLX"), "Length");
	G4double HalfLengthY = fPm->GetDoubleParameter(GetFullParmName("HLY"), "Length");
	G4double TransX = fPm->GetDoubleParameter(GetFullParmName("TransX"),"Length");
	G4double AAD = fPm->GetDoubleParameter(GetFullParmName("ApertureToIsoDistance"),"Length");

	//Create outerplate using dimensions
	G4Box* outerplate = new G4Box(fName, HalfLengthX, HalfLengthY, HalfLengthZ);
	
	//Call dimensions of field size
	G4double CutoutX = fPm->GetDoubleParameter(GetFullParmName("ApertureHalfWidthX"), "Length");
	G4double CutoutY = fPm->GetDoubleParameter(GetFullParmName("ApertureHalfWidthY"), "Length");
	
	//Create cutout using dimensions
	G4Box* cutout = new G4Box(fName, CutoutX, CutoutY, HalfLengthZ);
	
	//Define rotation and shift of aperture
	G4RotationMatrix* AlignMatrix = new G4RotationMatrix;
	const G4ThreeVector Vector(0,0,0);
	
	//Create solid union of cone and cylinder and rotate and shift cylinder relative to cone
	G4VSolid* fEnvelopeSolid = new G4SubtractionSolid(fName, outerplate, cutout);//, AlignMatrix, Vector); //the first solid called in G4UnionSolid is the one that is placed relative to its mother ()TransX, RotX, ...). The second solid is placed relative to this first solid.
	
	//Create logical volume
	G4LogicalVolume* fEnvelopeLog = CreateLogicalVolume(fEnvelopeSolid);
	
	//Create physical volume and place it
	G4ThreeVector* pos = new G4ThreeVector(TransX,0.0,AAD);
	G4RotationMatrix* rot = new G4RotationMatrix();
	rot->rotateX(0.0); rot->rotateY(0.0); rot->rotateZ(0.0);
	fEnvelopePhys = CreatePhysicalVolume(fName,fEnvelopeLog,rot,pos,fParentVolume);
	
	InstantiateChildren(fEnvelopePhys);
	
	return fEnvelopePhys;
}
