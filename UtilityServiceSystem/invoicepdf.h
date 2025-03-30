#ifndef INVOICE_PDF_H
#define INVOICE_PDF_H

#include "PDFGenerator.h"
#include "Bill.h" // Assume Bill.h exists with the Bill class

class InvoicePDF {
private:
    PDFGenerator pdf;
    Bill bill;

    void generateInvoice();

public:
    InvoicePDF(const std::string& filename, const Bill& bill);
    void generate();
};

#endif