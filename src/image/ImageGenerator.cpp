#include "ImageGenerator.h"
#include "lodepng.h"
#include <iostream>
#include <fstream>
#include <jpeglib.h>
#include <filesystem>
#include "gif.h"


void writeJPEG(const std::string &filename, const std::vector<unsigned char> &image, unsigned width, unsigned height) {
    jpeg_compress_struct cinfo{};
    jpeg_error_mgr jerr{};
    FILE *outfile;

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);

    if ((outfile = fopen(filename.c_str(), "wb")) == nullptr) {
        std::cerr << "Can't open " << filename << " for writing" << std::endl;
        return;
    }
    jpeg_stdio_dest(&cinfo, outfile);

    cinfo.image_width = width;
    cinfo.image_height = height;
    cinfo.input_components = 3; // RGB
    cinfo.in_color_space = JCS_RGB;

    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, 75, TRUE); // Quality level (0-100)

    jpeg_start_compress(&cinfo, TRUE);

    JSAMPROW row_pointer[1];
    std::vector<unsigned char> row(width * 3);

    while (cinfo.next_scanline < cinfo.image_height) {
        for (unsigned x = 0; x < width; ++x) {
            row[3 * x] = image[4 * (cinfo.next_scanline * width + x)];     // Red
            row[3 * x + 1] = image[4 * (cinfo.next_scanline * width + x) + 1]; // Green
            row[3 * x + 2] = image[4 * (cinfo.next_scanline * width + x) + 2]; // Blue
        }
        row_pointer[0] = row.data();
        jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }

    jpeg_finish_compress(&cinfo);
    fclose(outfile);
    jpeg_destroy_compress(&cinfo);
}

ImageGenerator::ImageGenerator(unsigned width, unsigned height, ImageFormat format)
        : width(width), height(height), mode(ColorMode::NONE), format(format) {}

void ImageGenerator::create(const std::string &file) {
    this->filename = file;
    image.assign(width * height * 4, 255); // RGBA, initialized to white

    // gradient
    for (unsigned y = 0; y < height; ++y) {
        for (unsigned x = 0; x < width; ++x) {
            unsigned char r = x % 256; // Red component
            unsigned char g = y % 256; // Green component
            unsigned char b = 128;     // Blue component (constant)
            unsigned char a = 255;     // Alpha component (fully opaque)

            // Calculate the index in the image buffer
            size_t index = 4 * (y * width + x);
            image[index] = r;    // Red
            image[index + 1] = g; // Green
            image[index + 2] = b; // Blue
            image[index + 3] = a; // Alpha
        }
    }
}

void ImageGenerator::applyColorMode(ColorMode colorMode) {
    if (format == ImageFormat::GIF) {
        std::cerr << "Color mode not supported for GIF images" << std::endl;
        return;
    }

    this->mode = colorMode;
    applyColorBlindMode();
}

void ImageGenerator::save() {
    std::filesystem::create_directories("dist");
    switch (format) {
        case ImageFormat::PNG:
            savePNG();
            break;
        case ImageFormat::PPM:
            savePPM();
            break;
        case ImageFormat::JPG:
            saveJPG();
            break;
        case ImageFormat::GIF:
            saveGIF();
            break;
        case ImageFormat::SVG:
            saveSVG();
            break;
        default:
            std::cerr << "Unsupported image format" << std::endl;
    }
}

void ImageGenerator::applyColorBlindMode() {
    if (mode == ColorMode::DEUTERANOPIA) {
        for (size_t i = 0; i < image.size(); i += 4) {
            unsigned char red = image[i];
            unsigned char green = image[i + 1];
            unsigned char blue = image[i + 2];

            // Simple transformation for deuteranopia
            image[i] = static_cast<unsigned char>(red * 0.8 + green * 0.2); // Red channel
            image[i + 1] = static_cast<unsigned char>(green * 0.8 + red * 0.2); // Green channel
            image[i + 2] = blue; // Blue channel remains unchanged
        }
    } else if (mode == ColorMode::BLACK_WHITE) {

        for (size_t i = 0; i < image.size(); i += 4) {
            unsigned char red = image[i];
            unsigned char green = image[i + 1];
            unsigned char blue = image[i + 2];

            // Calculate the grayscale value
            auto gray = static_cast<unsigned char>(0.299 * red + 0.587 * green + 0.114 * blue);

            // Set all channels to the grayscale value
            image[i] = gray;   // Red channel
            image[i + 1] = gray; // Green channel
            image[i + 2] = gray; // Blue channel
        }
    }
}

void ImageGenerator::savePNG() {
    std::vector<unsigned char> png;
    unsigned error = lodepng::encode(png, image, width, height);
    if (!error) {
        lodepng::save_file(png, "dist/" + filename);
        std::cout << "PNG image created: " << "dist/" + filename << std::endl;
    } else {
        std::cerr << "Encoder error " << error << ": " << lodepng_error_text(error) << std::endl;
    }
}

void ImageGenerator::savePPM() {
    std::ofstream file("dist/" + filename, std::ios::binary);
    if (file.is_open()) {
        file << "P6\n" << width << " " << height << "\n255\n";
        for (size_t i = 0; i < image.size(); i += 4) {
            file.put(static_cast<char>(image[i])); // Red channel
            file.put(static_cast<char>(image[i + 1])); // Green channel
            file.put(static_cast<char>(image[i + 2])); // Blue channel
        }
        file.close();
        std::cout << "PPM image created: " << "dist/" + filename << std::endl;
    } else {
        std::cerr << "Error creating PPM file: " << "dist/" + filename << std::endl;
    }
}

void ImageGenerator::saveJPG() {
    writeJPEG("dist/" + filename, image, width, height);
    std::cout << "JPEG image created: " << "dist/" + filename << std::endl;
}

void ImageGenerator::saveGIF() {
    GifWriter gif;
    int delay = 10;
    GifBegin(&gif, ("dist/" + filename).c_str(), width, height, delay);

    int numFrames = 30; // Number of frames in the GIF
    for (int frame = 0; frame < numFrames; ++frame) {
        // Create a gradient effect
        std::vector<uint8_t> frameData(width * height * 4, 0);

        for (unsigned y = 0; y < height; ++y) {
            for (unsigned x = 0; x < width; ++x) {
                // Calculate color values based on frame number
                unsigned char r = (x + frame * 5) % 256;
                unsigned char g = (y + frame * 5) % 256;
                unsigned char b = ((x + y + frame * 5) / 2) % 256;
                unsigned char a = 255; // Alpha channel (fully opaque)

                // Calculate the index in the image buffer
                size_t index = 4 * (y * width + x);
                frameData[index] = r;    // Red
                frameData[index + 1] = g; // Green
                frameData[index + 2] = b; // Blue
                frameData[index + 3] = a; // Alpha
            }
        }

        GifWriteFrame(&gif, frameData.data(), width, height, delay);
    }

    GifEnd(&gif);
    std::cout << "GIF image created: " << "dist/" + filename << std::endl;
}

void ImageGenerator::saveSVG() {
    std::ofstream file("dist/" + filename);
    if (file.is_open()) {
        file << R"(<svg xmlns="http://www.w3.org/2000/svg" width=")" << width << "\" height=\"" << height << "\">\n";
        file << "<rect width=\"100%\" height=\"100%\" fill=\"rgb(255,255,255)\"/>\n";
        for (size_t i = 0; i < image.size(); i += 4) {
            unsigned x = (i / 4) % width;
            unsigned y = (i / 4) / width;
            unsigned char r = image[i];
            unsigned char g = image[i + 1];
            unsigned char b = image[i + 2];
            file << "<rect x=\"" << x << "\" y=\"" << y << R"(" width="1" height="1" fill="rgb()"
                 << static_cast<int>(r) << "," << static_cast<int>(g) << "," << static_cast<int>(b) << ")\"/>\n";
        }
        file << "</svg>\n";
        file.close();
        std::cout << "SVG image created: " << "dist/" + filename << std::endl;
    } else {
        std::cerr << "Error creating SVG file: " << "dist/" + filename << std::endl;
    }
}
