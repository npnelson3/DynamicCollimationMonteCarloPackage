# Dynamic Collimation Monte Carlo package
By Nick Nelson <npnelson3@gmail.com>

For more informatin regarding the development of this package, please see: https://doi.org/10.1002/mp.14846
To cite this package, please cite through Zenodo: https://doi.org/10.5281/zenodo.4088273

# Features
The Dynamic Collimation Monte Carlo (DCMC) was developed to help facilitate the simulation of pencil beam scanning proton beams and collimation devices. The DCMC package utilized the the TOPAS Monte Carlo platform and consists of a generalized PBS source model and collimator component extensions. This guide assumes that users are fimiliar with the TOPAS Monte Carlo platform. For more info on TOPAS, please see [topasmc.org](topasmc.org).

The source model is particle generator extension for TOPAS and was specifically written to model a pencil beam scanning proton therapy beamline. It constists of a divergent point source placed at a user-specified SAD that samples a spot size function and back projects the sampled points to the SAD to deduce the angular distribution of the source. For most beamlines, magnets will deflect the beam in X and Y at different locations. This source model includes magnet-specific deflections given distances to the X and Y magnets from isocenter. If the magnet deflections are not invoked in the TOPAS parameter file, the divergent point source will rotate to produce the user-specified deflections.

The two collimation components included in the package are trimmers and apertures. The trimmers are modeled after the Dynamic Collimation System and linearly translate while rotating about the magnet planes to match the divergence of the scanned beam. The aperture is a Boolean geometry component with a user-specified cutout size and the native TOPAS commands can be used to alter the physical dimensions of the apertures.

# Installation
Please see the TOPAS documentation for installing extensions. The .cc and .hh files must be placed in your TOPAS extension folder and cmake must be called to compile the new, extended TOPAS application. For example, if you have all of the extension files in a folder called DCMC, and that's within a folder that houses any other extensions you may have (topas_extensions), go to your topas directory and extecute the following:
```
cmake -DTOPAS_EXTENSIONS_DIR=../topas_extensions/DCMC
make -j8
```

# TOPAS Usage
Once you have compiled your extended version of TOPAS, the following describes how you would use these extensions in the TOPAS parameter file.
## Source model (genericPBS)
This source was written to mimic a pencil beam scanning source for proton therapy. This source implements (optional) magnet-specific deflections downstream from a divergent point source. If the magnet-specific delfections are not wanted or required, the point source will rotate to provide the requested beam spot.

Below is a list of the arguments to genericPBS source requires:

  - **MeanEnergy** = Average energy of Guassianly distributed energy spectrum.
  - **EnergySigma** = Standard deviation of Gaussian energy spectrum.
  - **Xpos** = Position in X of central axis of beam at isocenter.
  - **Ypos** = Position in Y of central axis of beam at isocenter.
  - **SAD** = Source to axis distance. Distance the sampled point source is placed from isocenter. This parameter is something that would be optimized one a beamline-specific basis. This parameter can be manipulated to alter the angular distribution of the point source.
  - **XMagnetToIsoDistance** = Distance from isocenter to X bending magnet.
  - **YMagnetToIsoDistance** = Distance from isocenter to Y bending magnet.
  - **MagnetSpecificDeflections** = Boolean operator (True/False) on whether to invoke magnet-specific deflections. If true, deflections occur at magnet planes; if false, the point source rotates about the SAD to provide beam spot at (Xpos,Ypos).

**IMPORTANT**: When calling this source in the TOPAS parameter file, the parameters d:So/MySource/BeamEnergy and d:So/MySource/BeamEnergySpectrum must be defined for the source to run. It should be emphasized that the values entered here have no impact on the simulated energy spectrum, they just need to be defined given the way TOPAS is currently configured.

### Modifying the source model
The spot size at isocenter is the main component in this source model that would need to be changed when modeling a specific beamline. The genericPBS source samples from a Gaussian distributed fluence pattern at isocenter and back-projects the point source location to deduce the angular distribution required to match that spot size at isocenter. In it's current implementation, the source samples an asymmetric spot size (different sigma's in X and Y) specified by a polynomial of the form:

<p align="center">
<img src="https://render.githubusercontent.com/render/math?math=\sigma_{X or Y}(E) = a_0E^5 %2B a_1E^4 %2B a_2E^3 %2B a_3E^2 %2B a_4E %2B a_5">

These polynomials are defined and can be edited in Lines 60-66 of the genericPBS.cc file. They can also be modified to support lower or higher order polynomial functions.

## Trimmers (X1, X2, Y1, Y2)
The trimmer extensions are rectangular prisms that linearly translate and rotate depending upon the beam's position and whether the user wants a specific trimmer to intercept the beam or not. Below is a quick summary of the direction of motion for these extensions.
  - **X1**: Moves in the negative X-direction.
  - **X2**: Moves in the positive X-direction.
  - **Y1**: Moves in the negative Y-direction.
  - **Y2**: Moves in the positive Y-direction.
  
Each trimmer type can be called by setting s:Ge/MyTrimmer/Type = "X1". Below is a list of the arguments the trimmer extensions require after the type has been declared:
  - **Parent** = Parent geometry component.
  - **HalfThickness** = Half thickness of trimmer.
  - **HalfWidth** = Half width of trimmer.
  - **HalfLength** = Half length of trimmer.
  - **TrimmerOffset** = Linear distance between medial edge of trimmer and beam central axis.
  - **Material** = Material of trimmer blade. (see TOPAS documentation for default materials)
  - **TrimmerToAxisDistance** = Distance from center of trimmer to isocenter.
  - **DeflectionToIsoDistance** = Distance from bending magnet to isocenter. If MagnetSpecificDeflections are invoked in the source, this should be set to So/MySource/YMagnetToIsoDistance for the Y trimmers and So/MySource/XMagnetToIsoDistance for the X trimmers. If MagnetSpecificDeflections are not invoked, this should be set to So/MySource/SAD.
  - **Ypos or Xpos** = Beam position in X or Y. Ideally, this would be set to So/MySource/Xpos or So/MySource/Ypos for X and Y trimmers, respectively.
  - **EnableCollimation** = Boolean operator (True/False) on whether the trimmer should collimate the beam or not. If this is set to False, the trimmers will move to their default positions (centroid 6.5 cm from central axis).

## Apertures
Thea aperture extension is a Boolean geometry component defined by the subtraction of two rectangular prisms. The inputs to the aperture component are as follows:

  - **Parent** = Parent geometry component.
  - **HLZ** = Half Length in Z-direction.
  - **HLX** = Half Length in X-direction.
  - **HLY** = Half Length in Y-direction.
  - **TransX** = Translation in X-direction.
  - **TransY** = Translation in Y-direction.
  - **ApertureToIsoDistance** = Distance between centroid of aperture and isocenter.
  - **ApertureHalfWidthX** = Half width of aperture cutout in X-direction. This must always be greater than HLX.
  - **ApertureHalfWidthY** = Half width of aperture cutout in Y-direction. This must always be greater than HLY.
  - **Material** = Aperture component material. (see TOPAS documentation for default materials)
  
# Examples
Some example parameter files can be found in the [Examples](https://github.com/npnelson3/DynamicCollimationMonteCarloPackage/tree/master/Examples) folder.
