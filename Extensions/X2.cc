// Component for X2
//
// ********************************************************************
// *  TOPAS X2 Custom component of DCS                                *
// *                                                                  *
// *  Original Code written by: Nicholas P. Nelson on 9/9/2019        *
// *  Edits and TOPAS compatibility added by: Nicholas P. Nelson      *
// *  University of Wisconsin Medical Radiation Research Center       *	 	                    
// *                                                                  *
// ********************************************************************
//
#include "X2.hh"
#include "TsParameterManager.hh"
#include "G4Box.hh"
#include "G4SystemOfUnits.hh"
#define PI 3.14159265

X2::X2(TsParameterManager* pM, TsExtensionManager* eM, TsMaterialManager* mM, TsGeometryManager* gM,
			 TsVGeometryComponent* parentComponent, G4VPhysicalVolume* parentVolume, G4String& name) :
TsVGeometryComponent(pM, eM, mM, gM, parentComponent, parentVolume, name)
{
}

X2::~X2()
{
}

G4VPhysicalVolume* X2::Construct()
{
	BeginConstruction();
	// Load up dimensions of trimmers	
	G4double TrimmerHalfThickness = fPm->GetDoubleParameter(GetFullParmName("HalfThickness"),"Length");
	G4double TrimmerHalfWidth = fPm->GetDoubleParameter(GetFullParmName("HalfWidth"),"Length");
	G4double TrimmerOffset = fPm->GetDoubleParameter(GetFullParmName("TrimmerOffset"),"Length");
	G4double HalfLengthX = fPm->GetDoubleParameter(GetFullParmName("HalfLength"),"Length");
	G4double DefaultPositive = 65*mm;
	G4double DefaultRotation = 1.8*deg;
	G4String Material = fPm->GetStringParameter(GetFullParmName("Material"));
	// Get simulation-specific parameters
	G4double XAD = fPm->GetDoubleParameter(GetFullParmName("TrimmerToAxisDistance"),"Length");
	G4double MADx = fPm->GetDoubleParameter(GetFullParmName("DeflectionToIsoDistance"),"Length");
	G4double Xpos = fPm->GetDoubleParameter(GetFullParmName("Xpos"),"Length");

	// Handle collimation, first check to see
	G4bool CollimationTrigger = fPm->GetBooleanParameter(GetFullParmName("EnableCollimation"));

	// Create X2 trimmer
	// Home positions when collimation is disabled: Xcent = +/-6.5cm
	G4Box* envelopeSolid = new G4Box(fName, TrimmerHalfWidth, HalfLengthX, TrimmerHalfThickness);
	fEnvelopeLog = CreateLogicalVolume(envelopeSolid);//,Material,fName);
	if (CollimationTrigger == true && Xpos >= 0)
	{
	// Compute theta, medial edge, and then trimmer centroid location
        G4cout << "Medial edge for X2 at Isoplane = " << (Xpos + TrimmerOffset) << "mm" << G4endl;
	G4double XMedial = (Xpos + TrimmerOffset)* (1 - (XAD/MADx));
	G4cout << "Medial edge for X2 at Trimmer Plane = " << XMedial << "mm" << G4endl;
	G4double Theta = atan(XMedial/(MADx)); // Value in radians
	G4cout << "Computed theta for X2 = " << Theta * 180 / PI << "deg" << G4endl;
	G4double XTrimmerCenter = XMedial + (TrimmerHalfWidth/cos(Theta));
	//G4cout << "Centroid location for X2 = " << XTrimmerCenter * 0.1 << "cm" << G4endl;
	G4ThreeVector* pos = new G4ThreeVector(XTrimmerCenter,0.0,XAD);
	G4RotationMatrix* rot = new G4RotationMatrix();
	rot->rotateX(0.0); rot->rotateY(Theta); rot->rotateZ(0.0);
	fEnvelopePhys = CreatePhysicalVolume(fName,fEnvelopeLog,rot,pos,fParentVolume);
	}
	else
	{
	G4ThreeVector* pos = new G4ThreeVector(DefaultPositive,0.0,XAD);
	G4RotationMatrix* rot = new G4RotationMatrix();
	rot->rotateX(0.0); rot->rotateY(DefaultRotation); rot->rotateZ(0.0);
	fEnvelopePhys = CreatePhysicalVolume(fName,fEnvelopeLog,rot,pos,fParentVolume);
	}

	InstantiateChildren(fEnvelopePhys);
	
	return fEnvelopePhys;
	

}
