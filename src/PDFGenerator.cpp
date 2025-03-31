#include "PDFGenerator.h"
#include <iomanip>
#include <iostream>

/*

A simple implemenation of a PDF generator in pure c++.
Refer to PDF documentation at:
https://opensource.adobe.com/dc-acrobat-sdk-docs/pdfstandards/pdfreference1.7old.pdf

Also referred to:
https://stackoverflow.com/questions/23826472/manually-created-simple-pdf-using-pdf-reference-1-7-adobe-reader-xi-asking-to

The desired output from this class is something like:
%PDF-1.7
1 0 obj
<< /Type /Catalog /Pages 2 0 R >>
endobj
2 0 obj
<< /Type /Pages /Kids [3 0 R] /Count 1 >>
endobj
3 0 obj
<< /Type /Page /Parent 2 0 R /Resources << /Font << /F1 4 0 R >> >> /MediaBox [0 0 612 792] /Contents 5 0 R >>
endobj
4 0 obj
<< /Type /Font /Subtype /Type1 /BaseFont /Helvetica >>
endobj
5 0 obj
<< /Length 319 >>
stream
BT
/F1 15 Tf
100 750 Td
(Invoice ID: 1) Tj
0 -15 Td
(Customer Name: John Smith) Tj
0 -15 Td
(Provider Name: Enwin) Tj
0 -15 Td
(Service Name: Hydro Electric) Tj
0 -15 Td
(Bill Amount: $74.35) Tj
0 -15 Td
(Bill Period: $2024-11-1 to 2024-11-30) Tj
0 -15 Td
(Bill Date: 2025-3-30) Tj
0 -15 Td
(Due Date: 2025-4-29) Tj
ET

endstream
endobj
xref
0 6
0000000000 65535 f 
0000000009 00000 n 
0000000058 00000 n 
0000000115 00000 n 
0000000241 00000 n 
0000000311 00000 n 

trailer
<< /Size 6 /Root 1 0 R >>
startxref
681
%%EOF


*/
using namespace std;


PDFGenerator::PDFGenerator(const string& filename) : objectCount(0) {
    out.open(filename, ios::binary);
    writeHeader();
}

PDFGenerator::~PDFGenerator() {
    if (out.is_open()) {
        out.close();
    }
}

void PDFGenerator::writeHeader() {
    out << "%PDF-1.7\n";
}

int PDFGenerator::addObject(const string& content) {
    objectCount++;
    objectOffsets.push_back(out.tellp());
    writeObject(objectCount, content);
    return objectCount;
}

int PDFGenerator::addStreamObject(const string& streamContent) {
    objectCount++;
    objectOffsets.push_back(out.tellp());
    writeStreamObject(objectCount, streamContent);
    return objectCount;
}

void PDFGenerator::writeObject(int objNum, const string& content) {
    out << objNum << " 0 obj\n" << content << "\n" << "endobj\n";
}

void PDFGenerator::writeStreamObject(int objNum, const string& streamContent) {
    out << objNum << " 0 obj\n" << "<< /Length " << streamContent.length() << " >>\n"  << "stream\n" << streamContent << "\nendstream\n" << "endobj\n";
}


void PDFGenerator::writeCrossReferenceTable() {
    size_t xrefOffset = out.tellp();
    /*
    Section 3.4.3

    This code format is similiar to:
    Example 3.5
    xref
    0 6
    0000000003 65535 f
    0000000017 00000 n
    0000000081 00000 n
    0000000000 00007 f
    0000000331 00000 n
    0000000409 00000 n

    First the xref and 0 6 refer to how many entries there are. We have 6 which is where this number comes from.
    Then you need to define subsections for each object. We have 5, catalog, pages, page, font, and content stream.
    The code could be modified to dunamically render the current offset but we simply tested our file and harded coded these files
    for simplicity. This is an opportunity for a future enhancement to add flexibility to this PDF generator.
    */
    out << "xref\n0 " << (objectCount + 1) << "\n";  // Total entries: objects + null
    /*page 95 - The first entry in the table (object
    number 0) is always free and has a generation number of 65,535; it is the head of
    the linked list of free objects. 
    */

    out << "0000000000 65535 f \n";  // Null object (object 0)

    // Use dynamically tracked offsets from objectOffsets
    //Output should look like:
    /*
    */
    for (size_t i = 0; i < objectOffsets.size(); ++i) {
        out << setfill('0') << setw(10) << objectOffsets[i] << " 00000 n \n";
    }
     /*
    page 97
    The trailer of a PDF file enables an application reading the file to quickly find the
    cross-reference table and certain special objects. Applications should read a PDF 
    file from its end. The last line of the file contains only the end-of-file marker,
    %%EOF. (See implementation note 18 in Appendix H.) The two preceding lines
    contain the keyword startxref and the byte offset from the beginning of the file to
    the beginning of the xref keyword in the last cross-reference section. The
    startxref line is preceded by the trailer dictionary, consisting of the keyword
    trailer followed by a series of key-value pairs enclosed in double angle brackets
    (<<…>>). Thus, the trailer has the following overall structure:
    trailer
    << key1 value1
    key2 value2
    …
    keyn valuen
    >>
    startxref
    Byte_offset_of_last_cross-reference_section
    %%EOF

    This code will output:addObject
    trailer
    << /Size 6 /Root 1 0 R >>
    startxref
    "XRefOffset"
    %%EOF
    */   
    out << "\ntrailer\n<< /Size " << (objectCount + 1) << " /Root 1 0 R >>\nstartxref\n" << xrefOffset << "\n%%EOF\n";


    
}

void PDFGenerator::finalize() {
    //Correctly format all PDF elements
    writeCrossReferenceTable();
    //Close stream
    out.close();
}