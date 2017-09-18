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

#include <Eigen/Geometry>
#include <vtkPlaneSource.h>
#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkPoints.h>
using namespace Eigen;

//const std::string imagePath = "C:\\programacao\\estudo-mpr\\mpr\\"
const std::string imagePath = "C:\\teste\mpr\\";
//const std::string dicomDir = "C:\\dicom\\Marching Man";
const std::string dicomDir = "C:\\meus dicoms\\Marching Man";
const int screenWidth = 800;
const int screenHeight = 800;

int main(int argc, char** argv)
{
	try
	{
		//1) Carga da imagem
		Short3DImageType::Pointer originalImage = loadDicom(dicomDir);
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
		//teste, twntando calcular as texture coordinates usando a eigen
		Matrix3f m;
		m = AngleAxisf((30 * 3.14) / 180, Vector3f::UnitX())
			* AngleAxisf((0 * 3.14) / 180, Vector3f::UnitY())
			* AngleAxisf((0 * 3.14) / 180, Vector3f::UnitZ());
		cout << m << endl << "is unitary: " << m.isUnitary() << endl;
		Vector3f N, U, V;
		N << 0, 1, 0;
		U << 1, 0, 0;
		V << 0, 0, 1;
		N = V.cross(U);
		Vector3f _N, _U, _V;
		_N << -0.866, 0.5, 0;
		_U = _N.cross(V);
		_V = _N.cross(U);
		_N = _V.cross(_N);
		//Teste, tentando calcular com a vtkPlaneSource - Com isso aqui eu estou perto de conseguir as coordenadas de textura que eu quero.
		//Elas ainda estão entre -0.5, 0.5, mas parece que servem já.
		vtkSmartPointer<vtkPlaneSource> ps = vtkSmartPointer<vtkPlaneSource>::New();
		ps->SetCenter(0.5, 0.5, 0.5);
		ps->Update();
		vtkSmartPointer<vtkPolyData> resultingPlane = ps->GetOutput();
		array<double, 3> pt0, pt1, pt2, pt3;
		resultingPlane->GetPoint(0, pt0.data());
		resultingPlane->GetPoint(1, pt1.data());
		resultingPlane->GetPoint(2, pt2.data());
		resultingPlane->GetPoint(3, pt3.data());

		ps->SetNormal(0.866, -0.5, 0.0);
		ps->SetCenter(0.5, 0.5, 0.5);
		ps->Update();
		resultingPlane = ps->GetOutput();
		resultingPlane->GetPoint(0, pt0.data());
		resultingPlane->GetPoint(1, pt1.data());
		resultingPlane->GetPoint(2, pt2.data());
		resultingPlane->GetPoint(3, pt3.data());

		/////Lá em baixo setNormal faz isso aqui:

		//void vtkPlaneSource::SetNormal(double N[3])
		//{
		//	double n[3], rotVector[3], theta;
		//	//make sure input is decent
		//	n[0] = N[0];
		//	n[1] = N[1];
		//	n[2] = N[2];
		//	if (vtkMath::Normalize(n) == 0.0)
		//	{
		//		vtkErrorMacro(<< "Specified zero normal");
		//		return;
		//	}
		//	// Compute rotation vector using a transformation matrix.
		//	// Note that if normals are parallel then the rotation is either
		//	// 0 or 180 degrees.
		//	double dp = vtkMath::Dot(this->Normal, n);
		//	if (dp >= 1.0)
		//	{
		//		return; //zero rotation
		//	}
		//	else if (dp <= -1.0)
		//	{
		//		theta = 180.0;
		//		rotVector[0] = this->Point1[0] - this->Origin[0];
		//		rotVector[1] = this->Point1[1] - this->Origin[1];
		//		rotVector[2] = this->Point1[2] - this->Origin[2];
		//	}
		//	else
		//	{
		//		vtkMath::Cross(this->Normal, n, rotVector);
		//		theta = vtkMath::DegreesFromRadians(acos(dp));
		//	}
		//	// create rotation matrix
		//	vtkTransform *transform = vtkTransform::New();
		//	transform->PostMultiply();
		//	transform->Translate(-this->Center[0], -this->Center[1], -this->Center[2]);
		//	transform->RotateWXYZ(theta, rotVector[0], rotVector[1], rotVector[2]);
		//	transform->Translate(this->Center[0], this->Center[1], this->Center[2]);
		//	// transform the three defining points
		//	transform->TransformPoint(this->Origin, this->Origin);
		//	transform->TransformPoint(this->Point1, this->Point1);
		//	transform->TransformPoint(this->Point2, this->Point2);
		//	this->Normal[0] = n[0]; this->Normal[1] = n[1]; this->Normal[2] = n[2];
		//	this->Modified();
		//	transform->Delete();
		//}



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
				obj = make_shared<Object3d>(imagePath + "vertexShader.glsl", imagePath + "fragmentShader.glsl", originalImage);
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