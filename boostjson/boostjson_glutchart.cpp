// использование библиотеки boost для сериализации данных в json формате

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <ctime>
#include <boost/json/src.hpp> //хедер библиотеки JSON
#include <GL/freeglut.h>

using namespace boost::json;
using namespace std;

float vmin=0, vmax=0;
float h=0;

struct dat
{
	vector<float> data;
	int n;
};

dat soutput;

void display()
{
  //renderer
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  int ne = soutput.n;
  float w = 800 / ne;
  
  float nh = 600;
  float ch = nh/h;
  
  for (int i = 0; i < ne; i++)
  {
	  glBegin(GL_LINES);
	  glColor3f(1.0f,0.0f,0.0f);
	     glVertex2f((float)i*w,     (soutput.data[i]-vmin)*ch  );
		 glVertex2f((float)(i+1)*w, (soutput.data[i+1]-vmin)*ch);
	  glEnd();
  }
  glutSwapBuffers();

}

void getminmax()
{
	vmin = soutput.data[0];
	vmax = soutput.data[0];
	for (int i = 1; i < soutput.n; i++)
	{
		if (vmin < soutput.data[i]) vmin = soutput.data[i];
		if (vmax > soutput.data[i]) vmax = soutput.data[i];
	}
}

int main(int argc, char **argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(200, 200);

	glutCreateWindow("Window");

	glClearColor(0, 0, 0, 0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 800, 600, 0, -100, 100);
	glViewport(0, 0, 800, 600);

	srand(time(NULL));

	//Запись данных в json

	//создадим хранилище из структуры
	dat input;
	int n = rand() % 20 + 30;
	input.n = n;
	for (int i = 0; i < n+1; i++)
	{
		float x = i - n / 2;
		input.data.push_back(x*x*5-30);
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
	
	//... используя функцию value_to()
	soutput = { value_to<vector<float>>(fv.at("jdata")),
	           value_to<int>(fv.at("amt")) };

	//выводим итог
	for (int i = 0; i < soutput.n; i++)
	{
		cout << i << ") " << soutput.data[i] << endl;
	}

	getminmax();
	h = vmax - vmin;

	glutDisplayFunc(display);

	glutMainLoop();

	system("pause");

}