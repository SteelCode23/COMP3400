#include "PDFGenerator.h"
#include <iomanip>

PDFGenerator::PDFGenerator(const std::string& filename) : objectCount(0) {
    out.open(filename, std::ios::binary);
    if (!out.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
    }
    writeHeader();
}

PDFGenerator::~PDFGenerator() {
    if (out.is_open()) {
        out.close();
    }
}

void PDFGenerator::writeHeader() {
    out << "%PDF-1.7\n";
    out << "%\xE2\xE3\xCF\xD3\n";
}

int PDFGenerator::addObject(const std::string& content) {
    objectCount++;
    objectOffsets.push_back(out.tellp());
    writeObject(objectCount, content);
    return objectCount;
}

int PDFGenerator::addStreamObject(const std::string& streamContent) {
    objectCount++;
    objectOffsets.push_back(out.tellp());
    writeStreamObject(objectCount, streamContent);
    return objectCount;
}

void PDFGenerator::writeObject(int objNum, const std::string& content) {
    out << objNum << " 0 obj\n";
    out << content << "\n";
    out << "endobj\n";
}

void PDFGenerator::writeStreamObject(int objNum, const std::string& streamContent) {
    out << objNum << " 0 obj\n";
    out << "<< /Length " << streamContent.length() << " >>\n";
    out << "stream\n";
    out << streamContent;
    out << "\nendstream\n";
    out << "endobj\n";
}

void PDFGenerator::writeCrossReferenceTable() {
    size_t xrefOffset = out.tellp();
    out << "xref\n";
    out << "0 " << (objectCount + 1) << "\n";
    out << "0000000000 65535 f \n";
    for (size_t i = 0; i < objectOffsets.size(); ++i) {
        out << std::setfill('0') << std::setw(10) << objectOffsets[i] << " 00000 n \n";
    }
    out << "\n";
    out << "trailer\n";
    out << "<< /Size " << (objectCount + 1) << " /Root 1 0 R >>\n";
    out << "startxref\n";
    out << xrefOffset << "\n";
    out << "%%EOF\n";
}

void PDFGenerator::finalize() {
    writeCrossReferenceTable();
    out.close();
}