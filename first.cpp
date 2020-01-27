#include <iostream>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fstream>

using namespace std;

class List {//is the exct same as the one in the 2nd Task
public:
	List() { head = NULL; };
	~List();
	struct node {//pair key_value
		char* key;	 //k
		char* value; //val
		node *next;  //pointer to the next elem
	};
	node* getHead() const;  //get the lists head
	int empty() const;	//check if empty
	void addHead(const char* key, const char* value);	//allocate the head
	node* insert(node* n, const char* key, const char* value); // adding elem to the list
	void print(); //outprinting
private: 
	node* head;
};

class parser {//from the 2Task but a lil changed
public:
	parser(); //ïàðñåð
	List::node * getdata() const;
private:
	char *query();
	char *get();
	char *post();
	List list; //our list
};

class Game{//game class w the game start, writing to files, new file creation,result outout, string creation
public:
	Game(); //get the user guess
	~Game(); //destructorrrrr
	void play(); //game and forming the html doc
private:
	void javaProv(); //local send try checker
	char * gen(); //ãkey generating
	char * check(char *a, char *c); //guess checker
	void win(); //if WON
	void lose(); //if LOST
	void newfile(char * key, char * a, char * hint); //create new file w the cor answer and try count there
	void printfile(const char * p);//outprint file
	char *answer; //our answer
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
	
	return 0;
}

Game::Game(){//constructor. Create tho obj of class Game
	parser p;//÷èòàåì URL
	List::node * temp = p.getdata();//write to list cs "p" will be deleted at the end of func
	answer = new char[strlen(temp->value)+1];//create the answer string
	strcpy(answer, temp->value);//write to the answer string
	//all of the above needed to not hav any problems with memory
}

Game::~Game(){//destructor to delete the mem used by answer
	delete []answer;
}

void Game::play(){//first try START
	char * code = gen();//ãgenerate the cor answer
	
	if(!strcmp(answer, code))//åif u are VERY LUCKY  and won from the 1st try, output win
		win();
	else{
		lose();//if not then output lost and rewrite files and the html
		char * hint = check(answer, code);//íusing our guess generate the hint for the next try
		cout << "<center>Your tries so far:</center><br>\n";//outout the hint into html
		cout << "<center> " << answer << " " << hint << "</center>\n";//output the hint to html
		newfile(code, answer,  hint);//create 2 new files to have the cor answer and try count in 1st and hints and try history in the 2nd.
		delete hint;//delete the hint since we finished w it
	}
	
	delete code;//ódelete cor answer since we finished w it
	javaProv();//launch the next try check
}
//Java Script error checker
void Game::javaProv(){//to check the tries sent locally
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

char * Game::gen(){//ñgenerate the correct answer for the hint
	char *code = new char[5], *ptr = code;//string of cor answer + 0 elem, ptr
	int carr[6] = {0, 0, 0, 0, 0, 0};
	int count = 4, i = 0;
	
	srand(time(NULL));//the randomization of answers
	
	while(count){//generate the cor answer
		i = rand() % 6;//the work of random
		
		if(carr[i])//if 1 all is good
			continue;
			
		carr[i] = 1;
		*(ptr++) = '0' + 1 + i;
		count --;//traverse 4 times
	}
	*ptr = '\0';//add termination elem
	
	return code;//cor answer
}

char * Game::check(char *a, char *c){//create string for hint
	char *temp = new char[5], *ptr = temp, *tmp = NULL, *atemp = a;
	while(*a != '\0'){//till the string aint empty
		tmp = strchr(c, *a);//get the ptr to the 1st entering of the symbol into string
		if(tmp - c == a - atemp)//if num is in its place
			*(ptr++) = 'B';//then its a BULL
		else
			if(tmp != NULL)//if it aint in its place but in the string 
				*(ptr++) = 'C';//its a COW
		a++;//if itð not in the string we output nothing
	}
	
	*ptr='\0';//adding the termination elem to  end the string
	
	return temp;
}

void Game::win(){//VICTORY :D
	cout << "<center><h1>!!!!VICTORY!!!!</h1></center>\n";
	cout << "<img src='https://i.dlpng.com/static/png/6413508_preview.png'>";
}

void Game::lose(){//LOSER :(
	cout << "<center><h2>oh no no...Wanna try again~?</h2></center><br>\n";
	cout << "<div id=\"errors\">\n\n</div\n>";
	cout <<"<center><form name=\"adminsProfile\" action=\"http://localhost/cgi-bin/second.cgi\" method=\"POST\">\n \t<input type=\"text\" name=\"Code\" class=\"form_text\" id=\"code\">\n <button type=\"button\">Send</button>\n</form></center>\n";
}

void Game::newfile(char *key, char * a, char *hint){//generating file and writing all of info threre 4 future work
	ofstream info("info.txt", ios_base::out | ios_base::trunc);//file has the cor answer and the try count
	info << key << endl;//ïðàâèëüíûé îòâåò
	info << 1 << endl;//ïîïûòêà 1. Òàê êàê ýòî ïåðâàÿ èãðà
	info.close();//çàêðûâàåì ôàéë äëÿ çàïèñè
	
	ofstream hints("hints.txt", ios_base::out | ios_base::trunc);//file for hints and our guesses
	hints << a << " " << hint << endl;//guess : hint
	hints.close();//cansel the file 
}

void Game::printfile(const char * path){//file output
	ifstream f(path);
	char buf[100];
	while(!f.eof()){
		f.getline(buf,99);
		cout << buf << '\n';
	}
	f.close();
}
//ÔÓÍÊÖÈÈ ÈÇ ÇÀÄÀ×È ÍÎÌÅÐ 2
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

//×ÓÒÊÀ ÏÅÐÅÄÅËÀÍÍÛÉ ÏÀÐÑÅÐ
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

List::node * parser::getdata() const{//to know the head necessary to strat the game
	return list.getHead();
}

//using destructor to delete head and free the mem
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
