#include "../include/ReportEngine.h"
#include "../include/ReportFilter.h"
#include "../include/ReportWriter.h"
#include "../include/ReportSection.h"
#include "../include/ReportFormatter.h"
#include <ostream>
#include <sstream>
#include <utility>

ReportEngine::ReportEngine(const std::string& dateRange_)
    : filter(NULL), writer(NULL), dateRange(dateRange_), pagesGenerated(0) {}

ReportEngine::~ReportEngine() {
    delete filter;
    delete writer;
}

void ReportEngine::setFilter(ReportFilter* f) { filter = f; }
void ReportEngine::setWriter(ReportWriter* w) { writer = w; }

void ReportEngine::generate(std::ostream& out) {
    out << "\n=== Generating report for " << dateRange << " ===\n";
    if (filter) filter->apply(this);
    if (writer) writer->write(this, out);
    out << "=== Report complete: pages=" << pagesGenerated << " ===\n";
}

void ReportEngine::onPageReady(ReportSection* s) {
    ++pagesGenerated;
    if (s) {
        // Read back via Section's accessor (closes the cycle).
        // No storage — we just observe the page is done.
    }
}

int ReportEngine::consolidate(ReportEngine* source,
                              ReportFilter* criteria,
                              ReportWriter* target) {
    if (!source || !criteria || !target) return 0;
    // Re-apply the criteria on the source engine and route its writer output
    // into a scratch buffer; the number of pages emitted is the merge count.
    int before = source->getPagesGenerated();
    criteria->apply(source);
    std::ostringstream sink;
    target->write(source, sink);
    int merged = source->getPagesGenerated() - before;
    pagesGenerated += merged;
    return merged;
}

std::pair<ReportEngine*, ReportEngine*> ReportEngine::partition(
    ReportSection*   pivot,
    ReportFormatter* sharedFormatter) {
    // Two empty sibling engines sharing the same formatter style (read via
    // ReportFormatter::getStyle). Pivot's title selects the date-range suffix.
    std::string suffix = pivot ? (" / " + pivot->getTitle()) : std::string();
    std::string style  = sharedFormatter ? sharedFormatter->getStyle() : std::string("plain");
    ReportEngine* left  = new ReportEngine(dateRange + "-A" + suffix + " [" + style + "]");
    ReportEngine* right = new ReportEngine(dateRange + "-B" + suffix + " [" + style + "]");
    return std::make_pair(left, right);
}

const std::string& ReportEngine::getDateRange()      const { return dateRange; }
ReportFilter*      ReportEngine::getFilter()         const { return filter; }
int                ReportEngine::getPagesGenerated() const { return pagesGenerated; }
