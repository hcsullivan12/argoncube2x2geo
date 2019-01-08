#include <iostream>
#include <fstream>

float    rInc       = 1;
float    thetaInc   = 45;
float    diskRadius = 14.5 - 2;
unsigned n          = 100;

void makeLightSteering()
{
  std::string filename = "../config/LightSourceSteering.txt";
  std::ofstream file(filename.c_str());
  if (!file.is_open()) return;

  // 0 0
  for (int i = 1; i <= n; i++)
  {
    file << "0 0\n";
  }

  float r(1);
  // 90 degrees
  while (r < diskRadius)
  {
    for (int i = 1; i <= n; i++)
    {
      file << r << " " << "90\n";  
    }
    r = r + rInc;
  }
  r = 1;
  // 270 degrees
  while (r < diskRadius)
  {
    for (int i = 1; i <= n; i++)
    {
      file << r << " " << "270\n";  
    }
  r = r + rInc;
  }

  file.close();
}


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

