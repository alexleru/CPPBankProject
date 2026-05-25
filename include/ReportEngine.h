#ifndef REPORT_ENGINE_H
#define REPORT_ENGINE_H

#include <string>
#include <iosfwd>

// SCC D — fully isolated from SCC A/B/C.
class ReportFilter;
class ReportWriter;
class ReportSection;

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

    const std::string& getDateRange() const;
    ReportFilter*      getFilter()    const;
    int                getPagesGenerated() const;
};

#endif
