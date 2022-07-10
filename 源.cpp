#include<iostream>
#include<fstream>
#include<string>
using namespace std;

class timeT {
private:
	int _year;
	int _month;
	int _day;
public:
	//���캯��
	timeT(int y=2022, int m=1, int d=1) {
		_year = y;
		_month = m;
		_day = d;
	}
	//�����ַ�������
	timeT(char* str) {
		int i = 0;
		_year = 0;
		_month = 0;
		_day = 0;
		for (; str[i] != '/'; ++i) {
			_year = 10 * _year + (str[i] - '0');
		}
		++i;
		for (; str[i] != '/'; ++i) {
			_month = 10 * _month + (str[i] - '0');
		}
		++i;
		for (; str[i] != '\0'; ++i) {
			_day = 10 * _day + (str[i] - '0');
		}
	}
	//�������
	void display() {
		cout << _year << "/" << _month << "/" << _day << endl;
	}
};

//������
class taskT {
private:
	char _name[20];
	timeT _begintime;
	int _priority;
	int _category;
	timeT _warningtime;
	int _id;

public:
	taskT(char* name, timeT time1, int priority, int category, timeT time2, int id=0) {
		//��ֵ����
		int i = 0;
		for (; name[i] != '\0'; ++i) {
			_name[i] = name[i];
		}
		_name[i] = '\0';
		//��ֵʣ����
		_begintime = time1;
		_priority = priority;
		_category = category;
		_warningtime = time2;
		_id = id;
	}

	void display() {
		cout << "Task ID:" << _id << endl;
		cout << "task name: " << _name << endl;
		cout << "begin time:";
		_begintime.display();
		cout << "priority level:" << _priority << endl;
		cout << "category:" << _category << endl;
		cout << "warning time:";
		_warningtime.display();
	}

	void changeID(int id) {
		_id = id;
	}

	int getID() {
		return _id;
	}

};

struct linkNode {

	taskT* data;
	linkNode* next;

	linkNode(taskT* d = NULL, linkNode* n = NULL) {
		data = d;
		next = n;
	}
};

class linkList {
private:
	linkNode head;
public:
	linkList() {
		head.data = NULL;
		head.next = NULL;
	}

	void push(taskT* task) {
		head.next = new linkNode(task, head.next);
	}

	void display() {
		linkNode* p = &head;
		while (p->next) {
			p = p->next;
			p->data->display();
		}
	}

	taskT* pop() {
		taskT* current = head.next->data;
		head.next = head.next->next;
		return current;
	}

	bool del(int id) {
		linkNode* p = &head;
		while (p->next) {
			if (p->next->data->getID() == id) {//�ҵ���ɾ����ֵ
				linkNode* q = p->next;
				p->next = q->next;
				delete q;
				return true;
			}
			p = p->next;
		}
		return false;
	}
};

//�ַ����ж��Ƿ����
bool strcompare(char* str1, char* str2) {
	int i = 0;
	while (str1[i] != '\0' && str2[i] != '\0') {
		if (str1[i] != str2[i]) return false;
		++i;
	}
	if (str1[i] == str2[i]) return true;
	else return false;
}

//������������Ե��ʷָ�
void getCommand(int& count, char** commands) {
	while (cin.peek() != '\n'){
		cin >> commands[count];
		count++;
	}
	getchar();//���ʣ��Ļس�
}

//�û���½
bool userLogin() {
	fstream userinfo("userinfo.txt");
	if (!userinfo) {
		cout << "[INFO] File loaded error!" << endl;
		return false;
	}
	string username, password;
	userinfo >> username >> password;
	userinfo.close();
	string user, pass;
	cout << "[INFO] Please enter your username:";
	cin >> user;
	cout << "[INFO] Then enter your password:";
	cin >> pass;
	return user == username && pass == password;
}

//�����˵�
void func_help() {
	cout << "[INFO] This is help menu:" << endl;
	cout << "\"help\" : show all the commands supported." << endl;
	cout << "\"addtask\": add a new task to your log. Use: addtask [name] [begintime,yyyy/mm/dd] [priority] [category] [waringtime,yyyy/mm/dd]." << endl;
	cout << "\"quit\": quit the program." << endl;
	cout << "\"showq\": show all the tasks." << endl;
	cout << "\"del\": del task with id.Use: del [id]." << endl;
	cout << "\"user\": set your info.Use: user [username] [passwd]." << endl;
}

//�������
bool func_addtask(char** commands,int & logsnum,linkList & list) {
	//׼���½�����
	timeT time1(commands[2]);
	timeT time2(commands[5]);
	int pri = commands[3][0] - '0';
	int cate = commands[4][0] - '0';

	taskT* mytask=new taskT(commands[1], time1, pri, cate, time2, ++logsnum);
	list.push(mytask);
	return true;
}

//�˳�����
void func_quit(int & logsnum,linkList& list) {
	ofstream ofile("1.txt");
	ofile << logsnum;
	ofile.close();
	ofile.open("tasklist.txt");
	for (int i = 1; i <= logsnum; ++i) {
		taskT* current = list.pop();
		current->changeID(i);
		ofile.seekp((i - 1) * sizeof(taskT));
		ofile.write(reinterpret_cast<const char*>(current), sizeof(taskT));
	}
}

//��ʼ������
void Program_initialize(int& logsnum, linkList& list) {
	ifstream infile("1.txt");
	infile >> logsnum;
	infile.close();
	infile.open("tasklist.txt");
	char defaultname[] = "mytask";
	timeT defaulttime(2022,1,1);
	for (int i = 1; i <= logsnum; ++i) {
		taskT* mytask = new taskT(defaultname, defaulttime, 3, 3, defaulttime, 0);
		infile.seekg((i - 1) * sizeof(taskT));
		infile.read(reinterpret_cast<char *>(mytask), sizeof(taskT));
		list.push(mytask);
	}
}

//չʾ��������
void func_showq(int& logsnum, linkList& list) {
	list.display();
}

//��idɾ������
bool func_del(int& logsnum, linkList& list, char** commands) {
	int targetid = 0;
	for (int i = 0; commands[1][i] != '\0'; ++i) {
		targetid = targetid * 10 + commands[1][i] - '0';
	}
	if (list.del(targetid)) {
		logsnum--;
		return true;
	}
	return false;
}

//�޸��û���������
bool func_user(char** commands){
	cout << "[INFO] Your info will be updated as(y/n):" << endl;
	cout << "username: " << commands[1] << endl << "password: " << commands[2] << endl;
	char s;
	cin >> s;
	getchar();//����س�����
	if (s == 'y'){
		ofstream ofile("userinfo.txt");
		if(!ofile) return false;
		ofile << commands[1] << ' ' << commands[2];
		ofile.close();
		return true;
	}
	else{
		return false;
	}
	
}


int main() {
	//�������г�ʼ��

	//����Ƿ�ȱ����Ҫ�ļ�
	fstream file1("1.txt"), file2("tasklist.txt"), file3("userinfo.txt");
	if (!file1 || !file2 || !file3) {
		cout << "[INFO] Lack of important files." << endl;
		system("PAUSE");
		return -1;
	}
	file1.close();
	file2.close();
	file3.close();
	
	//��֤��½
	while (!userLogin()) {
		cout << "[INFO] Username or password was wrong!" << endl;
	}
	cout << "[INFO] Welcome back, this is your log system." << endl;
	char emp[2];
	cin.getline(emp, 2);//���cin�Ļ�����
	
	//�����������
	const int MAX_SIZE = 50;
	int commandcount = 0;//�����ܲ�������
	int opt = 0;//������������
	char** commands = new char* [MAX_SIZE];//�����������
	for (int i = 0; i < MAX_SIZE; ++i) {
		commands[i] = new char[MAX_SIZE];
	}
	int lognums = 0;
	linkList list;
	bool active = true;//�����������

	//����ʹ�õ�����
	char help[] = "help";
	char addtask[] = "addtask";
	char quit[] = "quit";
	char showq[] = "showq";
	char del[] = "del";
	char user[] = "user";

	//��ȡ����
	Program_initialize(lognums, list);
	
	cout << "[INFO] Program has been successfully initialized." << endl;//��ʼ���ɹ�

	while (active) {//������ѭ��		
		commandcount = 0;//�����ÿ�
		opt = 0;//�������ԭ
		//����洢�ÿ�
		for (int i = 0; i < MAX_SIZE; ++i) {
			for (int j = 0; j < MAX_SIZE; ++j) {
				commands[i][j] = ' ';
			}
		}

		cout << "[INFO] Waiting for command.(use \"help\" for help)" << endl;//��ʼ���ɹ�
		
		getCommand(commandcount, commands);//��ȡ����
		
		if (strcompare(commands[0],help)) opt = 1;//ת��������
		if (strcompare(commands[0], addtask)) opt = 2;
		if (strcompare(commands[0], quit)) opt = 3;
		if (strcompare(commands[0], showq))opt = 4;
		if (strcompare(commands[0], del)) opt = 5;
		if (strcompare(commands[0], user)) opt = 6;

		switch (opt) {
		//����help
		case 1: {
			if (commandcount != 1) {//�������
				cout << "[INFO] Command \"help\" has no argument." << endl;
				break;
			}
			func_help();
			break;
		}
		//����addtask
		case 2: {
			if (commandcount != 6) {
				cout << "[INFO] Command \"addtask\" has 6 arguments." << endl;
				break;
			}
			if (func_addtask(commands,lognums,list))cout << "New task successfully added." << endl;
			else cout << "Command failed." << endl;
			break;
		}
		//����quit
		case 3: {
			if (commandcount != 1) {
				cout << "[INFO] Command \"quit\" has no argument." << endl;
				break;
			}
			func_quit(lognums, list);
			active = false;
			break;
		}
		//չʾ����
		case 4: {
			if (commandcount != 1) {
				cout << "[INFO] Command \"showq\" has no argument." << endl;
				break;
			}
			func_showq(lognums, list);
			break;
		}
		//ɾ������
		case 5: {
			if (commandcount != 2) {
				cout << "[INFO] Command \"del\" has 1 argument." << endl;
				break;
			}
			if (func_del(lognums, list, commands)) {
				cout << "[INFO] This task has been deleted." << endl;
			}
			else {
				cout << "[INFO] Command failed." << endl;
			}
			break;
		}
		case 6:{
			if(commandcount != 3){
				cout << "[INFO] Command \"user\" has 2 arguments." << endl;
				break;
			}
			if (func_user(commands)){
				cout << "[INFO] Your info has been updated." << endl;
			}
			else {
				cout << "[INFO] Command failed." << endl;
			}
			break;
		}


		//�Ҳ����������
		default: {
			cout << "[INFO] This command is not defined.Check \"help\" for further information." << endl;
		}

		}//end switch




	}//end while

	system("PAUSE");

	return 0;
}
