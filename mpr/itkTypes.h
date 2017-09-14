#pragma once
#include <itkImage.h>
#include "myImageLoader.h"
typedef itk::Image<float, 2> ImageType;
typedef itk::myImageLoader<ImageType> ImageLoaderType;