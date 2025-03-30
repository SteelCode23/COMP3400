#include "invoicepdf.h"
#include "Customer.h"
#include "UtilityProvider.h"
#include "UtilityService.h"
#include "BillCalendar.h"
#include <sstream>

InvoicePDF::InvoicePDF(const std::string& filename, const Bill& bill)
    : pdf(filename), bill(bill) {}

void InvoicePDF::generateInvoiceContent() {
    Customer c;
    UtilityProvider up;
    UtilityService us;
    BillCalendar bc;
    pdf.addObject("<< /Type /Catalog /Pages 2 0 R >>");
    pdf.addObject("<< /Type /Pages /Kids [3 0 R] /Count 1 >>");
    pdf.addObject("<< /Type /Page /Parent 2 0 R /Resources << /Font << /F1 4 0 R >> >> /MediaBox [0 0 612 792] /Contents 5 0 R >>");
    pdf.addObject("<< /Type /Font /Subtype /Type1 /BaseFont /Helvetica >>");
    std::ostringstream contentStream;
    contentStream << "BT\n"; 
    contentStream << "/F1 12 Tf\n"; // Set font to Helvetica, 12pt
    contentStream << "100 750 Td\n";
    contentStream << "(Invoice ID: " << bill.getBillId() << ") Tj\n";
    contentStream << "0 -15 Td\n";
    contentStream << "(Customer Name: " << c.readCustomerById(bill.getCustomerId()).getName() << ") Tj\n";
    contentStream << "0 -15 Td\n";
    contentStream << "(Provider Name: " << up.readProviderById(bill.getProviderId()).getProviderName() << ") Tj\n";
    contentStream << "0 -15 Td\n";
    contentStream << "(Service Name: " << us.readServiceById(bill.getServiceId()).getServiceName() << ") Tj\n";
    contentStream << "0 -15 Td\n";
    contentStream << "(Bill Amount: $" << std::fixed << std::setprecision(2) << bill.getBillAmount() << ") Tj\n";
    contentStream << "0 -15 Td\n";
    contentStream << "(Bill Period: $" << std::fixed << int(bc.readBillCalendarById(bill.getBillCalendarID()).getStartDate().year())<< "-"  << unsigned(bc.readBillCalendarById(bill.getBillCalendarID()).getStartDate().month())<< "-"
    << unsigned(bc.readBillCalendarById(bill.getBillCalendarID()).getStartDate().day())<< " to " << 
    int(bc.readBillCalendarById(bill.getBillCalendarID()).getEndDate().year())<< "-" <<
    unsigned(bc.readBillCalendarById(bill.getBillCalendarID()).getEndDate().month())<< "-"<<
    unsigned(bc.readBillCalendarById(bill.getBillCalendarID()).getEndDate().day())<< ") Tj\n";
    contentStream << "0 -15 Td\n";
    contentStream << "(Bill Date: " << int(bill.getBillDate().year()) << "-" << unsigned(bill.getBillDate().month()) << "-" << unsigned(bill.getBillDate().day()) << ") Tj\n";
    contentStream << "0 -15 Td\n";
    contentStream << "(Due Date: " << int(bill.getDueDate().year()) << "-" << unsigned(bill.getDueDate().month()) << "-" << unsigned(bill.getDueDate().day()) << ") Tj\n";
    contentStream << "ET\n"; // End Text
    pdf.addStreamObject(contentStream.str());
}

void InvoicePDF::generate() {
    generateInvoiceContent();
    pdf.finalize();
}