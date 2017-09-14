#ifndef __myImageLoader_h
#define __myImageLoader_h

#include <itkImageSource.h>
#include <string>
namespace itk
{
	/*
	Carrega a imagem da forma como eu quero.
	Muitas das imagens que eu vou usar serão geradas pelo paint.net. Ele grava como multicanal mesmo quando é tudo cinza. Então eu preciso
	remover os multicanais e aproveito que já estou mexendo na imagem para fazer com que seus valores fiquem entre zero e um. Então essa
	classe carrega a imagem, a converte para luminância via RGBToLuminanceImageFilter e depois põe os escalares entre 0 e 1.
	Se a imagem já for em tom de cinza po-la como luminância não mudará muita coisa. Se for colorida, vai mudar. Como meu foco é imagens
	médicas que são tons de cinza isso não me importa de maneira alguma.
	*/
	template< class TImage>
	class myImageLoader :public ImageSource< TImage >
	{
	public:
		/** Standard class typedefs. */
		typedef myImageLoader         Self;
		typedef ImageSource< TImage > Superclass;
		typedef SmartPointer< Self >  Pointer;
		/** Method for creation through the object factory. */
		itkNewMacro(Self);
		/** Run-time type information (and related methods). */
		itkTypeMacro(myImageLoader, ImageSource);

		itkSetMacro(FileName, std::string);
	protected:
		myImageLoader(){
			m_FileName = "";
		}
		~myImageLoader(){}

		/** Does the real work. */
		virtual void GenerateData();
	private:
		myImageLoader(const Self &); //purposely not implemented
		void operator=(const Self &);  //purposely not implemented
		std::string m_FileName;
	};
} //namespace ITK


#ifndef ITK_MANUAL_INSTANTIATION
#include "myImageLoader.cpp"
#endif


#endif // __itkImageFilter_h