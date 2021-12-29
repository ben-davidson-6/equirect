#ifndef IMAGE_H
#define IMAGE_H

#include "CImg.h"

using namespace cimg_library;


class EquirectangularImage {
    public:
        EquirectangularImage(const char *);
        EquirectangularImage(CImg<unsigned char> &);
        const CImg<unsigned char> getImage();
        int width, height;
        void save(const char *);
    private:
        CImg<unsigned char> image;
};

#endif