# argoncube2x2geo
This project holds the detector construction code for ArgonCube2x2.

## Dependencies
* RapidJSON (Configuration)
* Geant4 (Building)
* ROOT (Visualization)

## Running and visualization
Edit the `envsetup.sh` script to point to your install directories. Then
```
mkdir build
cd build
cmake ..
make
```
To build the detector, run
```
./construct ../Configuration.json <GDML_OUTPUT_PATH>
```
or you can run the rebuild.sh script in the top directory. This will recompile the code and open the viewer. 
