#include "../include/ReportEngine.h"
#include "../include/ReportFilter.h"
#include "../include/ReportWriter.h"
#include "../include/ReportSection.h"
#include <ostream>

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

const std::string& ReportEngine::getDateRange()      const { return dateRange; }
ReportFilter*      ReportEngine::getFilter()         const { return filter; }
int                ReportEngine::getPagesGenerated() const { return pagesGenerated; }
