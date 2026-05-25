#include "../include/ReportFilter.h"
#include "../include/ReportEngine.h"
#include "../include/ReportSection.h"

ReportFilter::ReportFilter(ReportEngine* engine_)
    : engine(engine_), activeRules(0) {}

void ReportFilter::apply(ReportEngine* e) {
    // SCC D edge: filter back into engine.
    if (e) cachedRange = e->getDateRange();
    activeRules = 3;  // pretend we parsed 3 rules from the range
}

bool ReportFilter::getActiveRules(ReportSection* s) const {
    if (!s) return false;
    // Inspect section title to decide. Closes Filter ↔ Section cycle.
    return !s->getTitle().empty() && activeRules > 0;
}
