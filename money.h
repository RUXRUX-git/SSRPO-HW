#include <iomanip>

using namespace std;

struct Money {
	size_t  roubles;
	uint8_t kopecks;

	bool invariant() {
		return kopecks < 100;
	}

	Money operator+(const Money& rv) const {
		uint8_t kopecksSum = kopecks + rv.kopecks;
		return Money {roubles + rv.roubles + (kopecksSum / 100), static_cast<uint8_t>(kopecksSum % 100)};
	}

	bool operator<=(const Money& rv) const {
		if (roubles < rv.roubles) {
			return true;
		}
		else if (roubles == rv.roubles) {
			if (kopecks <= rv.kopecks) {
				return true;
			}
			else {
				return false;
			}
		}
		else {
			return false;
		}
	}

	bool operator==(const Money& rv) const {
		return (roubles == rv.roubles) && (kopecks == rv.kopecks); 
	}

	bool operator!=(const Money& rv) const {
		return !(*this == rv);
	}

	bool operator<(const Money& rv) const {
		return (*this != rv) && (*this <= rv);
	}

	bool operator>=(const Money& rv) const {
		return !(*this < rv);
	}

	Money operator*(size_t mul) const {
		size_t kopecks_multiplied = kopecks * mul;

		return Money {roubles + kopecks_multiplied / 100, static_cast<uint8_t>(kopecks_multiplied % 100)};
	}
};

Money strToMoney(const string& str) {
	size_t comma_pos = str.find(',');
	assert(comma_pos != 0);
	assert(comma_pos != string::npos);
	size_t roubles = stoul(str.substr(0, comma_pos));
	uint8_t kopecks = stoi(str.substr(comma_pos + 1));

	return Money {roubles, kopecks};
}

ostream& operator<<(ostream& os, Money mn) {
	os << mn.roubles << "," << setw(2) << setfill('0') << static_cast<int>(mn.kopecks);
	return os;
}