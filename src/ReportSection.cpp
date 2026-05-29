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

std::vector<ReportSection*> ReportSection::applyFilter(
    ReportFilter* f, ReportFormatter* fmt) const {
    std::vector<ReportSection*> matches;
    if (!f) return matches;
    // Cast away const for the back-callback (filter signature is non-const).
    ReportSection* self = const_cast<ReportSection*>(this);
    if (f->getActiveRules(self)) {
        if (fmt) fmt->format(self);
        matches.push_back(self);
    }
    return matches;
}

ReportSection* ReportSection::mergeWith(ReportSection* other, ReportFormatter* fmt) {
    if (!other) return this;
    // Title becomes "A + B" under the supplied formatter's style.
    std::string mergedTitle = title + " + " + other->getTitle();
    if (fmt) {
        fmt->format(this);
        mergedTitle += " [" + fmt->getStyle() + "]";
    }
    // Reuse the existing filter/formatter of this section for the merged result.
    return new ReportSection(mergedTitle, filter, fmt ? fmt : formatter);
}

const std::string& ReportSection::getTitle() const { return title; }
