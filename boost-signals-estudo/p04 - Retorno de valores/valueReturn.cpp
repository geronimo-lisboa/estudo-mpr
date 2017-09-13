//http://www.boost.org/doc/libs/1_54_0/doc/html/signals2/tutorial.html#idp164790912
#include <boost\signals2.hpp>//O header da boost. Esse header precisa que sua lib seja inclusa.
#include <boost\lexical_cast.hpp>
#include <iostream>
#include <memory>
#include <iostream>
using boost::lexical_cast;
//Um objetozinho que será usado como retorno.
class ResultData
{
private:
public:
	const std::string testData;

	ResultData(std::string str) :testData(str){

	}
};

//A classe do slot
class Bagulho{
private:
	const int id;
public:
	Bagulho(int i) : id(i){
	}
	std::shared_ptr<ResultData> operator()(std::string param)
	{
		std::shared_ptr<ResultData> myData = std::make_shared<ResultData>(param + " " + lexical_cast<std::string>(id));
		return myData;
	}
};

int main(int argc, char** argv){
	//Cria o sinal que retorna o objeto do tipo result data
	boost::signals2::signal<std::shared_ptr<ResultData>(std::string)> mySignal;
	//Cria o slot e liga ao sinal
	std::shared_ptr<Bagulho> mySlot = std::make_shared<Bagulho>(0);
	mySignal.connect(*mySlot);
	//Chama o sinal e pega o resultado.
	std::shared_ptr<ResultData> resultFromSlot = *mySignal("Vamos Flamengo");
	std::cout << resultFromSlot->testData << std::endl;
	return 0;
}


