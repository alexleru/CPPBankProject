# CPPBankProject: Complexity Enhancement for SCC Demonstration

**Purpose:** Make cyclic dependency (SCC) scenarios complex enough that an LLM cannot
successfully guess correct Java types without being given stub files.

**Problem with current project:** Despite having real SCC cycles, the project uses standard
banking vocabulary (`Customer`, `Bank`, `Loan`) and simple getter/setter patterns — LLM guesses
correctly even without stubs. The changes below make failures deterministic.

---

## Prompt

You are a C++ developer extending the CPPBankProject at
`C:\Users\AlLeshchuk\Documents\projects\CPPBankProject`.

The project is used to demonstrate automatic C++ → Java conversion using an LLM-based tool.
The goal of this extension is to make the cyclic dependency (SCC) scenarios complex enough
that the LLM **cannot** successfully guess correct Java types without being given stub files.
Currently the cycles are too "guessable" — standard banking vocabulary, simple getters/setters.

## TASK

Add the following changes to the existing C++ project. Do NOT break existing code.
Create new `.h` and `.cpp` files, and modify existing ones as listed.

### Project conventions that MUST be respected

These conventions are enforced by [CLAUDE.md](../CLAUDE.md) and verified by the build.
Every code sample below already complies — keep it that way when you implement.

- **C++03 only.** Compiled with `-std=c++03 -Wall -Wextra`, zero warnings.
  No `nullptr` (use `NULL`), no `auto`, no range-based `for`, no brace-init,
  no `>>` in nested templates (use `> >`), no `using` aliases (use `typedef`),
  no lambdas, no `enum class`, no `override`/`final`/`noexcept`.
- **Header guards:** `#ifndef FOO_H` / `#define FOO_H` / `#endif`. No `#pragma once`.
- **Headers live in `include/`, sources in `src/`.** Each header has one `.cpp`.
- **Include style in `.cpp` files:** `#include "../include/Foo.h"`.
- **Member naming:** plain (`score`, `matrix`, `engine`). No trailing underscore.
  Constructor parameters that shadow members use trailing underscore (`score_`, `matrix_`)
  only inside the initializer list.
- **Build system:** single hand-written cross-platform [Makefile](../Makefile).
  No CMake. New `.cpp` files must be appended to the explicit `SOURCES` list.

---

### CHANGE 1: Enhance SCC D (Report pipeline) with non-trivial cross-partner method signatures

**Modify [include/ReportEngine.h](../include/ReportEngine.h) and [src/ReportEngine.cpp](../src/ReportEngine.cpp)** — add these methods to the existing class. The header already forward-declares `ReportFilter`, `ReportWriter`, `ReportSection`; add a forward declaration for `ReportFormatter` and `#include <utility>` for `std::pair`.

```cpp
// Merges another engine's sections into this one, filtered by the given filter.
// Returns number of sections merged.
// SCC: ReportEngine <-> ReportFilter <-> ReportWriter — all three partners in one call.
int consolidate(ReportEngine* source, ReportFilter* criteria, ReportWriter* target);

// Splits the current report into two engines based on a section predicate.
// Both output engines share the same formatter.
// SCC: ReportEngine <-> ReportSection <-> ReportFormatter — two partners in return type.
std::pair<ReportEngine*, ReportEngine*> partition(
    ReportSection*    pivot,
    ReportFormatter*  sharedFormatter);
```

**Modify [include/ReportSection.h](../include/ReportSection.h) and [src/ReportSection.cpp](../src/ReportSection.cpp)** — add `#include <vector>` to the header, then:

```cpp
// Applies the filter and returns matching sub-sections (non-owning).
// SCC: ReportSection <-> ReportFilter <-> ReportFormatter — return type is vector of SCC partner.
std::vector<ReportSection*> applyFilter(ReportFilter* f, ReportFormatter* fmt) const;

// Merges this section with another using the given formatter's merge style.
// SCC: ReportSection <-> ReportFormatter — returns SCC partner type.
ReportSection* mergeWith(ReportSection* other, ReportFormatter* fmt);
```

**Modify [include/ReportWriter.h](../include/ReportWriter.h) and [src/ReportWriter.cpp](../src/ReportWriter.cpp)** — add `#include <vector>` and forward declarations for `ReportFilter` / `ReportSection`, then:

```cpp
// Writes multiple sections in batch, applying filter to each.
// Returns the list of sections that passed the filter (non-owning).
// SCC: ReportWriter <-> ReportSection <-> ReportFilter <-> ReportEngine — four partners.
std::vector<ReportSection*> batchWrite(
    const std::vector<ReportSection*>& sections,
    ReportFilter*                       filter,
    ReportEngine*                       engine);
```

**Why this helps demonstrate stubs:**
- `consolidate(ReportEngine*, ReportFilter*, ReportWriter*)` — LLM must know ALL THREE partner types simultaneously.
- `applyFilter()` returns `std::vector<ReportSection*>` → Java `List<ReportSection>` — must know exact type name.
- `batchWrite()` — receives collection AND two SCC partners — extremely hard to guess without stubs.

> **Note:** SCC D already has 5 members and is already Tier-B (see
> [SCC_DEMO_LAYOUT.md](SCC_DEMO_LAYOUT.md)). These additions strengthen cross-partner
> signatures within the existing SCC; they do not change tier classification.

---

### CHANGE 2: Add a new 4-class SCC with non-banking domain vocabulary

> **Fix vs. original:** An earlier draft had only 3 classes (`ScoreCard`, `ObligationMatrix`,
> `WeightingEngine`). A 3-node SCC is classified as **Tier-A** by the conversion plugin and
> processed **without stubs** (serial ordering only). To demonstrate the stub mechanism the
> SCC must have **4+ nodes** (Tier-B). A fourth class `TierClassifier` is added, which
> mutually depends on `ScoreCard`, `ObligationMatrix`, and `WeightingEngine` via stored
> back-pointers (not just method parameters), forming a real 4-node cycle in the include
> + method-call graph.

> **Impact on the "exactly three SCCs" invariant.** [CLAUDE.md](../CLAUDE.md) and
> [SCC_DEMO_LAYOUT.md](SCC_DEMO_LAYOUT.md) currently state the project has exactly three
> cyclic SCCs (mega-SCC, B, D). Applying this change adds a fourth (the credit-scoring
> subsystem). After applying, update the following docs:
> - [../CLAUDE.md](../CLAUDE.md) — "Architecture: four SCCs" table and SCC count text
> - [SCC_DEMO_LAYOUT.md](SCC_DEMO_LAYOUT.md) — overview table, "exactly three" wording, diagrams
> - [SCC_DEMO_PROJECT_PROMPT.md](SCC_DEMO_PROJECT_PROMPT.md) — original spec
> - [README.md](README.md) — project overview
> - [QUICK_REFERENCE.md](QUICK_REFERENCE.md) — class taxonomy
>
> The new SCC must not reference any class in mega-SCC / B / D, the same isolation rule
> that protects SCC D today.

Create a **credit scoring subsystem** with a 4-class mutual cycle. These class names are
intentionally NOT in standard LLM training data for banking systems.

**New file: `include/ScoreCard.h`**

```cpp
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
```

**New file: `include/ObligationMatrix.h`**

```cpp
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
```

**New file: `include/WeightingEngine.h`**

```cpp
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
```

**New file: `include/TierClassifier.h`**

```cpp
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
```

**New file: `src/ScoreCard.cpp`**

```cpp
#include "../include/ScoreCard.h"
#include "../include/ObligationMatrix.h"
#include "../include/WeightingEngine.h"
#include "../include/TierClassifier.h"

ScoreCard::ScoreCard(const std::string& ownerId_)
    : ownerId(ownerId_), score(0.0),
      matrix(NULL), engine(NULL), classifier(NULL) {}

double ScoreCard::recalibrate(ObligationMatrix* m, WeightingEngine* e) {
    std::vector<double> obligations = m->computeObligations(this, e);
    std::vector<double> normalized  = e->normalize(obligations, this);
    score = 0.0;
    for (size_t i = 0; i < normalized.size(); ++i) score += normalized[i];
    return score;
}

ScoreCard* ScoreCard::extractTier(int tier, ObligationMatrix* source) const {
    std::vector<ScoreCard*> cards = source->getByTier(tier);
    return cards.empty() ? NULL : cards.front();
}

void ScoreCard::registerWith(ObligationMatrix* m) {
    matrix = m;
    m->enroll(this);
}

int ScoreCard::currentTier(TierClassifier* c, ObligationMatrix* m) const {
    return c->classify(const_cast<ScoreCard*>(this), m);
}

void ScoreCard::assignClassifier(TierClassifier* c) {
    classifier = c;
}

double             ScoreCard::getScore()   const { return score; }
const std::string& ScoreCard::getOwnerId() const { return ownerId; }
```

**New file: `src/ObligationMatrix.cpp`**

```cpp
#include "../include/ObligationMatrix.h"
#include "../include/ScoreCard.h"
#include "../include/WeightingEngine.h"
#include "../include/TierClassifier.h"

ObligationMatrix::ObligationMatrix()
    : engine(NULL), classifier(NULL) {}

void ObligationMatrix::enroll(ScoreCard* card) {
    tiers[0].push_back(card);
}

std::vector<double> ObligationMatrix::computeObligations(
    ScoreCard* card, WeightingEngine* e) const {
    std::vector<double> raw;
    raw.push_back(card->getScore());
    raw.push_back(1.0 - card->getScore());
    return e->normalize(raw, card);
}

std::vector<ScoreCard*> ObligationMatrix::propagate(
    ScoreCard* updated, WeightingEngine* e) {
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
```

**New file: `src/WeightingEngine.cpp`**

```cpp
#include "../include/WeightingEngine.h"
#include "../include/ScoreCard.h"
#include "../include/ObligationMatrix.h"
#include "../include/TierClassifier.h"

WeightingEngine::WeightingEngine(double decayFactor_)
    : decayFactor(decayFactor_), matrix(NULL), classifier(NULL) {}

std::vector<double> WeightingEngine::normalize(
    const std::vector<double>& raw, ScoreCard* card) const {
    std::vector<double> result;
    double factor = 1.0 + card->getScore() * decayFactor;
    for (size_t i = 0; i < raw.size(); ++i) result.push_back(raw[i] / factor);
    return result;
}

std::vector<ScoreCard*> WeightingEngine::reweightMatrix(
    ObligationMatrix* m, double threshold) {
    matrix = m;
    std::vector<ScoreCard*> affected;
    std::vector<ScoreCard*> cards = m->getByTier(0);
    for (size_t i = 0; i < cards.size(); ++i) {
        if (cards[i]->getScore() > threshold) affected.push_back(cards[i]);
    }
    return affected;
}

void WeightingEngine::calibrate(ObligationMatrix* m, ScoreCard* referenceCard) {
    matrix = m;
    std::vector<double> obligations = m->computeObligations(referenceCard, this);
    if (!obligations.empty()) decayFactor *= obligations.front();
}

void             WeightingEngine::attachClassifier(TierClassifier* c) { classifier = c; }
double           WeightingEngine::getDecayFactor() const { return decayFactor; }
TierClassifier*  WeightingEngine::getClassifier()  const { return classifier; }
```

**New file: `src/TierClassifier.cpp`**

```cpp
#include "../include/TierClassifier.h"
#include "../include/ScoreCard.h"
#include "../include/ObligationMatrix.h"
#include "../include/WeightingEngine.h"
#include <algorithm>

TierClassifier::TierClassifier(int tierCount_)
    : tierCount(tierCount_), matrix(NULL), engine(NULL) {}

int TierClassifier::classify(ScoreCard* card, ObligationMatrix* m) const {
    // Use matrix to derive an upper-bound count of enrolled cards in tier 0;
    // this keeps a live read-dependency on the SCC partner type at call time.
    std::vector<ScoreCard*> enrolled = m->getByTier(0);
    double cohortFactor = enrolled.empty() ? 1.0 : 1.0;  // placeholder, real logic would use cohortFactor
    double score = card->getScore() * cohortFactor;
    int tier = static_cast<int>(score * tierCount);
    if (tier < 0) tier = 0;
    if (tier > tierCount - 1) tier = tierCount - 1;
    return tier;
}

std::map<int, std::vector<ScoreCard*> > TierClassifier::batchClassify(
    ObligationMatrix* m, WeightingEngine* e) {
    std::map<int, std::vector<ScoreCard*> > result;
    std::vector<ScoreCard*> affected = e->reweightMatrix(m, 0.0);
    for (size_t i = 0; i < affected.size(); ++i) {
        int tier = classify(affected[i], m);
        result[tier].push_back(affected[i]);
    }
    return result;
}

void TierClassifier::recalibrate(WeightingEngine* e, ScoreCard* referenceCard) {
    double factor   = e->getDecayFactor();
    double refScore = referenceCard->getScore();
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
```

**Why this helps demonstrate stubs:**
- 4-node SCC with stored back-pointers in all four classes → **Tier-B** → weighted greedy FAS writes stubs → stub mechanism is activated.
- `ScoreCard`, `ObligationMatrix`, `WeightingEngine`, `TierClassifier` are NOT in LLM's standard Java banking vocabulary.
- `batchClassify()` returns `std::map<int, std::vector<ScoreCard*> >` → Java `Map<Integer, List<ScoreCard>>` — nested generics with exact SCC partner type name.
- `recalibrate(WeightingEngine*, ScoreCard*)` — two SCC partners simultaneously — LLM will invent wrong names without stubs.
- `currentTier(TierClassifier*, ObligationMatrix*)` in `ScoreCard` — back-dependency that closes the 4-node cycle.

---

### CHANGE 3: Add a non-trivial overloaded method to the Mega-SCC (SCC C)

**Modify [include/RiskAnalyzer.h](../include/RiskAnalyzer.h) and [src/RiskAnalyzer.cpp](../src/RiskAnalyzer.cpp)** — the existing header already forward-declares `Bank`, `AuditLogger`, `Loan`, `Customer`, `BranchManager`. Add `#include <vector>` and:

```cpp
// Evaluates credit risk for a loan application requiring full approval chain context.
// SCC: takes FOUR SCC-C partners simultaneously — Loan, Customer, BranchManager, AuditLogger.
Money evaluateApplication(
    Loan*          application,
    Customer*      applicant,
    BranchManager* approvingManager,
    AuditLogger*   auditTrail);

// Aggregates portfolio risk across all loans of a customer.
// SCC: returns std::vector<Loan*> — collection of SCC partner type.
std::vector<Loan*> flagHighRisk(
    Customer*      customer,
    BranchManager* responsibleManager,
    Money          threshold);
```

**Why:** `evaluateApplication` takes **four SCC partners** simultaneously — the probability that
the LLM generates correct Java signatures for all four types without stubs is very low. Without
stubs, it will misname at least one parameter type or omit a parameter.

Return type is `Money` (the project's existing typedef from [Constants.h](../include/Constants.h)),
not `double`, to keep with the project's type vocabulary.

---

## Implementation Notes

1. All new class forward declarations must be consistent with existing project style
   (`class Foo;` inside the header that uses pointers to `Foo`).
2. Implement `.cpp` files with minimal but **non-empty** bodies — call the partner methods
   so the cycle is real (not just a syntactic include).
3. Update [Makefile](../Makefile): append the new sources to the explicit `SOURCES` list:
   `ScoreCard.cpp`, `ObligationMatrix.cpp`, `WeightingEngine.cpp`, `TierClassifier.cpp`.
   The Makefile does not glob — missing entries silently exclude the file from the build.
4. Use `#ifndef`/`#define`/`#endif` include guards. No `#pragma once`.
5. Use C++03 syntax only:
   - `NULL` not `nullptr`
   - explicit iterators or index loops, not range-based `for`
   - explicit type declarations, not `auto`
   - `> >` not `>>` for nested templates
   - `std::vector::push_back` / index init, not brace-init lists
   - `typedef`, not `using` aliases
6. Each method that crosses an SCC boundary must have a `// SCC:` comment explaining the relationship.
7. Build must remain warning-clean under `-Wall -Wextra`. Do not introduce unused variables
   or parameters; if a parameter is intentionally unused, mark it `/*name*/`.
8. Verify the new credit-scoring SCC stays isolated from mega-SCC / B / D:

   ```bash
   grep -E 'Account|Bank|Customer|Transaction|Loan|Audit|Notification|BranchManager|RiskAnalyzer|Report' \
       include/ScoreCard.h include/ObligationMatrix.h include/WeightingEngine.h include/TierClassifier.h \
       src/ScoreCard.cpp src/ObligationMatrix.cpp src/WeightingEngine.cpp src/TierClassifier.cpp
   # expected: no output
   ```

9. After applying CHANGE 2, update the project's load-bearing docs to reflect a fourth SCC:
   [../CLAUDE.md](../CLAUDE.md), [SCC_DEMO_LAYOUT.md](SCC_DEMO_LAYOUT.md),
   [SCC_DEMO_PROJECT_PROMPT.md](SCC_DEMO_PROJECT_PROMPT.md),
   [README.md](README.md), [QUICK_REFERENCE.md](QUICK_REFERENCE.md).
10. Optional: extend [src/main.cpp](../src/main.cpp) with a fifth menu option that exercises
    the credit-scoring pipeline end-to-end, mirroring how option 2 exercises SCC D. Wiring
    the cycle from `main` ensures the new classes are actually linked, not just compiled.

---

## Expected Outcome

| SCC                                  | Without stubs                                                                                    | With stubs                                       |
|--------------------------------------|--------------------------------------------------------------------------------------------------|--------------------------------------------------|
| D (Report)                           | `batchWrite` / `consolidate` — wrong parameter types, `List<ReportSection>` misspelled            | Correct signatures, exact type names from stubs  |
| New credit-scoring (4-node, Tier-B)  | `TierClassifier` renamed to `TierEngine` or `ClassifierTier`; `Map<Integer, List<ScoreCard>>` becomes `Map<Integer, List<Card>>` | Exact names from same-package stubs              |
| C (Mega, RiskAnalyzer)               | `evaluateApplication` — at least 1 of 4 partners wrong                                            | All four types correct from stubs                |

After these changes, running the plugin **without** SCC stub generation will produce
at least 10–15 compile errors from wrong Java type names. Running **with** SCC stubs will
compile cleanly on the first pass.

---

## Summary of changes vs. earlier draft

| # | Change                                                                                                  | Reason                                                                                              |
|---|---------------------------------------------------------------------------------------------------------|-----------------------------------------------------------------------------------------------------|
| 1 | CHANGE 2: 3 classes → **4 classes** (added `TierClassifier`)                                            | 3-node SCC = Tier-A, no stubs; 4+ nodes = Tier-B, FAS + stubs activate                              |
| 2 | `TierClassifier` carries stored back-pointers (`ObligationMatrix*`, `WeightingEngine*`)                 | Closes the cycle in the include graph, not only via method parameters                                |
| 3 | `ObligationMatrix` / `WeightingEngine` carry a `TierClassifier*` back-pointer                            | Symmetric stored edges back to the 4th node                                                          |
| 4 | All code samples rewritten in C++03 (no `nullptr`, `auto`, range-`for`, brace-init, `>>`, `using`)       | Matches project's hard constraint enforced by `-std=c++03 -Wall -Wextra`                             |
| 5 | Header guards switched to `#ifndef`                                                                     | Matches existing project style; no `#pragma once` anywhere                                           |
| 6 | Include paths in `.cpp` switched to `"../include/Foo.h"`                                                | Matches existing project style                                                                       |
| 7 | Build-file row in summary changed from `CMakeLists.txt` to [Makefile](../Makefile) `SOURCES`             | Project has no CMake — single hand-written Makefile                                                  |
| 8 | Wrong absolute path `C:\Users\RoKamnev\IdeaProjects\...` replaced with the actual project path           | Reproducibility / correctness                                                                        |
| 9 | Unused-variable issue in `TierClassifier::classify` fixed; `<algorithm>`, `<utility>`, `<vector>` adds   | Build must stay warning-clean under `-Wall -Wextra`                                                  |
| 10 | Added an explicit follow-up to update load-bearing docs ([CLAUDE.md](../CLAUDE.md), [SCC_DEMO_LAYOUT.md](SCC_DEMO_LAYOUT.md), …) | Adding a 4th SCC changes the project's stated "exactly three SCCs" invariant                       |
