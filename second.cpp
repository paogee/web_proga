#include <iostream>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fstream>
using namespace std;

class Game{//такой же как и first, только чутка другие функции
public:
	Game(); //получение ответа пользователя
	~Game();//деструктор
	void play(); //игра и формирование html документа
private:
	void javaProv(); //проверка отправляемого результата локально
	char * parse(char * url); //парсинг урла
	char * check(char *a, char *c); //проверка ответа
	void win(); //Если мы выйграли
	void lose(); //Если проиграли + вывод подсказок
	void printfile(const char *path); //добавление скрипта проверки
	char *answer; //ответ
};

class List {//Взят со второй задачи
public:
	List() { head = NULL; };
	~List();
	struct node {
		char* key;	 //ключ
		char* value; //значение
		node *next;  //указ-ль на след.
	};
	node* getHead() const;  //получение головы списка
	int empty() const;	//проверка на пустоту
	void addHead(const char* key, const char* value);	//задать голову
	node* insert(node* n, const char* key, const char* value); // добавление эл-та в список
	void print(); //печать
private: 
	node* head;
};

class parser {//взят со второй задачи и чутка переделан.
public:
	parser(); //парсер
	List::node * getdata() const;
private:
	char *query();
	char *get();
	char *post();
	List list; //список
};

struct fileInfo{//структура файла. Это правилльный ответ и кол-во попыток
	char key[5];
	int hintsCount;
};


class history{ //класс для работы с подсказками
public:
	history(const char * pi, const char * ph); //сохранение пути к файлу с информацией и к файлу с подсказками. pi-путь к info. ph-путь к hints
	fileInfo * getInfo(); //получение информации из файла
	void addhint(char *a, char *h); //добавление подсказки h и ответа a
	void update(fileInfo * i); //обновление инофрмации 
	void print(int count); //печать подсказок
	~history(); //удаление путей
private:
	char * path_to_info, //путь к файлу с информацией
		* path_to_hints; //путь к файлу c подсказкой
};

int main(){//создаем страницу и работаем с url
	cout << "Content-type: text/html\n\n";
	cout << "<html>\n";
	cout << "<head>\n";
	cout << "<title>Red Bull</title>\n";
	cout << "</head>\n";
	cout << "<body>\n";
	
	Game bestGame;
	bestGame.play();
	
	cout << "</body>\n";
	cout << "</html>\n";
}

Game::Game(){//консруктор. Создаем объект класса Game
	parser p;//читаем URL
	List::node * temp = p.getdata();//записываем в list, так как p удалится.
	answer = new char[strlen(temp->value)+1];//создаем строку ответа
	strcpy(answer, temp->value);//туда записываем
	//вся эта тема нужна, чтобы не было траблов с памятью.
}

Game::~Game(){//просто деструктор. Мы же выделяем память под answer, значит должны и удалить
	delete []answer;
}

void Game::play(){//старт нового хода игры
	history Hints("info.txt", "hints.txt");//открываем файл подсказок
	
	fileInfo * hintsInfo = Hints.getInfo();//получаем кол-во попыток и правильный ответ.
	char * code = hintsInfo->key;//узнаем правильный ответ из файлика
	
	if(!strcmp(answer, code))//вдруг ведденый ответ сходится с правильным, то победа
		win();
	else{
		lose();//иначе выводим поражение
		char * hint = check(answer, code);//генерируем подсказку
		cout << "Твои жалкие попытки победить:<br>\n";
		int count = (hintsInfo->hintsCount)++;//увеличиваем кол во попыток
		Hints.addhint(answer, hint);//добавляем подсказку в файлик
		Hints.update(hintsInfo);//обновляем информацию в инфо
		Hints.print(count+1);//печатаем попытку в файлик
		delete hint;//удаляем подсказку, так как ее уже отработали
	}
	
	delete hintsInfo;//удаляем данные из info, так как отработали
	javaProv();//запуск проверки отправляемой формы
}

void Game::javaProv(){//понятно тут все
	cout<< "<script type=\"text/javascript\">\n";
	cout<< "function chk(){\n";
		cout<< "var s = document.getElementById(\"code\").value;\n"; //Выбор проверяемого поля
		cout<< "var ediv = document.getElementById('errors');\n"; //Выбор блока ошибок
		cout<< "var form = document.getElementsByTagName('form')[0];\n"; //Выбор формы
		cout<< "ediv.innerHTML = \"\";\n"; //
		
		//Создание шаблона ошибки длины
		cout<< "var elen = document.createElement('div');\n";
		cout<< "elen.className = \"error\";\n";
		cout<< "elen.innerHTML = \"Invalid line length\";\n";
		
		//Создание шаблона ошибки неправильных цифр
		cout<< "var enms = document.createElement('div');\n";
		cout<< "enms.className = \"error\";\n";
		cout<< "enms.innerHTML = \"Invalid numbers\";\n";
		
		//Создание шаблона ошибки не идндивидуальности цифр
		cout<< "var eunq = document.createElement('div');\n";
		cout<< "eunq.className = \"error\";\n";
		cout<< "eunq.innerHTML = \"Non-unique numbers\";\n";

		cout<< "var flag=true;\n";

		cout<< "if(s.length != 4){\n";//неверная длина
			cout<< "ediv.append(elen);\n";
			cout<< "flag = false;\n";
		cout<< "}\n";

		cout<< "var f = false;\n";//неверные цифры
		cout<< "for(var i=0;i <= s.length-1;i++){\n";
			cout<< "if((s.charAt(i)<'1')||(s.charAt(i)>'6'))\n";
				cout<< "f = true;\n";
		cout<< "}\n";

		cout<< "if(f){";//если буквы
			cout<< "ediv.append(enms);\n";
			cout<< "flag = false;\n";
		cout<< "}\n";

		cout<< "f = false;\n";
		cout<< "var c = new Array(s.length);\n";
		cout<< "for(var i=0;i <= s.length-1;i++){\n";
			cout<< "c[i] = 0;\n";
		cout<< "}\n";

		cout<< "for(var i=0;i <= s.length-1;i++){\n";
			cout<< "if(c[s.charAt(i)-1] == 1){\n";
				cout<< "f = true;\n";
			cout<< "}else{\n";
				cout<< "c[s.charAt(i)-1] = 1;\n";
			cout<< "}\n";
		cout<< "}\n";
		cout<< "if(f){\n";
			cout<< "ediv.append(eunq);\n";
			cout<< "flag = false;\n";
		cout<< "}\n";

		cout<< "if(flag){\n";
			cout<< "form.submit()\n";
		cout<< "}\n";

	cout<< "}\n";

	cout<< "var b = document.getElementsByTagName('button')[0];\n";	
	cout<< "b.addEventListener('click', chk);\n";
	
	cout<< "document.addEventListener('keydown', function(event){\n";
        cout<< "if(event.keyCode == 13)\n";
		cout<< "{\n";
        	cout<< "event.preventDefault();\n";
          	cout<< "chk();\n";
      	cout<< "}\n";
      	cout<< "})\n";
	
cout<< "</script>\n";
}

char * Game::parse(char * url){//парсинг urlа
	char buf[5], *ptr = buf;
	while(*url != '=')
		url++;
	url ++;
	while(*url != '\0'){
		*(ptr++) = *(url++);
	}
		
	buf[4] = '\0';
	char * temp = new char(5);
	strcpy(temp, buf);
	return temp;
}

char * Game::check(char *a, char *c){//генерация подсказки и сравнение текузего ответа с правильным.
	char *temp = new char[5], *ptr = temp, *tmp = NULL, *atemp = a;
	while(*a != '\0'){
		tmp = strchr(c, *a);
		if(tmp - c == a - atemp)
			*(ptr++) = 'B';
		else
			if(tmp != NULL)
				*(ptr++) = 'C';
		a++;
	}
	
	*ptr='\0';
	
	return temp;
}

void Game::win(){
	cout << "Ну....ты типо выйграл. Молодец\n";
}

void Game::lose(){
	cout << "Пхах....Серъезно?!Ты не угадал....Мдэ....Ладно, погнали еще раз.<br>\n";
	cout << "<div id=\"errors\">\n\n</div\n>";
	cout <<"<form name=\"adminsProfile\" action=\"http://localhost/cgi-bin/second.cgi\" method=\"POST\">\n \t<input type=\"text\" name=\"Code\" class=\"form_text\" id=\"code\">\n <button type=\"button\">Send</button>\n</form>\n";
}

history::history(const char *pi, const char *ph){//узнаем пути к файлам, так как используем несколько раз
	int size = strlen(pi);//это для пути к info
	path_to_info = new char[size+1];
	strcpy(path_to_info, pi);
	
	size = strlen(ph);//это для пути в hints
	path_to_hints = new char[size+1];
	strcpy(path_to_hints, ph);
}

history::~history(){//так как мы выделили память под пути, то должны и удалить их. Логично - логично
	delete []path_to_info;
	delete []path_to_hints;
}

fileInfo * history::getInfo(){//тут узнаем информацию из файла info
	ifstream hints(path_to_info);//используем выше описаный метод
	fileInfo * temp = new fileInfo;//темп переменная для информации
	hints >> temp->key;//записываем правильный ответ
	hints >> temp->hintsCount;//записывем кол-во попыток
	hints.close();//закрываем файлик
	return temp;//возращаем для дальнейшей работы
}

void history::addhint(char * a, char *c){//добавим подсказку в наш файлик hints
	fstream hints(path_to_hints, ios_base::app);//открываем файлик
	hints << a << " "<< c << endl;//записываем 
	hints.close();//закрываем файлик
}

void history::update(fileInfo * i){//перезапись файла info. Нужно для кол-ва подсказок
	ofstream info("info.txt", ios_base::out | ios_base::trunc);//открываем
	info << i->key << endl;//тут просто перезаписываем, ничего не меняя
	info << i->hintsCount << endl;//обновленное кол-во записываем.
	info.close();//закрываем файлик
}

void history::print(int count){//печатаем файлик. И надо печатать только 10 последних подсказок.
	ifstream hints(path_to_hints);
	int j = count - 10, i;
	char buff[10];
	for( i = 0; i < j; i++)
		hints.getline(buff, 10);
	
	if(count > 10)
		count = 10;
		
	for(i=0; i<count;i++){
		hints.getline(buff, 10);
		cout << buff << "<br>\n";
	}
	
	hints.close();
}

void Game::printfile(const char * path){//печать файла
	ifstream f(path);
	char buf[100];
	while(!f.eof()){
		f.getline(buf,99);
		cout << buf << '\n';
	}
	f.close();
}
//ФУНКЦИИ ИЗ ВТОРОЙ ЗАДАЧИ
char* parser::post() { //функция возвращающая наш url, использующаяся при POST
	char* str = getenv("CONTENT_LENGTH");
	int size = atoi(str);
	char* url = new char[size + 1];
	fread(url, size, 1, stdin);
	url[size] = '\0';
	return url;
}


char* parser::get() { //функция возвращающая наш url, использующаяся при GET
	char* str = getenv("QUERY_STRING");
	int size = strlen(str);
	char* url = new char[size + 1];
	strcpy(url, str);
	return url;
}


char* parser::query() { //запрос-проверка на то, какой метод 
	char * method = getenv("REQUEST_METHOD");
	if (strcmp(method, "GET") == 0) //если метод GET то вызов соответствующей ф-ции 
		return get();
	if (strcmp(method, "POST") == 0) //если метод POST то вызов соответствующей ф-ции 
		return post();
}


parser::parser() {
	int i = 0;
	char value[100]; //буфер для значения
	char key[100];  //буфер для ключа
	char* temp = key; //переменная для изменения куда записывать
	char * str = query();

	while (str[i] != '\0' && list.empty()) {
		switch (str[i]) {
		case '=':
			*temp = '\0';
			temp = value;
			break;
		case '&':
			*temp = '\0';
			temp = key;
			list.addHead(key, value);
			break;
		case '+':
			*temp = ' ';
			temp++;
			break;
		default:
			*temp = str[i];
			temp++;
			break;
		}
		i++;
	}
	
	if(list.empty()){
		*temp = '\0';
		list.addHead(key, value);
	}else{
		List::node* pointer =list.getHead();
	
		while (str[i] != '\0') {
			switch (str[i]) {
			case '=':
				*temp = '\0';
				temp = value;
				break;
			case '&':
				*temp = '\0';
				temp = key;
				pointer=list.insert(pointer,key, value);
				break;
			case '+':
				*temp = ' ';
				temp++;
				break;
			default:
				
				*temp = str[i];
				temp++;
				break;
			}
			i++;
		}
		*temp = '\0';
		pointer = list.insert(pointer, key, value);
	}
	delete str;
}

List::node * parser::getdata() const{
	return list.getHead();
}


List::~List()
{
	if(head != NULL){
		node* prev = head;
		node* current = prev->next;
		if(current == NULL){
			delete prev->key;
			delete prev->value;
			delete prev;
		}else{
			while (current != NULL) {
				delete prev->key;
				delete prev->value;
				delete prev;
				prev = current;
				current=current->next;
			}	
		}
	}
	head=NULL;
}


List::node * List::getHead() const
{
	return head;
}


int List::empty() const
{
	return head==NULL;
}


void List::addHead(const char * key, const char * value)//добавление в голову
{
	this->head = new List::node;
	int szk = strlen(key), 
	szv = strlen(value);
	
	this->head->key = new char[100];
	this->head->value = new char[100];
	strcpy(this->head->key, key);
	strcpy(this->head->value, value);
}



List::node* List::insert(node* n, const char* key, const char* value) {//добавление элемента в список, возвращаем позицию после которой будем записывать новый
	n->next = new node;
	n = n->next;
	n->key = new char[strlen(key) + 1];
	n->value = new char[strlen(value) + 1];
	strcpy(n->key, key);
	strcpy(n->value, value);
	return n;
}
