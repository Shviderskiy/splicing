### Build

Сборка тестового исполняемого файла.
 
	user:~/example$ make foo


Сборка тестовой библиотеки для перехвата.
 
	user:~/example$ make lib


Запуск исполняемого файла с библиотекой перехвата.
 
	user:~/example$ make run

### Usage
<!-- 
Необходимо реализовать функцию splicing::init().
Именно в ней должна осуществляться устновка хуков.
-->

Для примера будем перехватывать функцию `bar` из `example/foo.cpp`.


	int bar(int param) {

    	::printf("-> bar [executable]\n");
    	::printf("   param =  %d\n", param);
    	return param + 1;
	}


Откомпилируем файл `foo.cpp`

	user:~/example$ make foo

и запустим чтобы посмотреть адрес функции `bar`.

	user:~/example$ ./foo
	-> main [executable]
   	   LD_PRELOAD not found
       &bar = 0x80484fd
	-> bar [executable]
       param =  100500
       result = 100501

> *Посмотреть адрес можно также при помощи gdb.*



Видно, что функция `bar` вызвалась с указанными в `example/foo.cpp` параметрами.

	int result = bar(100500);


Теперь укажем в файле `example/init.cpp` что нас интересует адрес `0x80484fd`.

	static void* bar_address = 
	    reinterpret_cast<void*>(0x80484fd);


Установка хука происходит в функции `splicing::init()`.

	splicing::set_hook(::bar_address, my_bar);

Теперь при вызове оригинальной функции `bar` из `example/foo` управление будет передано на определенную нами функцию `my_bar`. 

Обе функции `bar` и `my_bar` имеют одинаковую сигнатуру, и, для простоты примера, сборка происходит с одними и теми-же флагами.

> *Обработчиком хука не обязательно должна быть функция. Существует возможность передать управление по произвольному адресу, например на ассемблерную вставку. Но не стоит забывать, что в этом случае, следить за сохранностью стека придется самостоятельно.*


Если потребуется вызвать оригинальную функцию `bar`, то необходимо восстановить ее содержимое и извлечь ее адрес.

	int(*origin_bar)(int) = 
	    splicing::restore<int(int)>(::bar_address);



После восстановления оригинальной функции, в нее можно передать в любой параметр.

	int fictive_param = 400;
	return origin_bar(fictive_param);


Для того чтобы увидеть результат нужно собрать библиотеку и запустить исполняемый файл с `LD_PRELOAD`.

	user:~/example$ make lib
	user:~/example$ make run # LD_PRELOAD=./lib.so ./foo
	-> splicing::init [library]
   	   &my_bar = 0xb770b8dd
	-> main [executable]
	   LD_PRELOAD not found
       &bar = 0x80484fd
	-> my_bar [library]
   	   param = 100500
	-> bar [executable]
   	   param =  400
   	   result = 401

Как видно, `splicing::init` вызвался перед функцией `main`, что позволило модифицировать код функции целевого процесса.


> *Важно! Разница любых двух адресов, на которые будут устанавливаться хуки, не должна превышать 16 байт на x64 системах и 10 байт на x86.*

