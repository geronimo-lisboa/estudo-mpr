#include <GL/glew.h>
#include <Windows.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "itkTypes.h"
#include "misc.h"
#include <memory>
#include <vector>
#include "shaderUtils.h"
#include <map>
#include "rendering3d.h"
#include "loadDicom.h"

#include <itkResampleImageFilter.h>
#include <itkIdentityTransform.h>
#include <itkImageFileWriter.h>

const std::string imagePath ="C:\\programacao\\estudo-mpr\\mpr\\"
;
const int screenWidth = 800;
const int screenHeight = 800;

int main(int argc, char** argv)
{
	try
	{
		//1) Carga da imagem
		Short3DImageType::Pointer originalImage = loadDicom("C:\\dicom\\Marching Man");
		//Experiencia pra fazer o resize
		// Resize
#ifdef __teste_resample
		Short3DImageType::SizeType inputSize = originalImage->GetLargestPossibleRegion().GetSize();
		Short3DImageType::SizeType outputSize;
		outputSize[0] = inputSize[0] / 2;
		outputSize[1] = inputSize[1] / 2;
		outputSize[2] = inputSize[2] / 2;
		Short3DImageType::SpacingType outputSpacing;
		outputSpacing[0] = originalImage->GetSpacing()[0] * 2;
		outputSpacing[1] = originalImage->GetSpacing()[1] * 2;
		outputSpacing[2] = originalImage->GetSpacing()[2] * 2;
		typedef itk::IdentityTransform<double, 3> TransformType;
		typedef itk::ResampleImageFilter<Short3DImageType, Short3DImageType> ResampleImageFilterType;
		ResampleImageFilterType::Pointer resample = ResampleImageFilterType::New();
		resample->SetOutputParametersFromImage(originalImage);
		resample->SetInput(originalImage);
		resample->SetSize(outputSize);
		resample->SetOutputSpacing(outputSpacing);
		resample->SetTransform(TransformType::New());
		resample->UpdateLargestPossibleRegion();
		Short3DImageType::Pointer output = resample->GetOutput();
		itk::ImageFileWriter<Short3DImageType>::Pointer writer = itk::ImageFileWriter<Short3DImageType>::New();
		writer->SetInput(output);
		writer->SetFileName("c:\\dicom\\teste.mha");
		writer->Write();
#endif

		//2)Criação da janela/contexto/blablabla da glfw.
		GLFWwindow* window;
		//Seta o callback de erro.
		glfwSetErrorCallback([](int error, const char* description){
			std::stringstream ss;
			ss << "Erro: " << error << " : " << description;
			throw std::exception(ss.str().c_str());
		});
		//Se falhou em iniciar, morre
		if (!glfwInit()) { throw std::exception("Falha no glfwInit(). Tem algo MUITO errado acontecendo"); }
		//OpenGL 3.2
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		window = glfwCreateWindow(screenWidth, screenHeight, "BOA NOITE CARALHO", NULL, NULL);//A criação da janela é aqui
		if (!window)//Se falhou em criar a janela, morre.
		{
			glfwTerminate();
			throw std::exception("Não foi possivel criar a janela");
		}
		//Seta o callback de tecla;
		glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)//Esc = encerra o programa.
				glfwSetWindowShouldClose(window, GLFW_TRUE);
		});
		glfwMakeContextCurrent(window);
		// start GLEW extension handler
		glewExperimental = GL_TRUE;
		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			/* A fonte de erro mais comum é tentar iniciar a glew sem ter um contexto opengl funcionando,
			sem ter a tela ativa.*/
			std::stringstream ss;
			ss << "Erro de glew: " << glewGetErrorString(err);
			throw std::exception(ss.str().c_str());
		}
		//O makeCurrent() da glfw
		glfwSwapInterval(1);
		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

		shared_ptr<Object3d> obj = nullptr;
		bool isInitialized = false;
		while (!glfwWindowShouldClose(window))
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			if (!isInitialized)
			{
				obj = make_shared<Object3d>(imagePath + "vertexShader.glsl", imagePath + "fragmentShader.glsl", output);
				isInitialized = true;
			}
			else
			{
				obj->Render();
			}
			glfwPollEvents();
			glfwSwapBuffers(window);

		}
		//4) Fim do loop principal
		//Limpa tudo e morre.
		glfwDestroyWindow(window);
		glfwTerminate();
		exit(EXIT_SUCCESS);

	}
	catch (std::exception &ex)
	{
		std::cout << "Erro : " << ex.what() << endl;
		std::cout << "Pressione qqer tecla para continuar...";
		getchar();
		exit(EXIT_FAILURE);
	}

}