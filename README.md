# Подмножество утилиты find.

## Программа умеет:

- Первым аргументом принимать абсолютный путь, в котором будет производиться поиск файлов.
- По-умолчанию выводить в стандартный поток вывода все найденные файлы по этому пути
- Поддерживать аргумент -inum num. Аргумент задает номер инода
- Поддерживать аргумент -name name. Аргумент задает имя файла
- Поддерживать аргумент -size [-=+]size. Аргумент задает фильтр файлов по размеру(меньше, равен, больше)
- Поддерживать аргумент -nlinks num. Аргумент задает количество hardlink'ов у файлов
- Поддерживать аргумент -exec path. Аргумент задает путь до исполняемого файла, которому в качестве единственного аргумент нужно передать найденный в иерархии файл
- Поддерживать комбинацию аргументов. Например хочется найти все файлы с размером больше 1GB и скормить их утилите /usr/bin/sha1sum.
- Выполнять поиск рекурсивно, в том числе во всех вложенных директориях.