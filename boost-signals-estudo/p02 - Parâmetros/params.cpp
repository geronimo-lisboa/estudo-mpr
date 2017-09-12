#include <boost\signals2.hpp>//O header da boost. Esse header precisa que sua lib seja inclusa.
#include <iostream>
//Essa classe trata dois signals
class Bagulho{
private:
	const int id;
public:
	Bagulho(int i) : id(i){
	}
	//Tratando um tipo de signal
	void operator()(){
		std::cout << "Slot tipo 1" <<" bagulho id = "<<id<< std::endl;
	}
	//Tratando outro tipo de signal
	void operator()(int x){
		std::cout << "Slot tipo 2, param = "<<x <<" bagulho id = "<<id<< std::endl;
	}
};
int main(int argc, char** argv){
	//Dois tipos de sinal
	boost::signals2::signal<void()> sinalTipo1;
	boost::signals2::signal<void(int)> sinalTipo2;
	Bagulho *bagulho0 = new Bagulho(0);
	Bagulho *bagulho1 = new Bagulho(1);
	//Conecta os slots
	//Não posso passar o ponteiro mas sim a referência para o valor apontado pelo ponteiro, então uma dereferência basta.
	sinalTipo1.connect(*bagulho0);
	sinalTipo2.connect(*bagulho0);
	sinalTipo1.connect(*bagulho1);
	sinalTipo2.connect(*bagulho1);
	//Chama.
	sinalTipo1();
	sinalTipo2(10);
	return 0;
}


