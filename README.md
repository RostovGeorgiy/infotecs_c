
## О приложении
Я разработал приложение, состоящее из двух программ, взаимодействие которых реализовано посредством сокетов.
Программа 2 является сервером, программа 1 - клиентом.
Клиент имеет два потока.  
- Поток 1 принимает от пользователя строку и проверяет её на соответствие заданным параметрам. 
Если проверка успешна, производится вызов функции 1 с введенной строкой в качестве аргумента, после чего измененная в функции 1 строка помещается в общий буфер, из которого её считывает второй поток.
- Поток 2 находит сумму числовых элементов строки вызывая функцию 2, и выводит результат на экран. Затем измененная строка передаётся в программу 2 через сокет. В программе 2 вызывается функция 3 и проверяется соответствие строки двум параметрам: длина более  2 и кратность 32. При успешной проверке выводится информация о полученной строке, иначе выводится сообщение об ошибке.
Функции реализованы в виде динамической библиотеки. Они расположены в отдельной папке **include**. 
Функции объявлены в файле `funcs.h` и определены в файле `funcslib.cpp`. 
Следуя принципам ООП, все функции являются методами класса Functions, объекты которого создаются в программах 1 и 2 для работы с функциями.
Сокеты настроены на порт _8080_ и адрес _127.0.0.1_. Эти данные можно изменить в коде программ. 
Для компиляции используется CMake.


## Сборка и запуск приложения 
В папке **infotecs** необходимо создать папку **build**.
В папке build введите `cmake /adress/to/source/folder`, затем `make`.
Сначала запускается сервер: 
`./program2/program2`, 
затем клиент: 
`./program1/program1`.
Пользователь вводит строку в терминале клиента. 
При отключении клиента, сервер создает новый дескриптор сокета и ожидает переподключения клиента. При отключении сервера, требуется перезапустить сервер вручную.
