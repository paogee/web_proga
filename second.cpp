#include <iostream>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fstream>
using namespace std;

class Game{//similarto the one in first.cpp but w some changes
public:
	Game(); //get the user guess
	~Game();//destructorrrrr
	void play(); //game and forming the html doc
private:
	void javaProv(); //local send try checker
	char * parse(char * url); //url parsing
	char * check(char *a, char *c); //try ckecker
	void win(); //if we won
	void lose(); //if we lost + the hint output
	void printfile(const char *path); //adding the ckecking script
	char *answer; //cor answer
};

class List {//Âtaken from the 2Task
public:
	List() { head = NULL; };
	~List();
	struct node {
		char* key;	 //key
		char* value; //value
		node *next;  //ptr to next
	};
	node* getHead() const;  //get the head of list
	int empty() const;	//empty cheker
	void addHead(const char* key, const char* value);	//allocate head
	node* insert(node* n, const char* key, const char* value); // adding element to list
	void print(); //print
private: 
	node* head;
};

class parser {//from the 2Task but a lil changed
public:
	parser(); //parser
	List::node * getdata() const;
private:
	char *query();
	char *get();
	char *post();
	List list; //our list
};

struct fileInfo{//structure of the fiel. its the cor answer and try count
	char key[5];
	int hintsCount;
};


class history{ //class to work w hints
public:
	history(const char * pi, const char * ph); //saving the path to the files with information and the hint
	fileInfo * getInfo(); //get info from the file
	void addhint(char *a, char *h); //äadding hint "h" and answer "a"
	void update(fileInfo * i); //update the info
	void print(int count); //print the hints
	~history(); //deleting the paths to files
private:
	char * path_to_info, //path to the info.txt
		* path_to_hints; //path to the hints.txt
};

int main(){//create the page and working w url
	cout << "Content-type: text/html\n\n";
	cout << "<html>\n";
	cout << "<head>\n";
	cout << "<title>COWS'n'BULLS</title>\n";
	cout<<"<body background='http://2d.by/wallpapers/g/gorets_1.jpg'>";
	cout << "</head>\n";
	cout << "<body>\n";
	
	Game bestGame;
	bestGame.play();
	
	cout << "</body>\n";
	cout << "</html>\n";
}

Game::Game(){//constructor. Create tho obj of class Game
	parser p;//read our URL
	List::node * temp = p.getdata();//write to list cs "p" will be deleted at the end of func
	answer = new char[strlen(temp->value)+1];//create the answer string
	strcpy(answer, temp->value);//write to the answer string
		//all of the above needed to not hav any problems with memory
}

Game::~Game(){//estructor to delete the mem used by answer
	delete []answer;
}

void Game::play(){//new try START
	history Hints("info.txt", "hints.txt");//open hint file
	
	fileInfo * hintsInfo = Hints.getInfo();//get the try count and cor answer
	char * code = hintsInfo->key;//get the cor answer from the file
	
	if(!strcmp(answer, code))//åif u are VERY LUCKY  and won output win
		win();
	else{
		lose();//else output loss
		char * hint = check(answer, code);//generating the hin
		cout << "<center>Your tries so far:</center><br>\n";
		int count = (hintsInfo->hintsCount)++;//raise the ty count
		Hints.addhint(answer, hint);//äadd the hint generated to file
		Hints.update(hintsInfo);//renew the info in info.txt
		Hints.print(count+1);//write the try to file
		delete hint;//delete the hint since we finished w it
	}
	
	delete hintsInfo;//delete date from info cs we finishe w it
	javaProv();//launch the check of the sent form
}
//Java Script error checker
void Game::javaProv(){//ïîíÿòíî òóò âñå
	cout<< "<script type=\"text/javascript\">\n";
	cout<< "function chk(){\n";
		cout<< "var s = document.getElementById(\"code\").value;\n"; //chose the field to check
		cout<< "var ediv = document.getElementById('errors');\n"; //chose the error block
		cout<< "var form = document.getElementsByTagName('form')[0];\n"; //chose the form
		cout<< "ediv.innerHTML = \"\";\n"; //
		
		//input length error
		cout<< "var elen = document.createElement('div');\n";
		cout<< "elen.className = \"error\";\n";
		cout<< "elen.innerHTML = \"Invalid line length\";\n";
		
		//input wrong num error
		cout<< "var enms = document.createElement('div');\n";
		cout<< "enms.className = \"error\";\n";
		cout<< "enms.innerHTML = \"Invalid numbers\";\n";
		
		//inout non unique num error
		cout<< "var eunq = document.createElement('div');\n";
		cout<< "eunq.className = \"error\";\n";
		cout<< "eunq.innerHTML = \"Non-unique numbers\";\n";

		cout<< "var flag=true;\n";

		cout<< "if(s.length != 4){\n";//wroong length
			cout<< "ediv.append(elen);\n";
			cout<< "flag = false;\n";
		cout<< "}\n";

		cout<< "var f = false;\n";//wrong nums
		cout<< "for(var i=0;i <= s.length-1;i++){\n";
			cout<< "if((s.charAt(i)<'1')||(s.charAt(i)>'6'))\n";
				cout<< "f = true;\n";
		cout<< "}\n";

		cout<< "if(f){";//if letters
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

char * Game::parse(char * url){//parsing the URL
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

char * Game::check(char *a, char *c){//generating hint and comparing current guess to cor answer
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
	cout << "<center><h1>!!!!VICTORY!!!!</h1></center>\n";
	cout << "<img src='https://i.dlpng.com/static/png/6413508_preview.png'>";
}

void Game::lose(){
	cout << "<center><h2>oh no no...Wanna try again~?<h2></center><br>\n";
	cout << "<div id=\"errors\">\n\n</div\n>";
	cout <<"<center><form name=\"adminsProfile\" action=\"http://localhost/cgi-bin/second.cgi\" method=\"POST\">\n \t<input type=\"text\" name=\"Code\" class=\"form_text\" id=\"code\">\n <button type=\"button\">Send</button>\n</form></center>\n";
}

history::history(const char *pi, const char *ph){//to know the way to the paths cs used many times
	int size = strlen(pi);//path to info.txt
	path_to_info = new char[size+1];
	strcpy(path_to_info, pi);
	
	size = strlen(ph);//path to hints.txt
	path_to_hints = new char[size+1];
	strcpy(path_to_hints, ph);
}

history::~history(){//since we allocated some mem for the paths we need to free it now
	delete []path_to_info;
	delete []path_to_hints;
}

fileInfo * history::getInfo(){//get the info from the info.txt
	ifstream hints(path_to_info);//use the method above
	fileInfo * temp = new fileInfo;//temp var for the info
	hints >> temp->key;//write the correct answer
	hints >> temp->hintsCount;//write the try count
	hints.close();//close the file
	return temp;//return for further work
}

void history::addhint(char * a, char *c){//adding new hint to hints.txt
	fstream hints(path_to_hints, ios_base::app);//open file
	hints << a << " "<< c << endl;//write the hint
	hints.close();//close the file
}

void history::update(fileInfo * i){//rewrite the info file so the hints would work
	ofstream info("info.txt", ios_base::out | ios_base::trunc);//open file
	info << i->key << endl;//just rewriting not cjanging it
	info << i->hintsCount << endl;//renewed count write to file
	info.close();//close the file
}

void history::print(int count){//print to file, printing LAST 10 (cs the rules)
	ifstream hints(path_to_hints);
	int j = count - 10, i;
	char buff[10];
	for( i = 0; i < j; i++)
		hints.getline(buff, 10);
	
	if(count > 10)
		count = 10;
		
	for(i=0; i<count;i++){
		hints.getline(buff, 10);
		cout << "<center>" <<  buff << "<center><br>\n";
	}
	
	hints.close();
}

void Game::printfile(const char * path){//outprint file
	ifstream f(path);
	char buf[100];
	while(!f.eof()){
		f.getline(buf,99);
		cout << "<center>" << buf << "</center>" <<'\n';
	}
	f.close();
}
//ÔÓÍÊÖÈÈ ÈÇ ÂÒÎÐÎÉ ÇÀÄÀ×È
char* parser::post() { //func returning our url if using POST method
	char* str = getenv("CONTENT_LENGTH");
	int size = atoi(str);
	char* url = new char[size + 1];
	fread(url, size, 1, stdin);
	url[size] = '\0';
	return url;
}


char* parser::get() { //func returning our url if using GET method
	char* str = getenv("QUERY_STRING");
	int size = strlen(str);
	char* url = new char[size + 1];
	strcpy(url, str);
	return url;
}


char* parser::query() { //method checking query
	char * method = getenv("REQUEST_METHOD");
	if (strcmp(method, "GET") == 0) //calls get func if value GET
		return get();
	if (strcmp(method, "POST") == 0) //calls post func if value POST
		return post();
}


parser::parser() {
	int i = 0;
	char value[100]; //value buffer
	char key[100];  //key buffer
	char* temp = key; //variable to write to
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


void List::addHead(const char * key, const char * value)//adding to head
{
	this->head = new List::node;
	int szk = strlen(key), 
	szv = strlen(value);
	
	this->head->key = new char[100];
	this->head->value = new char[100];
	strcpy(this->head->key, key);
	strcpy(this->head->value, value);
}



List::node* List::insert(node* n, const char* key, const char* value) {//adding elem to the list, return thenext position index, to know new ones insert point
	n->next = new node;
	n = n->next;
	n->key = new char[strlen(key) + 1];
	n->value = new char[strlen(value) + 1];
	strcpy(n->key, key);
	strcpy(n->value, value);
	return n;
}
