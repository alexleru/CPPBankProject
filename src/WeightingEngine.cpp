#include "../include/WeightingEngine.h"
#include "../include/ScoreCard.h"
#include "../include/ObligationMatrix.h"
#include "../include/TierClassifier.h"

WeightingEngine::WeightingEngine(double decayFactor_)
    : decayFactor(decayFactor_), matrix(NULL), classifier(NULL) {}

std::vector<double> WeightingEngine::normalize(
    const std::vector<double>& raw, ScoreCard* card) const {
    std::vector<double> result;
    if (!card) return result;
    double factor = 1.0 + card->getScore() * decayFactor;
    if (factor == 0.0) factor = 1.0;
    for (size_t i = 0; i < raw.size(); ++i) result.push_back(raw[i] / factor);
    return result;
}

std::vector<ScoreCard*> WeightingEngine::reweightMatrix(
    ObligationMatrix* m, double threshold) {
    std::vector<ScoreCard*> affected;
    if (!m) return affected;
    matrix = m;
    std::vector<ScoreCard*> cards = m->getByTier(0);
    for (size_t i = 0; i < cards.size(); ++i) {
        if (cards[i] && cards[i]->getScore() > threshold) {
            affected.push_back(cards[i]);
        }
    }
    return affected;
}

void WeightingEngine::calibrate(ObligationMatrix* m, ScoreCard* referenceCard) {
    if (!m || !referenceCard) return;
    matrix = m;
    std::vector<double> obligations = m->computeObligations(referenceCard, this);
    if (!obligations.empty()) decayFactor *= obligations.front();
}

void             WeightingEngine::attachClassifier(TierClassifier* c) { classifier = c; }
double           WeightingEngine::getDecayFactor() const { return decayFactor; }
TierClassifier*  WeightingEngine::getClassifier()  const { return classifier; }
