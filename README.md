<!-- http://www.splice-design.com/wp-content/uploads/2010/03/SPL_logo2.png -->
<img 
    src="http://www.root6.com/wp-content/uploads/2015/12/Splice_logo.jpg.png"
	href="https://github.com/Shviderskiy/splicing"
    align="right"
    hspace="20px"
/>

## Description

Реализация техники перехвата любой функции (инструкции по определенному адресу) в исполняемом файле при помощи <a target="_blank" href="https://ru.wikipedia.org/wiki/%D0%9F%D0%B5%D1%80%D0%B5%D1%85%D0%B2%D0%B0%D1%82_(%D0%BF%D1%80%D0%BE%D0%B3%D1%80%D0%B0%D0%BC%D0%BC%D0%B8%D1%80%D0%BE%D0%B2%D0%B0%D0%BD%D0%B8%D0%B5)#.D0.A1.D0.BF.D0.BB.D0.B0.D0.B9.D1.81.D0.B8.D0.BD.D0.B3">splicing</a> и <a target="_blank" href="https://habrahabr.ru/post/199090/">LD_PRELOAD</a>.

Перед началом использования необходимо отключить механизм защиты <a target="_blank" href="https://ru.wikipedia.org/wiki/ASLR">ASLR</a> поскольку адреса перехвата должны иметь постоянные значения при каждом запуске процесса.

`sudo echo 0 > /proc/sys/kernel/randomize_va_space`

Вычисление адресов инструкций на основании смещений относительно точки входа не являлось основной целью.
Подобную логику предлагается реализовать самостоятельно.

### <a target="_blank" href="https://github.com/Shviderskiy/splicing/blob/master/example/README.md"> Usage example </a>

##