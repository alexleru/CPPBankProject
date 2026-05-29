#include "../include/ObligationMatrix.h"
#include "../include/ScoreCard.h"
#include "../include/WeightingEngine.h"
#include "../include/TierClassifier.h"

ObligationMatrix::ObligationMatrix()
    : engine(NULL), classifier(NULL) {}

void ObligationMatrix::enroll(ScoreCard* card) {
    if (!card) return;
    tiers[0].push_back(card);
}

std::vector<double> ObligationMatrix::computeObligations(
    ScoreCard* card, WeightingEngine* e) const {
    std::vector<double> raw;
    if (!card || !e) return raw;
    raw.push_back(card->getScore());
    raw.push_back(1.0 - card->getScore());
    return e->normalize(raw, card);
}

std::vector<ScoreCard*> ObligationMatrix::propagate(
    ScoreCard* updated, WeightingEngine* e) {
    if (!updated || !e) return std::vector<ScoreCard*>();
    return e->reweightMatrix(this, updated->getScore() * 0.1);
}

std::vector<ScoreCard*> ObligationMatrix::getByTier(int tier) const {
    std::map<int, std::vector<ScoreCard*> >::const_iterator it = tiers.find(tier);
    if (it == tiers.end()) return std::vector<ScoreCard*>();
    return it->second;
}

WeightingEngine* ObligationMatrix::getEngine()     const { return engine; }
void             ObligationMatrix::setEngine(WeightingEngine* e) { engine = e; }
void             ObligationMatrix::attachClassifier(TierClassifier* c) { classifier = c; }
TierClassifier*  ObligationMatrix::getClassifier() const { return classifier; }
