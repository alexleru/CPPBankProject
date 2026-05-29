#ifndef TIER_CLASSIFIER_H
#define TIER_CLASSIFIER_H

#include <vector>
#include <map>

class ScoreCard;         // forward decl — SCC partner
class ObligationMatrix;  // forward decl — SCC partner
class WeightingEngine;   // forward decl — SCC partner

// TierClassifier: assigns risk tiers to scorecards based on obligation data.
// Participates in SCC: ScoreCard <-> ObligationMatrix <-> WeightingEngine <-> TierClassifier.
// Stored back-pointers ensure the 4-node cycle is visible in the include graph
// (not only via method parameters).
class TierClassifier {
private:
    int               tierCount;
    ObligationMatrix* matrix;  // non-owning back-pointer (SCC edge)
    WeightingEngine*  engine;  // non-owning back-pointer (SCC edge)

public:
    explicit TierClassifier(int tierCount);

    // Assigns a risk tier (0..tierCount-1) to a scorecard using obligation context.
    // SCC: reads ScoreCard's score, uses ObligationMatrix for tier boundaries.
    int classify(ScoreCard* card, ObligationMatrix* m) const;

    // Batch-classifies all enrolled scorecards; returns tier -> scorecard mapping.
    // SCC: returns std::map<int, std::vector<ScoreCard*> > -> Java Map<Integer, List<ScoreCard>>
    //      with exact SCC partner type name — impossible to guess without stubs.
    std::map<int, std::vector<ScoreCard*> > batchClassify(
        ObligationMatrix* m,
        WeightingEngine*  e);

    // Re-calibrates tier boundaries using the weighting engine's decay factor
    // and a representative scorecard's current score.
    // SCC: takes WeightingEngine and ScoreCard — two SCC partners simultaneously.
    void recalibrate(WeightingEngine* e, ScoreCard* referenceCard);

    // Establishes stored back-pointers (closes the 4-node SCC at field level).
    void bind(ObligationMatrix* m, WeightingEngine* e);

    int               getTierCount() const;
    ObligationMatrix* getMatrix()    const;
    WeightingEngine*  getEngine()    const;
};

#endif
