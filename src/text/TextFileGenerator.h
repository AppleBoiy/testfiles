#ifndef TEXT_FILE_GENERATOR_H
#define TEXT_FILE_GENERATOR_H

#include <string>
#include <vector>

enum class TextFormat {
    JSON,
    CSV,
    TXT,
    LOG
};

class TextFileGenerator {
public:
    explicit TextFileGenerator(unsigned numEntries, TextFormat format = TextFormat::TXT);

    void create(const std::string &filename);

    void save();

private:
    unsigned numEntries;
    std::string filename;
    TextFormat format;
    std::string content;

    void generateRandomContent();

    void generateJSONContent();

    void generateCSVContent();

    void generateTXTContent();

    void generateLOGContent();
};

#endif // TEXT_FILE_GENERATOR_H
