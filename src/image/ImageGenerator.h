#ifndef IMAGE_GENERATOR_H
#define IMAGE_GENERATOR_H

#include <string>
#include <vector>

enum class ColorMode {
    NONE,
    DEUTERANOPIA,
    BLACK_WHITE
};

enum class ImageFormat {
    // raster
    PNG,
    PPM,
    JPG,
    GIF,

    // vector
    SVG,
};

class ImageGenerator {
public:
    ImageGenerator(unsigned width, unsigned height, ImageFormat format = ImageFormat::PNG);

    void create(const std::string &filename);

    void applyColorMode(ColorMode mode);

    void save();

private:
    unsigned width;
    unsigned height;
    std::string filename;
    std::vector<unsigned char> image;
    ColorMode mode;
    ImageFormat format;

    void applyColorBlindMode();

    void savePNG();

    void savePPM();

    void saveJPG();

    void saveGIF();

    void saveSVG();

};

#endif // IMAGE_GENERATOR_H
