#include <iostream>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fstream>

using namespace std;

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

class Game{//Класс игры, запуск, запись, создание нового файла,вывод резульатов, создание строки.
public:
	Game(); //получение ответа пользователя
	~Game(); //деструктор
	void play(); //игра
private:
	void javaProv(); //проверка отправляемого результата локально
	char * gen(); //generates the sorrect answer (seen in file + the tries))
	char * check(char *a, char *c); //compares the input answer and the correct from file
	void win(); //Если мы выйграли
	void lose(); //Если проиграли
	void newfile(char * key, char * a, char * hint); //создание нового файла. Пишем туда ответ и кол-во попыток
	void printfile(const char * p);//вывод файла
	char *answer; //ответ
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
	
	return 0;
}

Game::Game(){//консруктор. Создаем объект класса Game
	parser p;//читаем URL
	List::node * temp = p.getdata();//записываем в list, так как p удалится.
	answer = new char[strlen(temp->value)+1];//создаем строку ответа
	strcpy(answer, temp->value);//туда записываем
	//вся эта тема нужна, чтобы не было траблов с памятью.
}//all deted after the usage so that there are no probs w mem alloc

Game::~Game(){//просто деструктор. Мы же выделяем память под answer, значит должны и удалить
	delete []answer;
}

void Game::play(){//старт первого хода игры
	char * code = gen();//генерируем правильный ответ and writen to 'code'
	
	if(!strcmp(answer, code))//если вдруг с первого раза ответили правильно, то сразу победа
		win();
	else{
		lose();//если нет, то выводим, что проиграли и перезаписываем файлы и html
		char * hint = check(answer, code);//на основе нашего ответа, генерируем подсказку для след хода
		cout << "Твои жалкие попытки победить:<br>\n";//выводим в html подсказку
		cout << answer << " " << hint << "\n";//выводим в html подсказку
		newfile(code, answer,  hint);//создаем 2 новых файла, в которых будем хранить правильный ответ, кол-во ходов и подсказку.
		delete hint;//удаляем подсказку, так как уже отработали ее
	}
	
	delete code;//удаляем правильный ответ, так как уже отработали с ним
	javaProv();//запускаем проверку следущего введнного ответа
}
//самое непонятное, но в тоже время и простое для программистов Java языка. Фи одним словом
void Game::javaProv(){//джава скрипт, для проверки отправляемого результата
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
        cout<< "if(event.keyCode == 13) {\n";
        	cout<< "event.preventDefault();\n";
          	cout<< "chk();\n";
      	cout<< "}\n";
      	cout<< "})\n";
	
cout<< "</script>\n";
}

char * Game::gen(){//создание правильного ответа для подсказки
	char *code = new char[5], *ptr = code;//строка правильного ответа + нулевой элемент, указатель.
	int carr[6] = {0, 0, 0, 0, 0, 0};
	int count = 4, i = 0;
	
	srand(time(NULL));//запуск шабалай машины рандома
	
	while(count){//генерация кода
		i = rand() % 6;//работа шабалай машины
		
		if(carr[i])//если 1, то все оки
			continue;
			
		carr[i] = 1;
		*(ptr++) = '0' + 1 + i;
		count --;//4 раза идем
	}
	*ptr = '\0';//добавление нуля в конец
	
	return code;//правильный ответ
}

char * Game::check(char *a, char *c){//создание строки для подсказки
	char *temp = new char[5], *ptr = temp, *tmp = NULL, *atemp = a;
	while(*a != '\0'){//пока строка не станет пустой
		tmp = strchr(c, *a);//получаем указатель на первое вхождение символа в строку.
		if(tmp - c == a - atemp)//если цифорка на своём месте
			*(ptr++) = 'B';//то ето БЫК
		else
			if(tmp != NULL)//если она не на своем месте, но в строке
				*(ptr++) = 'C';//то есто КОРОВА
		a++;//а если ничего, то как бы ничего и не пишем
	}
	
	*ptr='\0';//добавим нулевой элемент, чтобы закончить строку
	
	return temp;
}

void Game::win(){//ПОБiДА
	cout << "Ну....ты типо выйграл. Молодец\n";
}

void Game::lose(){//Хнык хнык
	cout << "Пхах....Неудачник. У тебя интеллект -10....Ладно, попробуй еще раз!<br>\n";
	cout << "<div id=\"errors\">\n\n</div\n>";
	cout <<"<form name=\"adminsProfile\" action=\"http://localhost/cgi-bin/second.cgi\" method=\"POST\">\n \t<input type=\"text\" name=\"Code\" class=\"form_text\" id=\"code\">\n <button type=\"button\">Send</button>\n</form>\n";
}

void Game::newfile(char *key, char * a, char *hint){//генерация файла и запись туда всей информации для дальнейшей работы
	ofstream info("info.txt", ios_base::out | ios_base::trunc);//этот файл будет хранить у нас информацию о правильном ответе и о кол-ва попыток
	info << key << endl;//правильный ответ
	info << 1 << endl;//попытка 1. Так как это первая игра
	info.close();//закрываем файл для записи
	
	ofstream hints("hints.txt", ios_base::out | ios_base::trunc);//этот файл будет хранить у нас информацию о попытках и подсказках
	hints << a << " " << hint << endl;//ОТВЕТ : ПОДСКАЗКА
	hints.close();//закрываем файл для записи
}

void Game::printfile(const char * path){//вывод файла
	ifstream f(path);
	char buf[100];
	while(!f.eof()){
		f.getline(buf,99);
		cout << buf << '\n';
	}
	f.close();
}
//ФУНКЦИИ ИЗ ЗАДАЧИ НОМЕР 2
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

//ЧУТКА ПЕРЕДЕЛАННЫЙ ПАРСЕР
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

List::node * parser::getdata() const{//узнаем, где голова. Нужна для старта игры
	return list.getHead();
}

//Я УЧЕЛ ПРОШЛЫЕ ОШИБКИ! Теперь голову в конце уводим в пустоту
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
