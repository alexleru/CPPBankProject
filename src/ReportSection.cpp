#include "../include/ReportSection.h"
#include "../include/ReportFilter.h"
#include "../include/ReportFormatter.h"
#include <ostream>

ReportSection::ReportSection(const std::string& title_,
                             ReportFilter*    filter_,
                             ReportFormatter* formatter_)
    : title(title_), filter(filter_), formatter(formatter_) {}

void ReportSection::render(std::ostream& out) {
    out << "  -- Section: " << title << "\n";
    if (filter && !filter->getActiveRules(this)) {
        out << "     (filtered out)\n";
        return;
    }
    if (formatter) formatter->format(this);
    out << "     [rendered]\n";
}

const std::string& ReportSection::getTitle() const { return title; }
