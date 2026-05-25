#include "../include/ReportWriter.h"
#include "../include/ReportEngine.h"
#include "../include/ReportFormatter.h"
#include "../include/ReportFilter.h"
#include "../include/ReportSection.h"
#include <ostream>
#include <sstream>

ReportWriter::ReportWriter(ReportEngine* engine_, ReportFormatter* formatter_)
    : engine(engine_), formatter(formatter_) {}

ReportWriter::~ReportWriter() {
    delete formatter;
}

void ReportWriter::write(ReportEngine* e, std::ostream& out) {
    // Emit two pages so the cycle exercises more than once.
    ReportFilter* f = e ? e->getFilter() : NULL;
    for (int i = 1; i <= 2; ++i) {
        std::ostringstream titleStream;
        titleStream << "Page " << i;
        ReportSection section(titleStream.str(), f, formatter);
        section.render(out);
        if (formatter) out << "     style=" << formatter->getStyle() << "\n";
        if (e) e->onPageReady(&section);
    }
}
