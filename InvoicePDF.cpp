#include "InvoicePDF.h"
#include <sstream>

InvoicePDF::InvoicePDF(const std::string& filename, const Bill& bill)
    : pdf(filename), bill(bill) {}

void InvoicePDF::generateInvoiceContent() {
    // Object 1: Catalog
    pdf.addObject("<< /Type /Catalog /Pages 2 0 R >>");

    // Object 2: Pages
    pdf.addObject("<< /Type /Pages /Kids [3 0 R] /Count 1 >>");

    // Object 3: Page
    pdf.addObject("<< /Type /Page /Parent 2 0 R /Resources << /Font << /F1 4 0 R >> >> /MediaBox [0 0 612 792] /Contents 5 0 R >>");

    // Object 4: Font (Helvetica, a standard PDF font)
    pdf.addObject("<< /Type /Font /Subtype /Type1 /BaseFont /Helvetica >>");

    // Object 5: Content Stream (Invoice Details)
    std::ostringstream contentStream;
    contentStream << "BT\n"; // Begin Text
    contentStream << "/F1 12 Tf\n"; // Set font to Helvetica, 12pt
    contentStream << "100 750 Td\n"; // Move to position (100, 750)

    // Invoice Header
    contentStream << "(Invoice) Tj\n";
    contentStream << "0 -20 Td\n"; // Move down 20 points

    // Invoice Details
    contentStream << "(Bill ID: " << bill.getBillId() << ") Tj\n";
    contentStream << "0 -15 Td\n";
    contentStream << "(Customer ID: " << bill.getCustomerId() << ") Tj\n";
    contentStream << "0 -15 Td\n";
    contentStream << "(Provider ID: " << bill.getProviderId() << ") Tj\n";
    contentStream << "0 -15 Td\n";
    contentStream << "(Service ID: " << bill.getServiceId() << ") Tj\n";
    contentStream << "0 -15 Td\n";
    contentStream << "(Bill Amount: $" << std::fixed << std::setprecision(2) << bill.getBillAmount() << ") Tj\n";
    contentStream << "0 -15 Td\n";
    contentStream << "(Amount Paid: $" << std::fixed << std::setprecision(2) << bill.getAmountPaid() << ") Tj\n";
    contentStream << "0 -15 Td\n";
    contentStream << "(Bill Date: " << bill.getBillDate().year() << "-" << int(bill.getBillDate().month()) << "-" << int(bill.getBillDate().day()) << ") Tj\n";
    contentStream << "0 -15 Td\n";
    contentStream << "(Due Date: " << bill.getDueDate().year() << "-" << int(bill.getDueDate().month()) << "-" << int(bill.getDueDate().day()) << ") Tj\n";

    contentStream << "ET\n"; // End Text
    pdf.addStreamObject(contentStream.str());
}

void InvoicePDF::generate() {
    generateInvoiceContent();
    pdf.finalize();
}