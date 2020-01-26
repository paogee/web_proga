#include <iostream>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fstream>
using namespace std;

class Game{//����� �� ��� � first, ������ ����� ������ �������
public:
	Game(); //��������� ������ ������������
	~Game();//����������
	void play(); //���� � ������������ html ���������
private:
	void javaProv(); //�������� ������������� ���������� ��������
	char * parse(char * url); //������� ����
	char * check(char *a, char *c); //�������� ������
	void win(); //���� �� ��������
	void lose(); //���� ��������� + ����� ���������
	void printfile(const char *path); //���������� ������� ��������
	char *answer; //�����
};

class List {//���� �� ������ ������
public:
	List() { head = NULL; };
	~List();
	struct node {
		char* key;	 //����
		char* value; //��������
		node *next;  //����-�� �� ����.
	};
	node* getHead() const;  //��������� ������ ������
	int empty() const;	//�������� �� �������
	void addHead(const char* key, const char* value);	//������ ������
	node* insert(node* n, const char* key, const char* value); // ���������� ��-�� � ������
	void print(); //������
private: 
	node* head;
};

class parser {//���� �� ������ ������ � ����� ���������.
public:
	parser(); //������
	List::node * getdata() const;
private:
	char *query();
	char *get();
	char *post();
	List list; //������
};

struct fileInfo{//��������� �����. ��� ����������� ����� � ���-�� �������
	char key[5];
	int hintsCount;
};


class history{ //����� ��� ������ � �����������
public:
	history(const char * pi, const char * ph); //���������� ���� � ����� � ����������� � � ����� � �����������. pi-���� � info. ph-���� � hints
	fileInfo * getInfo(); //��������� ���������� �� �����
	void addhint(char *a, char *h); //���������� ��������� h � ������ a
	void update(fileInfo * i); //���������� ���������� 
	void print(int count); //������ ���������
	~history(); //�������� �����
private:
	char * path_to_info, //���� � ����� � �����������
		* path_to_hints; //���� � ����� c ����������
};

int main(){//������� �������� � �������� � url
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

Game::Game(){//����������. ������� ������ ������ Game
	parser p;//������ URL
	List::node * temp = p.getdata();//���������� � list, ��� ��� p ��������.
	answer = new char[strlen(temp->value)+1];//������� ������ ������
	strcpy(answer, temp->value);//���� ����������
	//��� ��� ���� �����, ����� �� ���� ������� � �������.
}

Game::~Game(){//������ ����������. �� �� �������� ������ ��� answer, ������ ������ � �������
	delete []answer;
}

void Game::play(){//����� ������ ���� ����
	history Hints("info.txt", "hints.txt");//��������� ���� ���������
	
	fileInfo * hintsInfo = Hints.getInfo();//�������� ���-�� ������� � ���������� �����.
	char * code = hintsInfo->key;//������ ���������� ����� �� �������
	
	if(!strcmp(answer, code))//����� �������� ����� �������� � ����������, �� ������
		win();
	else{
		lose();//����� ������� ���������
		char * hint = check(answer, code);//���������� ���������
		cout << "���� ������ ������� ��������:<br>\n";
		int count = (hintsInfo->hintsCount)++;//����������� ��� �� �������
		Hints.addhint(answer, hint);//��������� ��������� � ������
		Hints.update(hintsInfo);//��������� ���������� � ����
		Hints.print(count+1);//�������� ������� � ������
		delete hint;//������� ���������, ��� ��� �� ��� ����������
	}
	
	delete hintsInfo;//������� ������ �� info, ��� ��� ����������
	javaProv();//������ �������� ������������ �����
}

void Game::javaProv(){//������� ��� ���
	cout<< "<script type=\"text/javascript\">\n";
	cout<< "function chk(){\n";
		cout<< "var s = document.getElementById(\"code\").value;\n"; //����� ������������ ����
		cout<< "var ediv = document.getElementById('errors');\n"; //����� ����� ������
		cout<< "var form = document.getElementsByTagName('form')[0];\n"; //����� �����
		cout<< "ediv.innerHTML = \"\";\n"; //
		
		//�������� ������� ������ �����
		cout<< "var elen = document.createElement('div');\n";
		cout<< "elen.className = \"error\";\n";
		cout<< "elen.innerHTML = \"Invalid line length\";\n";
		
		//�������� ������� ������ ������������ ����
		cout<< "var enms = document.createElement('div');\n";
		cout<< "enms.className = \"error\";\n";
		cout<< "enms.innerHTML = \"Invalid numbers\";\n";
		
		//�������� ������� ������ �� ����������������� ����
		cout<< "var eunq = document.createElement('div');\n";
		cout<< "eunq.className = \"error\";\n";
		cout<< "eunq.innerHTML = \"Non-unique numbers\";\n";

		cout<< "var flag=true;\n";

		cout<< "if(s.length != 4){\n";//�������� �����
			cout<< "ediv.append(elen);\n";
			cout<< "flag = false;\n";
		cout<< "}\n";

		cout<< "var f = false;\n";//�������� �����
		cout<< "for(var i=0;i <= s.length-1;i++){\n";
			cout<< "if((s.charAt(i)<'1')||(s.charAt(i)>'6'))\n";
				cout<< "f = true;\n";
		cout<< "}\n";

		cout<< "if(f){";//���� �����
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

char * Game::parse(char * url){//������� url�
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

char * Game::check(char *a, char *c){//��������� ��������� � ��������� �������� ������ � ����������.
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
	cout << "��....�� ���� �������. �������\n";
}

void Game::lose(){
	cout << "����....��������?!�� �� ������....���....�����, ������� ��� ���.<br>\n";
	cout << "<div id=\"errors\">\n\n</div\n>";
	cout <<"<form name=\"adminsProfile\" action=\"http://localhost/cgi-bin/second.cgi\" method=\"POST\">\n \t<input type=\"text\" name=\"Code\" class=\"form_text\" id=\"code\">\n <button type=\"button\">Send</button>\n</form>\n";
}

history::history(const char *pi, const char *ph){//������ ���� � ������, ��� ��� ���������� ��������� ���
	int size = strlen(pi);//��� ��� ���� � info
	path_to_info = new char[size+1];
	strcpy(path_to_info, pi);
	
	size = strlen(ph);//��� ��� ���� � hints
	path_to_hints = new char[size+1];
	strcpy(path_to_hints, ph);
}

history::~history(){//��� ��� �� �������� ������ ��� ����, �� ������ � ������� ��. ������� - �������
	delete []path_to_info;
	delete []path_to_hints;
}

fileInfo * history::getInfo(){//��� ������ ���������� �� ����� info
	ifstream hints(path_to_info);//���������� ���� �������� �����
	fileInfo * temp = new fileInfo;//���� ���������� ��� ����������
	hints >> temp->key;//���������� ���������� �����
	hints >> temp->hintsCount;//��������� ���-�� �������
	hints.close();//��������� ������
	return temp;//��������� ��� ���������� ������
}

void history::addhint(char * a, char *c){//������� ��������� � ��� ������ hints
	fstream hints(path_to_hints, ios_base::app);//��������� ������
	hints << a << " "<< c << endl;//���������� 
	hints.close();//��������� ������
}

void history::update(fileInfo * i){//���������� ����� info. ����� ��� ���-�� ���������
	ofstream info("info.txt", ios_base::out | ios_base::trunc);//���������
	info << i->key << endl;//��� ������ ��������������, ������ �� �����
	info << i->hintsCount << endl;//����������� ���-�� ����������.
	info.close();//��������� ������
}

void history::print(int count){//�������� ������. � ���� �������� ������ 10 ��������� ���������.
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

void Game::printfile(const char * path){//������ �����
	ifstream f(path);
	char buf[100];
	while(!f.eof()){
		f.getline(buf,99);
		cout << buf << '\n';
	}
	f.close();
}
//������� �� ������ ������
char* parser::post() { //������� ������������ ��� url, �������������� ��� POST
	char* str = getenv("CONTENT_LENGTH");
	int size = atoi(str);
	char* url = new char[size + 1];
	fread(url, size, 1, stdin);
	url[size] = '\0';
	return url;
}


char* parser::get() { //������� ������������ ��� url, �������������� ��� GET
	char* str = getenv("QUERY_STRING");
	int size = strlen(str);
	char* url = new char[size + 1];
	strcpy(url, str);
	return url;
}


char* parser::query() { //������-�������� �� ��, ����� ����� 
	char * method = getenv("REQUEST_METHOD");
	if (strcmp(method, "GET") == 0) //���� ����� GET �� ����� ��������������� �-��� 
		return get();
	if (strcmp(method, "POST") == 0) //���� ����� POST �� ����� ��������������� �-��� 
		return post();
}


parser::parser() {
	int i = 0;
	char value[100]; //����� ��� ��������
	char key[100];  //����� ��� �����
	char* temp = key; //���������� ��� ��������� ���� ����������
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


void List::addHead(const char * key, const char * value)//���������� � ������
{
	this->head = new List::node;
	int szk = strlen(key), 
	szv = strlen(value);
	
	this->head->key = new char[100];
	this->head->value = new char[100];
	strcpy(this->head->key, key);
	strcpy(this->head->value, value);
}



List::node* List::insert(node* n, const char* key, const char* value) {//���������� �������� � ������, ���������� ������� ����� ������� ����� ���������� �����
	n->next = new node;
	n = n->next;
	n->key = new char[strlen(key) + 1];
	n->value = new char[strlen(value) + 1];
	strcpy(n->key, key);
	strcpy(n->value, value);
	return n;
}
