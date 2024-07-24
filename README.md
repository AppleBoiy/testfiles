# files generator

Various files generator for testing purposes.

## Libraries

    lodepng: 
    Library Path: lib/lodepng/lodepng.cpp
    Include Path: lib/lodepng

    libharu (for PDF generation if applicable):
    Include Path: /opt/homebrew/Cellar/libharu/2.4.4/include
    
    libjpeg: 
    Find Package: find_package(JPEG REQUIRED)

    gif_lib (for GIF generation):
    Find Package: find_package(GIF REQUIRED)
    Include Path: lib/gif

## Usage

example: main.cpp
modify the main.cpp file to generate the desired file type.

```c++
#include "image/ImageGenerator.h"
#include "text/TextFileGenerator.h"
#include "Generator.h"

void generateImage() {
    // RASTER Image Generator Example
    ImageGenerator generatorPNG(400, 400, ImageFormat::PNG);
    generatorPNG.create("image.png");
    generatorPNG.applyColorMode(ColorMode::NONE);
    generatorPNG.save();

    // ... rest of the raster image formats
}

void generateTextFile() {
    // Text File Generator Example
    TextFileGenerator generatorJSON(100, TextFormat::JSON);
    generatorJSON.create("data.json");
    generatorJSON.save();

    // ... rest of the text file formats
}


int main() {
//    generateImage();
//    generateTextFile();
    Generator generator("file", FileType::EXECUTABLE);
    generator.generate();
    return 0;
```

### Build

```shell
$ mkdir build && cd build
$ cmake ..
$ make
$ ./testfiles # this will generate the files in the build/dist directory
```

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

