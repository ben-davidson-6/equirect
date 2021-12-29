#include "image.h"

EquirectangularImage::EquirectangularImage(const char * filename): image(filename) {
    width = image.width();
    height = image.height();
}

EquirectangularImage::EquirectangularImage(CImg<unsigned char> &image): image(image) {
    width = image.width();
    height = image.height();
}

void EquirectangularImage::save(const char * filename) {
    image.save_jpeg(filename);
}

const CImg<unsigned char> EquirectangularImage::getImage() {
    return image;
}