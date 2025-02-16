#include <iostream>
#include <stdio.h>
#include "funcs.h"
#include <thread>
#include <future>
#include <mutex>
#include <condition_variable>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <fstream>

using namespace std;



mutex mtx;
condition_variable cv;
bool data_ready = false;


void t1()
{
	Functions funcs; //Создаем объект класса Functions
	while (true)
	{	
		string str;
		
		cin >> str;
		bool isNum = true;
		//Проверка введенной пользователем строки на соответствие требуемым параметрам
		for (char i : str)
			if (isdigit(i) == 0)
				isNum = false;
		if (str.length() > 64)
			cout << "String should have less than 64 elements!" << endl;
		if (isNum == false)
			cout << "String should have only numbers!" << endl;
		else
		{
			std::lock_guard<mutex> lock(mtx);
			
			funcs.f1(str); 
			data_ready = true;
			cv.notify_one();
		}
	}
}

//Поток 2 является клиентом, связывающимся с программой 2 посредством сокетов.
void t2()
{    Functions funcs2; //Создаем объект класса Functions
    
    //IP и порт 
    char serverIp[] = "127.0.0.1"; int port = 8080;  

    char msg[1500]; 
    extern string buffer;
    
     
    struct hostent* host = gethostbyname(serverIp); 
    sockaddr_in sendSockAddr;   
    bzero((char*)&sendSockAddr, sizeof(sendSockAddr)); 
    sendSockAddr.sin_family = AF_INET; 
    sendSockAddr.sin_addr.s_addr = 
        inet_addr(inet_ntoa(*(struct in_addr*)*host->h_addr_list));
    sendSockAddr.sin_port = htons(port);	
    while(1) {
    int clientSd = socket(AF_INET, SOCK_STREAM, 0);
    //установка соединения
    int status = connect(clientSd,
                         (sockaddr*) &sendSockAddr, sizeof(sendSockAddr));
    if(status < 0)
    {
        cout<<"Error connecting to socket!"<<endl;
        return;
    }
    cout << "Connected to the server!" << endl;
    cout << "Enter string:" << endl;
 
    int bytesRead, bytesWritten;
    
    while(1)
    {
        memset(&msg, 0, sizeof(msg));
        
        
        std::unique_lock<mutex> lock(mtx);

		cv.wait(lock, [] {return data_ready;  });
		string str = buffer;		      //Получаем модифицированную строку из общего буфера
		cout << "Recieved data from buffer: " << str << endl; //Выводим полученные данные на экран
		int f2_result;
		buffer = "";  //Зачищаем буфер
		f2_result = funcs2.f2(str);	      //Вызываем функцию 2, получаем сумму элементов строки, являющихся числами
		cout << "Sum of numerical elements: " << f2_result << endl;
		data_ready = false;
		cout << "Enter string:" << endl;
		
        
        strcpy(msg, str.c_str());
        
        bytesWritten = send(clientSd, (char*)&msg, strlen(msg), 0);	//Отправляем строку в программу 2 
        if(bytesWritten == -1)
        {
        	cout << "Error when sending to server!" << endl;
        	
        }
        
      
        memset(&msg, 0, sizeof(msg));
        bytesRead = recv(clientSd, (char*)&msg, sizeof(msg), 0);	//Принимаем ответ от сервера, означающий, что сообщение //получено

       	if(!strcmp(msg, "1") == 0)
       	{
       		cout << "Server has quit the session. Restart server manually." << endl;
       		break;
       	}
        
        
     
        
    }	

    close(clientSd);
}
    
}


int main() {
	//Создаем 2 потока, в которых вызываются функции
	thread th1(t1);
	thread th2(t2);
	th1.join();
	th2.join();
	return 0;
}
