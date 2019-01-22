# Majorana
This project holds the Geant4 simulation code for the SiPM wheel.

## Physics
An isotropic TPB emission generator simulates a slightly diffuse point-like source incident on an acrylic disk. The photons undergo total internal reflection until they are either detected by a surrounding SiPM or refracted. 

## Configuration
The simulation uses the LightSourceSteering.txt file for placement of the emission generator. 
There are two simulation modes which require different formats for the LightSourceSteering.txt.
### Point
This mode place the center of the emission generator at the points listed in the LightSourceSteeringFile, smearing the position using the sourcePosSigma parameter in config/Configuration.json, e.g.
```
x y n
0 0 5000
0 9 4500
-4 0 6000
.
.
.
```
or 
```
r theta n
0 0 5000
9 90 4500
4 180 6000
```
where n is the number of photons to fire for the event. 

### Voxel 
This mode requires an input voxelization.txt file listing the voxelIDs and positions, e.g. assuming a 1 cm2 voxel size
```
voxelID x y
1 0 0
2 1 0
3 2 0
.
.
.
```
Then the LightSourceSteering.txt file might read
```
voxelID n
1 5000
1 4500
3 6000
.
.
.
```
## Visualization
Supply the following command at runtime
```
./simulate <path_to_config.json> --vis ON
```
