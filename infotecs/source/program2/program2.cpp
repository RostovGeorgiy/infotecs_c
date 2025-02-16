#include <iostream>
#include "funcs.h"
#include <string>
#include <stdio.h>
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
//Client side
int main(int argc, char *argv[])
{ //Сервер 
    
    int port = 8080;//Номер порта

    char msg[1500];
     
    //Установка соединения посредством сокетов
    sockaddr_in servAddr;
    bzero((char*)&servAddr, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(port);
 
    int serverSd = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSd < 0)
    {
        cerr << "Error establishing the server socket" << endl;
        exit(0);
    }

    int bindStatus = bind(serverSd, (struct sockaddr*) &servAddr, 
        sizeof(servAddr));
    if(bindStatus < 0)
    {
        cerr << "Error binding socket to local address" << endl;
        exit(0);
    }
    
    cout << "Waiting for a client to connect..." << endl;

    listen(serverSd, 5);
    
    sockaddr_in newSockAddr;
    socklen_t newSockAddrSize = sizeof(newSockAddr);
    //Принимаем запрос от клиента. При потере связи  сервер будет ожидать переподключения клиента
    while(1)
    {
    int newSd = accept(serverSd, (sockaddr *)&newSockAddr, &newSockAddrSize);
    if(newSd < 0)
    {
        cerr << "Error accepting request from client!" << endl;
        exit(1);
    }
    cout << "Connected with client!" << endl;
    
   
    int bytesRead, bytesWritten = 0;
    bool f3_result;
    string str = "1";
    Functions func3; //Создаем объект класса Functions
    while(1)
    {
   
        memset(&msg, 0, sizeof(msg));

        bytesRead = recv(newSd, (char*)&msg, sizeof(msg), 0); //Принимаем строку от клиента
        if(bytesRead == 0) 
        {
            cout << "Client has quit the session, waiting for reconnection" << endl; 
            break;
        }
        
        
        f3_result = func3.f3(msg); //Проверка полученной строки с помощью функции 3
        if(f3_result == true) 
        {
        	cout << "Recieved data: " << msg << endl;
    	}
    	else cout << "Data error!" << endl; //Выводим сообщение об ошибке, если функция возвращает false.
        
        memset(&msg, 0, sizeof(msg)); 
        strcpy(msg, str.c_str());
    
        bytesWritten = send(newSd, (char*)&msg, strlen(msg), 0); // Отправляем "1" для подтверждения успешного получения сообщения от клиента 
        
    }
    
    close(newSd);   //Закрываем дескриптор если связь потеряна. Будет создан новый дескриптор
    
    }
    close(serverSd);
    
    return 0;    
}
