#include <iostream>
#include <fstream>

unsigned nVoxels = 100;

void makeLightSteering()
{
  std::string filename = "../config/LightSourceSteering.txt";
  std::ofstream file(filename.c_str());
  if (!file.is_open()) return;

  /*// 180 degrees
  for (int r = 0; r <= 14; r++)
  {
    for (int n = 1; n <= 10; n++)
    {
      file << r << " " << "180\n";  
    }
  }
  // 0 degrees
  for (int r = 1; r <= 14; r++)
  {
    for (int n = 1; n <= 10; n++)
    {
      file << r << " " << "0\n";  
    }
  }*/
  for (int i = 1; i <= 1000; i++)
  {
    file << "0 0\n";
  }

  file.close();
}
