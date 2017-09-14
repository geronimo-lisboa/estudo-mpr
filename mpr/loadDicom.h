#pragma once
#include <string>
#include <itkImage.h>
#include <itkImageSeriesReader.h>
#include <itkGDCMImageIO.h>
#include <itkGDCMSeriesFileNames.h>
#include <itkCommand.h>

//Typedefs
typedef itk::Image<short, 3> Short3DImageType;
typedef itk::ImageSeriesReader<Short3DImageType> Reader3dType;
typedef itk::GDCMImageIO ImageIOType;
typedef itk::GDCMSeriesFileNames NamesGeneratorType;
typedef std::vector< std::string >    SeriesIdContainer;
typedef std::vector< std::string >   FileNamesContainer;


namespace itk
{
	class myProgressObserver :public Command
	{
	public:
		//ITK macro - takes care of defining the static New() that all
		//itk object must have to be compatible with ITK's memory manegement.
		itkNewMacro(myProgressObserver);

		void Execute(itk::Object *caller, const itk::EventObject & event)
		{
			Execute((const itk::Object *)caller, event);
		}

		void Execute(const itk::Object * object, const itk::EventObject & event)
		{
			const itk::ProcessObject  *processObj = dynamic_cast<const itk::ProcessObject*>(object);
			std::cout << processObj->GetProgress() << std::endl;
		}
	};
}

static inline Short3DImageType::Pointer loadDicom(std::string pathToDir)
{
	Reader3dType::Pointer reader = Reader3dType::New();
	ImageIOType::Pointer imageIOObject = ImageIOType::New();
	reader->SetImageIO(imageIOObject);
	itk::myProgressObserver::Pointer obs = itk::myProgressObserver::New();
	NamesGeneratorType::Pointer nameGenerator = NamesGeneratorType::New();
	//NEW - Registers the observer
	nameGenerator->AddObserver(itk::ProgressEvent(), obs);
	nameGenerator->SetUseSeriesDetails(true);
	nameGenerator->SetDirectory(pathToDir);
	//This list has all the images in the directory
	SeriesIdContainer seriesUID = nameGenerator->GetSeriesUIDs();
	FileNamesContainer fileNames = nameGenerator->GetFileNames(seriesUID[0]);
	reader->SetFileNames(fileNames);
	reader->AddObserver(itk::ProgressEvent(), obs);
	reader->Update();
	Short3DImageType::Pointer result = reader->GetOutput();
	result->Print(std::cout);//My image, loaded.
	return result;
}
