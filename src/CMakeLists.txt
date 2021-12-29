project(equirect)

set(HEADER_FILES includes/image.h includes/sphere.h)

set(SOURCE_FILES image.cpp sphere.cpp transformations.cpp)

add_library(equirect STATIC ${SOURCE_FILES} ${HEADER_FILES})