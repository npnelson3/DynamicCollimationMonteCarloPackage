// Component for Y1
//
// ********************************************************************
// *  TOPAS Y1 Custom component of DCS                                *
// *                                                                  *
// *  Original Code written by: Nicholas P. Nelson on 9/9/2019        *
// *  Edits and TOPAS compatibility added by: Nicholas P. Nelson      *
// *  University of Wisconsin Medical Radiation Research Center       *	 	                    
// *                                                                  *
// ********************************************************************
//
#include "Y1.hh"
#include "TsParameterManager.hh"
#include "G4Box.hh"
#include "G4SystemOfUnits.hh"
#define PI 3.14159265

Y1::Y1(TsParameterManager* pM, TsExtensionManager* eM, TsMaterialManager* mM, TsGeometryManager* gM,
			 TsVGeometryComponent* parentComponent, G4VPhysicalVolume* parentVolume, G4String& name) :
TsVGeometryComponent(pM, eM, mM, gM, parentComponent, parentVolume, name)
{
}

Y1::~Y1()
{
}

G4VPhysicalVolume* Y1::Construct()
{
	BeginConstruction();
	// Load up dimensions of trimmers	
	G4double TrimmerHalfThickness = fPm->GetDoubleParameter(GetFullParmName("HalfThickness"),"Length");
	G4double TrimmerHalfWidth = fPm->GetDoubleParameter(GetFullParmName("HalfWidth"),"Length");
	G4double TrimmerOffset = fPm->GetDoubleParameter(GetFullParmName("TrimmerOffset"),"Length");
	G4double HalfLengthY = fPm->GetDoubleParameter(GetFullParmName("HalfLength"),"Length");
	G4double DefaultPositive = 65*mm;
	G4double DefaultRotation = 1.8*deg;
	G4String Material = fPm->GetStringParameter(GetFullParmName("Material"));
	// Get simulation-specific parameters
	G4double YAD = fPm->GetDoubleParameter(GetFullParmName("TrimmerToAxisDistance"),"Length");
	G4double MADy = fPm->GetDoubleParameter(GetFullParmName("DeflectionToIsoDistance"),"Length");
	G4double Ypos = fPm->GetDoubleParameter(GetFullParmName("Ypos"),"Length");

	// Handle collimation, first check to see
	G4bool CollimationTrigger = fPm->GetBooleanParameter(GetFullParmName("EnableCollimation"));

	// Create Y1 trimmer
	// Home positions when collimation is disabled: Xcent = +/-6.5cm
	G4Box* envelopeSolid = new G4Box(fName, HalfLengthY, TrimmerHalfWidth, TrimmerHalfThickness);
	fEnvelopeLog = CreateLogicalVolume(envelopeSolid);//,Material,fName);
	if (CollimationTrigger == true && Ypos <= 0)
	{
	// Compute theta, medial edge, and then trimmer centroid location
	G4cout << "Medial edge for Y1 at Isoplane = " << (Ypos - TrimmerOffset) << "mm" << G4endl;
	G4double YMedial = (Ypos - TrimmerOffset)* (1 - (YAD/MADy));
	G4cout << "Medial edge for Y1 at Trimmer Plane = " << YMedial << "mm" << G4endl;
	G4double Theta = atan(YMedial/(MADy)); // Value in radians
	G4cout << "Computed theta for Y1 = " << Theta * 180 / PI << "deg" << G4endl;
	G4double YTrimmerCenter = YMedial - (TrimmerHalfWidth/cos(Theta));
	//G4cout << "Centroid location for Y1 = " << YTrimmerCenter * 0.1 << "cm" << G4endl;
	G4ThreeVector* pos = new G4ThreeVector(0.0,YTrimmerCenter,YAD);
	G4RotationMatrix* rot = new G4RotationMatrix();
	rot->rotateX(-1.0*Theta); rot->rotateY(0.0); rot->rotateZ(0.0);
	fEnvelopePhys = CreatePhysicalVolume(fName,fEnvelopeLog,rot,pos,fParentVolume);
	}
	else
	{
	G4ThreeVector* pos = new G4ThreeVector(0.0,-1.0*DefaultPositive,YAD);
	G4RotationMatrix* rot = new G4RotationMatrix();
	rot->rotateX(1.0*DefaultRotation); rot->rotateY(0.0); rot->rotateZ(0.0);
	fEnvelopePhys = CreatePhysicalVolume(fName,fEnvelopeLog,rot,pos,fParentVolume);
	}

	InstantiateChildren(fEnvelopePhys);
	
	return fEnvelopePhys;
	

}
