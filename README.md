<!-- http://www.splice-design.com/wp-content/uploads/2010/03/SPL_logo2.png -->
<img 
    src="http://www.root6.com/wp-content/uploads/2015/12/Splice_logo.jpg.png"
	href="https://github.com/Shviderskiy/splicing"
    align="right"
    hspace="20px"
/>

## Overview

Реализация техники перехвата любой функции (инструкции по определенному адресу) в исполняемом файле при помощи [splicing](https://ru.wikipedia.org/wiki/Перехват_(программирование)#.D0.A1.D0.BF.D0.BB.D0.B0.D0.B9.D1.81.D0.B8.D0.BD.D0.B3) и [LD_PRELOAD](https://habrahabr.ru/post/199090/).

Перед началом использования стоит отключить механизм защиты [ASLR](https://ru.wikipedia.org/wiki/ASLR), поскольку адреса перехвата должны иметь постоянные значения при каждом запуске процесса.

`sudo echo 0 > /proc/sys/kernel/randomize_va_space`

Вычисление адресов инструкций на основании смещений относительно точки входа не являлось основной целью.
Подобную логику предлагается реализовать самостоятельно.


### [Usage example](https://github.com/Shviderskiy/splicing/blob/master/example/README.md)
