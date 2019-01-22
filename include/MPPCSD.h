#ifndef MPPCSD_HH
#define MPPCSD_HH

#include "G4VSensitiveDetector.hh"

class G4Step;
class G4HCofThisEvent;

namespace majorana
{

class MPPCSD : public G4VSensitiveDetector
{

  public:

    MPPCSD(G4String name);
    virtual ~MPPCSD();
 
    virtual void Initialize(G4HCofThisEvent* );
    virtual G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* );

    virtual void EndOfEvent(G4HCofThisEvent* );
    virtual void clear();
    void DrawAll();
    void PrintAll();
 
  private:
};
}
#endif
