#ifndef REPORT_SECTION_H
#define REPORT_SECTION_H

#include <string>
#include <iosfwd>

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
    const std::string& getTitle() const;
};

#endif
