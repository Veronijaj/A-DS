# Работа с хеш-таблицей, размещеной в основной памяти
## Основные свойства таблицы
Просматриваемая таблица на основе вектора. <br />
Элементы пространства ключей таблицы заданы следующей структурой:<br />
```
 struct KeySpace {
// признак занятости элемента
BusyType busy;
// ненулевой ключ элемента
KeyType key;
// ключ родительского элемента, может быть нулевым
KeyType par;
// указатель на информацию<br />
Item *info;
};
```
Максимальный размер таблицы ограничен величиной msize и указывается при её инициализации.<br />
В таблице не может быть двух элементов с одинаковыми значениями ключей, а значение ключа
родительского элемента для элемента должно совпадать с каким-либо значением ключа элемента,
существующего в таблице, либо быть нулевым.<br />
Для таблицы предусмотрены следующие особые операции:<br />
* удаление из таблицы элемента, заданного своим ключом, без реорганизации таблицы; при этом
также должны быть удалены (рекурсивно) все элементы, ссылающиеся на удаляемый;
* реорганизация таблицы (сборка мусора: сжатие таблицы, путем удаления свободных элементов).<br />
## Типы данных<br />
Должны поддерживаться следующие типы данных:<br />
* ключ — беззнаковое целое число;
* значение — нуль-терминированная строка произвольной длины.
## Общее задание