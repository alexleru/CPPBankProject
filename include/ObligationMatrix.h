#ifndef OBLIGATION_MATRIX_H
#define OBLIGATION_MATRIX_H

#include <vector>
#include <map>

class ScoreCard;        // forward decl — SCC partner
class WeightingEngine;  // forward decl — SCC partner
class TierClassifier;   // forward decl — SCC partner

// ObligationMatrix: tracks obligation weights per scorecard.
// Participates in SCC: ScoreCard <-> ObligationMatrix <-> WeightingEngine <-> TierClassifier.
class ObligationMatrix {
private:
    std::map<int, std::vector<ScoreCard*> > tiers;       // non-owning entries
    WeightingEngine*                        engine;      // non-owning back-pointer (SCC edge)
    TierClassifier*                         classifier;  // non-owning back-pointer (SCC edge)

public:
    ObligationMatrix();

    // Registers a scorecard in this matrix.
    // SCC: ObligationMatrix <- ScoreCard back-registration.
    void enroll(ScoreCard* card);

    // Computes weighted obligations for the given scorecard using the weighting engine.
    // SCC: takes ScoreCard and WeightingEngine — both SCC partners.
    std::vector<double> computeObligations(ScoreCard* card, WeightingEngine* e) const;

    // Propagates a score update from one card to all related cards through the matrix.
    // SCC: returns std::vector<ScoreCard*> — collection of SCC partner type.
    std::vector<ScoreCard*> propagate(ScoreCard* updated, WeightingEngine* e);

    // Returns all scorecards enrolled for a given tier.
    std::vector<ScoreCard*> getByTier(int tier) const;

    WeightingEngine*  getEngine()      const;
    void              setEngine(WeightingEngine* e);
    void              attachClassifier(TierClassifier* c);
    TierClassifier*   getClassifier()  const;
};

#endif
