# ПРОСТОЙ КАЛЬКУЛЯТОР

Простой интерактивный калькулятор

Также этот файл с форматированием можно прочитать [тут](https://gist.github.com/ipsavitsky/cbd1123b2c095a87abc01ac1fdb15cd8)

Тут описаны только общие положения, более точная документация - в `docs/`(Doxyfile с конфигурацией на месте)

## Сборка

`make` - обычная сборка, без дебаг-информации, санитайзеров и покрытия кода(аналогично `make calc`)

`make calc_asan` - соберет с санитайзером

`make calc_cov` - соберет с контролем покрытия

`make fuzz` - соберет с фаззером

В `CORPUS` лежат нагенеренные им тесты)

Немного о фаззере

1. Поскольку ввод в рекурсивный спуск по-сути строка, то ее легко адаптировать под фаззер
2. Фаззер тестит только выражения без переменных, но их самому можно потестить в пользовательском интерфейсе)
3. Поскольку спуск рекурсивный, некоторые особые последовательности символов могут сильно "раздуть" занимаемую память стековыми кадрами, поэтому имеет смысл выставлять `-max_len` (я тестировал на `-max_len=1500`, работает нормально)
4. Начальный корпус есть в папке CORPUS, однако там настолько мало примеров что они почти никак не влияют на работу фаззера
5. Вывода в поток ошибок много, но зато когда попадаются примеры - они длинные и интересные :)
6. Есть еще файлы формата `test_*.c`, они для отдельных тестов отдельных библиотек, оставил я их тут просто так)

## Программный интерфейс

### `stack.h`

Простая библиотека для работы со стеком.

Сам стек в данном случае представляет собой массив `void*`, в котором укладываются элементы форматом

`|данные1|размер данных1|данные2|размер данных2...`

Обратный порядок выбран для того чтобы для операции pop() не проходиться по всему стеку с начала

### `RPN.h`

Простая библиотека для обработки польской инверсной записи

Сам полиз в данном случае тоже массив `void*`, в котором данные укладываются следующим образом

`|размер данных1|функция обработки данных1|данные1|размер данных2|функция обработки данных2|данные2`

Заметим, что в случае закладывания в полиз функции, данных с ней может не идти

Немного о выравнивании:

Размер и функция должны быть выровнены так как их выравнивает структура

```c
struct input_data {
       Size_elem size;
       Calculate_elem f;
};
```

а данные должны быть выровнены вручную. В size хранится размер `Calculate_elem`+размер данных С УЧЕТОМ выравнивания(зачем там `Calculate_elem` - сам не знаю, стоит убрать чтобы его там не было).

### `arithm_func.h`

Содержит в себе арифметические функции, ИСПОЛЬЗУЕМЫЕ КАЛЬКУЛЯТОРОМ(и только калькулятором)
Все функции тут - типа `Calculate_elem`. Созданы только для того чтобы потом быть использованы стековой машиной для вычисления полиза.

### `rec_desc.h`

Рекурсивный спуск, преобразующий инфексную запись, воспринимаемую человеком, в постфиксную, воспринимаемую полизом, реализованным в `RPN.h`

Обрабатывает 4 стандартные арифметические операции, скобки и унарный минус.
В спорных моментах обычно консультировался с тем, как обрабатывает выражения си.
Например, запись `3 - -7` неверная, а вот `3 - (-7)` - верная. Калькулятор игнорирует пробелы(как и другие вайтспейсы, например `\t`).

### `errors.h`

Ничего особо интересного, только енум возможных ошибок + функция преобразования ошибки в текст

## Пользовательский интерфейс

### `calculator.c`

Простейший repl интерфейс для этого калькулятора.

Может 2 вещи:

1. добавлять переменные
2. вычислять выражения

Если в выражении есть символ `=`, то это инициализация новой переменной. ВСЕ что стоит до равно становится именем переменной(это не костыль, это синтаксис такой!). Правая часть этого выражения обрабатывается рекурсивным спуском и подсчитывается. Например, такая последовательность команд корректна

```
>>> a= 122
>>> b= a-5
a = 122.000000
122.000000 - 5.000000 = 117.000000
```

Таким образом, в b будет лежать 117.(после а и b не стоит пробелов, потому что = стоит всегда на конце имени перемнной).

(ВАЖНО!) `q`(как единичный символ) зарезервирован для выхода из калькулятора. Теоретически, завести переменную `q` можно, но тогда выводить ее в выражении ИЛИ написать `q(пробел)`(опять же, это НЕ! костыль)

Если написать прсто выражение без знака равно, то это выражение вычислится и его результат выведется в виде `result = (число)`, напрмер

```
>>> kak= 56
>>> zhe= 123.32
>>> kruto= .001
>>> 0.001 * kak + zhe * kruto
kak = 56.000000
0.001000 * 56.000000 = 0.056000
zhe = 123.320000
kruto = 0.001000
123.320000 * 0.001000 = 0.123320
0.056000 + 0.123320 = 0.179320
result = 0.179320
>>> q
```

В случае какой-либо ошибки в выражении, будет выводиться сообщение об ошибке

```
>>> hello=123
>>> privet + 15
Error: unknown variable
result = 0.000000
>>>
```

## TODO

- [ ] переписать механизм таблицы переменных с меньшим обращением к менеджеру памяти(возможно, имеет смысл выделять память на n переменных за раз а потом либо расширять на множитель двух, либо обрезать ее искусственно)
- [ ] научиться по-нормальному парсить пробел в присвоении переменной(возможно, cтоит добавить обработку `=` в спуск)
- [ ] (ВАЖНО!)научиться переопределять переменные
- [ ] убрать размер Calculate_elem() из size(повременить с этим, может понадобиться в шелле, но пока что это лишние арифметические действия)