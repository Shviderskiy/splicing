### Build

Подготовка к сборке.

	user:~# git clone https://github.com/Shviderskiy/splicing.git ~/splicing
	user:~# cd ~/splicing/example


Сборка тестового исполняемого файла.
 
	user:~/splicing/example$ make foo


Сборка тестовой библиотеки для перехвата.
 
	user:~/splicing/example$ make lib


Запуск исполняемого файла с библиотекой перехвата.
 
	user:~/splicing/example$ make run

> *Запуск с огромной вероятностью произойдет некорректно! См. Usage.*

### Usage

Для примера будем перехватывать функцию `bar` из `foo.cpp`.


	int bar(int param) {

    	::printf("-> bar [executable]\n");
    	::printf("   param =  %d\n", param);
    	return param + 1;
	}


Откомпилируем файл `foo.cpp`

	user:~/splicing/example$ make foo

и запустим чтобы посмотреть адрес функции `bar`.

	user:~/splicing/example$ ./foo
	-> main [executable]
   	   LD_PRELOAD not found
       &bar = 0x80484fd
	-> bar [executable]
       param =  100500
       result = 100501

> *Посмотреть адрес можно также при помощи gdb.*



Видно, что функция `bar` вызвалась с указанными в `foo.cpp` параметрами.

	int result = bar(100500);


Теперь укажем в файле `init.cpp`, что нас интересует адрес `0x80484fd`.

	static void* bar_address = 
	    reinterpret_cast<void*>(0x80484fd);


Код, отвечающий за установку хука, находится в функции `splicing::init()`.

	splicing::set_hook(::bar_address, my_bar);

Теперь при вызове оригинальной функции `bar` из файла `foo` управление будет передано на определенную нами функцию `my_bar`. 

Обе функции `bar` и `my_bar` имеют одинаковую сигнатуру, и, для простоты примера, сборка происходит с одними и теми-же флагами, а значит их прологи не будут отличаться.

> *Обработчиком хука не обязательно должна быть функция. Существует возможность передать управление по произвольному адресу, например на ассемблерную вставку. Но не стоит забывать, что в этом случае, следить за сохранностью стека придется самостоятельно.*


Если потребуется вызвать оригинальную функцию `bar`, то необходимо восстановить ее содержимое и извлечь ее адрес.

	int(*origin_bar)(int) = 
	    splicing::restore<int(int)>(::bar_address);



После восстановления оригинальной функции, в нее можно передать в любой параметр.

	int fictive_param = 400;
	return origin_bar(fictive_param);


Для того чтобы увидеть результат нужно собрать библиотеку и запустить исполняемый файл с `LD_PRELOAD`.

	user:~/splicing/example$ make lib
	user:~/splicing/example$ make run # LD_PRELOAD=./lib.so ./foo
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

