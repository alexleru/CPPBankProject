#ifndef REPORT_WRITER_H
#define REPORT_WRITER_H

#include <iosfwd>

class ReportEngine;
class ReportFormatter;
class ReportSection;

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
};

#endif
