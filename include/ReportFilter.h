#ifndef REPORT_FILTER_H
#define REPORT_FILTER_H

#include <string>

class ReportEngine;
class ReportSection;

class ReportFilter {
private:
    ReportEngine* engine;       // non-owning back-pointer
    int           activeRules;
    std::string   cachedRange;

public:
    ReportFilter(ReportEngine* engine);

    void apply(ReportEngine* e);                  // calls e->getDateRange()
    bool getActiveRules(ReportSection* s) const;  // uses s->getTitle()
};

#endif
