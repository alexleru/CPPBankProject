#include "../include/ScoreCard.h"
#include "../include/ObligationMatrix.h"
#include "../include/WeightingEngine.h"
#include "../include/TierClassifier.h"

ScoreCard::ScoreCard(const std::string& ownerId_)
    : ownerId(ownerId_), score(0.0),
      matrix(NULL), engine(NULL), classifier(NULL) {}

double ScoreCard::recalibrate(ObligationMatrix* m, WeightingEngine* e) {
    if (!m || !e) return score;
    std::vector<double> obligations = m->computeObligations(this, e);
    std::vector<double> normalized  = e->normalize(obligations, this);
    score = 0.0;
    for (size_t i = 0; i < normalized.size(); ++i) score += normalized[i];
    return score;
}

ScoreCard* ScoreCard::extractTier(int tier, ObligationMatrix* source) const {
    if (!source) return NULL;
    std::vector<ScoreCard*> cards = source->getByTier(tier);
    return cards.empty() ? NULL : cards.front();
}

void ScoreCard::registerWith(ObligationMatrix* m) {
    matrix = m;
    if (m) m->enroll(this);
}

int ScoreCard::currentTier(TierClassifier* c, ObligationMatrix* m) const {
    if (!c || !m) return 0;
    return c->classify(const_cast<ScoreCard*>(this), m);
}

void ScoreCard::assignClassifier(TierClassifier* c) {
    classifier = c;
}

double             ScoreCard::getScore()   const { return score; }
const std::string& ScoreCard::getOwnerId() const { return ownerId; }
