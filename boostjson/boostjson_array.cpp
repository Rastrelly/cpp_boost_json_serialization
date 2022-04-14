// использование библиотеки boost для сериализации данных в json формате

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <ctime>
#include <boost/json/src.hpp> //хедер библиотеки JSON

using namespace boost::json;
using namespace std;

struct dat
{
	vector<int> data;
	int n;
};

int main()
{

	srand(time(NULL));

	//Запись данных в json

	//создадим хранилище из структуры
	dat input;
	int n = rand() % 20 + 30;
	input.n = n;
	for (int i = 0; i < n; i++)
	{
		input.data.push_back(rand() % 30);
	}

	value jv =
	{
		{"amt",input.n},
		{"jdata",input.data},
	};
	
	
	//пишем заготовленный шаблон в файл
	
	serializer sr;
	ofstream dataser;
	dataser.open("outp_data.json");

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
	datared.open("outp_data.json");
	std::string line="",txt="";
	while (!datared.eof())
	{
		getline(datared, line);
		txt += line;
	}

	//преобразуем символы в json-хранилище
	stream_parser p;
	p.reset();
	p.write(txt);
	value fv = p.release();

	std::string fo = serialize(fv);
	
	//читаем данные из хранилища в структуру...
	dat soutput;

	//... используя функцию value_to()
	soutput = { value_to<vector<int>>(fv.at("jdata")),
	           value_to<int>(fv.at("amt")) };

	//выводим итог
	for (int i = 0; i < soutput.n; i++)
	{
		cout << i << ") " << soutput.data[i] << endl;
	}

	system("pause");

}