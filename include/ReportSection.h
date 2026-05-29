#ifndef REPORT_SECTION_H
#define REPORT_SECTION_H

#include <string>
#include <iosfwd>
#include <vector>

class ReportFilter;
class ReportFormatter;

class ReportSection {
private:
    std::string      title;
    ReportFilter*    filter;     // non-owning
    ReportFormatter* formatter;  // non-owning

public:
    ReportSection(const std::string& title,
                  ReportFilter*    filter,
                  ReportFormatter* formatter);

    void render(std::ostream& out);  // calls filter->getActiveRules(this), formatter->format(this)

    // Applies the filter and returns matching sub-sections (non-owning).
    // SCC: ReportSection <-> ReportFilter <-> ReportFormatter — return type is vector of SCC partner.
    std::vector<ReportSection*> applyFilter(ReportFilter* f, ReportFormatter* fmt) const;

    // Merges this section with another using the given formatter's merge style.
    // SCC: ReportSection <-> ReportFormatter — returns SCC partner type.
    ReportSection* mergeWith(ReportSection* other, ReportFormatter* fmt);

    const std::string& getTitle() const;
};

#endif
