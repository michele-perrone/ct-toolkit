# CT Toolkit - A simple Computer Tomography toolkit (Work in progress...)
This program can be run in GUI mode or command-line mode.  
Main features:
* Visualization of grayscale 512x512 computer tomography (CT) images
* Colorization of CT images with LUT color palettes 
* Compression of grayscale CT images using a recursive quad-tree algorithm

## Installation
Compile from source using [the CMake file](/src/CMakeLists.txt) (instructions will follow soon) or the download the latest binary release for Linux, Windows or MacOS (to be added soon).

## Usage
The program can be used in command-line mode or GUI mode. For most users, GUI mode is recommended and is meant to be as straightforward as possible. What the GUI does is calling the functions implemented by command-line program. It allows:
* Loading uncompressed CT images and showing them in grayscale or color (if a LUT palette is loaded)
* Loading previously compressed CT images with the quad-tree algorithm and showing them in grayscale or color (if a LUT palette is loaded)
* Loading an arbitrary LUT color palette to be applied upon a CT image
* Saving the compressed grayscale CT image

Saving the colorized CT image is currently unsupported (will do).   
You can try out the features on the CT images and LUT tables in [SampleImages/](/SampleImages) and [SampleLUTs/](/SampleLUTs) respectively.

## How does it work
### About quad-tree compression
Quad-tree compression is a (potentially) lossy recursive algorithm. The idea is to find regions of grayscale images which share the same or similar luminance. We expect it to work well with CT images, mainly due to their extensive black areas. The algorithm follows these steps:
1. Subdivide the grayscale image in four equal squares
2. Check inside each square if the luminance is the same for every pixel (or above some threshold). We traverse the square as a matrix row, checking the difference between each pixel with the top-left pixel. Alternatively, thresholding could be implemented by comparing the luminance of the current pixel with the average luminance of the entire square
3. If the luminance differences inside the square are below the threshold, we can store the information about the square. The top-left coordinate, bottom-right coordinate and luminance value suffice
4. If the luminance differences inside the square are below the threshold, go back to step 1 and subdivide the current square into four squares.
5. Stop subdividing once all squares contain pixels with the luminance differences above the threshold

### CT images and colorization
CT images have an .IMG extension and a fixed resolution of 512*512 pixels. Each pixel has a signed 12-bit luminance value ranging from -2048 to 2047 (-2^12/2 to 2^12/2-1).   
LUT tables associate each possible luminance value to a 24-bit RGB color value (8 bits per color channel, i.e. 0 to 255). They can be seen as matrices with 256 rows and 3 columns, where each row corresponds to a luminance value. The only thing we have to be careful about is that original [-2048, 2047) luminance range has to be rescaled in order to pick the correct LUT color:   

´LUT color index = ( ( CT luminance + 2048 ) / 4095 ) * 255´


### Code structure (still not there)
Head over to the [source folder](/src) if you want to know how the code is structured.

## TODO
* Status bar when compressing / decompressing images
* Save uncompressed colorized images
* Save compressed colorized images
* Improve the GUI: window resizing depending on screen resolution and image size, widget scaling, theming...
* Optimize BinaryTools (current bottleneck for compression / decompression)

