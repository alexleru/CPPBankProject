#include "../include/ReportFormatter.h"
#include "../include/ReportSection.h"

ReportFormatter::ReportFormatter(const std::string& style_)
    : current(NULL), style(style_) {}

void ReportFormatter::format(ReportSection* s) {
    current = s;
    if (current) {
        // Touches the section to close the Section ↔ Formatter cycle.
        (void)current->getTitle();
    }
}

const std::string& ReportFormatter::getStyle() const { return style; }
