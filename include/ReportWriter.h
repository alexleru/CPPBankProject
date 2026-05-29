#ifndef REPORT_WRITER_H
#define REPORT_WRITER_H

#include <iosfwd>
#include <vector>

class ReportEngine;
class ReportFormatter;
class ReportSection;
class ReportFilter;

class ReportWriter {
private:
    ReportEngine*    engine;     // non-owning back-pointer
    ReportFormatter* formatter;  // owning

public:
    ReportWriter(ReportEngine* engine, ReportFormatter* formatter);
    ~ReportWriter();

    // Creates a ReportSection, renders it, asks formatter for style,
    // then notifies engine via onPageReady(section). Closes the cycle.
    void write(ReportEngine* e, std::ostream& out);

    // Writes multiple sections in batch, applying filter to each.
    // Returns the list of sections that passed the filter (non-owning).
    // SCC: ReportWriter <-> ReportSection <-> ReportFilter <-> ReportEngine — four partners.
    std::vector<ReportSection*> batchWrite(
        const std::vector<ReportSection*>& sections,
        ReportFilter*                       filter,
        ReportEngine*                       engine);
};

#endif
