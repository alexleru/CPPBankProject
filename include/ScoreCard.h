#ifndef SCORE_CARD_H
#define SCORE_CARD_H

#include <string>

class ObligationMatrix;  // forward decl — SCC partner
class WeightingEngine;   // forward decl — SCC partner
class TierClassifier;    // forward decl — SCC partner (4th node)

// ScoreCard: aggregates obligation weights into a final credit score.
// Participates in SCC: ScoreCard <-> ObligationMatrix <-> WeightingEngine <-> TierClassifier.
// All partner pointers are non-owning back-pointers (no ownership of partners).
class ScoreCard {
private:
    std::string       ownerId;
    double            score;
    ObligationMatrix* matrix;      // non-owning back-pointer (SCC edge)
    WeightingEngine*  engine;      // non-owning back-pointer (SCC edge)
    TierClassifier*   classifier;  // non-owning back-pointer (SCC edge)

public:
    explicit ScoreCard(const std::string& ownerId);

    // Recalibrates this scorecard using the obligation matrix and weighting engine.
    // SCC: takes both SCC partners as parameters — they depend on ScoreCard in return.
    double recalibrate(ObligationMatrix* m, WeightingEngine* e);

    // Extracts a sub-scorecard for a specific obligation tier.
    // SCC: returns non-owning pointer; source is SCC partner ObligationMatrix.
    ScoreCard* extractTier(int tier, ObligationMatrix* source) const;

    // Registers this scorecard with the obligation matrix.
    // SCC: establishes the back-pointer cycle ScoreCard -> ObligationMatrix.
    void registerWith(ObligationMatrix* m);

    // Returns the current risk tier assigned by the classifier.
    // SCC: ScoreCard -> TierClassifier back-pointer.
    int currentTier(TierClassifier* c, ObligationMatrix* m) const;

    // Assigns a TierClassifier to this scorecard (establishes SCC back-pointer).
    void assignClassifier(TierClassifier* c);

    double             getScore()   const;
    const std::string& getOwnerId() const;
};

#endif
