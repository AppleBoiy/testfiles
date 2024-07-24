#include "Generator.h"
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <filesystem>
#include <utility>
#include <hpdf_doc.h>
#include <hpdf.h>
#include <sstream>

Generator::Generator(std::string filename, FileType type, int numPages)
        : filename(std::move(filename)), type(type), numPages(numPages) {
    std::filesystem::create_directory("dist");
}

void Generator::generate() {
    switch (this->type) {
        case FileType::PDF:
            this->generate_random_content();
            generatePDF();
            break;
        case FileType::EXECUTABLE:
            generateExecutable();
            break;
        case FileType::BINARY:
            generateBinary();
            break;
        default:
            throw std::invalid_argument("Unsupported file type");
    }
}

void Generator::generate_random_content() {
    for (int i = 0; i < numPages; i++) {
        std::string content;
        for (int j = 0; j < 10; j++) {
            content += "Page " + std::to_string(i) + " Line " + std::to_string(j) + "\n";
        }
        contents.push_back(content);
    }
}

void Generator::generatePDF() {

    HPDF_Doc pdf;
    HPDF_Font font;
    HPDF_Page page;

    pdf = HPDF_New(nullptr, nullptr);
    if (!pdf) {
        throw std::runtime_error("Error creating PDF object");
    }

    font = HPDF_GetFont(pdf, "Helvetica", nullptr);
    if (!font) {
        throw std::runtime_error("Font not found");
    }

    for (int i = 0; i < numPages; i++) {
        page = HPDF_AddPage(pdf);
        HPDF_Page_SetSize(page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);
        HPDF_Page_SetTextLeading(page, 12);
        HPDF_Page_BeginText(page);
        HPDF_Page_SetFontAndSize(page, font, 12);
        HPDF_REAL height = HPDF_Page_GetHeight(page);

        // get the content for this page
        const auto &content = contents[i];
        std::istringstream stream(content);
        std::string line;
        HPDF_REAL y_pos = height - 20;
        while (std::getline(stream, line)) {
            HPDF_Page_TextOut(page, 20, y_pos, line.c_str());
            y_pos -= 12;
        }

        HPDF_Page_EndText(page);
    }

    HPDF_SaveToFile(pdf, ("dist/" + filename).c_str());

    if (HPDF_GetError(pdf) != HPDF_OK) {
        throw std::runtime_error("Error saving PDF file");
    }

    HPDF_Free(pdf);
    std::cout << "PDF file created: " << "dist/" + filename << std::endl;
}

void Generator::generateExecutable() {

    // create temporary file
    std::ofstream file("dist/" + filename + ".cpp");
    auto content = R"(int main() {"AppleBoiy!"; return 0;})";
    file << content;
    file.close();
    // compile the file (disable warnings)
    std::string command = "g++ -w dist/" + filename + ".cpp -o dist/" + filename;
    system(command.c_str());

    // remove the temporary file
    std::filesystem::remove("dist/" + filename + ".cpp");

    std::cout << "Executable file created: " << "dist/" + filename << std::endl;
}

void Generator::generateBinary() {
    std::ofstream file("dist/" + filename, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file for writing");
    }

    const auto key = 0xAA;
    for (int i = 0; i < 128; i++) {
        char byte = static_cast<char>(i ^ key); // XOR encryption
        file.write(&byte, sizeof(byte)); // Write the byte to the file
    }

    file.close();
    std::cout << "Binary file created: " << "dist/" + filename << std::endl;
}
