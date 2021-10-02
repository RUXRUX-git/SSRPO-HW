#include <iostream>
#include <cassert>
#include <vector>
#include <memory>
#include <fstream>

#include "money.h"
#include "collector.h"

using namespace std;

const size_t MAX_CAR_PART_LENGTH  = 50;
const size_t MAX_CAR_MAKE_LENGTH  = 30;
const size_t MAX_CAR_MODEL_LENGTH = 40;
const Money  MIN_CAR_PART_COST = Money{50, 0};
const Money  MAX_CAR_PART_COST = Money{1'000'000'000, 0};

bool readBool(std::istream& is) {
	uint8_t number_result;
	is.read(reinterpret_cast<char *>(&number_result), sizeof(number_result));
	return (number_result == 0) ? false : true;
}

void writeBool(std::ostream& os, bool value) {
	uint8_t file_value = (value) ? 1 : 0;
	writeNumber<uint8_t>(os, file_value);
}

class CarPart : public ICollectable{
private:
	string _car_part;
	string _car_make;
	string _car_model;
	Money _cost;
	bool _available;

protected:
	bool invariant() {
		return _car_part.size() <= MAX_CAR_PART_LENGTH 
			&& _car_make.size() <= MAX_CAR_MAKE_LENGTH
			&& _car_model.size() <= MAX_CAR_MODEL_LENGTH
			&& _cost.invariant()
			&& _cost <= MAX_CAR_PART_COST
			&& _cost >= MIN_CAR_PART_COST;
	}
public:
	CarPart() = delete;
	CarPart(const CarPart&) = delete;

	CarPart& operator=(const CarPart&) = delete;

	CarPart(const string& car_part, const string& car_make, const string& car_model, Money cost, bool available) 
	      : _car_part(car_part), _car_make(car_make), _car_model(car_model), _cost(cost), _available(available)
	{
		assert(invariant());
	}

	const string& getCarPart()      const { return _car_part;  }
	const string& getCarMake()      const { return _car_make;  }
	const string& getCarModel()     const { return _car_model; }
	const Money   getCost()         const { return _cost;      }
	const bool    getAvailability() const { return _available; }

	bool write(ostream& os) override {
		writeString(os, _car_part);
		writeString(os, _car_make);
		writeString(os, _car_model);
		writeNumber(os, _cost.roubles);
		writeNumber(os, _cost.kopecks);
		writeBool  (os, _available);

		return os.good();
	}
};

class ItemCollector : public ACollector {
public:
	shared_ptr<ICollectable> read(istream& is) override {
		string  car_part  = readString(is, MAX_CAR_PART_LENGTH);
		string  car_make  = readString(is, MAX_CAR_MAKE_LENGTH);
		string  car_model = readString(is, MAX_CAR_MODEL_LENGTH);
		size_t  roubles   = readNumber<size_t>(is);
		uint8_t kopecks   = readNumber<uint8_t>(is);
		bool    available = readBool(is);

		return make_shared<CarPart>(car_part, car_make, car_model, Money {roubles, kopecks}, available);
    }
};

bool performCommand(const vector<string> & args, ItemCollector & col)
{
    if (args.empty()) {
        return false;
    }

    else if (args[0] == "l" || args[0] == "load") {
        string filename = (args.size() == 1) ? "hw.data" : args[1];

        if (!col.loadCollection(filename))
        {
            cerr << "Ошибка при загрузке файла '" << filename << "'" << endl;
            return false;
        }

        return true;
    }

 	else if (args[0] == "s" || args[0] == "save") {
        string filename = (args.size() == 1) ? "hw.data" : args[1];

        if (!col.saveCollection(filename))
        {
            cerr << "Ошибка при сохранении файла '" << filename << "'" << endl;
            return false;
        }

        return true;
    }

    else if (args[0] == "c" || args[0] == "clean") {
        if (args.size() != 1)
        {
            cerr << "Некорректное количество аргументов команды clean" << endl;
            return false;
        }

        col.clean();

        return true;
    }

    else if (args[0] == "a" || args[0] == "add") {
        if (args.size() != 6)
        {
            cerr << "Некорректное количество аргументов команды add" << endl;
            return false;
        }

        col.addItem(make_shared<CarPart>(args[1].c_str(), args[2].c_str(), args[3].c_str(), 
        	                             strToMoney(args[4].c_str()), (args[5] == "true") ? true : false));
        return true;
    }

    else if (args[0] == "r" || args[0] == "remove") {
        if (args.size() != 2)
        {
            cerr << "Некорректное количество аргументов команды remove" << endl;
            return false;
        }

        col.removeItem(stoul(args[1]));
        return true;
    }

    else if (args[0] == "u" || args[0] == "update") {
        if (args.size() != 7)
        {
            cerr << "Некорректное количество аргументов команды update" << endl;
            return false;
        }

        col.updateItem(stoul(args[1]), make_shared<CarPart>(args[2].c_str(), args[3].c_str(), args[4].c_str(), 
        	                             strToMoney(args[5].c_str()), (args[6] == "true") ? true : false));
        return true;
    }

    else if (args[0] == "v" || args[0] == "view") {
        if (args.size() != 1)
        {
            cerr << "Некорректное количество аргументов команды view" << endl;
            return false;
        }

        size_t count = 0;
        for(size_t i=0; i < col.getSize(); ++i)
        {
            const CarPart & item = static_cast<CarPart &>(*col.getItem(i));

            if (!col.isRemoved(i))
            {
                cout << "[" << i << "] "
                		<< "Автозапчасть: "  << item.getCarPart()  << endl 
                		<< "Марка машины: "  << item.getCarMake()  << endl
                		<< "Модель машины: " << item.getCarModel() << endl
                		<< "Стоимость автозапчасти: "      << item.getCost()     << " рублей"    << endl
                		<< "Наличие: " << ((item.getAvailability()) ? ("Есть") : ("Нет")) << " в наличии" << endl;
                count ++;
            }
        }

        cout << "Количество элементов в коллекции: " << count << endl;

        return true;
    }

    cerr << "Недопустимая команда '" << args[0] << "'" << endl;
    return false;
}

int main() {
    ItemCollector col;

    for (string line; getline(cin, line); )
    {
        if (line.empty())
            break;

        istringstream  iss(line);
        vector<string> args;

        for(string str; iss.good();)
        {
            iss >> str;
            args.emplace_back(str);
        }

        if (!performCommand(args, col))
            return 1;
    }

    cout << "Выполнение завершено успешно" << endl;
    return 0;
}