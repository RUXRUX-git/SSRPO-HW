<div align="center">
Министерство науки и высшего образования Российской Федерации <br />
Федеральное государственное бюджетное образовательное учреждение <br />
высшего образования <br />
«Московский государственный технический университет <br />
имени Н.Э. Баумана <br />
(национальный исследовательский университет)» <br />
(МГТУ им. Н.Э. Баумана)
</div>
<hr />
<div align="center">
ФАКУЛЬТЕТ ИНФОРМАТИКА И СИСТЕМЫ УПРАВЛЕНИЯ <br />
КАФЕДРА КОМПЬЮТЕРНЫЕ СИСТЕМЫ И СЕТИ (ИУ6)
</div>
<br />
<div align="center">
ОТЧЕТ <br />
по домашнему заданию <br />
по дисциплине "Современные средства разработки <br />
программного обеспечения"
</div>
<br />

Преподаватель: Фетисов М.В.

Студент группы ИУ6-55Б Ибрагимов Руслан Сиражутинович.

## Описание задания

Задача №13: "Магазин автозапчастей".

Карточка CarPart должна содержать следующие данные:

* Название автозапчасти
* Марка автомобиля
* Модель автомобиля
* Стоимость
* Наличие

## Цели работы

На примере программы на языке С++:

* изучить объекты с разными видами состояний;
* определить инвариант класса;
* освоить работу с обобщёнными контейнерами стандартной библиотеки С++
* освоить работу с потоками ввода / вывода стандартной библиотеки С++
* освоить использование основных паттернов объектно-ориентированного проектирования
* освоить применение инструментов git и GitLab

## Адрес проекта

Проект хранится в репозитории по адресу: <!-- Добавить -->

Описание классов хранится в репозитории по адресу: <!-- Добавить -->

## Инвариант класса Person

```cpp
    bool invariant() {
        return _car_part.size() <= MAX_CAR_PART_LENGTH 
            && _car_make.size() <= MAX_CAR_MAKE_LENGTH
            && _car_model.size() <= MAX_CAR_MODEL_LENGTH
            && _cost.invariant()
            && _cost <= MAX_CAR_PART_COST
            && _cost >= MIN_CAR_PART_COST;
    }
```

## Выводы

<!-- Добавить -->
