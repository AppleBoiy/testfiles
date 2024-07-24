#include "TextFileGenerator.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <filesystem>

namespace fs = std::filesystem;

TextFileGenerator::TextFileGenerator(unsigned numEntries, TextFormat format)
        : numEntries(numEntries), format(format) {}

void TextFileGenerator::create(const std::string &file) {
    this->filename = file;
    generateRandomContent();
}

void TextFileGenerator::save() {
    // Ensure the 'dist' directory exists
    fs::create_directories("dist");

    std::ofstream file("dist/" + filename);
    if (file.is_open()) {
        file << content;
        file.close();
        std::cout << "Text file created: dist/" << filename << std::endl;
    } else {
        std::cerr << "Error creating text file: dist/" << filename << std::endl;
    }
}

void TextFileGenerator::generateRandomContent() {
    std::stringstream ss;
    switch (format) {
        case TextFormat::JSON:
            generateJSONContent();
            break;
        case TextFormat::CSV:
            generateCSVContent();
            break;
        case TextFormat::TXT:
            generateTXTContent();
            break;
        case TextFormat::LOG:
            generateLOGContent();
            break;
    }
}

void TextFileGenerator::generateJSONContent() {
    std::stringstream ss;
    ss << "{\n";
    for (unsigned i = 0; i < numEntries; ++i) {
        ss << "  \"entry" << i << "\": " << (random() % 1000);
        if (i < numEntries - 1) {
            ss << ",";
        }
        ss << "\n";
    }
    ss << "}\n";
    content = ss.str();
}

void TextFileGenerator::generateCSVContent() {
    std::stringstream ss;
    for (unsigned i = 0; i < numEntries; ++i) {
        ss << "entry" << i << "," << (random() % 1000) << "\n";
    }
    content = ss.str();
}

void TextFileGenerator::generateTXTContent() {
    std::stringstream ss;
    for (unsigned i = 0; i < numEntries; ++i) {
        ss << "This is entry number " << i << ": " << (random() % 1000) << "\n";
    }
    content = ss.str();
}

void TextFileGenerator::generateLOGContent() {
    std::stringstream ss;
    for (unsigned i = 0; i < numEntries; ++i) {
        ss << "Log entry " << i << ": Random value " << (random() % 1000) << "\n";
    }
    content = ss.str();
}
