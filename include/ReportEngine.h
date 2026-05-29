#ifndef REPORT_ENGINE_H
#define REPORT_ENGINE_H

#include <string>
#include <iosfwd>
#include <utility>

// SCC D — fully isolated from SCC A/B/C.
class ReportFilter;
class ReportWriter;
class ReportSection;
class ReportFormatter;

// Root of the reporting pipeline. Owns its filter and writer.
class ReportEngine {
private:
    ReportFilter* filter;  // owning
    ReportWriter* writer;  // owning
    std::string   dateRange;
    int           pagesGenerated;

public:
    ReportEngine(const std::string& dateRange);
    ~ReportEngine();

    void setFilter(ReportFilter* f);
    void setWriter(ReportWriter* w);

    void generate(std::ostream& out);
    void onPageReady(ReportSection* s);   // back-callback from ReportWriter

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

    const std::string& getDateRange() const;
    ReportFilter*      getFilter()    const;
    int                getPagesGenerated() const;
};

#endif
