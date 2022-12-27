/*
İsmail Mert Öztürk	-	402506
*/

	// Uygulama İçeriği;
/*

	Uygulama çalıştırıldığında;

		->	Menü hiyerarşisi tanımlanır. Bunun için n-ary tree olarak adlandırılan yapı kullanılmıştır.
		->	Uygulama zamanı ayarlanır. Bunun için courier.txt dosyası okunur ve son gerçekleşen siparişin bitiş zamanı esas alınır.
			Daha önce hiç sipariş yapılmamış veya böyle bir dosya bulunmuyor ise default zaman tanımlanır.
		->	Uygulama menüler arası geçiş için döngüye girer ve kullanıcı seçimlerine göre ilerler.


	Kullanılan sınıflar hakkında bilgiler;

		* Time
			->	Zaman işlemlerini gerçekleyen sınıftır.
			->	operator overloading kavramı etkili bir şekilde kullanılmıştır.

		* Clothes
			->	Kıyafet işlemlerini gerçekleyen sınıftır.

		* Order
			->	Sipariş işlemlerini gerçekleyen sınıftır.
			->	Clothes sınıfını miras almıştır.

		* Person
			->	Kişi bilgilerini içeren bir temel sınıftır.

		* Client
			->	Müşteri bilgilerini tutan ve kullanmamızı sağlayan sınıftır.
			->	Person sınıfını miras almıştır.

		* Admin
			->	Yönetici bilgilerini tutan ve kullanmamızı sağlayan sınıftır.
			->	Person sınıfını miras almıştır.

		* Courier
			->	Kurye bilgilerini tutan ve kullanmamızı sağlayan sınıftır.
			->	Person sınıfını miras almıştır.

		* Invoice
			->	Fatura işlemlerini gerçekleyen sınıftır.

		* Buffer
			->	Menü bilgilerini, hataları ve uyarıları konsol ekranı aracılığıyla kullanıcıya iletmeye yarayan sınıftır.
			->	Belli fonksiyonlar kullanarak çıktıyı daha anlaşılır ve göze hoş gelecek şekilde iletir.

		* Validator
			->	Çeşitli yerlerde kullanıcıdan alınan girişlerin belli durumları sağlayıp sağlamamasını test eden sınıftır.
			->	Duruma göre true veya false vererek kullanıcının uyarılmasını sağlar.

		* Action
			->	Uygulamanın ana fonksiyonlarını gerçekleştiren sınıflar bu sınıfıntan türer.
			->	Bütün sınıflarca kullanılan ortak fonksiyonları (dosya okuma/yazma, kullanıcı girişi alma) CommonActions sınıfı gerçekleştirir.
			->	Asıl işleri yapan sınıflar bu Action ve CommonActions sınıflarını miras alarak kendi görevlerini yerine getirir.
			->	Her sınıfın görevi kendi alanlarında yorum satırı eklenerek açıklanmıştır.

		* MenuNode
			->	Uygulama menü yapısındaki her bir menü bir node'dan oluşmaktadır. MenuNode sınıfı bu yapıyı tutar.
			->	Her node'un actionHandler özelliği vardır ve o menünün işlevine göre bir Action sınıfı atanır.

		* MenuTree
			->	Uygulama menü yapısının kendisidir. root node constructor ile belirlenir ve diğer node'lar gerekli yerlere eklenerek yapı oluşturulur.
			->	Yapı olarak n-ary tree esas alınmıştır.

		* Application
			->	Uygulamayı başlatıcı sınıftır.
			->	Uygulama çalıştırılıdığında init() fonksiyonu devreye girer.
			->	Önce karşılama ekranı gösterilir, ardından uygulamanın menü yapısı inşa edilir ve gerisi kullanıcının seçimlerine bırakılır.
*/

#include <iostream>		// cin<<	cout>>
#include <conio.h>		// _getch()
#include <fstream>		// ifstream		ofstream	fstream

#include <cctype>		// isascii()	tolower()
#include <regex>		// regex_search()	regex_match()	regex()
#include <string>		// string
#include <vector>		// vector

using namespace std;

// proje verilerinin saklandığı dosyaların yolu ve dosya isimleri buradan değiştirilebilir.
// dosyaların kök dizini (roothPath) değiştirilecekse mevcut bir yer olmalıdır. aksi halde dosyalar oluşmayacak/okunmayacaktır.
// bu değişiklikler uygulama başlatılmadan önce yapılmalıdır.
static const string rootPath = ".\\data\\";
static const string adminFile = "admin.txt";
static const string clientFile = "client.txt";
static const string feedbackFile = "feedback.txt";
static const string productFile = "product.txt";
static const string courierFile = "courier.txt";
static const string invoiceFile = "invoice.txt";
static const string orderFile = "order.txt";
//

//	zaman sınıfı
#pragma region Time
class Time
{
public:
	Time(const string&);
	Time(int _d = 0, int _h = 0, int _m = 0);

	Time operator+(const Time& other);
	bool operator>(const Time& other);
	bool operator<(const Time& other);

	int getDay() const;
	int getHour() const;
	int getMinute() const;
	string getStrTime() const;
	string getStrFullTime() const;
	
	void setDay(const int& _d);
	void setHour(const int& _h);
	void setMinute(const int& _m);
	void setTime(const int& _d, const int& _h, const int& _m);
private:
	int day;	// gün
	int hour;	// saat
	int minute;	// dakika
};

Time::Time(const string& str) {
	// like -> "00:11:40"
	size_t idx1, idx2;

	idx1 = str.find(':');
	idx2 = str.find(':', idx1 + 1);

	int _day = stoi(str.substr(0, idx1));
	int _hour = stoi(str.substr(idx1 + 1, idx2 - idx1 - 1));
	int _min = stoi(str.substr(idx2 + 1, str.length()));

	setTime(_day, _hour, _min);
}

Time::Time(int _d, int _h, int _m) {
	setTime(_d, _h, _m);
}

Time Time::operator+(const Time& other) {

	int sumDay = day + other.getDay();
	int sumHour = hour + other.getHour();
	int sumMinute = minute + other.getMinute();

	if (sumMinute > 59) {
		sumHour++;
		sumMinute %= 60;
	}
	if (sumHour > 23) {
		sumDay++;
		sumHour %= 24;
	}

	return Time(sumDay, sumHour, sumMinute);
}
bool Time::operator>(const Time& other) {
	if (day > other.day)
		return true;
	if (day == other.day && hour > other.hour)
		return true;
	if (day == other.day && hour == other.hour && minute > other.minute)
		return true;
	return false;
}
bool Time::operator<(const Time& other) {
	if (day < other.day)
		return true;
	if (day == other.day && hour < other.hour)
		return true;
	if (day == other.day && hour == other.hour && minute < other.minute)
		return true;
	return false;
}

int Time::getDay() const {
	return day;
}
int Time::getHour() const {
	return hour;
}
int Time::getMinute() const {
	return minute;
}
string Time::getStrTime() const {
	string strHour, strMin;
	strHour = (hour / 10 == 0) ? "0" + to_string(hour) : to_string(hour);
	strMin = (minute / 10 == 0) ? "0" + to_string(minute) : to_string(minute);
	return strHour + ":" + strMin;
}
string Time::getStrFullTime() const {
	string strDay, strHour, strMin;
	strDay = (day / 10 == 0) ? "0" + to_string(day) : to_string(day);
	strHour = (hour / 10 == 0) ? "0" + to_string(hour) : to_string(hour);
	strMin = (minute / 10 == 0) ? "0" + to_string(minute) : to_string(minute);
	return strDay + ":" + strHour + ":" + strMin;
}

void Time::setDay(const int& _d) {
	day = _d;
}
void Time::setHour(const int& _h) {
	hour = _h;
}
void Time::setMinute(const int& _m) {
	minute = _m;
}
void Time::setTime(const int& _d, const int& _h, const int& _m) {
	setDay(_d);
	setHour(_h);
	setMinute(_m);
}
#pragma endregion

//	kiyafet sınıfı
#pragma region Clothes
class Clothes
{
public:
	Clothes();
	Clothes(string& str);
	Clothes(string& _category, string& _name, string& _size, string& _color, double& _price);

	string getClotheString() const;

	string getCategory() const;
	string getName() const;
	string getSize() const;
	string getColor() const;
	double getPrice() const;

	void setClothes(const string&, const string&, const string&, const string&, const double&);
	void setCategory(const string&);
	void setName(const string&);
	void setSize(const string&);
	void setColor(const string&);
	void setPrice(const double&);
private:
	string category;	// kategori
	string name;		// kıyafet adı
	string size;		// boyut
	string color;		// renk
	double price;		// fiyat
};

Clothes::Clothes() = default;
Clothes::Clothes(string& str) {
	size_t idx1, idx2, idx3, idx4;
	
	string _category, _name, _price, _size, _color;

	idx1 = str.find('-');
	idx2 = str.find('-', idx1 + 1);
	idx3 = str.find('-', idx2 + 1);
	idx4 = str.find('-', idx3 + 1);

	_category = str.substr(0, idx1);
	_name = str.substr(idx1 + 1, idx2 - idx1 - 1);
	_price = str.substr(idx2 + 1, idx3 - idx2 - 1);
	_size = str.substr(idx3 + 1, idx4 - idx3 - 1);
	_color = str.substr(idx4 + 1, str.length());

	setClothes(_category, _name, _size, _color, stod(_price));
}
Clothes::Clothes(string& _category, string& _name, string& _size, string& _color, double& _price) {
	setClothes(_category, _name, _size, _color, _price);
}

void Clothes::setClothes(const string& _cat, const string& _name, const string& _size, const string& _col, const double& _pri) {
	setCategory(_cat);
	setName(_name);
	setSize(_size);
	setColor(_col);
	setPrice(_pri);
}
void Clothes::setCategory(const string& _cat) {
	category = _cat;
}
void Clothes::setName(const string& _name) {
	name = _name;
}
void Clothes::setSize(const string& _size) {
	size = _size;
}
void Clothes::setColor(const string& _col) {
	color = _col;
}
void Clothes::setPrice(const double& _pri) {
	price = _pri;
}

string Clothes::getClotheString() const {
	return category + '-' + name + '-' + size + '-' + color + '-' + to_string(price);
}
string Clothes::getCategory() const {
	return category;
}
string Clothes::getName() const {
	return name;
}
string Clothes::getSize() const {
	return size;
}
string Clothes::getColor() const {
	return color;
}
double Clothes::getPrice() const {
	return price;
}
#pragma endregion

//	siparis sınıfı
#pragma region Order
class Order : public Clothes
{
public:
	Order(string& str);
	Order(string& _category, string& _name, string& _size, string& _color, double& _price);
	Order(int& id, Clothes& clothe, Time& _ordertime, Time& _deliverytime);

	string getOrderString() const;
	string getBasket() const;

	int getOrderID() const;
	Time getOrderTime() const;
	Time getDeliveryTime() const;

	void setOrder(const int& id, const Time& _orderTime, const Time& _deliveryTime);
	void setOrderID(const int& id);
	void setOrderTime(const Time& _orderTime);
	void setDeliveryTime(const Time& _deliveryTime);

private:
	int orderID;		// sipariş no
	Time orderTime;		// sipariş fiyatı
	Time deliveryTime;	// siparis ulasim zamanı
};

Order::Order(string& str)
	: Clothes(str) {
}
Order::Order(string& _category, string& _name, string& _size, string& _color, double& _price)
	: Clothes(_category, _name, _size, _color, _price) {
}
Order::Order(int& id, Clothes& clothe, Time& _ordertime, Time& _deliverytime)
	: Clothes(clothe) {
	setOrder(id, _ordertime, _deliverytime);
}
// orders.txt dosyasından nasıl bilgi okuyacaksın ona göre düzenle.
// shoppingAction'da updateOrderFile fonksiyonu ile birlikte çalışacak.
string Order::getOrderString() const {
	string str = to_string(orderID) + '-' + orderTime.getStrTime() + '-' + deliveryTime.getStrTime();
	return str;
}
string Order::getBasket() const {
	string str = getCategory() + '-' + getName() + '-' + getSize() + '-' + getColor() + '-' + to_string(getPrice());
	return str;
}
int Order::getOrderID() const {
	return orderID;
}
Time Order::getOrderTime() const {
	return orderTime;
}
Time Order::getDeliveryTime() const {
	return deliveryTime;
}

void Order::setOrder(const int& id, const Time& _orderTime, const Time& _deliveryTime) {
	setOrderID(id);
	setOrderTime(_orderTime);
	setDeliveryTime(_deliveryTime);
}
void Order::setOrderID(const int& id) {
	orderID = id;
}
void Order::setOrderTime(const Time& _orderTime) {
	orderTime = _orderTime;
}
void Order::setDeliveryTime(const Time& _deliveryTime) {
	deliveryTime = _deliveryTime;
}
#pragma endregion

//	kisi sınıfı
#pragma region Person
class Person
{
public:
	Person();
	Person(const string& _fullName, const string& _phone);
	string fullName;	// ad soyad
	string phoneNumber;	// telefon
};

Person::Person() = default;
Person::Person(const string& _fullName, const string& _phone) {
	fullName = _fullName;
	phoneNumber = _phone;
}
#pragma endregion

//	kullanici sınıfı
#pragma region Client
class Client : public Person
{
public:
	Client(const string& str);
	Client(const string& _fullName, const string& _userName, const string& _pw, const string& _bday, const string& _email, const string& _phone, const string& _address, const string& _code = "NaN");
	
	string getClientString() const;

	string getFullName() const;
	string getPhoneNumber() const;
	string getUserName() const;
	string getPassword() const;
	string getEmail() const;
	string getBirthday() const;
	string getAddress() const;
	string getDiscountCode() const;

	void setPassword(const string&);
	void setDiscountCode(const string&);
private:
	string userName;		// kullanıcı adı
	string password;		// şifre
	string email;			// eposta
	string birthday;		// doğum tarihi
	string address;			// adres
	string discountCode;	// indirim kodu
};

Client::Client(const string& _fullName, const string& _userName, const string& _pw, const string& _bday, const string& _email, const string& _phone, const string& _address, const string& _code)
	:	Person(_fullName, _phone) {
	userName = _userName;
	password = _pw;
	birthday = _bday; 
	email = _email;
	phoneNumber = _phone;
	address = _address;
	discountCode = _code;
}
Client::Client(const string& str) {
	size_t idx1, idx2, idx3, idx4, idx5, idx6, idx7;

	idx1 = str.find('-');
	idx2 = str.find('-', idx1 + 1);
	idx3 = str.find('-', idx2 + 1);
	idx4 = str.find('-', idx3 + 1);
	idx5 = str.find('-', idx4 + 1);
	idx6 = str.find('-', idx5 + 1);
	idx7 = str.find('-', idx6 + 1);

	fullName = str.substr(0, idx1);
	userName = str.substr(idx1 + 1, idx2 - idx1 - 1);
	password = str.substr(idx2 + 1, idx3 - idx2 - 1);
	birthday = str.substr(idx3 + 1, idx4 - idx3 - 1);
	email = str.substr(idx4 + 1, idx5 - idx4 - 1);
	phoneNumber = str.substr(idx5 + 1, idx6 - idx5 - 1);
	address = str.substr(idx6 + 1, idx7 - idx6 - 1);
	discountCode = str.substr(idx7 + 1, str.length());
}

string Client::getClientString() const {
	return fullName + "-" + userName + "-" + password + "-" + birthday + "-" + email + "-" + phoneNumber + "-" + address + "-" + discountCode;
}
string Client::getFullName() const {
	return fullName;
}
string Client::getPhoneNumber() const {
	return phoneNumber;
}
string Client::getUserName() const {
	return userName;
}
string Client::getPassword() const {
	return password;
}
string Client::getEmail() const {
	return email;
}
string Client::getBirthday() const {
	return birthday;
}
string Client::getAddress() const {
	return address;
}
string Client::getDiscountCode() const {
	return discountCode;
}

void Client::setPassword(const string& pass) {
	password = pass;
}
void Client::setDiscountCode(const string& code) {
	discountCode = code;
}
#pragma endregion

//	yonetici sınıfı
#pragma region Admin
class Admin : public Person
{
public:
	Admin(const string& _pw);
private:
	string password;	// şifre
};

Admin::Admin(const string& _pw)
	: Person("admin", "00000000000") {
	password = _pw;
}
#pragma endregion

//	kurye sınıfı
#pragma region Courier
class Courier : public Person
{
public:
	Courier(const string& str);
	Courier(int& id, string& name, string& telno);

	string getCourierString() const;

	int getID() const;
	string getName() const;
	string getPhoneNumber() const;
	Time getAvailableTime() const;
	string getShipmentsAndTimes();

	vector<int> makeOrderIDs(const string&);
	vector<Time> makeOrderTimes(const string&);

	
	void setAvailableTime();
	void addOrder(const int&, const Time&);
private:
	int courierID;				// kurye id
	Time availableTime;			// uygun zaman
	vector<int> orderIDs;		// sipariş numaraları
	vector<Time> orderTimes;	// sipariş bitişler
};

Courier::Courier(int& id, string& _name, string& _phone)
	: Person(_name, _phone) {
	courierID = id;
	orderIDs = {};
	orderTimes = {};
	availableTime = Time(0, 10, 0);
}
Courier::Courier(const string& str) {
// courierID, name, phoneNumber, availableTime, orderIDs, orderTimes
	size_t idx1, idx2, idx3, idx4, idx5;
	string _orderIDs, _orderTimes;

	idx1 = str.find('-');
	idx2 = str.find('-', idx1 + 1);
	idx3 = str.find('-', idx2 + 1);
	idx4 = str.find('-', idx3 + 1);
	idx5 = str.find('-', idx4 + 1);

	courierID = stoi(str.substr(0, idx1));
	fullName = str.substr(idx1 + 1, idx2 - idx1 - 1);
	phoneNumber = str.substr(idx2 + 1, idx3 - idx2 - 1);
	availableTime = Time(str.substr(idx3 + 1, idx4 - idx3 - 1));

	_orderIDs = str.substr(idx4 + 1, idx5 - idx4 - 1);
	_orderTimes = str.substr(idx5 + 1, str.length());
	orderIDs = makeOrderIDs(_orderIDs);
	orderTimes = makeOrderTimes(_orderTimes);
}

string Courier::getCourierString() const {
	string str;
	string str_ids = "", str_times = "";

	str = to_string(getID()) + '-' + getName() + '-' + getPhoneNumber() + '-' + getAvailableTime().getStrFullTime() + '-';
	

	if (orderIDs.size() == 0)
		str_ids = "<>";
	else {
		for (int i : orderIDs)
			str_ids = str_ids + '<' + to_string(i) + '>';
	}

	if (orderTimes.size() == 0)
		str_times = "<>";
	else {
		for (Time t : orderTimes)
			str_times = str_times + '<' + t.getStrFullTime() + '>';
	}

	str = str + str_ids + '-' + str_times;
	return str;
}
string Courier::getName() const {
	return fullName;
}
string Courier::getPhoneNumber() const {
	return phoneNumber;
}
int Courier::getID() const {
	return courierID;
}
Time Courier::getAvailableTime() const {
	return availableTime;
}
string Courier::getShipmentsAndTimes() {
	string shipments;
	int size = orderIDs.size();
	for (int i = 0; i < size; i++) {
		shipments = shipments + "<" + to_string(orderIDs.at(i)) + "," + orderTimes.at(i).getStrFullTime() + ">";
	}
	return shipments;
}

vector<int> Courier::makeOrderIDs(const string& str) {
	vector<int> temp_orderIDs;

	string current;
	size_t _idx = 0, __idx = 0;

	int count = 0;
	for (int i = 0; i < str.size(); i++)
		if (str[i] == '<') count++;

	for (int i = 0; i < count; i++) {
		_idx = str.find('<', _idx);
		__idx = str.find('>', __idx);

		if (_idx == __idx - 1)
			return temp_orderIDs;


		current = str.substr(_idx + 1, __idx - _idx - 1);
		temp_orderIDs.push_back(stoi(current));
		_idx++; __idx++;
	}
	return temp_orderIDs;
}
vector<Time> Courier::makeOrderTimes(const string& str) {
    // <00:11:40><01:01:20>
	vector<Time> temp_orderTimes;

	string current;
	size_t _idx = 0, __idx = 0;

	int count = 0;
	for (int i = 0; i < str.size(); i++)
		if (str[i] == '<') count++;

	for (int i = 0; i < count; i++) {
		_idx = str.find('<', _idx);
		__idx = str.find('>', __idx);

		if (_idx == __idx - 1)
			return temp_orderTimes;

		current = str.substr(_idx + 1, __idx - _idx - 1);
		temp_orderTimes.push_back(Time(current));
		_idx++; __idx++;
	}
	return temp_orderTimes;
}

void Courier::setAvailableTime() {
	Time _available = orderTimes.at(0);
	for (auto& each : orderTimes) {
		if (each > _available)
			_available = each;
	}
	availableTime = _available;
}
void Courier::addOrder(const int& id, const Time& _t) {
	orderIDs.push_back(id);
	orderTimes.push_back(_t);
}
#pragma endregion

//	fatura sınıfı
#pragma region Invoice
class Invoice
{
public:
	Invoice();
	Invoice(string& str);
	Invoice(int& _id, string& _fullname, string& _username, string& _phoneno, string& _address, string& _discode, Time& _orderTime, Time& _deliveryTime, vector<Order>& _basket);

	string getInvoiceString() const;

	int getID() const;
	string getFullName() const;
	string getUserName() const;
	string getPhoneNumber() const;
	string getAddress() const;
	string getDiscountCode() const;
	Time getOrderTime() const;
	Time getDeliveryTime() const;
	vector<Order> getOrderBasket() const;
	vector<Order> makeOrderVector(string& list);

	void setInvoice(int& _id, string& _fullname, string& _username, string& _phoneno, string& _address, string& _discode, Time& _orderTime, Time& _deliveryTime, vector<Order>& _basket);
	void setID(const int&);
	void setFullName(const string&);
	void setUserName(const string&);
	void setPhoneNumber(const string&);
	void setAddress(const string&);
	void setDiscountCode(const string&);
	void setOrderTime(const Time&);
	void setDeliveryTime(const Time&);
	void setOrderBasket(const vector<Order>&);
private:
	int id;
	string fullName;
	string userName;
	string phoneNumber;
	string address;
	string discountCode;
	Time orderTime;
	Time deliveryTime;
	vector<Order> orderBasket;
};

Invoice::Invoice() = default;
Invoice::Invoice(int& _id, string& _fullname, string& _username, string& _phoneno, string& _address, string& _discode, Time& _orderTime, Time& _deliveryTime, vector<Order>& _basket) {
	setInvoice(
		_id,
		_fullname,
		_username,
		_phoneno,
		_address,
		_discode,
		_orderTime,
		_deliveryTime,
		_basket
	);
}
Invoice::Invoice(string& str) {

	int _id;
	string _fullName, _userName, _phoneNumber, _address, _discountCode, _orderList;
	Time _orderTime, _deliveryTime;
	vector<Order> _orderBasket;
	size_t idx1, idx2, idx3, idx4, idx5, idx6, idx7, idx8;

	idx1 = str.find('-');
	idx2 = str.find('-', idx1 + 1);
	idx3 = str.find('-', idx2 + 1);
	idx4 = str.find('-', idx3 + 1);
	idx5 = str.find('-', idx4 + 1);
	idx6 = str.find('-', idx5 + 1);
	idx7 = str.find('-', idx6 + 1);
	idx8 = str.find('-', idx7 + 1);

	_id = stoi(str.substr(0, idx1));
	_fullName = str.substr(idx1 + 1, idx2 - idx1 - 1);
	_userName = str.substr(idx2 + 1, idx3 - idx2 - 1);
	_phoneNumber = str.substr(idx3 + 1, idx4 - idx3 - 1);
	_address = str.substr(idx4 + 1, idx5 - idx4 - 1);
	_discountCode = str.substr(idx5 + 1, idx6 - idx5 - 1);
	_orderTime = Time(str.substr(idx6 + 1, idx7 - idx6 - 1));
	_deliveryTime = Time(str.substr(idx7 + 1, idx8 - idx7 - 1));
	_orderList = str.substr(idx8 + 1, str.length());
	_orderBasket = makeOrderVector(_orderList);

	setInvoice(
		_id,
		_fullName,
		_userName,
		_phoneNumber,
		_address,
		_discountCode,
		_orderTime,
		_deliveryTime,
		_orderBasket
	);
}

string Invoice::getInvoiceString() const {
	string str, basket;
	str = to_string(id) + '-' + fullName + '-' + userName + '-' + phoneNumber + '-' + address + '-' + discountCode + '-' + orderTime.getStrFullTime() + '-' + deliveryTime.getStrFullTime() + '-';
	for (Order each : orderBasket) {
		basket = basket + '<' + each.getBasket() + '>';
	}
	str = str + basket;
	return str;
}
int Invoice::getID() const {
	return id;
}
string Invoice::getFullName() const {
	return fullName;
}
string Invoice::getUserName() const {
	return userName;
}
string Invoice::getPhoneNumber() const {
	return phoneNumber;
}
string Invoice::getAddress() const {
	return address;
}
string Invoice::getDiscountCode() const {
	return discountCode;
}
Time Invoice::getOrderTime() const {
	return orderTime;
}
Time Invoice::getDeliveryTime() const {
	return deliveryTime;
}
vector<Order> Invoice::getOrderBasket() const {
	return orderBasket;
}
vector<Order> Invoice::makeOrderVector(string& basket) {
	vector<Order> temp;

	string current;
	int count = 0;
	size_t _idx = 0, __idx = 0;

	size_t idx1, idx2, idx3, idx4;
	string _category, _name, _size, _color, _price;
	double price;

	for (int i = 0; i < basket.size(); i++)
		if (basket[i] == '<') count++;

	for (int i = 0; i < count; i++) {
		_idx = static_cast<short int>(basket.find('<', _idx));
		__idx = basket.find('>', __idx);

		current = basket.substr(_idx + 1, __idx - _idx - 1);

		idx1 = current.find('-');
		idx2 = current.find('-', idx1 + 1);
		idx3 = current.find('-', idx2 + 1);
		idx4 = current.find('-', idx3 + 1);

		_category = current.substr(0, idx1);
		_name = current.substr(idx1 + 1, idx2 - idx1 - 1);
		_size = current.substr(idx2 + 1, idx3 - idx2 - 1);
		_color = current.substr(idx3 + 1, idx4 - idx3 - 1);
		_price = current.substr(idx4 + 1, current.length());
		price = stod(_price);

		temp.push_back(Order(_category, _name, _size, _color, price));

		_idx++; __idx++;
	}
	return temp;
}

void Invoice::setInvoice(int& _id, string& _fullname, string& _username, string& _phoneno, string& _address, string& _discode, Time& _orderTime, Time& _deliveryTime, vector<Order>& _basket) {
	setID(_id);
	setFullName(_fullname);
	setUserName(_username);
	setPhoneNumber(_phoneno);
	setAddress(_address);
	setDiscountCode(_discode);
	setOrderTime(_orderTime);
	setDeliveryTime(_deliveryTime);
	setOrderBasket(_basket);
}
void Invoice::setID(const int& _id) {
	id = _id;
}
void Invoice::setFullName(const string& _fullName) {
	fullName = _fullName;
}
void Invoice::setUserName(const string& _userName) {
	userName = _userName;
}
void Invoice::setPhoneNumber(const string& _phoneNum) {
	phoneNumber = _phoneNum;
}
void Invoice::setAddress(const string& _address) {
	address = _address;
}
void Invoice::setDiscountCode(const string& _disCode) {
	discountCode = _disCode;
}
void Invoice::setOrderTime(const Time& _orderTime) {
	orderTime = _orderTime;
}
void Invoice::setDeliveryTime(const Time& _deliveryTime) {
	deliveryTime = _deliveryTime;
}
void Invoice::setOrderBasket(const vector<Order>& _basket) {
	orderBasket = _basket;
};
#pragma endregion

//	cout işlemlerinin çoğunu gerçekleştiren sınıf.
#pragma region Buffer
class Buffer {
public:
    static const string inputArea;
    static const string horizontalTopDivider;
    static const string horizontalBottomDivider;

    static const string invalidInputError;
    static const string confirmExitWarning;
    static const string pressAnyKeyWarning;
    // log in warnings - errors
    static const string noRegisteredUsersWarning;
    static const string logInFailedError;
    static const string logInSuccessful;
    // register warnings - errors
    static const string userNameAlreadyTaken;

	static const vector<string> welcomePage;

    // verilen string bloğunu yazdırır.
    static void print(const string& str);
    // verilen string argümanları sırayla yazdırır. endl işlemi uygular.
    static void printArgs(const vector<string>& args);
    //
    static void printInRect(const string& str, int maxLen);
    //
    static void printArgsInRect(const vector<string>& args, int magLex, bool topMargin);      // vec, maxLen, topMargin
    //
    static void printSubmenuInRect(const vector<string>& args, int maxLen, bool topMargin);   // vec, maxLen, topMargin
    //
    static void printUserInputScreen(const string& _type, int maxLen, bool topMargin, bool _newline);   // inputType, maxLen, topMargin, newline
    //
    static void printCustomGuideline(const string& _type);
    //
    static void printCustomInvoice(const Invoice& inv);
    //
    static void printCustomOrderTracking(const string&, const string&, const string&);
private:
    static const char topL = char(201);
    static const char botL = char(200);
    static const char topR = char(187);
    static const char botR = char(188);
    static const char row = char(205);
    static const char column = char(186);

    static const string margin;
    static const string padding;

    // root side guides
    static const vector<string> logInGuide;
    static const vector<string> signUpGuide;
    // client side guides
    static const vector<string> shoppingGuide1;
    static const vector<string> shoppingGuide2;
    static const vector<string> shoppingGuide3;
    //static const vector<string> orderTrackingGuide;
    static const vector<string> takeFeedbackGuide;
    static const vector<string> changePwGuide;
    // admin side guides
    static const vector<string> productEntryGuide;
    static const vector<string> readFeedbackGuide;
    static const vector<string> discountCodeGuide;
    static const vector<string> addCourierGuide;

    static int findMaxBodyLen(const vector<string>&);
};

string const Buffer::margin = string(20, ' ');
string const Buffer::padding = string(5, ' ');

string const Buffer::inputArea = '\n' + margin + ">>> ";
string const Buffer::horizontalTopDivider = '\n' + margin + topL + string(55, row) + topR;
string const Buffer::horizontalBottomDivider = '\n' + margin + botL + string(55, row) + botR;

string const Buffer::invalidInputError = R"(Gecersiz secim yapildi. Lutfen mevcut seceneklerden birini tuslayiniz!)";
string const Buffer::confirmExitWarning = R"(Cikmak istediginizden emin misiniz? y/n tuslayiniz.)";
string const Buffer::pressAnyKeyWarning = R"(Menuye donmek icin herhangi bir tusa basiniz.)";

// log in warnings - errors
string const Buffer::noRegisteredUsersWarning = R"(Sistemde kayitli kullanici bulunamadi! Lutfen once kayit olunuz.)";
string const Buffer::logInFailedError = R"(Giris yapma islemi basarisiz oldu. Lutfen kullanici adi ve sifrenizi kontrol ediniz.)";
string const Buffer::logInSuccessful = R"(Giris yapma islemi basarili!)";

const vector<string> Buffer::welcomePage = {
	"Uygulamaya Hos Geldiniz!",
	"",
	"Bu yazilim Object Oriented Programming dersi donem odevi icin hazirlanmistir.",
	"",
	"Gecis yapmak istediginiz menunun numarasini tuslayarak uygulamayi kullanabilirsiniz.",
	"",
	"Devam etmek icin herhangi bir tusa basiniz."
};
// root side guides
const vector<string> Buffer::logInGuide = {
	"Kullanici giris yapma menusune hos geldiniz.",
	"",
	"Kullanici adinizi ve sifrenizi eksiksiz ve dogru bir sekilde giriniz."
};
const vector<string> Buffer::signUpGuide = {
	"Kayit olma menusune hos geldiniz.",
	"Girdiginiz karakterlerin ascii'ye uygun oldugundan emin olunuz.",
	"",
	"Ad Soyad:",
	"   * Maksimum 20 karakter olmali."
	"",
	"Kullanici adi:",
	"   * 6-24 karakter uzunlugunda olmalidir.",
	"   * Bosluk karakteri iceremez.",
	"   * Rakam icerebilir, '_' disinda ozel karakterler iceremez.",
	"",
	"Sifre:",
	"   * 8-15 karakter uzunlugunda olmalidir.",
	"   * Bosluk karakteri iceremez.",
	"   * Guvenlik acisindan buyuk-kucuk harf, rakam ve ozel karakter icermelidir.",
	"   * Ozel karakterler : ! @ ^ #  + $ % & = * ? / , . : < > ( ) [ ] { }",
	"",
	"Dogum tarihi GG.AA.YYYY formatina uygun olmalidir.",
	"E-posta xxxxx@xxxxx.com formatina uygun olmalidir.",
	"Telefon numarasi 11 karakter olmalidir.    orn: 05505505050",
	"Mevcut Adresler: Ortahisar,Akcaabat,Vakfikebir,Besikduzu,Yomra,Arsin,Arakli."
};
// client side guides
const vector<string> Buffer::shoppingGuide1{
	"Urun Secimi Ekranina Hos Geldiniz!",
	"",
	"Lutfen satin almak istediginiz urunun kategori numarasini tuslayiniz.",
};
const vector<string> Buffer::shoppingGuide2{
	"Kategori Secimi Basarili!",
	"",
	"Lutfen satin almak istediginiz urunun numarasini tuslayiniz.",
};
const vector<string> Buffer::shoppingGuide3{
	"Urun Secimi Basarili!",
	"",
	"Lutfen satin almak istediginiz urunu ozellestiriniz.",
	"Gosterilen alanlarda uygun buldugunuz secenegin numarasini tuslayiniz.",
};
//const vector<string> Buffer::orderTrackingGuide;
const vector<string> Buffer::takeFeedbackGuide = {
	"Geri bildirimde bulunma menusune hos geldiniz.",
	"",
	"Urun ve sistem ile ilgili gorus ve onerilerinizi belirtebilirsiniz.",
	"",
	"Girdiginiz metindeki karakterlerin ascii'ye uygun oldugundan emin olunuz.",
	"Girdiginiz metin maksimum 50 karakter olmalidir ve '-' karakteri icermemelidir.",
	"",
	"Enter tusuna bastiginiz anda metniniz tarafimiza iletilecektir."
};
const vector<string> Buffer::changePwGuide = {
	"Sifre degistirme menusune hos geldiniz.",
	"Once eski sifrenizi ardindan degistirmek istediginiz yeni sifrenizi girin.",
	"",
	"Sifre:",
	"   * 8-15 karakter uzunlugunda olmalidir.",
	"   * Bosluk karakteri iceremez.",
	"   * Guvenlik acisindan buyuk-kucuk harf, rakam ve ozel karakter icermelidir.",
	"   * Ozel karakterler : ! @ ^ #  + $ % & = * ? / , . : < > ( ) [ ] { }"
};
// admin side guides
const vector<string> Buffer::productEntryGuide = {
	"Kiyafet/Urun Ekleme menusune hos geldiniz.",
	"Girdiginiz metindeki karakterlerin ascii'ye uygun oldugundan emin olunuz.",
	"",
	"Oncelikle eklemek istediginiz urunun kategori bilgisini giriniz.",
	"Mevcut kategoriler:  Elbise, Tisort, Pantolon, Gomlek, Etek ve Ayakkabi",
	"",
	"Ardindan sirasiyla urunun ismini ve fiyatini giriniz.",
	"Urun adi maksimum 25 karakter olmalidir.",
	"Urun fiyatini sadece rakam kullanarak belirtiniz.",
	"Urun fiyatinda kusurati xxx.xx formatinda kullaniniz.   ornek: 179.90"
};
const vector<string> Buffer::readFeedbackGuide = {
	"Geri bildirimleri okuma menusune hos geldiniz.",
	"",
	"Urun ve sistem ile ilgili gorus ve oneriler bu menu uzerinden okunabilir.",
	"",
	"Ilgili kullanicilarin adi ve ardindan yaptiklari geri bildirimler siralanmistir."
};
const vector<string> Buffer::discountCodeGuide = {
	"Kullaniciya indirim kodu tanimlama menusune hos geldiniz.",
	"",
	"Kod tanimlayacaginiz kullanicinin adini dogru bir sekilde girin.",
	"Ardindan tanimlayacaginiz indirim kodunu girin.",
	"Indirim Kodu:",
	"   * ascii karakter setine uymalidir.",
	"   * 6 karakter uzunlugunda olmalidir.",
	"   * Rakam, harf ve '-' disinde ozel karakter icerebilir.",
	"   * Bosluk karakteri iceremez."
};
const vector<string> Buffer::addCourierGuide = {
	"Kurye ekleme menusune hos geldiniz.",
	"",
	"Tanimlamak istediginiz kuryenin tam adini girin.",
	"Girdiginiz karakterlerin ascii'ye uygun oldugundan emin olun.",
	"Ardindan kuryenin telefon numarasini girin.",
	"Telefon numarasi 11 haneli olmalidir. ornek: 05051234567"
};

int Buffer::findMaxBodyLen(const vector<string>& vec) {
	size_t max = 0;
	for (string item : vec) {
		if (item.length() > max)
			max = item.length();
	}
	return static_cast<int>(max);
}

void Buffer::print(const string& str) {
    cout << str;
}

void Buffer::printArgs(const vector<string>& vect) {
    for (auto& each : vect)
        cout << each << endl;
}

void Buffer::printInRect(const string& str, int maxLen) {

	cout << endl;

	size_t strLen = str.length();
	size_t bodyLen;
	if (!maxLen)
		bodyLen = str.length();
	else
		bodyLen = maxLen;

	size_t bodyWPaddingLen = bodyLen + 10;
	size_t bodyWoPaddingLen = bodyLen;
	string bodyWithoutPadding(bodyWoPaddingLen, ' ');
	string bodyWithPadding(bodyWPaddingLen, ' ');

	cout << margin << topL << string(bodyWPaddingLen, row) << topR << endl;
	
	cout << margin << column << padding << str;
	cout << string(bodyWoPaddingLen - strLen, ' ') << padding << column << endl;

	cout << margin << botL << string(bodyWPaddingLen, row) << botR << endl;
}

void Buffer::printArgsInRect(const vector<string>& vec, int maxLen, bool topMargin) {
	cout << endl;

	int bodyLen;
	if (!maxLen)
		bodyLen = findMaxBodyLen(vec);
	else
		bodyLen = maxLen;

	int bodyWPaddingLen = bodyLen + 10;
	int bodyWoPaddingLen = bodyLen;
	string bodyWithoutPadding(bodyWoPaddingLen, ' ');
	string bodyWithPadding(bodyWPaddingLen, ' ');

	cout << margin << topL << string(bodyWPaddingLen, row) << topR << endl;
	if(topMargin)
		cout << margin << column << bodyWithPadding << column << endl;
	for (int i = 0; i < vec.size(); i++)
	{
		size_t curLen = vec.at(i).length();
		cout << margin << column << padding << vec.at(i) << padding;
		cout << string(bodyWoPaddingLen - curLen, ' ') << column << endl;
	}
	if (topMargin)
		cout << margin << column << bodyWithPadding << column << endl;
	cout << margin << botL << string(bodyWPaddingLen, row) << botR << endl;
}

void Buffer::printSubmenuInRect(const vector<string>& vec, int maxLen, bool topMargin) {
	if (topMargin)
		cout << endl << endl;

	int bodyLen;
	if (!maxLen)
		bodyLen = findMaxBodyLen(vec);
	else
		bodyLen = maxLen;

	int bodyWPaddingLen = bodyLen + 14;
	int bodyWoPaddingLen = bodyLen;
	string bodyWithoutPadding(bodyWoPaddingLen, ' ');
	string bodyWithPadding(bodyWPaddingLen, ' ');

	cout << margin << topL << string(bodyWPaddingLen, row) << topR << endl;
	cout << margin << column << bodyWithPadding << column << endl;
	for (int i = 0; i < vec.size(); i++)
	{
		size_t curLen = vec.at(i).length();
		cout << margin << column << padding << '[' << i + 1 << "] " << vec.at(i) << padding;
		cout << string(bodyWoPaddingLen - curLen, ' ') << column << endl;
	}
	cout << margin << column << bodyWithPadding << column << endl;
	cout << margin << botL << string(bodyWPaddingLen, row) << botR << endl;
}

void Buffer::printUserInputScreen(const string& _type, int maxLen, bool topMargin, bool _newline) {
	if (topMargin)
		cout << endl << endl;

	string str;
	if (_type.compare("fullName") == 0)
		str = "Ad Soyad      :  ";
	else if (_type.compare("userName") == 0)
		str = "Kullanici Adi :  ";
	else if (_type.compare("password") == 0)
		str = "Sifre         :  ";
	else if (_type.compare("birthdate") == 0)
		str = "Dogum Tarihi  :  ";
	else if (_type.compare("email") == 0)
		str = "E-posta       :  ";
	else if (_type.compare("phoneNumber") == 0)
		str = "Telefon No    :  ";
	else if (_type.compare("address") == 0)
		str = "Adres         :  ";
	else
		str = _type;

	int bodyWPaddingLen = maxLen + 25;
	int bodyWoPaddingLen = maxLen;
	string bodyWithoutPadding(bodyWoPaddingLen, ' ');
	string bodyWithPadding(bodyWPaddingLen, ' ');

	if (_newline)
		cout << endl;
	else
		cout << margin << topL << string(bodyWPaddingLen, row) << topR << endl;
	cout << margin << ' '  << padding << str << padding;
}

void Buffer::printCustomGuideline(const string& name) {
	vector<string> guideVec;
	// root side guides
	if (name.compare("signUp") == 0)
		guideVec = Buffer::signUpGuide;
	else if (name.compare("logIn") == 0)
		guideVec = Buffer::logInGuide;

	// client side guides
	else if (name.compare("takeFeedback") == 0)
		guideVec = Buffer::takeFeedbackGuide;
	else if (name.compare("changePw") == 0)
		guideVec = Buffer::changePwGuide;
	else if (name.compare("shopping1") == 0)
		guideVec = Buffer::shoppingGuide1;
	else if (name.compare("shopping2") == 0)
		guideVec = Buffer::shoppingGuide2;
	else if (name.compare("shopping3") == 0)
		guideVec = Buffer::shoppingGuide3;

	// admin side guides
	else if (name.compare("readFeedback") == 0)
		guideVec = Buffer::readFeedbackGuide;
	else if (name.compare("discountCode") == 0)
		guideVec = Buffer::discountCodeGuide;
	else if (name.compare("productEntry") == 0)
		guideVec = Buffer::productEntryGuide;
	else if (name.compare("addCourier") == 0)
		guideVec = Buffer::addCourierGuide;

	Buffer::printArgsInRect(guideVec, 80, true);
}

void Buffer::printCustomInvoice(const Invoice& inv) {
	const string divider = string(5, ' ') + string(70, 238) + string(5, ' ');

	// top divider
	cout << endl << margin << topL << string(80, row) << topR << endl;
	// empty row
	cout << margin << column << string(80, ' ') << column << endl;
	// 1. row 
	// marka ve fatura no
	const string brandName = "SHOOP CENTER";
	const size_t brandNameLength = brandName.length();

	string invoiceID = to_string(inv.getID());
	invoiceID.insert(0, 3 - invoiceID.length(), '0');
	const string invoiceNo = "Fatura No       :   " + invoiceID;
	
	cout << margin << column << string(5, ' ');
	cout << brandName;
	cout << string(40 - brandNameLength, ' ');
	cout << invoiceNo;
	cout << string(30 - invoiceNo.length(), ' ');
	cout << string(5, ' ') << column << endl;
	// 2. row
	// tarih
	const string orderTime = "Siparis Saati   : " + inv.getOrderTime().getStrTime();
	
	cout << margin << column << string(45, ' ');
	cout << orderTime;
	cout << string(30 - orderTime.length(), ' ');
	cout << string(5, ' ') << column << endl;
	// 3. row
	// saat

	const string deliveryTime = "Teslim Saati    : " + inv.getDeliveryTime().getStrTime();

	cout << margin << column << string(45, ' ');
	cout << deliveryTime;
	cout << string(30 - deliveryTime.length(), ' ');
	cout << string(5, ' ') << column << endl;
	// empty row
	//cout << column << string(80, ' ') << column << endl;
	// 4. row
	// alıcı bilgileri 1
	
	const string clientInfoHeader = "Alici Bilgileri";
	const string clientInfoName = inv.getFullName();
	const string clientInfoUserName = inv.getUserName();
	const string clientInfoPhone = inv.getPhoneNumber();
	const string clientInfoAddress = inv.getAddress();
	cout << margin << column << string(5, ' ');
	cout << clientInfoHeader;
	cout << string(75 - clientInfoHeader.length(), ' ') << column << endl;
	// alıcı bilgileri divider
	cout << margin << column << string(5, ' ');
	cout << string(clientInfoHeader.length(), 238);
	cout << string(75 - clientInfoHeader.length(), ' ') << column << endl;
	// alıcı bilgileri 2
	cout << margin << column << string(5, ' ');
	cout << clientInfoName;
	cout << string(75 - clientInfoName.length(), ' ') << column << endl;
	// alıcı bilgileri 3
	cout << margin << column << string(5, ' ');
	cout << clientInfoUserName;
	cout << string(75 - clientInfoUserName.length(), ' ') << column << endl;
	// alıcı bilgileri 4
	cout << margin << column << string(5, ' ');
	cout << clientInfoPhone;
	cout << string(75 - clientInfoPhone.length(), ' ') << column << endl;
	// alıcı bilgileri 5
	cout << margin << column << string(5, ' ');
	cout << clientInfoAddress;
	cout << string(75 - clientInfoAddress.length(), ' ') << column << endl;
	// empty row
	cout << margin << column << string(80, ' ') << column << endl;
	// 5. row
	// urun sepeti header
	const string productBasketHeader1 = "Urun Adi";
	const string productBasketHeader2 = "Beden";
	const string productBasketHeader3 = "Renk";
	const string productBasketHeader4 = "Fiyat";

	cout << margin << column << string(5, ' ');
	cout << productBasketHeader1 << string(40 - productBasketHeader1.length(), ' ');
	cout << productBasketHeader2 << string(10 - productBasketHeader2.length(), ' ');
	cout << productBasketHeader3 << string(10 - productBasketHeader3.length(), ' ');
	cout << productBasketHeader4 << string(10 - productBasketHeader4.length(), ' ');
	cout << string(5, ' ') << column << endl;
	// urun sepeti divider
	cout << margin << column << divider << column << endl;
	// urunler
	double total = 0;
	double tempPrice;
	string productName;
	string productSize;
	string productColor;
	string productPrice;
	for (auto& item : inv.getOrderBasket()) {
		tempPrice = item.getPrice();
		total += tempPrice;

		productName = item.getCategory() + " - " + item.getName();
		productSize = item.getSize();
		productColor = item.getColor();
		productPrice = to_string(tempPrice).substr(0, to_string(tempPrice).find('.') + 3);
		productPrice = productPrice + "$";

		cout << margin << column << string(5, ' ');
		cout << productName << string(40 - productName.length(), ' ');
		cout << productSize << string(10 - productSize.length(), ' ');
		cout << productColor << string(10 - productColor.length(), ' ');
		cout << productPrice << string(10 - productPrice.length(), ' ');
		cout << string(5, ' ') << column << endl;
	}
	// urun sepeti divider
	cout << margin << column << divider << column << endl;

	// ara toplam
	const string subTotal = "Ara Toplam   : ";
	const string subTotalPrice = to_string(total).substr(0, to_string(total).find('.') + 3);
	cout << margin << column << string(50, ' ');
	cout << subTotal;
	cout << subTotalPrice << "$";
	cout << string(15 - subTotalPrice.length() - 1, ' ') << column << endl;
	// discount code
	const bool codeAvailable = inv.getDiscountCode().compare("NaN") != 0;
	const string discountCode = "Indirim Kuponu : " + ( codeAvailable ? inv.getDiscountCode(): " -- ");
	const string codePercent = "%10";
	cout << margin << column << string(5, ' ');
	cout << discountCode;
	if (!codeAvailable) {
		cout << string(60 - discountCode.length(), ' ');
		cout << "%0";
		cout << string(16 - codePercent.length(), ' ');
	}
	else {
		cout << string(60 - discountCode.length(), ' ');
		cout << codePercent;
		cout << string(15 - codePercent.length(), ' ');
	}
	cout << column << endl;
	// divider
	cout << margin << column << divider << column << endl;
	// genel toplam
	if (codeAvailable)
		total *= 0.9;

	const string generalTotal = "Genel Toplam : ";
	const string generalTotalPrice = to_string(total).substr(0, to_string(total).find('.') + 3);
	cout << margin << column << string(50, ' ');
	cout << generalTotal;
	cout << generalTotalPrice << "$";
	cout << string(15 - generalTotalPrice.length() - 1, ' ') << column << endl;
	// divider
	cout << margin << column << string(80, ' ') << column << endl;
	cout << margin << botL << string(80, row) << botR << endl;
}

void Buffer::printCustomOrderTracking(const string& str, const string& fullName, const string& address) {

	// orderID, orderTime, deliveryTime, courierName, courierPhoneNumber, userName

	// parsing string
	size_t idx1, idx2, idx3, idx4, idx5;
	idx1 = str.find('-');
	idx2 = str.find('-', idx1 + 1);
	idx3 = str.find('-', idx2 + 1);
	idx4 = str.find('-', idx3 + 1);
	idx5 = str.find('-', idx4 + 1);

	string orderID, orderTime, deliveryTime, courierName, courierPhoneNumber, userName;
	orderID = str.substr(0, idx1);
	orderTime = str.substr(idx1 + 1, idx2 - idx1 - 1);
	deliveryTime = str.substr(idx2 + 1, idx3 - idx2 - 1);
	courierName = str.substr(idx3 + 1, idx4 - idx3 - 1);
	courierPhoneNumber = str.substr(idx4 + 1, idx5 - idx4 - 1);
	userName = str.substr(idx5 + 1, str.length());
	// fullName
	// address

	// geçici template
	Buffer::printArgsInRect({
		"Fatura Numarasi    : " + orderID,
		"-------------------------------------",
		"Cikis Birimi       : Trabzon Subesi",
		"Cikis Saati        : " + orderTime,
		"-------------------------------------",
		"Teslim Birimi      : " + address + " Subesi",
		"Teslim Saati       : " + deliveryTime,
		"-------------------------------------",
		"Alicinin;",
		"     Adi           : " + fullName,
		"     Kullanici Adi : " + userName,
		"-------------------------------------",
		"Kuryenin;",
		"     Adi           : " + courierName,
		"     Telefonu      : " + courierPhoneNumber,
		},0, true);
}
#pragma endregion

//	kullanıcının yaptığı girişlerin geçerliliğini kontrol eden sınıf.
#pragma region Validator
class Validator
{
public:
	bool validateAsciiInput(const string&);
	bool validateDigitInput(const string&);
	bool validateFullName(const string&);
	bool validateUserName(const string&);
	bool validatePassword(const string&);
	bool checkPasswordStrength(const string&);
	bool validateBirthDate(const string&);
	bool validateEmail(const string&);
	bool validatePhoneNumber(const string&);
	bool validateAddress(string);
	bool validateDiscountCode(const string&);
	bool validateComment(const string&);

private:
	inline bool containsHyphen(const string&);
	inline bool containsNonAscii(const string&);
	inline bool containsBlank(const string&);
	inline bool containsDigit(const string&);
};
/// <summary>
///		verilen string içerisinde ascii'ye uymayan karakter var mı kontrol eder.
///		ascii'ye uymayan karakter varsa false, yoksa true döner.
/// </summary>
bool Validator::validateAsciiInput(const string& str) {
	if (containsHyphen(str))
		return false;

	return !containsNonAscii(str);
}
/// <summary>
///		isim, soyisim
///		- maksimum 20 karakter uzunluğunda olmalıdır.
///		- ascii karakter setine uymalıdır.
///		- '-' karakteri içermemelidir.
/// </summary>
bool Validator::validateFullName(const string& str) {
	if (containsHyphen(str))
		return false;

	size_t length = str.length();

	if (length < 1 || length > 20)
		return false;

	if (!regex_search(str, regex("^[a-zA-Z ]*$")))	// regex for only a-z alphabetics and space
		return false;

	return true;
}
/// <summary>
///		kullanıcı adı;
///		- non-ascii karakter içeremez.
///		- en az 6 en fazla 24 karakter içerebilir.
///		- boşluk karakteri ve özel karakterler içeremez, _ ve rakam içerebilir.
/// </summary>
/// 
bool Validator::validateUserName(const string& str) {
	if(containsHyphen(str))
		return false;

	size_t length = str.length();
	
	// check length
	if ((length < 6) || (length > 24))
		return false;
	// a-z alpha, digit, _ will be OK. otherwise false
	if (!regex_search(str, regex("^[a-zA-Z0-9_]*$")))
		return false;
	
	return true;
}
/// <summary>
///		şifre;
///		- non-ascii karakter içeremez.
///		- en az 8 en fazla 15 karakter içerebilir.
///		- en az 1 rakam ve harf içermelidir.
///		- boşluk karakteri içeremez. seçili özel karakterleri içerebilir.	! @ ^ # + $ % & = * ? / , . : < > ( ) [ ] { }
/// </summary>
bool Validator::validatePassword(const string& str) {
	if(containsHyphen(str))
		return false;

	bool checkDigit = false;
	bool checkAlphaChar = false;
	size_t length = str.length();

	// check length
	if ((length < 8) || (length > 15))
		return false;

	// regex for;
	// digit:	[\\d]
	// alpha:	[a-zA-Z]
	// space:	[ ]+
	// special character:	[!@^#+$%&=*?_/,.:<>\\(\\)\\[\\]\\{\\}]
	// all combine and negate:		[^(!@^#+$%&=*?_/,.:<>\\(\\)\\[\\]\\{\\})(\\d)(a-zA-Z)]

	// check digit
	if (regex_search(str, regex("[\\d]")))
		checkDigit = true;

	// check alpha character
	if (regex_search(str, regex("[a-zA-Z]")))
		checkAlphaChar = true;

	// check non valid characters
	if (regex_search(str, regex("[^(!@^#+$%&=*?_/,.:<>\\(\\)\\[\\]\\{\\})(\\d)(a-zA-Z)]")))
		return false;

	if (checkDigit && checkAlphaChar)
		return true;
	else
		return false;
}
/// <summary>
///		- email xxxxx@xxxx.com formatina uygun olmalidir.
/// </summary>
bool Validator::validateEmail(const string& str) {
	if(containsHyphen(str))
		return false;

	if (!regex_match(str, regex("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+")))
		return false;

	return true;
}
/// <summary>
///		doğum tarihi;
///		- GG.AA.YYYY formatina uygun olmalidir.
/// </summary>
bool Validator::validateBirthDate(const string& str) {
	if(containsHyphen(str))
		return false;

	// check shape
	if (!regex_match(str, regex("\\b\\d{1,2}[.]\\d{1,2}[.]\\d{4}\\b")))
		return false;

	// check validity
	string substr_dd = str.substr(0, str.find("."));
	string substr_mm = str.substr(str.find(".")+1, str.find(".",2));
	string substr_yyyy = str.substr(str.rfind(".")+1, str.length());

	// stoi c++ icinde bir fonksiyondur ve string to int islemini yapar.
	short int day = stoi(substr_dd);
	short int month = stoi(substr_mm);
	short int year = stoi(substr_yyyy);

	if (month < 1 || month > 12)
		return false;

	else if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12)
	{
		if (day < 1 || day > 31)
			return false;
	}

	else if (month == 4 || month == 6 || month == 9 || month == 11)
	{
		if (day < 1 || day > 30)
			return false;
	}
	else if (month == 2)
	{
		if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)	// leap year
		{
			if (day < 1 || day > 29)
				return false;
		}
		else if (day < 1 || day > 28)
			return false;
	}
	return true;
}
/// <summary>
///		adres;
///		- mevcut adresler içerisinden biri olmalıdır.
/// </summary>
bool Validator::validateAddress(string str) {
	if(containsHyphen(str))
		return false;

	// adres bilgisinde butun karakterleri lowercase yap.
	for (int i = 0; i < str.length(); i++)
		str[i] = tolower(str[i]);

	if (str.compare("ortahisar"))
		return true;
	else if (str.compare("akcaabat"))
		return true;
	else if (str.compare("vakfikebir"))
		return true;
	else if (str.compare("besikduzu"))
		return true;
	else if (str.compare("yomra"))
		return true;
	else if (str.compare("arsin"))
		return true;
	else if (str.compare("arakli"))
		return true;

	return false;
}
/// <summary>
///		telefon numarası;
///		- 11 karakter olmalıdır.
///		- sadece rakam içermelidir.
/// </summary>
bool Validator::validatePhoneNumber(const string& str) {
	if(containsHyphen(str))
		return false;

	if (str.length() != 11)
		return false;

	// check is all digit
	if (!regex_search(str, regex("^[0-9]+$")))
		return false;

	return true;
}
/// <summary>
///		geri bildirim;
///		- maksimum 50 karakter olmalıdır.
///		- ascii karakter dizisine uymalıdır.
///		- '-' karakteri içermemelidir.
/// </summary>
bool Validator::validateComment(const string& str) {
	if(containsHyphen(str))
		return false;

	if (str.length() < 1 || str.length() > 50)
		return false;

	if (containsNonAscii(str))
		return false;

	if (containsHyphen(str))
		return false;

	return true;
}

bool Validator::validateDigitInput(const string& str) {		
	if(containsHyphen(str))
		return false;

	return regex_match(str, regex("^\\d+(\\.\\d{1,})?$"));
}
/// <summary>
/// 
/// </summary>
bool Validator::checkPasswordStrength(const string& str) {
	if(containsHyphen(str))
		return false;

	unsigned short int counter = 0;

	// check digit
	if (regex_search(str, regex("[\\d]")))
		counter++;

	// check lowercase letters
	if (regex_search(str, regex("[a-z]")))
		counter++;

	// check uppercase letters
	if (regex_search(str, regex("[A-Z]")))
		counter++;

	// check special characters
	if (regex_search(str, regex("[!@^#+$%&=*?_/,.:<>\\(\\)\\[\\]\\{\\}]")))
		counter++;

	switch (counter)
	{
	case 2:
		return false;
	case 3:
		return false;
	case 4:
		return true;
	default:
		return false;
	}
}
/// <summary>
///		indirim kodu;
///		- 6 karakter uzunluğunda olmalıdır.
///		- ascii karakter setine uymalıdır.
///		- boşluk ve '-' karakterini içermemelidir.
/// </summary>
bool Validator::validateDiscountCode(const string& str) {
	if(containsHyphen(str))
		return false;

	if (str.length() != 6)
		return false;

	if (containsNonAscii(str))
		return false;

	if (containsBlank(str))
		return false;

	return true;
}

bool Validator::containsHyphen(const string& str) {
	if (regex_search(str, regex("-")))
		return true;

	return false;
}
bool Validator::containsBlank(const string& str) {
	if (regex_search(str, regex(" ")))
		return true;

	return false;
}
bool Validator::containsNonAscii(const string& str) {
	for (int i = 0; i < str.length(); i++) {
		if (!isascii(str[i]))
			return true;
	}
	return false;
}
bool Validator::containsDigit(const string& str) {
	if (regex_search(str, regex("\\d")))
		return true;

	return false;
}
#pragma endregion

//	aksiyon sınıfları
#pragma region Actions


#pragma region BaseClass
class Action {
public:
	Time APPLICATION_TIME;
	Admin* adminPtr = NULL;
	Client* clientPtr = NULL;

	virtual void doAction() {}
	virtual bool checkAction() { return false; }
	void incrementTime();
	void setApplicationTime();
};
/// <summary>
///	Uygulama çalıştırıldığında uygulama zamanını ayarlamak için bu fonksiyon devreye girer.
///
/// Uygulamanın başlangıç zamanı, son aktif değişimin zamanı esas alınarak yapılır. Bu değişim doğal olarak müşterinin ürün siparişi
///	yaptığı zamandır. Yapılan son siparişin teslim zamanı esas alınır ve uygulama buradan devam eder.
///
///	Eğer uygulamada daha önce hiç sipariş olmamış ise default zaman ile uygulama başlatılır.
/// </summary>
void Action::setApplicationTime() {
	vector<string> temp;
	ifstream file(rootPath + courierFile);
	// sorun var ise böyle bir dosya yoktur app süresi default olarak başlasın.
	if (file.fail()) {
		APPLICATION_TIME = Time(0, 10, 0);
		return;
	}

	if (file.is_open())
	{
		string currentLine;
		while (!file.eof())
		{
			getline(file, currentLine);
			if (currentLine != "")
				temp.push_back(currentLine);
		}
		file.close();
	}

	// boyutu 0 ise hiç kurye yoktur app süresi default olarak başlasın.
	if (temp.size() == 0) {
		APPLICATION_TIME = Time(0, 10, 0);
		return;
	}

	vector<Courier> couriers;
	for (string& line : temp) {
		couriers.push_back(Courier(line));
	}
	
	Courier lastOrderCourier = couriers.at(0);
	for (int i = 0; i < couriers.size(); i++) {
		if (couriers.at(i).getAvailableTime() > lastOrderCourier.getAvailableTime())
			lastOrderCourier = couriers.at(i);
	}

	APPLICATION_TIME = Time(lastOrderCourier.getAvailableTime().getStrFullTime());
}
/// <summary>
/// Her menü geçişi olduğunda bu fonksiyon çalışır ve uygulama zamanını 5 dakika arttırır.
/// </summary>
void Action::incrementTime() {
	APPLICATION_TIME = APPLICATION_TIME + Time(0, 0, 1);
}
#pragma endregion

#pragma region CommonActions
class CommonActions {
public:
	vector<string> readFile(const string&);	// fileName
	string inputSequence(const bool);		// hiddenView
	string getUserInput(const string&, bool, bool, bool);		// input type, margin, _newline, hiddenview

	void createFile(const string&);					// fileName
	void writeFile(const string&, const string&);	// fileName, string data
	void writeFile(const Client&);
	void writeFile(const vector<Client>&);
	void writeFile(const Courier&);
	void writeFile(const vector<Courier>&);
	void writeFile(const Invoice&);

	Validator* validator;
};
/// <summary>
/// Kullanıcı giriş sekansı.
/// </summary>
string CommonActions::inputSequence(bool hiddenView) {
	char ch;
	string str;
	for (int i = 0;;)
	{
		ch = _getch();
		// backspace pressed
		if (ch == '\b' && i > 0)
		{
			cout << "\b \b";
			str.pop_back();
			i--;
		}
		// enter pressed
		else if (ch == '\r')
			break;
		else if (ch != '\b')
		{
			str.push_back(ch);
			i++;
			cout << (hiddenView ? '*' : ch);
		}
	}
	return str;
}
/// <summary>
/// Kullanıcı girişini dikdörtgen alan içerisinde alır.
/// </summary>
string CommonActions::getUserInput(const string& _type, bool _margin, bool _newline, bool _hiddenview) {
	Buffer::printUserInputScreen(_type, 30, _margin, _newline);	// inputType, maxLen, topMargin, _newline
	string str = inputSequence(_hiddenview);
	return str;
}
/// <summary>
/// Seçili dosya içerisindeki her bir satırı alır ve bir vector içerisine ekleyerek geri döndürür.
/// </summary>
vector<string> CommonActions::readFile(const string& fname) {
	vector<string> temp;

	ifstream file;
	file.open(rootPath + fname);

	// sorun var ise boş vector döndür.
	if (file.fail())
		return temp;

	if (file.is_open())
	{
		string currentLine;
		while (!file.eof())
		{
			getline(file, currentLine);
			if (currentLine != "")
				temp.push_back(currentLine);
		}
		file.close();
	}
	return temp;
}
/// <summary>
/// Seçili dosya isminde bir dosya oluşturur.
/// </summary>
void CommonActions::createFile(const string& fname) {
	ofstream createFile(rootPath + fname);
	createFile.close();
}
/// <summary>
/// overload edilmiş writeFile fonksiyonları.
/// Bu fonksiyonların temel amacı seçili dosyanın içeriğini güncellemektir.
/// </summary>
void CommonActions::writeFile(const string& fname, const string& data) {
	ofstream file;
	file.open(rootPath + fname, std::ios_base::app);
	file << data;
	file << '\n';
	file.close();
}
void CommonActions::writeFile(const Client& client) {
	ofstream file;
	file.open(rootPath + clientFile, std::ios_base::app);
	file << client.getClientString();
	file << '\n';
	file.close();
}
void CommonActions::writeFile(const vector<Client>& clientDetails) {
	fstream file;
	file.open(rootPath + clientFile, ofstream::out | ofstream::trunc);
	file.clear();
	file.close();

	file.open(rootPath + clientFile, std::ios_base::app);
	for (Client currentUser : clientDetails) {
		file << currentUser.getClientString();
		file << '\n';
	}
	file.close();
}
void CommonActions::writeFile(const Courier& courier) {
	ofstream file;
	file.open(rootPath + courierFile, std::ios_base::app);
	file << courier.getCourierString();
	file << '\n';
	file.close();
}
void CommonActions::writeFile(const vector<Courier>& couriers) {
	fstream file;
	file.open(rootPath + courierFile, ofstream::out | ofstream::trunc);
	file.clear();
	file.close();

	file.open(rootPath + courierFile, std::ios_base::app);
	for (Courier currentCourier : couriers) {
		file << currentCourier.getCourierString();
		file << '\n';
	}
	file.close();
}
void CommonActions::writeFile(const Invoice& inv) {
	ofstream file;
	file.open(rootPath + invoiceFile, std::ios_base::app);
	file << inv.getInvoiceString();
	file << '\n';
	file.close();
}
#pragma endregion

#pragma region exitAction
class ExitAction : public Action {
public:
	virtual void doAction();
private:
	bool confirmExit();
};
/// <summary>
/// 1)	confirmExit() fonksiyonu çağrılır ve kullanıcının yaptığı giriş sonucunda true veya false elde edilir.
/// 
/// 2)	true bilgisi alınırsa if bloğu çalışır ve uygulamadan çıkış işlemi gerçekleşir.
/// 
/// 3)	false bilgisi alındığı takdirde kullanıcı bilgilendirilir ve üst menüye geçilir.
/// </summary>
void ExitAction::doAction() {
	if (confirmExit())
		exit(1);
	else
		// TODO: FIXIT:
		Buffer::printInRect("Bir ust menuye donuluyor.", 0);
}
/// <summary>
/// 1)	Kullanıcıya çıkış işlemini teyit etmesi için bir mesaj gösterilir.
/// 
/// 2)	'y' veya 'Y' girilmesi durumunda fonksiyon true döndürür.
/// 
/// 3)	'n' veya 'N' girilmesi durumunda fonksiyon false döndürür.
/// 
/// 4)	Bu iki durum dışında bir giriş yapılması halinde kullanıcı uyarılır, 1. aşamaya geri dönülür.
/// </summary>
/// <returns></returns>
bool ExitAction::confirmExit() {
	// 'ExitAction::confirmExit': not all control paths return a value

	Buffer::printInRect(Buffer::confirmExitWarning, 0);

	Buffer::print(Buffer::inputArea);
	string c;
	cin >> c;
	system("cls");

	if (c == "y" || c == "Y")
		return true;
	else if (c == "n" || c == "N")
		return false;
	else
	{
		Buffer::printInRect(Buffer::invalidInputError, 0);
		return confirmExit();
	}
}
#pragma endregion

#pragma region signUpAction
class SignUpAction : public Action, public CommonActions {
public:
	virtual void doAction();
	virtual bool checkAction();
private:
	bool checkIfClientExists(const string&, vector<Client>&);
};
/// <summary>
/// 1)	Kullanıcının menüyü etkili kullanması için bir kılavuz mesajı yayınlanır.
/// 
/// 2)	Gerekli işlemler ve kontroller checkAction() fonksiyonu ile yapılır.
/// 
/// 3)	Herhangi bir sorun ortaya çıktığında checkAction() içerisinden kullanıcı bilgilendirilir.
///		İşlemler durdurulur ve menüden çıkılır.
/// 
/// 4)	İşlemler sorunsuz tamamlanırsa kullanıcı bilgilendirilir, menüden çıkılır.
/// </summary>
void SignUpAction::doAction() {

	Buffer::printCustomGuideline("signUp");

	if (checkAction()) {
		system("cls");
		Buffer::printInRect("kullanici basariyla kaydedildi!", 0);
	}
	else
		return;
}
/// <summary>
/// 1)	readFile() fonksiyonu ile client.txt dosyasındaki bilgiler okunur.
/// 
/// 2)	vector<string> nesnesi halinde tutulan bilgiler vector<Client> nesnesine dönüştürülür.
/// 
/// 3)	Kullanıcıdan tam isim, kullanıcı adı, şifre, doğum tarihi, e-posta, telefon numarası ve adres bilgisi istenir.
///		Her bir bilgi için gerekli kontroller yapılır. Eğer herhangi bir sorun çıkarsa kullanıcı bilgilendirilir,
///		işlemler durdurulur ve fonksiyon false değerini döndürür.
/// 
/// 4)	Koşulların hepsi sağlanıyorsa bu bilgiler ile bir Client nesnesi oluşturulur.
///		overload edilen writeFile(const Client&) fonksiyonu kullanılarak client.txt dosyası güncellenir.
/// 
/// 5)	fonksiyon true değerini döndürür.
/// </summary>
/// <returns> Tüm aşamalar sağlanırsa true, aksi halde false. </returns>
bool SignUpAction::checkAction(){
	vector<string> vectorDetails = readFile(clientFile);
	
	// vectoru Client nesnesine dönüştürür.
	vector<Client> clientDetails;
	for (string& user : vectorDetails)
		clientDetails.push_back(Client(user));
	
	// fullName bilgisini iste.
	string fullName = getUserInput("fullName", false, false, false);
	// fullName geçerli mi
	if (!validator->validateFullName(fullName)) {
		system("cls");
		Buffer::printInRect("Isim bilgisi gecerli degil! Lutfen gecerli bir isim giriniz.", 70);
		return false;
	}

	// userName bilgisini iste.
	string userName = getUserInput("userName", false ,true, false);
	// userName geçerli mi
	if (!validator->validateUserName(userName)) {
		system("cls");
		Buffer::printInRect("Kullanici adi gecerli degil! Lutfen gecerli bir kullanici adi giriniz.", 70);
		return false;
	}

	// dosyada kullanıcı sayısı 0 değil ise userName dosyada mevcut mu kontrol et:
	if (vectorDetails.size() != 0)
	{
		// girilen userName dosyada halihazırda var mı kontrol et.
		if (checkIfClientExists(userName, clientDetails))
		{
			system("cls");
			Buffer::printInRect("Bu kullanici adi alinmis! Lutfen baska bir kullanici adi seciniz.", 70);
			return false;
		}
	}

	// password bilgisini iste.
	string password = getUserInput("password", false, true, true);
	// password geçerli mi
	if (!validator->validatePassword(password)) {
		system("cls");
		Buffer::printInRect("Sifre gecerli degil! Lutfen gecerli bir sifre giriniz.", 70);
		return false;
	}
	// password yeterince güçlü mü
	if (!validator->checkPasswordStrength(password)) {
		system("cls");
		Buffer::printInRect("Sifre yeterince guclu degil! Lutfen gecerli bir sifre giriniz.", 70);
		return false;
	}

	// birthday bilgisini iste.
	string birthday = getUserInput("birthdate", false, true, false);
	// birthday geçerli mi
	if (!validator->validateBirthDate(birthday)) {
		system("cls");
		Buffer::printInRect("Dogum tarihi gecerli degil! Lutfen gecerli bir dogum tarihi giriniz.", 70);
		return false;
	}

	// email bilgisini iste.
	string email = getUserInput("email", false, true, false);
	// email geçerli mi
	if (!validator->validateEmail(email)) {
		system("cls");
		Buffer::printInRect("E-posta gecerli degil! Lutfen gecerli bir e-posta giriniz.", 70);
		return false;
	}

	// phoneNo bilgisini iste.
	string phoneNo = getUserInput("phoneNumber", false, true, false);
	// phoneNo geçerli mi
	if (!validator->validatePhoneNumber(phoneNo)) {
		system("cls");
		Buffer::printInRect("Telefon numarasi gecerli degil! Lutfen gecerli bir telefon giriniz.", 70);
		return false;
	}

	// address bilgisini iste.
	string address = getUserInput("address", false, true, false);
	// adres geçerli mi
	if (!validator->validateAddress(address)) {
		system("cls");
		Buffer::printInRect("Adres bilgisi gecerli degil! Lutfen gecerli bir adres giriniz.", 70);
		return false;
	}

	// bottom divider
	Buffer::print(Buffer::horizontalBottomDivider);

	Client user = Client(fullName, userName, password, birthday, email, phoneNo, address);
	// kullanıcıyı dosyaya kaydet
	writeFile(user);
	return true;
}
/// <summary>
/// 1)	userName nesnesi mevcut giriş yapmaya çalışan kullanıcının kullanıcı adını temsil eder.
/// 
/// 2)	clientDetails nesnesi client.txt dosyasındaki bütün kullanıcı bilgilerini Client nesnesi olarak
///		depolayan bir vector'dur.
/// 
/// 3)	Her bir Client nesnesi içerisinde kullanıcı adı karşılaştırılması yapılır.
///		Herhangi bir eşleşme sağlanırsa bu kullanıcı sistemde kayıtlı demektir. true döndürülür.
/// 
/// 4)	Eşleşme sağlanamazsa false döndürülür.
/// </summary>
/// <param name="userName"> Kullanıcı Adı </param>
/// <param name="clientDetails"> Bütün Kullanıcıların Bilgileri </param>
/// <returns> Eşleşme olursa true, olmazsa false. </returns>
bool SignUpAction::checkIfClientExists(const string& userName, vector<Client>& clientDetails) {
	for (Client& user : clientDetails) {
		if (userName.compare(user.getUserName()) == 0)
			return true;
	}
	return false;
}
#pragma endregion

#pragma region logInAction
class LogInAction : public Action, public CommonActions {
public:
	virtual bool checkAction();
	LogInAction(const string& _type) { userType = _type; }
private:
	string userType;
	void fillClientPtr(const string&, vector<Client>&);
	bool checkIfClientMatches(const string&, const string&, vector<Client>&);
	bool checkIfAdminMatches(const string&, const vector<string>);
};
/// <summary>
/// 1)	Giriş yapan kullanıcı yönetici mi yoksa müşteri mi belirlenir.
/// 
/// 2)	Giriş yapan yönetici ise;
///			- readFile() fonksiyonu ile admin.txt dosyasından yönetici bilgileri okunur.
///			- şifre bilgisinin girilmesi istenir.
///			- girilen bilgi eşleşmiyorsa bilgilendirme mesajı yayınlanır, fonksiyon false döndürür.
///			- eşleşme sağlanıyorsa adminPtr pointer'ının içeriği güncel bilgiler ile doldurulur.
///			- girişin başarılı olduğuna dair mesaj yayınlanır ve fonksiyon true döndürür.
/// 
/// 3)	Giriş yapan kişi müşteri ise;
///			- readFile() fonksiyonu ile client.txt dosyasından müşteri bilgileri okunur.
///			- dosya boş ise uyarı mesajı yayınlanır ve fonksiyon false döndürür.
///			- dosya dolu ise her bir kullanıcı bilgisi Client nesnesine dönüştürülür.
///			- kullanıcıdan kullanıcı adı ve şifre bilgilerini girmesi istenir.
///			- checkIfClientMatches() fonksiyonu ile böyle bir kullanıcı sistemde var mı veya eşleşme sağlanıyor mu kontrol edilir.
///			- eşleşme sağlanamazsa kullanıcı bilgilendirilir ve fonksiyon false döndürür.
///			- eşleşme sağlanırsa kullanıcı bilgilendirilir ve fonksiyon true döndürür.
/// </summary>
/// <returns> Giriş yapma işlemi başarılı olursa true, aksi halde false. </returns>
bool LogInAction::checkAction() {

	if (userType.compare("admin") == 0)	{
		// admin

		vector<string> details = readFile(adminFile);

		// böyle bir dosya yok demektir. oluşturup işeriğini dolduracağız.
		if (details.size() == 0) {
			createFile("admin");
			writeFile("admin", "123456");
			details = readFile(adminFile);
		}

		string password = getUserInput("password", true, false, true);
		if (!checkIfAdminMatches(password, details)) {
			// kullanıcıyı bilgilendir. giriş başarısız.
			system("cls");
			Buffer::printInRect(Buffer::logInFailedError, 0);
			return false;
		}
		else {
			adminPtr = new Admin(password);
			// kullanıcıyı bilgilendir. giriş başarılı.
			// Buffer::print(Buffer::horizontalBottomDivider);
			system("cls");
			Buffer::printInRect(Buffer::logInSuccessful, 0);
			return true;
		}
	}
	else {	
		// kullanıcı

		// dosyadaki bilgileri oku.
		vector<string> details = readFile(clientFile);

		// hiç kullanıcı var mı kontrol et.
		if (details.empty()) {
			Buffer::printInRect(Buffer::noRegisteredUsersWarning, 0);
			return false;
		}

		Buffer::printCustomGuideline("logIn");

		// bilgileri Client nesnesine dönüştür.
		vector<Client> clientDetails;
		for (string& user : details)
			clientDetails.push_back(Client(user));
		//

		// kullanıcıdan bilgileri al.
		string userName = getUserInput("userName", true, false, false);
		string password = getUserInput("password", false, true, true);	//_margin, _newline, hiddenview

		// bilgilerin dosyada karşılığı olan bir kullanıcı varmı kontrol et.
		if (!checkIfClientMatches(userName, password, clientDetails)) {
			// kullanıcıyı bilgilendir. giriş başarısız.
			system("cls");
			Buffer::printInRect(Buffer::logInFailedError, 0);
			return false;
		}
		else {
			//clientPtr = new Client(userName, password);
			fillClientPtr(userName, clientDetails);
			// kullanıcıyı bilgilendir. giriş başarılı.
			system("cls");
			Buffer::printInRect(Buffer::logInSuccessful, 0);
			return true;
		}
	}
}
/// <summary>
/// 1)	clientPtr giriş yapıldıktan sonra menüler arasında geçiş yapıldığında mevcut kullanıcının bilgilerini
///		tutan bir pointer'dir.
/// 
/// 2)	Giriş işlemi başarılı olduğunda pointer'in içeriği giriş yapan kullanıcının bilgileri ile doldurulur.
/// </summary>
/// <param name="userName"> Kullanıcı Adı </param>
/// <param name="clientDetails"> Bütün Kullanıcıların Bilgileri </param>
void LogInAction::fillClientPtr(const string& userName, vector<Client>& clientDetails) {
	for (Client& user : clientDetails) {
		if (userName.compare(user.getUserName()) == 0) {
			clientPtr = new Client(user);
			return;
		}
	}
}
/// <summary>
/// 1)	userName ve password değişkenleri kullanıcıdan alınan bilgilerdir. clientDetails client.txt dosyasından
///		alınan bütün kullanıcıların bilgilerini tutan bir nesnesir.
/// 
/// 2)	vector içerisindeki her bir Client için userName ve password bilgisi karşılaştırılır.
/// 
/// 3)	Herhangi bir eşleşme sağlanırsa true, eşleşme olmazsa false döndürülür.
/// </summary>
/// <param name="userName"> Kullanıcı Adı </param>
/// <param name="password"> Şifre </param>
/// <param name="clientDetails"> Bütün Kullanıcılarin Bilgileri </param>
/// <returns> Kullanıcı bilgileri eşleşirse true, aksi halde false. </returns>
bool LogInAction::checkIfClientMatches(const string& userName, const string& password, vector<Client>& clientDetails) {
	for (Client& user : clientDetails) {
		if (userName.compare(user.getUserName()) == 0 && password.compare(user.getPassword()) == 0)
			return true;
	}
	return false;
}
/// <summary>
/// 1)	password değişkeni adminDetails nesnesi içerisinde herhangi bir eşleşme sağlıyor mu kontrol edilir.
/// 
/// 2)	Eşleşme olursa true, hiçbir eşleşme sağlanamazsa false döndürülür.
/// </summary>
/// <param name="password"> Şifre </param>
/// <param name="adminDetails"> Bütün Admin Bilgileri </param>
/// <returns> Yönetici bilgileri eşleşirse true, aksi halde false. </returns>
bool LogInAction::checkIfAdminMatches(const string& password, const vector<string> adminDetails) {
	for (int i = 0; i < adminDetails.size(); i++)
		if (password.compare(adminDetails.at(i)) == 0)
			return true;
	return false;
}
#pragma endregion

#pragma region productEntryAction
class ProductEntryAction : public Action, public CommonActions {
public:
	virtual void doAction();
private:
	bool checkCategory(string& category);
};
/// <summary>
/// 1)	Kullanıcının menüyü etkili kullanması için bir kılavuz mesajı yayınlanır.
/// 
/// 2)	Girilmek istenen ürünün kategori bilgisi istenir.
/// 
/// 3)	checkCategory() fonksiyonu ile kategori bilgisi geçerli mi kontrol edilir.
///		Geçerli değil ise kullanıcı bilgilendirilir, işlemler durdurulur ve menüden çıkılır.
/// 
/// 4)	Kategori bilgisi geçerli ise ürünün adı istenir.
///		Ürün adı ascii karakter setine uymuyor ise kullanıcı bilgilendirilir, işlemler durdurulur ve menüden çıkılır.
/// 
/// 5)	Ürün adı bilgisi geçerli ise ürün fiyatı bilgisi istenir.
///		Ürün fiyatı doğru formatta yazılmış mı kontrol edilir. Doğru değilse kullanıcı bilgilendirilir, işlemler durdurulur ve menüden çıkılır.
/// 
/// 6)	Tüm koşullar sağlandığı takdirde overload edilen writeFile(const Clothe&) fonksiyonu ile product.txt dosyası güncellenir.
/// 
/// 7)	İşlemin tamamlandığına dair kullanıcı bilgilendirilir ve menüden çıkılır.
/// </summary>
void ProductEntryAction::doAction() {

	Buffer::printCustomGuideline("productEntry");

	// ürün kategorisini al.
	string category = getUserInput("Kategori     : ", true, false, false);
	// geçerli mi kontrol et.
	if (!checkCategory(category)) {
		// kullaniciya hata fırlat. geçersiz kategori girildi.
		system("cls");
		Buffer::printInRect("Kategori bilgisi gecerli degil! Lutfen gecerli bir kategori girin.", 70);
		return;
	}

	// ürün ismini al.
	string productName = getUserInput("Urun Adi     : ", false, true, false);
	// geçerli mi kontrol et.
	if (!validator->validateAsciiInput(productName) || productName.length() > 25)
	{
		system("cls");
		Buffer::printInRect("Urun adi gecerli degil! Lutfen gecerli bir urun adi girin.", 70);
		return;
	}

	// ürün fiyatını al.
	string price = getUserInput("Urun Fiyati  : ", false, true, false);
	// geçerli mi kontrol et.
	if (!validator->validateDigitInput(price)) {
		// kullaniciya hata firlat
		system("cls");
		Buffer::printInRect("Fiyat bilgisi gecerli degil! Lutfen gecerli bir fiyat girin.", 70);
		return;
	}

	Buffer::print(Buffer::horizontalBottomDivider);

	string newClothe = category + '-' + productName + '-' + price;
	writeFile(productFile, newClothe);

	// kullaniciyi bilgilendir islem basarili.
	system("cls");
	Buffer::printInRect("Islem basariyla tamamlandi!", 50);
}
/// <summary>
/// 1)	Girilmek istenen kategori bilgisi category değişkeninde tutulur.
/// 
/// 2)	category nesnesinin her bir karakteri küçük karaktere çevrilir.
/// 
/// 3)	Mevcut kategoriler olan Elbise, Tişört, Pantolon, Gömlek, Etek ve Ayakkabı ile karşılaştırma yapılır.
///		
/// 4) Herhangi bir kategori ile eşleşme olursa true, aksi halde false döndürülür.
/// </summary>
/// <param name="category"> Kategori Bilgisi </param>
/// <returns> Girilen kategori mevcutsa true, aksi halde false. </returns>
bool ProductEntryAction::checkCategory(string& category) {
	// kategori bilgisinde butun karakterleri lowercase yap.
	for_each(category.begin(), category.end(), [](char& c) { c = tolower(c); });
	// Elbise, Tişört, Pantolon, Gömlek, Etek ve Ayakkabı
	if (category.compare("elbise") == 0)
		return true;
	if (category.compare("tisort") == 0)
		return true;
	if (category.compare("pantolon") == 0)
		return true;
	if (category.compare("gomlek") == 0)
		return true;
	if (category.compare("etek") == 0)
		return true;
	if (category.compare("ayakkabi") == 0)
		return true;
	return false;
}
#pragma endregion

#pragma region addCourierAction
class AddCourierAction : public Action, public CommonActions {
public:
	virtual void doAction();
};
/// <summary>
/// 1)	Kullanıcının menüyü etkili kullanması için bir kılavuz mesajı yayınlanır.
/// 
/// 2)	Eklenecek olan kuryenin ismi istenir.
///		Girilen isim ascii karakter setine uymuyorsa kullanıcı bilgilendirilir, işlemler durdurulur ve menüden çıkılır.
/// 
/// 3)	Üstteki koşul sağlandığı takdirde eklenecek kuryenin telefon numarası istenir.
///		Girilen numara geçerli mi kontrol edilir.
///		Geçerli değil ise kullanıcı bilgilendirilir, işlemler durdurulur ve menüden çıkılır.
/// 
/// 4)	Koşullar sağlandığı takdirde;
///			- readFile() fonksiyonu ile courier.txt dosyası okunur ve toplam kurye sayısına göre bir kurye numarası belirlenir.
///			- elde edilen bilgiler ile yeni bir Courier nesnesi oluşturulur.
///	
/// 5)	overload edilen writeFile(const Courier&) fonksiyonu ile courier.txt dosyası güncellenir.
/// 
/// 6)	İşlemin tamamlandığına dair kullanıcı bilgilendirilir ve menüden çıkılır.
/// </summary>
void AddCourierAction::doAction() {

	Buffer::printCustomGuideline("addCourier");

	string courierName = getUserInput("Kurye Adi        : ", true, false, false);
	// validate courier name
	if (!validator->validateFullName(courierName))
	{
		// kullaniciyi uyar.
		system("cls");
		Buffer::printInRect("Kurye ismi gecerli degil! Lutfen gecerli bir kurye ismi girin.", 70);
		return;
	}

	string phoneNumber = getUserInput("Telefon Numarasi : ", false, true, false);
	// validate phone number
	if (!validator->validatePhoneNumber(phoneNumber))
	{
		// kullaniciyi uyar.
		system("cls");
		Buffer::printInRect("Telefon numarasini gecerli degil! Lutfen gecerli bir telefon girin.", 70);
		return;
	}

	Buffer::print(Buffer::horizontalBottomDivider);

	int courierID = readFile(courierFile).size();
	Courier newCour = Courier(courierID, courierName, phoneNumber);
	writeFile(newCour);

	// islem basarili
	Buffer::printInRect("Kurye ekleme islemi basariyle tamamlandi!", 50);
}
#pragma endregion

#pragma region viewInvoceAction
class ViewInvoiceAction : public Action, public CommonActions {
public:
	virtual void doAction();
};
/// <summary>
/// 1)	readFile() fonksiyonu ile invoice.txt dosyasındaki bilgiler alınır.
/// 
/// 2)	Dosya boş ise kullanıcı bilgilendirilir, işlemler durdurulur ve menüden çıkılır.
/// 
/// 3)	Dosya dolu ise Buffer::printCustomInvoice() fonksiyonu kullanılarak her bir fatura sırayla kullanıcıya gösterilir.
/// 
/// 4)	Çıkış için 'q' veya 'Q' tuşlanması beklenir. Gerekli tuşlamalar yapıldığında menüden çıkılır.
/// </summary>
void ViewInvoiceAction::doAction() {
	char c;
	vector<string> invos = readFile(invoiceFile);

	size_t size = invos.size();

	if (size == 0) {
		Buffer::printInRect("Sistemde mevcut fatura bulunmamaktadir!", 0);
		return;
	}

	Invoice item;
	int counter = 0;
	while (true)
	{
		system("cls");
		Buffer::printArgsInRect({ "Sistemde mevcut " + to_string(size) + " adet fatura bulunmaktadir.","Cikmak icin Q tuslayin.","Sonraki faturayi goruntulemek icin herhangi bir tus tuslayin."}, 0, false);

		item = Invoice(invos.at(counter));
		Buffer::printCustomInvoice(item);

		counter = (counter + 1) % size;

		c = _getch();
		if(c == 'Q' || c == 'q')
			break;
	}
	system("cls");
}
#pragma endregion

#pragma region readFeedbackAction
class ReadFeedbackAction : public Action, public CommonActions {
public:
	virtual void doAction();
private:
	void displayFeedbacks(const vector<string>&);
};
/// <summary>
/// 1)	readFile() fonksiyonu ile feedback.txt içerisindeki bütün geri bildirimler alınır.
/// 
/// 2)	Dosya boş ise kullanıcı bilgilendirilir, işlemler durdurulur ve menüden çıkılır.
/// 
/// 3)	Dosya dolu ise menüyü tanıtan bir mesaj yayınlanır.
/// 
/// 4)	displayFeedbacks() fonksiyonu ile bütün mesajlar gösterilir.
/// </summary>
void ReadFeedbackAction::doAction() {

	vector<string> feedbackDetails = readFile(feedbackFile);
	if (feedbackDetails.size() == 0)
	{
		// feedback.txt dosyası mevcut fakat hiç kullanıcı geri dönüşü yapılmamış.
		Buffer::printInRect("Goruntulenecek kullanici geri bildirimi yok!", 0);
		return;
	}

	Buffer::printCustomGuideline("readFeedback");
	displayFeedbacks(feedbackDetails);
}
/// <summary>
/// 1)	feedback.txt dosyası içerisindeki her bir geri bildirim vector<string> olarak alınır.
/// 
/// 2)	Her bir geri bildirim 'kullanıcı adı - yorum' olarak saklanmaktadır.
///		Kullanıcı adı ve yorum bilgileri değişkenlere atanır.
/// 
/// 3)	Fonksiyon kullanılarak bütün yorumlar kullanıcıya gösterilir.
/// </summary>
/// <param name="vec"> Kullanıcılar tarafından yapılan bütün geri bildirimler. </param>
void ReadFeedbackAction::displayFeedbacks(const vector<string>& vec) {
	string fullName, userName, comment;
	size_t idx1, idx2;
	for (string line : vec) {
		idx1 = line.find('-');
		idx2 = line.find('-', idx1 + 1);

		fullName = line.substr(0, idx1);
		userName = line.substr(idx1 + 1, idx2 - idx1 - 1);
		comment = line.substr(idx2 + 1, line.length());
		Buffer::printArgsInRect({fullName + "  -  " + userName, comment }, 60, false);	// vec, maxlen, topmargin
	}
}
#pragma endregion

#pragma region discountCodeAction
class DiscountCodeAction : public Action, public CommonActions {
public:
	virtual void doAction();
private:
	bool checkUserExists(const string&, vector<string>&);
	void updateFile(const string&, const string&, vector<string>&);
};
/// <summary>
/// 1)	Kullanıcının menüyü etkili kullanması için bir kulavuz mesajı yayınlanır.
/// 
/// 2)	readFile() fonksiyonu ile client.txt dosyasındaki bilgiler alınır.
///		Dosya boş ise sistemde kayıtlı kullanıcı yoktur ve indirim kodu eklenemez.
///		İşlemler sonlandırılır, kullanıcı bilgilendirilir ve menüden çıkılır.
/// 
/// 3)	Dosya dolu ise indirim tanımlanacak olan kullanıcının kullanıcı adı istenir.
/// 
/// 4)	Girilen kullanıcı adı sistemde kayıtlı mı kontrol edilir.
///		Kayıtlı değil ise işlemler sonlandırılır, kullanıcı bilgilendirilir ve menüden çıkılır.
/// 
/// 5)	Girilen kullanıcı sistemde kayıtlı ise tanımlanacak olan indirim kodu istenir.
/// 
/// 6)	Girilen indirim kodu koşulları sağlamıyorsa işlemler sonlandırılır, kullanıcı bilgilendirilir ve menüden çıkılır.
/// 
/// 7)	Girilen indirim kodu koşulları sağlıyorsa updateFile() fonksiyonu ile client.txt dosyasındaki bilgiler güncellenir.
/// 
/// 8)	İşlemin tamamlandığına dair kullanıcı bilgilendirilir ve menüden çıkılır.
/// </summary>
void DiscountCodeAction::doAction() {

	Buffer::printCustomGuideline("discountCode");

	vector<string> userDetails = readFile(clientFile);

	if (userDetails.size() == 0)
	{
		// dosya boş ise kullanıcı yoktur ve indirim kodu eklenemez.
		system("cls");
		Buffer::printInRect("Sistemde kayitli kullanici olmadigi icin herhangi bir indirim kodu tanimlanamiyor!", 0);
		return;
	}

	// kullanıcı adını iste.
	Buffer::printUserInputScreen("userName", 30, true, false);
	string userName = inputSequence(false);

	if (!checkUserExists(userName, userDetails))
	{
		Buffer::print(Buffer::horizontalBottomDivider);
		// kullanıcı mevcut değil ise indirim kodu eklenemez.
		Buffer::printInRect("Belirtilen kullanici sistemde kayitli olmadigi icin indirim kodu tanimlanamiyor!", 0);
		return;
	}

	// indirim kodunu iste
	Buffer::printUserInputScreen("Indirim Kodu  :  ", 50, false, true);
	string discountCode = inputSequence(false);
	Buffer::print(Buffer::horizontalBottomDivider);

	if (!validator->validateDiscountCode(discountCode)) {
		// indirim kodu geçerli değil.
		system("cls");
		Buffer::printInRect("Girdilen indirim kodu kosulları saglamiyor!", 0);
		return;
	}

	updateFile(userName, discountCode, userDetails);
	system("cls");
	Buffer::printInRect("Kullaniciya indirim kodu tanimlama islemi basarili!", 0);
}
/// <summary>
/// 1)	Bütün kullanıcıların bilgilerini içeren userDetails nesnesi içerisindeki her bir string Client nesnesine dönüştürülür.
/// 
/// 2)	Dönüştürülen her bir nesnenin kullanıcı adı bilgisi ile userName karşılaştırılır.
/// 
/// 3)	Eşleşen kullanıcı nesnesinin setDiscountCode() fonksiyonu kullanılarak discountCode bilgisi güncellenir.
/// 
/// 4)	Güncellenen bilgiler overload edilen writeFile(const vector<Client>&) fonksiyonu ile client.txt dosyasına kaydedilir.
/// </summary>
/// <param name="userName"> İndirim tanımlanmak istenen kullanıcının kullanıcı adı. </param>
/// <param name="discountCode"> Tanımlanmak istenen indirim kodu. </param>
/// <param name="userDetails"> Bütün kullanıcıların bilgilerini içeren nesne. </param>
void DiscountCodeAction::updateFile(const string& userName, const string& discountCode, vector<string>& userDetails) {
	vector<Client> finalVec;
	for (string& user : userDetails) {
		Client temp = Client(user);
		if (userName.compare(temp.getUserName()) == 0)
			temp.setDiscountCode(discountCode);
		finalVec.push_back(temp);
	}
	// update client file
	writeFile(finalVec);
}
/// <summary>
/// 1)	clientDetails her bir kullanıcının bilgilerini temsil eden string'lerden oluşan bir vector'dur.
///		Bu bilgiler client.txt dosyasından elde edilmiştir.
/// 
/// 2)	Her bir kullanıcı bilgisi içerisinden gerekli işlemler yapılarak kullanıcı adı bilgisi userName elde edilir.
/// 
/// 3)	İndirim kodu tanımlanmak istenen kullanıcı adı bilgisini içeren current nesnesi ile userName nesnesi eşleşiyor
///		ise true döndürülür.
/// 
/// 4)	Hiçbir kullanıcı adı bilgisi ile eşleşme sağlanamaz ise false döndürülür.
/// </summary>
/// <param name="current"> Kullanıcı Adı </param>
/// <param name="clientDetails"> Her bir kullanıcının bilgisini temsil eden string'ler vector içerisinde tutulur.</param>
/// <returns>
///		Kullanıcının mevcut olması veya olmaması durumuna bağlı olarak true veya false döner.
/// </returns>
bool DiscountCodeAction::checkUserExists(const string& current, vector<string>& clientDetails) {
	string userName, password;
	size_t idx1, idx2;
	for (string line : clientDetails) {
		Client user = Client(line);
		if (current.compare(user.getUserName()) == 0)
			return true;
	}
	return false;
}
#pragma endregion

#pragma region shoppingAction
class ShoppingAction : public Action, public CommonActions {
public:
	virtual void doAction();
private:
	int categorySize = 6, sizeSize = 6, colorSize = 6;

	void showProducts(vector<Clothes>& products);
	void showCustomization(Clothes& product);
	void showBasket(vector<Clothes>& basket);
	void showInvoice(vector<Order>& orders);

	Time addTimeByLocation();
	void updateOrderFile(Order& order, Courier& courier);

	int getChoice(const int& choiceSize);
	vector<Clothes> getProducts(string& categoryName);

	vector<Courier> getCouriers();
	bool quitOrContinue();
};
/// <summary>
/// 1)	Kullanıcının menüyü etkili kullanması için bir kulavuz mesajı yayınlanır.
/// 
/// 2)	Sistemde kurye var mı kontrol edilir. Yoksa işlemler durdurulur, menüden çıkılır.
/// 
/// 3)	Sistemde kayıtlı kurye varsa kullanıcının mevcut ürünlere göre alışveriş yapması için belirli fonksiyonlar sırayla çalıştırılır.
/// 
/// 4)	Alışveriş işlemi sorunsuz tamamlanırsa gerekli dosyalar güncellenir. Aksi halde menüden çıkılır.
/// 
/// 5)	İşlemlerin tamamlandığına dair kullanıcı bilgilendirilir.
/// /// </summary>
void ShoppingAction::doAction() {

	vector<Courier> couriers = getCouriers();
	if (couriers.size() == 0) {
		system("cls");
		Buffer::printArgsInRect({ "Sistemde kayitli kurye bulunmadigindan islem gerceklestirilemiyor!" }, 0, true);
		return;
	}

	string cont;
	bool showInvoiceFlag = false;

	int categoryChoice;
	vector<string> categories = { "Elbise", "Tisort", "Pantolon", "Gomlek", "Etek", "Ayakkabi" };

	int productChoice;
	vector<Clothes> products;

	int _sizeChoi;
	vector<string> sizes = { "XS", "S", "M", "L", "XL", "XXL" };

	int _colorChoi;
	vector<string> colours = { "Siyah", "Beyaz", "Kirmizi", "Sari", "Yesil", "Mavi" };

	Clothes currentProduct;
	vector<Clothes> basket;

	int invoiceID = readFile(invoiceFile).size() + 1;
	Time currentTime = Time(APPLICATION_TIME.getStrFullTime());
	Time deliveryTime;

	// müşteri istediği kadar ürün alabilir.
	// bu yüzden loop içinde olmalı
	while (true) {
		system("cls");

		Buffer::printCustomGuideline("shopping1");
		Buffer::printSubmenuInRect(categories, 20, true);
		Buffer::print(Buffer::inputArea);

		categoryChoice = getChoice(categorySize);

		if (categoryChoice == 0)
		{
			system("cls");
			Buffer::printArgsInRect({ "Lutfen gecerli bir kategori numarasi tuslayiniz!" }, 0, true);
			if (quitOrContinue())
				break;
			continue;
		}

		products = getProducts(categories.at(categoryChoice - 1));

		if (products.size() == 0)
		{
			system("cls");
			Buffer::printArgsInRect({"Secili kategoride urun stoklarimiz tukenmistir!" }, 0, true);
			if (quitOrContinue())
				break;
			continue;
		}

		system("cls");
		Buffer::printCustomGuideline("shopping2");
		showProducts(products);
		Buffer::print(Buffer::inputArea);

		productChoice = getChoice(products.size());

		if (productChoice == 0)
		{
			system("cls");
			Buffer::printArgsInRect({ "Lutfen gecerli bir kategori numarasi tuslayiniz!" }, 0, true);
			if (quitOrContinue())
				break;
			continue;
		}

		system("cls");
		Buffer::printCustomGuideline("shopping3");
		currentProduct = products.at(productChoice - 1);
		showCustomization(currentProduct);

		Buffer::printUserInputScreen("Beden Numarasi : ", 50, false, false);
		cin >> _sizeChoi;
		Buffer::printUserInputScreen("Renk Numarasi  : ", 50, false, true);
		cin >> _colorChoi;

		if (_sizeChoi < 0 || _sizeChoi > sizeSize)
		{
			system("cls");
			Buffer::printArgsInRect({ "Lutfen gecerli bir beden numarasi tuslayiniz!" }, 0, true);
			if (quitOrContinue())
				break;
			continue;
		}
		if (_colorChoi < 0 || _colorChoi > colorSize)
		{
			system("cls");
			Buffer::printArgsInRect({ "Lutfen gecerli bir beden numarasi tuslayiniz!" }, 0, true);
			if (quitOrContinue())
				break;
			continue;
		}

		showInvoiceFlag = true;
		currentProduct.setSize(sizes.at(_sizeChoi - 1));
		currentProduct.setColor(colours.at(_colorChoi - 1));

		basket.push_back(Clothes(currentProduct));

		system("cls");
		Buffer::printArgsInRect({"Sepetinizde asagidaki urunler bulunmaktadir."},0, true);
		showBasket(basket);

		if (quitOrContinue())
			break;
	}

	if (showInvoiceFlag)
	{

		// couriers değişkeni vector<Courier> tipinde bir nesnedir.
		// algoritmaya göre en uygun kuryeyi seçelim.
		int index = 0;
		Courier available = couriers.at(0);
	
		for (int i = 0; i < couriers.size(); i++) {
			if (couriers.at(i).getAvailableTime() < available.getAvailableTime()) {
				available = couriers.at(i);
				index = i;
			}
		}

		Time appendix = addTimeByLocation();

		// kuryenin availableTime' ı APPLICATION_TIME' dan büyükse bunun üzerine ekle.
		// APPLICATION_TIME daha büyük ise bu saat üzerine ekle.
		if (available.getAvailableTime() > currentTime) {
			deliveryTime = available.getAvailableTime() + appendix;
			available.addOrder(invoiceID, deliveryTime);
		}
		else {
			deliveryTime = currentTime + appendix;
			available.addOrder(invoiceID, deliveryTime);
		}

		available.setAvailableTime();

		couriers.at(index) = available;

		vector<Order> orders;
		for (Clothes& each : basket) {
			orders.push_back(Order(invoiceID, each, currentTime, deliveryTime));
		}
		// update courier file
		writeFile(couriers);
		// update order file
		updateOrderFile(orders.at(0), available);

		system("cls");
		Buffer::printArgsInRect(
			{
				"Bizi tercih ettiginiz icin tesekkur ederiz!",
				"Faturaniz ekte gosterilmistir.",
				"Siparisinizin takibini ilgili menuden yapabilirsiniz.",
				"Cikis yapmak icin herhangi bir tusa basiniz."
			}, 0, true
		);
		
		// show incoice and update invoice file.
		showInvoice(orders);
	}
}
/// <summary>
/// 1)	orders nesnesi kullanıcının yaptığı bütün alışveriş ürünlerini ve ayrıntılarını tutan vector'dur.
/// 
/// 2)	orders vectoru ve clientPtr ile birlikte fatura oluşturmak için gerekli bütün bilgileri elde edilir.
///		Bu bilgiler kullanılarak bir Invoice nesnesi oluşturulur.
/// 
/// 3)	overload edilen writeFile(const Invoice&) fonksiyonunu kullanarak invoice.txt dosyasındaki bilgiler güncellenir.
/// 
/// 4) Buffer::printCustomInvoice() fonksiyonu kullanılarak fatura kullanıcıya gösterilir.
/// </summary>
/// <param name="orders"> Mevcut Siparişler </param>
void ShoppingAction::showInvoice(vector<Order>& orders) {
	string fullname, username, phoneno, address, discode;
	Time ordertime, deliverytime;
	int id = orders.at(0).getOrderID();
	fullname = clientPtr->getFullName();
	username = clientPtr->getUserName();
	phoneno = clientPtr->getPhoneNumber();
	address = clientPtr->getAddress();
	discode = clientPtr->getDiscountCode();
	ordertime = orders.at(0).getOrderTime();
	deliverytime = orders.at(0).getDeliveryTime();

	Invoice inv = Invoice(id, fullname, username, phoneno, address, discode, ordertime, deliverytime, orders);

	// update invoice file
	writeFile(inv);
	
	Buffer::printCustomInvoice(inv);
}
/// <summary>
/// 1)	product nesnesi halihazırda seçilmiş olan kıyafeti işaret eder.
/// 
/// 2)	Kıyafet özelleştirmek için mevcut seçenekler kullanıcıya sunulur.
/// </summary>
/// <param name="product"> Kıyafet Nesnesi </param>
void ShoppingAction::showCustomization(Clothes& product) {
	Buffer::printArgsInRect(
		{
			"Kategori  : " + product.getCategory(),
			"Urun adi  : " + product.getName(),
			"Fiyat     : " + to_string(float(product.getPrice())) + "$",
			"",
			"Mevcut Bedenler         Mevcut Renkler",
			"---------------         --------------",
			" 1 - XS                  1 - Siyah",
			" 2 - S                   2 - Beyaz",
			" 3 - M                   3 - Kirmizi",
			" 4 - L                   4 - Sari",
			" 5 - XL                  5 - Yesil",
			" 6 - XXL                 6 - Mavi",
		},
		50, true
		);
}
/// <summary>
/// Kullanıcının alışveriş boyunca seçtiği ürünleri ve bu ürünlerin fiyatları listelenir ve kullanıcıya gösterilir.
/// </summary>
/// <param name="basket"> Alışveriş süresince seçilen bütün kıyafetler. </param>
void ShoppingAction::showBasket(vector<Clothes>& basket) {
	string _cat, _nam, _price, _siz, _col;

	string margin(20, ' ');
	
	cout << endl << endl;
	cout << margin << "          Kategori ve Urun Adi          -     Beden     -     Renk      -     Fiyat     " << endl;
	cout << margin << "----------------------------------------------------------------------------------------" << endl;

	for (Clothes& current : basket) {

		_cat = current.getCategory();
		_nam = current.getName();
		_siz = current.getSize();
		_col = current.getColor();

		_price = to_string(current.getPrice());
		
		//  40 - 15 - 15 - 15
		cout << margin;
		cout << _cat << string(10 - _cat.length(), ' ');
		cout << _nam << string(30 - _nam.length(), ' ');
		cout << "-";
		cout << string(7, ' ') << _siz << string(8 -_siz.length(), ' ');
		cout << "-";
		cout << string(5, ' ') << _col << string(10 - _col.length(), ' ');
		cout << "-";
		cout << string(5, ' ') << _price << string(10 - _price.length() , ' ') << endl << endl;
	}
}
/// <summary>
/// 1)	Seçili kıyafetler vector<Clothes> olarak product nesnesinde tutulur.
/// 
/// 2)	for döngüsü içerisinde Buffer::printArgsInRect() fonksiyonu ile kıyafetlerin özellikleri kullanıcıya gösterilir.
/// </summary>
/// <param name="products"> Kıyafetler </param>
void ShoppingAction::showProducts(vector<Clothes>& products) {
	int counter = 0;
	for (Clothes& current : products) {
		counter++;
		Buffer::printArgsInRect({"Urun No   : " + to_string(counter), "Kategori  : " + current.getCategory(), "Urun adi  : " + current.getName(), "Fiyat     : " + to_string(current.getPrice()) + "$"}, 50, true);
	}
}
/// <summary>
/// 1)	Seçili kategori bilgisi categoryName nesnesinde tutulur.
/// 
/// 2)	readFile() fonksiyonu ile product.txt dosyası okunur.
/// 
/// 3)	Dosya içerisindeki ürünlerden kategori bilgisi istenen kategori ile eşleşenler seçilir ve Clothes nesnesi olarak vector'e eklenir.
///		Bu vector<Clothes> geri döndürülür.
/// </summary>
/// <param name="categoryName"> Seçilen Kategori </param>
/// <returns> Bu kategoriye uyan bütün kıyafetler. </returns>
vector<Clothes> ShoppingAction::getProducts(string& categoryName) {
	categoryName[0] = tolower(categoryName[0]);

	vector<string> allProducts = readFile(productFile);
	vector<Clothes> products;
	int counter = 0;
	string cat, temp;

	if (allProducts.size() == 0)
		return products;

	for (string& line : allProducts) {
		cat = line.substr(0, line.find('-'));
		if (cat.compare(categoryName) == 0) {
			temp = line + "-NaN-NaN";
			products.push_back(Clothes(temp));
		}
	}
	return products;
}
/// <summary>
/// 1) readFile() fonksiyonu ile courier.txt dosyası okunur ve vector<string> olarak allCouriers nesnesinde tutulur.
/// 
/// 2)	for döngüsü içerisinde allCouriers'in içinde bulununan bütün string bilgiler Courier nesnesine dönüştürülür
///		ve vector<Courier> olarak couriers nesnesinde saklanır. Ardından bu nesne geri döndürülür.
/// </summary>
/// <returns> Bütün kuryeler. </returns>
vector<Courier> ShoppingAction::getCouriers() {
	vector<string> allCouriers= readFile(courierFile);
	vector<Courier> couriers;

	if (allCouriers.size() == 0)
		return couriers;

	for (string& line : allCouriers) {
		couriers.push_back(Courier(line));
	}
	return couriers;
}
/// <summary>
/// 1)	choiceSize yapılabilecek en büyük seçim numarasını temsil eder.
/// 
/// 2)	Kullanıcıdan bir integer seçim yapılması istenir.
/// 
///	3)	Eğer 0 ile choiceSize aralığında bir sayı girilmişse girilen sayı geri döndürülür.
///		Aksi halde 0 döndürülür.
/// </summary>
/// <param name="choiceSize"> Maksimum Seçim Boyutu </param>
/// <returns> Kullanıcıdan Alınan Girdi </returns>
int ShoppingAction::getChoice(const int& choiceSize) {
	int choice;
	cin >> choice;
	if (choice > 0 && choice <= choiceSize)
		return choice;
	return 0;
}
/// <summary>
/// 1)	Kullanıcının adres bilgisi clientPtr ile alınır.
/// 
/// 2)	adres bilgisine göre Time nesnesi oluşturulur ve geri döndürülür.
/// </summary>
/// <returns> Eklenecek zaman bilgisini tutan Time nesnesi. </returns>
Time ShoppingAction::addTimeByLocation() {
	string address = clientPtr->getAddress();

	for (int i = 0; i < address.length(); i++)
		address[i] = tolower(address[i]);

	if (address.compare("ortahisar") == 0)
		return Time(0, 0, 35);	// 35dk
	if (address.compare("akcaabat") == 0)
		return Time(0, 0, 50);	// 50dk
	if (address.compare("vakfikebir") == 0)
		return Time(0, 1, 30);	// 90dk
	if (address.compare("besikduzu") == 0)
		return Time(0, 1, 50);	// 110dk
	if (address.compare("yomra") == 0)
		return Time(0, 0, 55);	// 55dk
	if (address.compare("arsin") == 0)
		return Time(0, 1, 10);	// 70dk
	if (address.compare("arakli") == 0)
		return Time(0, 1, 40);	// 100dk
	
	// program bu kod bloğu hiçbir zaman çalışmayacak şekilde tasarlanmıştır.
	// eklenme nedeni derleyici uyarısından kaçınmaktır.
	else
		return Time(0, 0, 0);
}
/// <summary>
/// 1)	Kullanıcının çıkış yapıp yapmamasını tespit etmek için kullanıcıdan girdi alınır.
/// 
/// 2)	Eğer girdi 'q' veya 'Q' ise true, aksi halde false döndürülür.
/// </summary>
/// <returns> Çıkış koşulu sağlanırsa true, aksi halde false. </returns>
bool ShoppingAction::quitOrContinue() {
	Buffer::printArgsInRect({ "Cikmak icin Q tuslayiniz.", "Alisverise devam etmek icin herhangi bir tus tuslayiniz."}, 0, true);
	// burada inputSeq yerine cin daha mantıklı gibi
	string seq = inputSequence(false);
	if (seq.compare("Q") == 0 || seq.compare("q") == 0)
		return true;
	return false;
}
// Order::getSelf() ile birlikte kullanılacak. ona göre düzenle.
/// <summary>
/// 1)	Kullanıcının yaptığı alışveriş sonucunda oluşan siparişi order.txt dosyasına eklenir ve dosya güncellenir.
/// </summary>
/// <param name="order"> Sipariş Bilgileri </param>
/// <param name="courier"> Kurye Bilgileri </param>
void ShoppingAction::updateOrderFile(Order& order, Courier& courier) {
	string str = order.getOrderString() + '-' + courier.getName() + '-' + courier.getPhoneNumber() + '-' + clientPtr->getUserName();
	ofstream file;
	file.open(rootPath + orderFile, std::ios_base::app);
	file << str;
	file << '\n';
	file.close();
}
#pragma endregion

#pragma region orderTrackingAction
class OrderTrackingAction : public Action, public CommonActions {
public:
	virtual void doAction();
private:
	vector<string> getUserOrders(const string&);
};
/// <summary>
/// 1)	Kullanıcının menüyü etkili kullanması için bir kılavuz mesajı yayınlanır.
/// 
/// 2)	Aktif logIn işlemi yapmış kullanıcının bilgileri clientPtr vasıtası ile çekilir.
/// 
/// 3)	getUserOrders() fonksiyonuna kullanıcı adı parametre olarak verilir ve bu fonksiyon
///			kullanıcının siparişlerini vector<string> olarak geri döndürür.
/// 
/// 4)	Elde edilen vector'un boyutu kontrol edilir. Eğer 0'a eşitse kullanıcının siparişi yoktur demektir.
///		Kullanıcı bilgilendirilir ve menüden çıkılır.
/// 
/// 5)	Elde edilen vector'un boyutu 0'dan farklı ise kullanıcının mevcut siparişi veya siparişleri vardır demektir.
///		Kullanıcıya kaç adet siparişinin olduğu hakkında bilgi verilir.
///		Buffer::printCustomOrder() fonksiyonu ile dosyadaki bilgiler ve kullanıcı bilgileri harmanlanarak kullanıcıya gösterilir.
/// 
/// 6)	Kullanıcı 'q' veya 'Q' tuşlamadığı sürece sonraki sipariş gösterilir.
/// 
/// 7)	'q' veya 'Q' tuşlandığında işlemler sonra erer, menüden çıkılır.
/// </summary>
void OrderTrackingAction::doAction() {

	string fullName = clientPtr->getFullName();
	string userName = clientPtr->getUserName();
	string address = clientPtr->getAddress();

	vector<string> orders = getUserOrders(userName);
	size_t size = orders.size();

	if (size == 0) {
		system("cls");
		Buffer::printInRect("Sistemde kayitli siparisiniz bulunmuyor!", 0);
		return;
	}

	int counter = 0;
	char c;

	while (true)
	{
		system("cls");
		Buffer::printArgsInRect({ "Sistemde mevcut " + to_string(size) + " adet siparisiniz bulunmaktadir.","Cikmak icin Q tuslayin.","Sonraki siparis durumunu goruntulemek icin herhangi bir tus tuslayin." }, 0, false);

		Buffer::printCustomOrderTracking(orders.at(counter), fullName, address);

		counter = (counter + 1) % size;

		c = _getch();
		if (c == 'Q' || c == 'q')
			break;
	}
	system("cls");
}
/// <summary>
/// 1)	readFile() fonksiyonu ile order.txt dosyası içerisinden bilgiler okunur, vector<string> olarak depolanır.
/// 
/// 2)	vector<string> vektör içerisinde string'ler her bir siparişin içeriğini temsil eder.
/// 
/// 3)	Kullanıcı adına göre her bir sipariş bilgisi filtrelenir ve ikinci bir vector<string> içerisinde depolanır.
/// 
/// 4)	Filtrelenmiş siparişleri içeren vector döndürülür.
/// </summary>
/// <param name="userName"> Kullanıcı Adı </param>
/// <returns>
///		kullanıcı adına göre filtreleyerek seçilen siparişler vector<string> olarak döndürülür.
///		dosya boş ise veya kullanıcının herhangi bir siparişi yok ise boş bir vector<string> döndürür.
/// </returns>
vector<string> OrderTrackingAction::getUserOrders(const string& userName) {
	vector<string> userOrders;
	vector<string> allOrders = readFile(orderFile);

	// orders.txt boş ise bu kullanıcının siparişi yoktur.
	if (allOrders.size() == 0)
		return userOrders;

	// dosya dolu ise siparişlar arasından bu kullanıcıya ait olanları seçelim.
	for (string& str : allOrders) {
		if (str.substr(str.rfind('-') + 1, str.length()).compare(userName) == 0) {
			userOrders.push_back(str);
		}
	}
	return userOrders;
}
#pragma endregion

#pragma region takeFeedbackAction
class TakeFeedbackAction : public Action, public CommonActions {
public:
	virtual void doAction();
};
/// <summary>
/// 1)	Kullanıcının menüyü etkili kullanması için bir kulavuz mesajı yayınlanır.
/// 
/// 2)	Kullanıcıdan yapacağı geri bildirimi girmesi istenir.
/// 
/// 3)	Yapılan geri bildirimde ascii karakter setine uymayan karakter var mı kontrol edilir.
///		Eğer varsa işleme devam edilmez, kullanıcı bilgilendirilir ve menüden çıkılır.
/// 
/// 4)	Koşul sağlandığı takdirde yapılan geri bildirim dosyaya kaydedilir.
/// 
/// 6)	İşlemin tamamlandığına dair kullanıcı bilgilendirilir ve menüden çıkılır.
/// </summary>
void TakeFeedbackAction::doAction() {

	Buffer::printCustomGuideline("takeFeedback");
	string comment;
	Buffer::print(Buffer::inputArea);
	comment = inputSequence(false);

	// yorumun geçerliliğini test et.
	if (!validator->validateComment(comment)) {
		system("cls");
		Buffer::printInRect("Girdiginiz metin kosullari saglamiyor! Lutfen tekrar deneyin.", 0);
		return;
	}
	// yorum geçerli, sisteme ekle.
	string userComment = clientPtr->getFullName() + '-' + clientPtr->getUserName() + '-' + comment;
	writeFile(feedbackFile, userComment);

	// kullanıcıya islemin basarili oldugunu bildir.
	system("cls");
	Buffer::printInRect("Geri bildiriminiz icin tesekkur ederiz!", 50);
}
#pragma endregion

#pragma region pwChangeAction
class PasswordChangeAction : public Action, public CommonActions {
public:
	virtual void doAction();
private:
	void updateClientDetails(const string&, const string&);
};
/// <summary>
/// 1)	Kullanıcının menüyü etkili kullanması için bir kılavuz mesajı yayınlanır.
/// 
/// 2)	Kullanıcıdan mevcut şifresini girmesi istenir.
///		Girilen şifre mevcut şifre ile eşleşiyor mu kontrol edilir.
///		Şifre eşleşmiyorsa işleme devam edilmez, kullanıcı bilgilendirilir ve menüden çıkılır.
///		Şifre eşleşiyorsa sonraki aşamaya geçilir.
/// 
/// 3)	Kullanıcıdan değiştirmek istediği şifreyi girmesi istenir.
///		Ardından bu şifreyi teyit etmek amaçlı tekrar girmesi istenir.
/// 
/// 4)	Bu iki girdi üzerinden kontroller yapılır.
///			* Yeni girilen şifre ve teyit etmek için girilen şifre aynı değilse,
///			* Yeni şifre eski şifre ile aynıysa,
///			* Yeni şifre gerekli koşulları sağlamıyorsa,
///			* Yeni şifre yeterince güçlü değilse
///		işleme devam edilmez, kullanıcı bilgilendirilir ve menüden çıkılır.
///		
/// 5)	Koşulların hepsi sağlandığı takdirde kullanıcı şifresi hem dosyadan hem de
///		clientPtr üzerinden değiştirilir.
/// 
/// 6)	İşlemin tamamlandığına dair kullanıcı bilgilendirilir ve menüden çıkılır.
/// </summary>
void PasswordChangeAction::doAction() {

	Buffer::printCustomGuideline("changePw");

	// mevcut şifreyi iste.
	string oldPassword = getUserInput("Eski Sifre        :  ", true, false, true);
	// teyit et.
	if (oldPassword.compare(clientPtr->getPassword()) != 0) {
		system("cls");
		Buffer::printInRect("Degistirmeye calistiginiz sifreniz dogru degil!", 0);
		return;
	}

	string newPassword1 = getUserInput("Yeni Sifre        :  ", false, true, true);
	string newPassword2 = getUserInput("Yeni Sifre Tekrar :  ", false, true, true);
	Buffer::print(Buffer::horizontalBottomDivider);


	if (newPassword1.compare(newPassword2) != 0) {
		system("cls");
		Buffer::printInRect("Yeni girdiginiz sifreler uyusmuyor!", 0);
		return;
	}

	if (oldPassword.compare(newPassword1) == 0) {
		system("cls");
		Buffer::printInRect("Yeni sifreniz eski sifreniz ile ayni olamaz!", 0);
		return;
	}

	if (!validator->validatePassword(newPassword1)) {
		system("cls");
		Buffer::printInRect("Yeni girdiginiz sifre gecerli degil!", 0);
		return;
	}

	if (!validator->checkPasswordStrength(newPassword1)) {
		system("cls");
		Buffer::printInRect("Yeni girdiginiz sifre yeterince guclu degil!", 0);
		return;
	}

	updateClientDetails(clientPtr->getUserName(), newPassword1);
	clientPtr->setPassword(newPassword1);

	system("cls");
	Buffer::printInRect("Sifre basariyla degistirilmistir!", 0);
}
/// <summary>
/// 1)	readFile() fonksiyonu ile dosyadan bütün kullanıcıların bilgileri vector<string> olarak alınır.
/// 
/// 2)	for döngüsü ile vector'de bulunan her bir string içerisindeki bilgiler Client nesnesine dönüştürülür.
/// 
/// 3)	Mevcut kullanıcının kullanıcı adı ile Client nesnesindeki kullanıcı adı bilgisi karşılaştırılır.
///		Bilgiler eşleşiyorsa bu nesne şuan logIn eden kullanıcının bilgilerine işaret eden nesne olduğu anlamına gelir.
///		Bu nesne içerisinde setPassword() methodu kullanılarak eski şifre yeni şifre ile düzenlenir.
///	
/// 4)	vector<string> halinde bulunan kullanıcı bilgileri yapılan işlemler sonucunda vector<Client> şekline çevrilmiştir.
/// 
/// 5)	overload edilen writeFile(const Client&) fonksiyonu kullanılarak bütün bilgiler mevcut dosya üzerine güncel haliyle yazılır.
/// </summary>
/// <param name="userName"> Kullanıcı Adı </param>
/// <param name="newPassword"> Yeni Şifre </param>
void PasswordChangeAction::updateClientDetails(const string& userName, const string& newPassword) {
	vector<string> clientDetails = readFile(clientFile);

	vector<Client> finalVec;
	for (string& user : clientDetails) {
		Client temp = Client(user);
		if (userName.compare(temp.getUserName()) == 0)
			temp.setPassword(newPassword);
		finalVec.push_back(temp);
	}
	// update client file
	// overloaded writeFile(const Client&) function
	writeFile(finalVec);
}
#pragma endregion

#pragma region prevMenuAction
/// <summary>
/// ust menuye donmek icin bu noktada herhangi bir islem yapmaya gerek yok.
/// islem MenuTree::menu() fonksiyonu içerisinde gerekli koşul sağlandığında gerçekleşiyor.
/// </summary>
class PreviousMenuAction : public Action {
};
#pragma endregion


#pragma endregion

//	düğüm yapısı
#pragma region MenuNode
class MenuNode {
public:
    MenuNode(const string&);
    string name;
    string actionName;
    MenuNode* parent;
    Action* actionHandler;
    vector<MenuNode*> children;

    bool haveSubmenu = false;
    bool haveCondition = false;
    unsigned short int submenuCount = 0;
    unsigned short int actionID;

    int waitKey();
    void showSubmenus();
};
MenuNode::MenuNode(const string& _name) {
	name = _name;
}
/// <summary>
/// Kullanıcının menü geçişleri arasında yaptığı seçimi kontrol eder.
/// Eğer yapılan seçim mevcut ise seçeneğin indeksini, aksi halde -1 döndürür.
/// </summary>
int MenuNode::waitKey() {
	Buffer::print(Buffer::inputArea);

	string s_key;
	cin >> s_key;

	if (s_key.length() != 1)
		return -1;

	char key = s_key.at(0);

	if (key < 49 || key > 57)
		return -1;
	else {
		int intKey = atoi(&key);
		return intKey <= submenuCount ? intKey-1 : -1;
	}
}
/// <summary>
/// Mevcut node'un alt menülerini gösterir.
/// </summary>
void MenuNode::showSubmenus() {
	vector<string> data;
	for (int i = 0; i < submenuCount; i++)
		data.push_back(children.at(i)->name);

	Buffer::printSubmenuInRect(data, 50, true);
}
#pragma endregion

//	ağaç yapısı
#pragma region MenuTree
class MenuTree {
public:
    MenuNode* root;
    MenuNode* currentNode;

    MenuTree();
    MenuTree(MenuNode*);
    MenuTree(const string&);
    MenuNode* getNewNode(const string&);

    void menu();
    void initSubmenus();
    void showSubmenuScreen();
    void addNode(MenuNode*, const string&);                         // _parent, _data
    void addNode(MenuNode*, const string&, const string&);          // _parent, _data, _action
    void addNode(MenuNode*, const string&, const string&, bool);    // _parent, _data, _action, _condition
    void changeCurrentNode(MenuNode*);
    void waitUntilKeyPressed();

    void syncCurrentUser();
    void delCurrentUser();

    Action* actionSelector(const string&);
    bool quitOrTryAgain();
};
MenuTree::MenuTree() {
    MenuNode* node = getNewNode("root");
    root = node;
    currentNode = node;
    root->actionHandler = actionSelector("root");
    root->parent = NULL;
    root->haveSubmenu = true;
}
MenuTree::MenuTree(MenuNode* node) {
    root = node;
    currentNode = node;
    root->parent = NULL;
}
MenuTree::MenuTree(const string& _name) {
    MenuNode* node = getNewNode(_name);
    root = node;
    currentNode = node;
    root->parent = NULL;
}
/// <summary>
/// Verilen isim bilgisine göre yeni bir node oluşturulur ve geri döndürülür.
/// </summary>
MenuNode* MenuTree::getNewNode(const string& _name) {
    MenuNode* node = new MenuNode(_name);
    return node;
}

// overloaded addNode functions.

// actions yok. submenu var
void MenuTree::addNode(MenuNode* _parent, const string& _name) {
    MenuNode* node = getNewNode(_name);
    node->parent = _parent;
    node->actionHandler = actionSelector("");
    node->haveSubmenu = true;
    _parent->children.push_back(node);
    _parent->submenuCount++;
}
// actions var. submenu yok.
void MenuTree::addNode(MenuNode* _parent, const string& _name, const string& _action) {
    MenuNode* node = getNewNode(_name);
    node->parent = _parent;
    node->actionHandler = actionSelector(_action);
    node->haveSubmenu = false;
    _parent->children.push_back(node);
    _parent->submenuCount++;
}
// actions var. condition var. submenu var.
void MenuTree::addNode(MenuNode* _parent, const string& _name, const string& _action, bool _condition) {
    MenuNode* node = getNewNode(_name);
    node->parent = _parent;
    node->haveCondition = true;
    node->haveSubmenu = true;
    node->actionHandler = actionSelector(_action);
    _parent->children.push_back(node);
    _parent->submenuCount++;
}
/// <summary>
/// Müşteri veya yönetici girişi yapan kullanıcının bilgilerini menüler arası geçişte erişebilmek için bir nesnede tutulur.
/// Alt node'un client veya admin pointer'ı üst node'un pointer bilgisi ile eşitlenir.
/// </summary>
void MenuTree::syncCurrentUser() {
    if (currentNode->parent->name.compare("Musteri Girisi") == 0)
        currentNode->actionHandler->clientPtr = currentNode->parent->actionHandler->clientPtr;
    else if (currentNode->parent->name.compare("Yonetici Girisi") == 0)
       currentNode->actionHandler->adminPtr = currentNode->parent->actionHandler->adminPtr;
}
/// <summary>
/// Halihazırda giriş yapmış müşteri veya yönetici nesnesinin silindiği fonksiyondur.
/// </summary>
void MenuTree::delCurrentUser() {
	if(currentNode->parent->name.compare("Musteri Girisi") == 0)
		delete currentNode->actionHandler->adminPtr;
	else if(currentNode->parent->name.compare("Yonetici Girisi") == 0)
		delete currentNode->actionHandler->clientPtr;
}
/// <summary>
/// Mevcut düğümün kullanıcının yaptığı seçime bağlı olarak değiştirildiği fonksiyondur.
/// Değişme işlemi yapılırken uygulama zamanı sonraki node'a aktarılır ve uygulama zamanı arttırılır.
/// </summary>
void MenuTree::changeCurrentNode(MenuNode* node) {
    // burada zamanı stabil tutmak için değiştireceğimiz node'un süresini şu anki node'un süresi ile eşitliyoruz.
    node->actionHandler->APPLICATION_TIME = currentNode->actionHandler->APPLICATION_TIME;

    currentNode = node;
    // her node değişiminde zamanı arttır.
    currentNode->actionHandler->incrementTime();
}
/// <summary>
/// Menüler arası geçişi sağlayan, aksiyonların çalıştığı fonksiyondur.
/// Uygulama burada loop'a girer.
/// </summary>
void MenuTree::menu() {
    system("cls");

    // eğer currentNode bir menu ise ve diğer node'lara geçişi sağlıyorsa bir aksiyonu yoktur.
    // diğer node'lara geçiş için bu blok devreye girer
    if (currentNode->haveSubmenu)
    {
        // submenu'su var ve kondisyonu var ise (pw validate işlemi)
        if (currentNode->haveCondition)
        {
            // kondisyon ekranı gelsin ve kullanıcı doldursun.
            // eğer başarılı olursa alt menüler gösterilsin.
            // admin ve client login ekranı
            if (currentNode->actionHandler->checkAction()) {
               // 
                showSubmenuScreen();
            }
            // başarısız olursa kullanıcıya sorulsun, üst menüye dönülsün mü yoksa tekrar giriş yapmayı denesin mi
            // tekrar kondisyon ekranı gelsin ve kullanıcı doldursun.
            else
            {
                if (quitOrTryAgain())
                    menu();
                else
                {
                    changeCurrentNode(currentNode->parent);
                    menu();
                }

            }
        }
        // kondisyonu yok ise direk menü seçim ekranı gelir ve waitkey() fonksiyonu çalışır.
        else
            showSubmenuScreen();
    }
    // eğer currentNode' un submenu'su yoksa o node'un direk bir aksiyonu vardır.
    // aksiyonu gerçekleştirmek için bu blok devreye girer
    else
    {
        // previousMenuAction ozel bir fonksiyon. üst menüye dönmeyi amaçlıyor.
        // bu işlemi Action* pointeri içerisinden gerçeklertiremiyoruz. bu yüzden node ismini kontrol ediyor,
        // eğer amaçlanan isim olan 'Geri Don' ile eşleşirse bir üst menü işlemi gerçekleştiriliyor.
        if (currentNode->name.compare("Geri Don") == 0)
        {
            delCurrentUser();

            changeCurrentNode(currentNode->parent);
            changeCurrentNode(currentNode->parent);
            menu();
        }
        // diğer fonksiyonlar için genel kullanım.
        else
        {
            syncCurrentUser();
            // o node için spesifik olan doActions() fonksiyonu çalıştırılır.
            currentNode->actionHandler->doAction();
            // işlem tamamlandıktan sonra submenu'ye dönmek için kullanıcıdan herhangi bir tuş beklenir.
            waitUntilKeyPressed();
            system("cls");
            // tuşlama işlemi gerçekleştirildikten sonra geçilecek olan node'un submenu'leri görüntülenir.
            changeCurrentNode(currentNode->parent);
            showSubmenuScreen();
        }
    }
}
/// <summary>
/// Her bir menü düğümünün gerçekleştirmesi gereken görevler vardır.
/// Bu görevi gerçekleştirmelerini sağlayan kendi data member'leri olan actionHandler' dır.
/// Bu görevler belli başlı class'lar aracılığı ile gerçeklenecektir.
/// Seçili aksiyon ismine göre class'lar atanır.
/// </summary>
/// <param name="_action"> Aksiyon Adı </param>
/// <returns> Action nesnesi </returns>
Action* MenuTree::actionSelector(const string& _action) {
    // root actions
    if (_action.compare("exitAction") == 0) { return new ExitAction; }
    if (_action.compare("signUpAction") == 0) { return new SignUpAction; }
    // log in actions
    if (_action.compare("adminLogInAction") == 0) { return new LogInAction("admin"); }
    if (_action.compare("clientLogInAction") == 0) { return new LogInAction("client"); }
    // admin side actions
    if (_action.compare("productEntryAction") == 0) { return new ProductEntryAction; }
    if (_action.compare("addCourierAction") == 0) { return new AddCourierAction; }
    if (_action.compare("readFeedbackAction") == 0) { return new ReadFeedbackAction; }
    if (_action.compare("discountCodeAction") == 0) { return new DiscountCodeAction; }
    if (_action.compare("viewInvoiceAction") == 0) { return new ViewInvoiceAction; }
    // client side actions
    if (_action.compare("shoppingAction") == 0) { return new ShoppingAction; }
    if (_action.compare("orderTrackingAction") == 0) { return new OrderTrackingAction; }
    if (_action.compare("takeFeedbackAction") == 0) { return new TakeFeedbackAction; }
    if (_action.compare("passwordChangeAction") == 0) { return new PasswordChangeAction; }
    if (_action.compare("previousMenuAction") == 0) { return new PreviousMenuAction; }

    // root, geri dön ve sisteme giriş menülerinin spesifik bir işlevi yok.
    // base Action class'ı onlar için yeterlidir.
    return new Action;
}
/// <summary>
/// Uygulamanın iskelet yapısını oluşturan fonksiyon.
/// Ağaç yapısı kullanılarak birbiri ile bağlantılı menüler art arda eklenir.
/// </summary>
void MenuTree::initSubmenus() {
    addNode(root, "Sisteme Giris");
    addNode(root->children.at(0), "Yonetici Girisi", "adminLogInAction", true);
    addNode(root->children.at(0)->children.at(0), "Urun Girisi", "productEntryAction");
    addNode(root->children.at(0)->children.at(0), "Kurye Ekle", "addCourierAction");
    addNode(root->children.at(0)->children.at(0), "Sikayet / Oneri Oku", "readFeedbackAction");
    addNode(root->children.at(0)->children.at(0), "Indirim Kodu", "discountCodeAction");
    addNode(root->children.at(0)->children.at(0), "Fatura Goruntule", "viewInvoiceAction");
    addNode(root->children.at(0)->children.at(0), "Geri Don", "previousMenuAction");
    addNode(root->children.at(0), "Musteri Girisi", "clientLogInAction", true);
    addNode(root->children.at(0)->children.at(1), "Kiyafet Kategorileri ve Urun Secimi", "shoppingAction");
    addNode(root->children.at(0)->children.at(1), "Siparis Takip", "orderTrackingAction");
    addNode(root->children.at(0)->children.at(1), "Sikayet / Oneri", "takeFeedbackAction");
    addNode(root->children.at(0)->children.at(1), "Sifre Degistir", "passwordChangeAction");
    addNode(root->children.at(0)->children.at(1), "Geri Don", "previousMenuAction");
    addNode(root->children.at(0), "Geri Don", "previousMenuAction");
    addNode(root, "Uye Kaydi", "signUpAction");
    addNode(root, "Cikis", "exitAction");
}
/// <summary>
/// MenuNode class'ı ile birlikte çalışan bir fonksiyondur.
/// Kullanıcı alt menüler arasından bir seçim yapar.
/// Seçim geçerli ise seçilen menüye geçiş yapılır. Aksi halde kullanıcı uyarılır ve tekrar seçim yapması istenir.
/// </summary>
void MenuTree::showSubmenuScreen() {
    currentNode->showSubmenus();
    int choice = currentNode->waitKey();
    if (choice != -1)
    {
        changeCurrentNode(currentNode->children.at(choice));
        menu();
    }
    else
    {
        system("cls");
        Buffer::printInRect(Buffer::invalidInputError, 0);
        waitUntilKeyPressed();
        system("cls");
        showSubmenuScreen();
    }
}
/// <summary>
/// Kullanıcıdan herhangi bir karakter tuşlanması beklenir.
/// Tuşlama gelene kadar uygulama beklemeye geçer.
/// </summary>
void MenuTree::waitUntilKeyPressed() {
    Buffer::printInRect(Buffer::pressAnyKeyWarning, 0);
    _getch();
}
/// <summary>
/// Kullanıcının mevcut menüden mi yoksa bir üst menüden mi devam edeceğini kontrol edildiği fonksiyon.
/// </summary>
/// <returns></returns>
bool MenuTree::quitOrTryAgain() {
    Buffer::printInRect("Tekrar giris yapmayi denemek ister misiniz? y/n", 0);
    Buffer::print(Buffer::inputArea);
    char c;
    cin >> c;
    if (c == 'y')
        return true;

    else if (c == 'n')
        return false;

    else
    {
        system("cls");
        Buffer::printInRect(Buffer::invalidInputError, 0);
        return quitOrTryAgain();
    }
}
#pragma endregion

//	uygulama başlatıcı sınıf
#pragma region Application
class Application {
public:
	void init();

	void initMenuTree();
	void showAboutScreen();
};
// uygulamayı ve class larını init eden fonksiyon.
void Application::init() {
	//system("Color E0");
	// uygulama ilk calistiginda bu fonksiyon calisir. kullanici karsilama ekranini gösterir.
	showAboutScreen();
	// menuler arasında gezinmemizi sağlayan tree yapısını init eder.
	initMenuTree();
}

void Application::initMenuTree() {
	MenuTree* tree = new MenuTree;
	tree->initSubmenus();

	// uygulama zamanı ayarlanır.
	tree->root->actionHandler->setApplicationTime();

	// uygulama mainloop'a girer.
	tree->menu();
}
/// <summary>
/// Uygulama tanıtım ekranı.
/// </summary>
void Application::showAboutScreen() {
	Buffer::printArgsInRect(Buffer::welcomePage, 0, true);
	_getch();
}
#pragma endregion

int main()
{
	Application app;
	app.init();
}
