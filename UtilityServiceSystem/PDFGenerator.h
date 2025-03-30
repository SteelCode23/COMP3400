#ifndef PDF_GENERATOR_H
#define PDF_GENERATOR_H

#include <string>
#include <vector>
#include <fstream>

class PDFGenerator {
private:
    std::ofstream out;
    std::vector<size_t> objectOffsets; // Track byte offsets for cross-reference table
    int objectCount;

    void writeHeader();
    void writeObject(int objNum, const std::string& content);
    void writeStreamObject(int objNum, const std::string& streamContent);
    void writeCrossReferenceTable();
    void writeTrailer();

public:
    PDFGenerator(const std::string& filename);
    ~PDFGenerator();

    int addObject(const std::string& content);
    int addStreamObject(const std::string& streamContent);
    void finalize();
};

#endif