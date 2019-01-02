#ifndef MPPCSD_HH
#define MPPCSD_HH

#include "G4DataVector.hh"
#include "G4VSensitiveDetector.hh"

#include <vector>

class G4Step;
class G4HCofThisEvent;

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
  
    void SetMPPCPositions(const std::vector<G4ThreeVector>& positions);

  private:

    G4DataVector* m_mppcPositionsX;
    G4DataVector* m_mppcPositionsY;
    G4DataVector* m_mppcPositionsZ;
};

#endif
