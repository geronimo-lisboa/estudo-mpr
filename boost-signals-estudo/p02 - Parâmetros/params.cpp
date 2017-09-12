////Esse exemplo demonstra:
////1) Como usar uma classe como slot sem implementar interface alguma
////2) Como fazer sinais-slots que usem parâmetros.
////3) Como ter vários slots no mesmo sinal (sem controle de ordem de execução)
#include <boost\signals2.hpp>
#include <boost\lexical_cast.hpp>
#include <iostream>
using boost::lexical_cast;

class Bagulho
{
private:
	const int id;
public:
	Bagulho(int i) : id(i)
	{
	}
	//Um monte de coisas...
	void operator()()//Tratando um tipo de signal
	{
		std::cout << "Slot tipo 1" <<" bagulho id = "<<id<< std::endl;
	}
	void operator()(int x)//Tratando outro tipo de signal
	{
		std::cout << "Slot tipo 2, param = "<<x <<" bagulho id = "<<id<< std::endl;
	}
};

int main(int argc, char** argv)
{
	//Dois tipos de sinal
	boost::signals2::signal<void()> sinalTipo1;
	boost::signals2::signal<void(int)> sinalTipo2;

	Bagulho *bagulho0 = new Bagulho(0);
	Bagulho *bagulho1 = new Bagulho(1);

	//Conecta os slots
	sinalTipo1.connect(*bagulho0);//Não posso passar o ponteiro mas sim a referência para o valor apontado pelo ponteiro, então uma dereferência basta.
	sinalTipo2.connect(*bagulho0);
	sinalTipo1.connect(*bagulho1);
	sinalTipo2.connect(*bagulho1);

	//Chama.
	sinalTipo1();
	sinalTipo2(10);

	return 0;
}


