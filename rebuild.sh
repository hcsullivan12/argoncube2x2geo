topDir=$( cd $(dirname ${BASH_SOURCE[0]}) && pwd )
filename="detector.gdml"
filePath=$topDir/$filename

rm $filePath
cd build
make

./construct $filePath && (cd ../ && source drawDetector.sh)
