#ifndef WEIGHTING_ENGINE_H
#define WEIGHTING_ENGINE_H

#include <vector>

class ScoreCard;         // forward decl — SCC partner
class ObligationMatrix;  // forward decl — SCC partner
class TierClassifier;    // forward decl — SCC partner

// WeightingEngine: applies normalization weights to obligation vectors.
// Participates in SCC: ScoreCard <-> ObligationMatrix <-> WeightingEngine <-> TierClassifier.
class WeightingEngine {
private:
    double            decayFactor;
    ObligationMatrix* matrix;      // non-owning back-pointer (SCC edge)
    TierClassifier*   classifier;  // non-owning back-pointer (SCC edge)

public:
    explicit WeightingEngine(double decayFactor);

    // Normalizes the raw obligation vector for the given scorecard.
    // SCC: reads ScoreCard's current score to adjust normalization bounds.
    std::vector<double> normalize(
        const std::vector<double>& raw,
        ScoreCard*                 card) const;

    // Re-weights the entire obligation matrix based on current scorecards.
    // SCC: returns std::vector<ScoreCard*> — collection of SCC partner type.
    std::vector<ScoreCard*> reweightMatrix(
        ObligationMatrix* m,
        double            threshold);

    // Calibrates decay factor using aggregate score from the matrix.
    // SCC: takes both ObligationMatrix and ScoreCard — two SCC partners.
    void calibrate(ObligationMatrix* m, ScoreCard* referenceCard);

    // Registers a TierClassifier (establishes SCC back-pointer).
    void attachClassifier(TierClassifier* c);

    double          getDecayFactor() const;
    TierClassifier* getClassifier()  const;
};

#endif
