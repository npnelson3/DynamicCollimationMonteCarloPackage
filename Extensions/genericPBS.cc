// Particle Generator for genericPBS
//
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


#include "genericPBS.hh"
#include "Randomize.hh"
#include "TsParameterManager.hh"


genericPBS::genericPBS(TsParameterManager* pM, TsGeometryManager* gM, TsGeneratorManager* pgM, G4String sourceName) :
TsVGenerator(pM, gM, pgM, sourceName), fMeanEnergy(1), fEnergySigma(1), fXpos(1), fYpos(1), fSAD(1)
{
	ResolveParameters();
}


genericPBS::~genericPBS()
{
}

// Load parameters from parameter file
void genericPBS::ResolveParameters() {
	TsVGenerator::ResolveParameters();

  	fMeanEnergy = fPm->GetDoubleParameter(GetFullParmName("MeanEnergy"), "Energy");
	fEnergySigma = fPm->GetDoubleParameter(GetFullParmName("EnergySigma"), "Energy");
	fXpos = fPm->GetDoubleParameter(GetFullParmName("Xpos"), "Length");
	fYpos = fPm->GetDoubleParameter(GetFullParmName("Ypos"), "Length");
	fSAD = fPm->GetDoubleParameter(GetFullParmName("SAD"), "Length");
	fMADx = fPm->GetDoubleParameter(GetFullParmName("XMagnetToIsoDistance"),"Length");
	fMADy = fPm->GetDoubleParameter(GetFullParmName("YMagnetToIsoDistance"),"Length");
	fMagPresent = fPm->GetBooleanParameter(GetFullParmName("MagnetSpecificDeflections"));
	//G4cout << GetFullParmName("SAD") << G4endl;

}

// Particle Generator
void genericPBS::GeneratePrimaries(G4Event* anEvent)
{
	if (CurrentSourceHasGeneratedEnough())
		return;
	TsPrimaryParticle p;

    // Determine Particle Starting Energy
    G4double Energy = G4RandGauss::shoot(fMeanEnergy,fEnergySigma);

    // incorporate beamlet divergence
    // Generic spot size, coefficients of polynomial fit
    G4double aSx[]={0.000000000075,-0.000000069,0.0000145,-0.00243,0.1092,7.5};
    G4double sigmaSx = aSx[0]*pow(Energy,5)+aSx[1]*pow(Energy,4)+aSx[2]*pow(Energy,3)+aSx[3]*pow(Energy,2)+aSx[4]*pow(Energy,1)+aSx[5]; //in [mm]
    
    // incorporate beamlet divergence
    // Generic spot size
    G4double aSy[]={-0.000000000028,0.000000007945,0.00000188,-0.00075,0.035,7.266};
    G4double sigmaSy = aSy[0]*pow(Energy,5)+aSy[1]*pow(Energy,4)+aSy[2]*pow(Energy,3)+aSy[3]*pow(Energy,2)+aSy[4]*pow(Energy,1)+aSy[5]; //in [mm]
        
    G4double x = G4RandGauss::shoot(0,sigmaSx); // Distance from normal at iso
    G4double y = G4RandGauss::shoot(0,sigmaSy); // Distance from normal at iso
    //G4cout << "Sampled coordiates (x,y): (" << x << "," << y << ")" << G4endl;     
    G4double R = std::sqrt(pow(fSAD,2)+pow(x,2)+pow(y,2));
    
    G4double r_samp[3]; // proton incident starting direction unit vector, r_samp
    r_samp[0]=x/R;
    r_samp[1]=y/R;
    r_samp[2]=-1*fSAD/R;

//G4cout << "The (u,v,w) sampled direction is: (" << r_samp[0] << ", " << r_samp[1] << ", " << r_samp[2] << ")" << G4endl;

if (fMagPresent == true)
{
    G4double p_samp[3];
    p_samp[0] = 0;
    p_samp[1] = 0;
    p_samp[2] = fSAD;

    // Compute Distance to MADy and update position
    G4double S = (fMADy - fSAD)/r_samp[2]; 
    G4double p_1[3];
    p_1[0] = S*r_samp[0];
    p_1[1] = S*r_samp[1];
    p_1[2] = fMADy;

    // rotation around the x-axis (y translation) cosine and sine
    G4double RY = std::sqrt(pow(fMADy,2)+pow(fYpos,2));
    G4double rxcos = (fMADy)/RY;
    G4double rxsin = fYpos/RY;

    G4double r_yx[3];
    r_yx[0] = r_samp[0];
    r_yx[1] = r_samp[1]*rxcos - r_samp[2]*rxsin;
    r_yx[2] = r_samp[1]*rxsin + r_samp[2]*rxcos; 

    // Comnpute Distance to MADx and update position
    G4double Lambda = (fMADx-fMADy)/(r_yx[2]);
    G4double p_2[3];

    // Position Vector at X plane
    p_2[0] = (S + Lambda)*r_samp[0];
    p_2[1] = S*r_samp[1] + Lambda*(r_samp[1]*rxcos - r_samp[2]*rxsin);
    p_2[2] = fMADx;

    G4double RX = std::sqrt(pow(fMADx,2)+pow(fXpos,2));
    G4double rysin = -1*(fXpos)/RX;
    G4double rycos = fMADx/RX;

    G4double r_emit[3];
    r_emit[0] = r_yx[0]*rycos + r_yx[2]*rysin;
    r_emit[1] = r_yx[1];
    r_emit[2] = (r_yx[0]*rysin + r_yx[2]*rycos);
    
    // renormalize
    r_emit[0]=r_emit[0]/std::sqrt(pow(r_emit[0],2)+pow(r_emit[1],2)+pow(r_emit[2],2));
    r_emit[1]=r_emit[1]/std::sqrt(pow(r_emit[0],2)+pow(r_emit[1],2)+pow(r_emit[2],2));
    r_emit[2]=r_emit[2]/std::sqrt(pow(r_emit[0],2)+pow(r_emit[1],2)+pow(r_emit[2],2));

	    p.posX = p_2[0];
	    p.posY = p_2[1];
	    p.posZ = fMADx;

	    p.dCos1 = r_emit[0];
	    p.dCos2 = r_emit[1];
	    p.dCos3 = r_emit[2];
}
else
{
    // rotation around the x-axis (y translation) cosine and sine
    G4double RY = std::sqrt(pow(fSAD,2)+pow(fYpos,2));
    G4double rxcos = fSAD/RY;
    G4double rxsin = fYpos/RY;

    G4double n_new[3];
    n_new[0] = r_samp[0];
    n_new[1] = r_samp[1]*rxcos - r_samp[2]*rxsin;
    n_new[2] = r_samp[1]*rxsin + r_samp[2]*rxcos;
    
    // rotation around the y-axis (x translation) cosine and sine
    G4double RX = std::sqrt(pow(fSAD,2)+pow(fXpos,2));
    G4double rycos = fSAD/RX;
    G4double rysin = -1*fXpos/RX; // flip sign since angle around x is negated

    n_new[0] = n_new[0]*rycos + n_new[2]*rysin;
    n_new[1] = n_new[1];
    n_new[2] = (n_new[0]*rysin + n_new[2]*rycos);
    
    // renormalize
    n_new[0]=n_new[0]/std::sqrt(pow(n_new[0],2)+pow(n_new[1],2)+pow(n_new[2],2));
    n_new[1]=n_new[1]/std::sqrt(pow(n_new[0],2)+pow(n_new[1],2)+pow(n_new[2],2));
    n_new[2]=n_new[2]/std::sqrt(pow(n_new[0],2)+pow(n_new[1],2)+pow(n_new[2],2));

	    p.posX = 0.;
	    p.posY = 0.;
	    p.posZ = fSAD;

	    p.dCos1 = n_new[0];
	    p.dCos2 = n_new[1];
	    p.dCos3 = n_new[2];

}
    	p.kEnergy = Energy;
	p.particleDefinition = fParticleDefinition;
	p.isOpticalPhoton = 0;
// works with 1 (below)
	p.isNewHistory = 1;
	p.weight = 1;
		
	p.isOpticalPhoton = fIsOpticalPhoton;
	p.isGenericIon = fIsGenericIon;
	p.ionCharge = fIonCharge;
//       Optional printouts for energy and SAD for debugging
//G4cout << "The Energy is: " << Energy << " MeV " << G4endl;
//G4cout << "The SAD is: " << fSAD << " mm " << G4endl;
//        G4cout << G4endl;
	
	GenerateOnePrimary(anEvent, p);
	
	AddPrimariesToEvent(anEvent);
}
