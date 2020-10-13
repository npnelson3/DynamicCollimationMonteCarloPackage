//
// ********************************************************************
// *  generic PBS scanning source extension for TOPAS                 *
// *                                                                  *
// *  Code contributors: N. Nelson and B. Smith                       *
// *                                                                  *
// *  Developed at:                                                   *
// *  University of Wisconsin Medical Radiation Research Center       *	 	                    
// *                                                                  *
// ********************************************************************
//

#ifndef genericPBS_hh
#define genericPBS_hh

#include "TsVGenerator.hh"


class genericPBS : public TsVGenerator
{
public:
	genericPBS(TsParameterManager* pM, TsGeometryManager* gM, TsGeneratorManager* pgM, G4String sourceName);
	~genericPBS();

	void ResolveParameters();
// added by nick
	//G4String GetFullParmName(const char*);
	//void UpdateForSpecificParameterChange();
	
	void GeneratePrimaries(G4Event* );
	
private:
	G4double fMeanEnergy;
	G4double fEnergySigma;
	G4double fNominalEnergy;
	G4double fXpos;
	G4double fYpos;
	G4double fSAD;
	G4double fMADx;
	G4double fMADy;
	G4bool fMagPresent;

};
#endif
