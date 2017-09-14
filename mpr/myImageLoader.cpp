#ifndef __myImageLoader_hxx
#define __myImageLoader_hxx

#include "myImageLoader.h"
#include "itkObjectFactory.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include <itkImageFileReader.h>
#include <itkImage.h>
#include <itkRGBAPixel.h>
#include <itkRGBToLuminanceImageFilter.h>
#include <itkIntensityWindowingImageFilter.h>
namespace itk
{
	template< class TImage>
	void myImageLoader< TImage>::GenerateData()
	{
		if (m_FileName == "") throw itk::ExceptionObject("Must inform the filename");
		//Carga do arquivo
		typedef unsigned char                           ComponentType;
		typedef itk::RGBPixel< ComponentType >          InputPixelType;
		typedef itk::Image< InputPixelType, 2 > InputImageType;
		typedef itk::ImageFileReader< InputImageType >  ReaderType;
		ReaderType::Pointer reader = ReaderType::New();
		reader->SetFileName(m_FileName.c_str());

		typedef itk::RGBToLuminanceImageFilter< InputImageType, typename TImage > RGBToLuminanceType;
		RGBToLuminanceType::Pointer filter = RGBToLuminanceType::New();
		filter->SetInput(reader->GetOutput());
		filter->Update();

		typedef itk::IntensityWindowingImageFilter<typename TImage, typename TImage> IntensityFilter;
		IntensityFilter::Pointer normalizer = IntensityFilter::New();
		normalizer->SetInput(filter->GetOutput());
		normalizer->SetWindowMinimum(0);
		normalizer->SetWindowMaximum(255.0);
		normalizer->SetOutputMinimum(0);
		normalizer->SetOutputMaximum(1.0);
		normalizer->Update();

		typename TImage::Pointer output = this->GetOutput();
		typename TImage::RegionType region;
		typename TImage::IndexType start;
		start[0] = 0;
		start[1] = 0;
		if (output->ImageDimension == 3)start[1] = 0;
		typename TImage::SizeType size;
		size[0] = filter->GetOutput()->GetLargestPossibleRegion().GetSize()[0];
		size[1] = filter->GetOutput()->GetLargestPossibleRegion().GetSize()[1];
		if (output->ImageDimension==3)
			size[2] = filter->GetOutput()->GetLargestPossibleRegion().GetSize()[2];
		region.SetSize(size);
		region.SetIndex(start);
		output->SetRegions(region);
		output->Allocate();
		itk::ImageRegionIterator<TImage> outputIterator(output, output->GetLargestPossibleRegion());
		itk::ImageRegionIterator<TImage> inputIterator(normalizer->GetOutput(), normalizer->GetOutput()->GetLargestPossibleRegion());
		while (!outputIterator.IsAtEnd())
		{
			outputIterator.Set(inputIterator.Get());
			++inputIterator;
			++outputIterator;
		}
	}

}// end namespace


#endif
