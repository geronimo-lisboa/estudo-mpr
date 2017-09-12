//O exemplo mais simples de Signals na boost.
//Signals2 é um sistema de slots - sinais similar ao da Qt.Sinais são callbacks com multiplos alvos, também conhecido como eventos. Sinais são
//conectados a slots , os recebedores dos callbacks.
//Nesse exemplo eu crio o sinal(mySignal) e conecto a ele um slot, o HelloWorld.
#include <boost\signals2.hpp>
#include <iostream>
struct HelloWorld {
	void operator()()const
	{
		std::cout << "PING" << std::endl;
	}
};

int main(int argc, char** argv)
{
	boost::signals2::signal<void()> mySignal;
	HelloWorld hello;
	mySignal.connect(hello);
	mySignal();

	return 0;
}