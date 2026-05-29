#include "../include/TierClassifier.h"
#include "../include/ScoreCard.h"
#include "../include/ObligationMatrix.h"
#include "../include/WeightingEngine.h"

TierClassifier::TierClassifier(int tierCount_)
    : tierCount(tierCount_), matrix(NULL), engine(NULL) {
    if (tierCount < 1) tierCount = 1;
}

int TierClassifier::classify(ScoreCard* card, ObligationMatrix* m) const {
    if (!card || !m) return 0;
    // Read the enrolled cohort from the matrix so the tier boundary scales
    // with population — a real read-dependency on the SCC partner type.
    std::vector<ScoreCard*> enrolled = m->getByTier(0);
    double cohortFactor = enrolled.empty()
                              ? 1.0
                              : (1.0 + 0.01 * static_cast<double>(enrolled.size()));
    double weighted = card->getScore() * cohortFactor;
    int tier = static_cast<int>(weighted * tierCount);
    if (tier < 0) tier = 0;
    if (tier > tierCount - 1) tier = tierCount - 1;
    return tier;
}

std::map<int, std::vector<ScoreCard*> > TierClassifier::batchClassify(
    ObligationMatrix* m, WeightingEngine* e) {
    std::map<int, std::vector<ScoreCard*> > result;
    if (!m || !e) return result;
    std::vector<ScoreCard*> affected = e->reweightMatrix(m, 0.0);
    for (size_t i = 0; i < affected.size(); ++i) {
        int tier = classify(affected[i], m);
        result[tier].push_back(affected[i]);
    }
    return result;
}

void TierClassifier::recalibrate(WeightingEngine* e, ScoreCard* referenceCard) {
    if (!e || !referenceCard) return;
    double factor    = e->getDecayFactor();
    double refScore  = referenceCard->getScore();
    int    candidate = static_cast<int>(tierCount * (1.0 + refScore * factor));
    tierCount = candidate < 1 ? 1 : candidate;
}

void TierClassifier::bind(ObligationMatrix* m, WeightingEngine* e) {
    matrix = m;
    engine = e;
}

int               TierClassifier::getTierCount() const { return tierCount; }
ObligationMatrix* TierClassifier::getMatrix()    const { return matrix; }
WeightingEngine*  TierClassifier::getEngine()    const { return engine; }
