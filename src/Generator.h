#ifndef TESTFILES_GENERATOR_H
#define TESTFILES_GENERATOR_H

#include <vector>
#include <string>

enum class FileType {
    PDF,
    EXECUTABLE,
    BINARY
};

class Generator {
public:
    Generator(std::string filename, FileType type, int numPages = 1);

    void generate();

private:
    void generatePDF();

    void generateExecutable();

    void generateBinary();

    void generate_random_content();

    std::string filename;
    FileType type;
    int numPages = 1;
    std::vector<std::string> contents;


};


#endif //TESTFILES_GENERATOR_H
