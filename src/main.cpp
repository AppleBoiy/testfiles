#include "image/ImageGenerator.h"
#include "text/TextFileGenerator.h"
#include "Generator.h"

void generateImage() {
    // RASTER Image Generator Example
    ImageGenerator generatorPNG(400, 400, ImageFormat::PNG);
    generatorPNG.create("image.png");
    generatorPNG.applyColorMode(ColorMode::NONE);
    generatorPNG.save();

    ImageGenerator generatorJPG(400, 400, ImageFormat::JPG);
    generatorJPG.create("image.jpg");
    generatorJPG.applyColorMode(ColorMode::NONE);
    generatorJPG.save();

    ImageGenerator generatorPPM(400, 400, ImageFormat::PPM);
    generatorPPM.create("image.ppm");
    generatorPPM.applyColorMode(ColorMode::NONE);
    generatorPPM.save();

    ImageGenerator generatorGIF(400, 400, ImageFormat::GIF);
    generatorGIF.create("image.gif");
    generatorGIF.save();


    // VECTOR Image Generator Example
    ImageGenerator generatorSVG(400, 400, ImageFormat::SVG);
    generatorSVG.create("image.svg");
    generatorSVG.applyColorMode(ColorMode::NONE);
    generatorSVG.save();

}

void generateTextFile() {
    // Text File Generator Example
    TextFileGenerator generatorJSON(100, TextFormat::JSON);
    generatorJSON.create("data.json");
    generatorJSON.save();

    TextFileGenerator generatorCSV(100, TextFormat::CSV);
    generatorCSV.create("data.csv");
    generatorCSV.save();

    TextFileGenerator generatorTXT(100, TextFormat::TXT);
    generatorTXT.create("data.txt");
    generatorTXT.save();

    TextFileGenerator generatorLOG(100, TextFormat::LOG);
    generatorLOG.create("data.log");
    generatorLOG.save();
}


int main() {
    generateImage();
    generateTextFile();

    Generator generator("file", FileType::EXECUTABLE);
    generator.generate();

    Generator generator1("file.bin", FileType::BINARY);
    generator1.generate();

    Generator generator2("file.pdf", FileType::PDF, 2);
    generator2.generate();

    return 0;
}
