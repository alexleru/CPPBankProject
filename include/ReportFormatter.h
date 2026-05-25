#ifndef REPORT_FORMATTER_H
#define REPORT_FORMATTER_H

#include <string>

class ReportSection;

class ReportFormatter {
private:
    ReportSection* current;  // non-owning (just tracks most-recently-formatted)
    std::string    style;

public:
    ReportFormatter(const std::string& style);

    void format(ReportSection* s);  // sets current, reads s->getTitle()
    const std::string& getStyle() const;
};

#endif
