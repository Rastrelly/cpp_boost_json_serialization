// использование библиотеки boost для сериализации данных в json формате

#include <iostream>
#include <string>
#include <fstream>
#include <boost/json/src.hpp> //хедер библиотеки JSON

using namespace boost::json;
using namespace std;

struct dat
{
	float val; bool n; std::string nm;
};

int main()
{
	//Запись данных в json

	//создадим хранилище из структуры
	dat input = {12.7f,true,"Testroll"};

	printf("Object input:\n%f\n%s\n%s\n\n", input.val, input.n ? "true" : "false", input.nm.c_str());

	value jv =
	{
		{"jval",input.val},
		{"jn",input.n},
		{"jnm",input.nm}
	};
	
	
	//пишем заготовленный шаблон в файл
	
	serializer sr;
	ofstream dataser;
	dataser.open("outp.json");

	// включаем сериализатор с указателем на наше хранилище
	sr.reset(&jv);

	// пишем данные циклом
	while (!sr.done())
	{
		// используем отдельный буфер
		char buf[BOOST_JSON_STACK_BUFFER_SIZE];
		// заполняем буфер символами
		dataser << sr.read(buf);
	}

	dataser.close();

	//Чтение данных из json
	
	//обычное считывание символов через ifstream
	ifstream datared;
	datared.open("outp.json");
	std::string line="",txt="";
	while (!datared.eof())
	{
		getline(datared, line);
		txt += line;
	}

	printf("Intermediate output:\n%s\n\n", txt.c_str());

	//преобразуем символы в json-хранилище
	stream_parser p;
	p.reset();
	p.write(txt);
	value fv = p.release();

	std::string fo = serialize(fv);
	printf("Final output:\n%s\n\n", fo.c_str());

	//читаем данные из хранилища в структуру...
	dat soutput = {0.0f,false,""};

	//... используя функцию value_to()
	soutput = { value_to<float>(fv.at("jval")),
	           value_to<bool>(fv.at("jn")),
	           value_to<std::string>(fv.at("jnm")) };

	//выводим итог
	printf("Object output:\n%f\n%s\n%s\n\n",soutput.val,soutput.n ? "true" : "false",soutput.nm.c_str());

	system("pause");

}