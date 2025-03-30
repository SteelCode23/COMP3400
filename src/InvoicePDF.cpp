#include "invoicepdf.h"
#include "Customer.h"
#include "UtilityProvider.h"
#include "UtilityService.h"
#include "BillCalendar.h"
#include <sstream>

/*
Refer to PDF Documentation at:
https://opensource.adobe.com/dc-acrobat-sdk-docs/pdfstandards/pdfreference1.7old.pdf
*/
InvoicePDF::InvoicePDF(const std::string& filename, const Bill& bill)
    : pdf(filename), bill(bill) {}

void InvoicePDF::generateInvoice() {
    Customer c;
    UtilityProvider up;
    UtilityService us;
    BillCalendar bc;
    
    // Based on Example 3.12 from PDF 1.7  (page 135),
    // We added in a Font object to support all invoice detail elements.
    pdf.addObject("<< /Type /Catalog /Pages 2 0 R >>");  //Catalog
    pdf.addObject("<< /Type /Pages /Kids [3 0 R] /Count 1 >>");  //  Pages
    pdf.addObject("<< /Type /Page /Parent 2 0 R /Resources << /Font << /F1 4 0 R >> >> /MediaBox [0 0 612 792] /Contents 5 0 R >>");  //  Page
    pdf.addObject("<< /Type /Font /Subtype /Type1 /BaseFont /Helvetica >>");  // Font

    std::ostringstream contentStream;
    //Beings a text block.
    contentStream << "BT\n"; 
    // Sets font
    contentStream << "/F1 15 Tf\n"; 
    //moves to coordinates (100, 750) on the page, TD means text delta.
    contentStream << "100 750 Td\n";
    //Write Invoice ID
    contentStream << "(Invoice ID: " << bill.getBillId() << ") Tj\n";
    //Move 15 points down
    contentStream << "0 -15 Td\n";
    //Write customer name
    contentStream << "(Customer Name: " << c.readCustomerById(bill.getCustomerId()).getName() << ") Tj\n";
    contentStream << "0 -15 Td\n";
    //Write Provider Name
    contentStream << "(Provider Name: " << up.readProviderById(bill.getProviderId()).getProviderName() << ") Tj\n";
    contentStream << "0 -15 Td\n";
    //Write Service Name
    contentStream << "(Service Name: " << us.readServiceById(bill.getServiceId()).getServiceName() << ") Tj\n";
    contentStream << "0 -15 Td\n";
    //Write Bill Amount
    contentStream << "(Bill Amount: $" << std::fixed << std::setprecision(2) << bill.getBillAmount() << ") Tj\n";
    contentStream << "0 -15 Td\n";
    //Write Bill Period
    contentStream << "(Bill Period: $" << std::fixed << int(bc.readBillCalendarById(bill.getBillCalendarID()).getStartDate().year())<< "-"  << unsigned(bc.readBillCalendarById(bill.getBillCalendarID()).getStartDate().month())<< "-"
    << unsigned(bc.readBillCalendarById(bill.getBillCalendarID()).getStartDate().day())<< " to " << 
    int(bc.readBillCalendarById(bill.getBillCalendarID()).getEndDate().year())<< "-" <<
    unsigned(bc.readBillCalendarById(bill.getBillCalendarID()).getEndDate().month())<< "-"<<
    unsigned(bc.readBillCalendarById(bill.getBillCalendarID()).getEndDate().day())<< ") Tj\n";
    contentStream << "0 -15 Td\n";
    //Write Bill Data
    contentStream << "(Bill Date: " << int(bill.getBillDate().year()) << "-" << unsigned(bill.getBillDate().month()) << "-" << unsigned(bill.getBillDate().day()) << ") Tj\n";
    contentStream << "0 -15 Td\n";
    //Write Due Date
    contentStream << "(Due Date: " << int(bill.getDueDate().year()) << "-" << unsigned(bill.getDueDate().month()) << "-" << unsigned(bill.getDueDate().day()) << ") Tj\n";
    // End Text
    contentStream << "ET\n"; 
    pdf.addStreamObject(contentStream.str());
}

void InvoicePDF::generate() {
    generateInvoice();
    pdf.finalize();
}