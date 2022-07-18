#include <iostream>
#include <string>
#include <time.h>
#include <Windows.h>
#include <conio.h>
using namespace std;

struct Card {
	int number;
	string suit;
	string name;
};

struct Stack {
	Card info;
	int numberInStack;
	Stack *next;
};

Stack* user, *bot, *table, *bufStackDeck;  // колода пользователя, колода бота, карты на столе, буфер для заполнения колод при добавлении карт(чтобы номера были по списку)

int Delete(Card* deck, int key, int size) {    // удаление элементов колоды
	for (int i = key; i < size; i++) {
		deck[i] = deck[i + 1];
	}
	return size - 1;
}

string trumpCardStr; // козырь


Stack* Create(Stack* beg, Card info, int i){     // создание/добавление колоды
	Stack* New = new Stack;
	New->info = info;
	New->next = beg;
	if (i > 0) {    // i > 0 если у нас колода, i == 0 если стол
		New->numberInStack = i;
	}
	return New;
}

void View(Stack* beg) {   // просмотр колоды
	Stack* cur1 = beg;
	while (cur1 != NULL) {
		if(cur1->numberInStack > 0)
		cout << cur1->numberInStack << ". ";
			cout << cur1->info.name << " || " << cur1->info.suit << endl;
		cur1 = cur1->next;
	}
}


void Delete_deck(Stack** beg) {     // очистка стека
	Stack *buf;
	while (*beg != NULL) {
		buf = *beg;
		*beg = (*beg)->next;
		delete buf;
	}
}

Stack* deleteFromDeck(Stack* beg, int num) {          // удаление одного элемента стека
	Stack *cur = beg;
	int f = 0;

	int count = 0;
	while (cur != NULL) {    // узнаем длину стека
		count++;
		cur = cur->next;
	}

	if (count == 1) {      // если один элемент - возвращаем нуль
		cur = NULL;
		return NULL;
	}

	cur = beg;
	while (cur->next != NULL) {   // удаляем из стека
		if (cur->numberInStack == num && cur->next->next == NULL) {     // если последний элемент
			cur->next = NULL;
			return beg;
		}
		if (cur->numberInStack == num && cur == beg) {         // если первый
			return beg->next;
		}
 		if (cur->next->numberInStack == num) {       // любой другой
			cur->next = cur->next->next;
			return beg;
		}
		cur = cur->next;
	}
	return NULL;
}

Card Extract(Stack* beg, int key) {       // извлечение карты из рукава на стол
	Card buf;
	Stack *bufStack = beg;

	
	if (beg == NULL) {
		return {0, "0", "0"};
	}

	bufStack = beg;

	while (bufStack != NULL) {
		if (bufStack->numberInStack == key) {
			buf = bufStack->info;
			break;
		}
		bufStack = bufStack->next;
	}
	return buf;
}

Card MothionBot(Stack* botDeck, Stack* table) {       // защита бота
	Stack *cur = botDeck;
	Card buf;

		while (cur != NULL) {
			if (cur->info.number > table->info.number && cur->info.suit == table->info.suit) { // если номер карты больше и масть совпадает - возвращаем эту карту
				buf = cur->info;
				bot = deleteFromDeck(bot, cur->numberInStack);
				return buf;
			}
			cur = cur->next;
		}

		bool f = false;  // проверка является ли последняя карта на столе козырем
		if (table->info.suit == trumpCardStr) {
			f = true;
		}
		cur = botDeck;
		while (cur != NULL) {
			if (f == true) {   // если это козырь сравниваем номер карты из рукава и номер карты на столе
				if (cur->info.suit == trumpCardStr && cur->info.number > table->info.number) {
					buf = cur->info;
					bot = deleteFromDeck(bot, cur->numberInStack);
					return buf;
				}
			}      
			if (f == false) {  // если последняя карта на столе не козырь, просто отбиваемся козырем из рукава
				if (cur->info.suit == trumpCardStr) {
					buf = cur->info;
					bot = deleteFromDeck(bot, cur->numberInStack);
					return buf;
				}
			}
			cur = cur->next;
		}
		buf = { -1, " 2 ", "2" };   // если нет подходящих карт чтобы отбить - возвращаем "пустое" значение
		return buf;
}

Card MothionBotAttack(Stack *botDeck, Stack *table) {     // атака бота
	Stack *botF = botDeck, *tableF = table;
	Card buf;
	while (tableF != NULL) {  // двойной цикл чтобы сверить каждую карту со стола с каждой картой из рукава
		botF = botDeck;
		while (botF != NULL) {
			if (tableF->info.number == botF->info.number) {   //  если номера карт в рукаве и на столе совпадают - подкидываем эту карту
				buf = botF->info;
				bot = deleteFromDeck(bot, botF->numberInStack);
				return buf;
			}
			botF = botF->next;
		}
		tableF = tableF->next;
	}

	buf = { -1, "-1", "-1" };   // если нет подходящих карт чтобы подкинуть - возвращаем "пустое" значение
	return buf;
}

int Quantity(Stack* beg) {   // вычисление длины стека (карт в рукаве)
	if (beg == NULL) {
		return 0;
	}
	Stack *cur = beg;
	int a = 0;
	while (cur != NULL) {
		a++;
		cur = cur->next;
	}
	return a;
}

int main() {
	setlocale(LC_ALL, "Russian");

	// привязка команды rand к часам на компе (чтобы не было псевдослучайных чисел)
	long stime;
	long ltime;
	ltime = time(NULL);
	stime = (unsigned int)ltime / 2;
	srand(stime);




	Card deck[36];    // массив колоды
	deck[0].number = 6;
	deck[0].suit = "chirva";
	deck[0].name = "six";

	deck[1].number = 6;
	deck[1].suit = "creste";
	deck[1].name = "six";

	deck[2].number = 6;
	deck[2].suit = "buba";
	deck[2].name = "six";

	deck[3].number = 6;
	deck[3].suit = "pika";
	deck[3].name = "six";

	deck[4].number = 7;
	deck[4].suit = "chirva";
	deck[4].name = "seven";

	deck[5].number = 7;
	deck[5].suit = "creste";
	deck[5].name = "seven";

	deck[6].number = 7;
	deck[6].suit = "buba";
	deck[6].name = "seven";

	deck[7].number = 7;
	deck[7].suit = "pika";
	deck[7].name = "seven";

	deck[8].number = 8;
	deck[8].suit = "chirva";
	deck[8].name = "eight";

	deck[9].number = 8;
	deck[9].suit = "creste";
	deck[9].name = "eight";

	deck[10].number = 8;
	deck[10].suit = "buba";
	deck[10].name = "eight";

	deck[11].number = 8;
	deck[11].suit = "pika";
	deck[11].name = "eight";

	deck[12].number = 9;
	deck[12].suit = "chirva";
	deck[12].name = "nine";

	deck[13].number = 9;
	deck[13].suit = "creste";
	deck[13].name = "nine";

	deck[14].number = 9;
	deck[14].suit = "buba";
	deck[14].name = "nine";

	deck[15].number = 9;
	deck[15].suit = "pika";
	deck[15].name = "nine";

	deck[16].number = 10;
	deck[16].suit = "chirva";
	deck[16].name = "ten";

	deck[17].number = 10;
	deck[17].suit = "creste";
	deck[17].name = "ten";

	deck[18].number = 10;
	deck[18].suit = "buba";
	deck[18].name = "ten";

	deck[19].number = 10;
	deck[19].suit = "pika";
	deck[19].name = "ten";

	deck[20].number = 11;
	deck[20].suit = "chirva";
	deck[20].name = "Jack";

	deck[21].number = 11;
	deck[21].suit = "creste";
	deck[21].name = "Jack";

	deck[22].number = 11;
	deck[22].suit = "buba";
	deck[22].name = "Jack";

	deck[23].number = 11;
	deck[23].suit = "pika";
	deck[23].name = "Jack";

	deck[24].number = 12;
	deck[24].suit = "chirva";
	deck[24].name = "Quenn";

	deck[25].number = 12;
	deck[25].suit = "creste";
	deck[25].name = "Quenn";

	deck[26].number = 12;
	deck[26].suit = "buba";
	deck[26].name = "Quenn";

	deck[27].number = 12;
	deck[27].suit = "pika";
	deck[27].name = "Quenn";

	deck[28].number = 13;
	deck[28].suit = "chirva";
	deck[28].name = "King";

	deck[29].number = 13;
	deck[29].suit = "creste";
	deck[29].name = "King";

	deck[30].number = 13;
	deck[30].suit = "buba";
	deck[30].name = "King";

	deck[31].number = 13;
	deck[31].suit = "pika";
	deck[31].name = "King";

	deck[32].number = 14;
	deck[32].suit = "chirva";
	deck[32].name = "Tuz";

	deck[33].number = 14;
	deck[33].suit = "creste";
	deck[33].name = "Tuz";

	deck[34].number = 14;
	deck[34].suit = "buba";
	deck[34].name = "Tuz";

	deck[35].number = 14;
	deck[35].suit = "pika";
	deck[35].name = "Tuz";
	int size = 35;


	bool motion = true;   // булевое значение кто ходит



	for (int i = 0; i < 6; i++) {     // заполнение колоды пользователя
		int random = rand() % size;
		user = Create(user, deck[random], i + 1);   // i+1 - чтобы знать номер карты в стеке
		size = Delete(deck, random, size);
	}

	for (int i = 0; i < 6; i++) {   // заполнение колоды бота
		int random = rand() % size;   // берём рандомное число до размера изначальной колоды
		bot = Create(bot, deck[random], i + 1);     // создаём элемент стека передавая в инвормационную часть рандомный элемент массива
		size = Delete(deck, random, size);        // уменьшаем размер массива
	}

	int trumpCard = rand() % size;    // определение козыря
	trumpCardStr = deck[trumpCard].suit;




	// блок отвечающий за то, кто первый ходит (определяем минимальный номер в колоде у пользователя и бота и сравниваем их)
	Stack *buf1 = bot, *buf2 = user;   
	int minBot = buf1->info.number, minUser = buf2->info.number;
	for (int i = 0; i < 6; i++) {
		while (buf1 != NULL) {
			if (buf1->info.suit == trumpCardStr && buf1->info.number < minBot) {  
				minBot = buf1->info.number;
			}
			if (buf2->info.suit == trumpCardStr && buf2->info.number < minBot) {
				minUser = buf2->info.number;
			}
			buf1 = buf1->next;
			buf2 = buf2->next;
		}
	}
	if (minBot < minUser) {
		cout << "Первый ходит бот" << endl;
		motion = false;
	}
	if (minBot > minUser) {
		cout << "Первым ходите вы" << endl;
		motion = true;
	}
	Sleep(2000);
	



	bool botAttack = true;   // переменная атаки бота (если значение false - защита)
	bool userAttack = true;  // переменная атаки пользователя
	int buferCardInt;   // буфер номера карты в рукаве
	Card buferCard;   // буфер карты
	bool bito = false;   // переменная бито
	int  y = 0; // у - для заполнения номеров карт в рукаве по возрастанию 
	int quantityUser, quantityBot;  // количество карт бота и пользователя в рукаве
	Stack* pointer;
	while (user != NULL && bot != NULL)
	{
		d:
		while (bito == false) {
		g:
			system("cls");
			cout << "Осталось " << size << " карт" << endl;
			cout << "Козырь - " << trumpCardStr << endl;
			cout << "-----------------" << endl;

			if (table != NULL){
				cout << "Карты на столе:" << endl;
				View(table);
				cout << "-----------------" << endl;

		}
			if (motion == true) {  // ход пользователя
				motion = false;  // следующий ход бота

				if (table != NULL) {   // если карты на столе уже есть
					if (userAttack == true) {    // если пользователь атакует
						e:
						cout << "Вы ходите:" << endl << endl;
						View(user);
						cout << "Или нажмите '0' для бито ";
						buferCardInt = _getch();
						if (buferCardInt == 48) {
							cout << endl << "Бито" << endl;
							Sleep(2000);
							botAttack = true;  // если бито - следующей будет атака бота
							userAttack = false;  // а пользователь будет защищаться
							bito = true;  // завершаем раунд
							goto d;
						}
						
						// переводим из номеров клавиш клавиатуры в простые числа
						if (buferCardInt == 49) buferCardInt = 1;
						if (buferCardInt == 50) buferCardInt = 2;
						if (buferCardInt == 51) buferCardInt = 3;
						if (buferCardInt == 52) buferCardInt = 4;
						if (buferCardInt == 53) buferCardInt = 5;
						if (buferCardInt == 54) buferCardInt = 6;
						if (buferCardInt == 55) buferCardInt = 7;
						if (buferCardInt == 56) buferCardInt = 8;
						if (buferCardInt == 57) buferCardInt = 9;
						
						buferCard = Extract(user, buferCardInt);   // извлекаем карту из рукава
						Stack *tableB = table;
						while (tableB != NULL) {
							if (tableB->info.number == buferCard.number) {   // если номер желаемой для подкидки карты совпадает с номером карты на столе
								user = deleteFromDeck(user, buferCardInt);   // удаляем её из рукава
								table = Create(table, buferCard, 0);   // добавляем её на стол
								botAttack = false;   // бот будет отбиваться
								goto d;
							}
							tableB = tableB->next;
						}
							cout << endl << "Карта не подходит" << endl;
							goto e;
					}
					else {   // пользователь отбивается
					b:
						cout << "Вы отбиваетесь:" << endl << endl;
						View(user);
						cout << "Или нажмите '0' чтобы загрести ";
						buferCardInt = _getch();
	

						if (buferCardInt == 48) {
							cout << endl << "Беру" << endl;
							while (table != NULL) {
								user = Create(user, table->info, 1);
								table = table->next;
							}
							Sleep(2000);
							userAttack = false;
							botAttack = true;
							bito = true;
							goto d;
						}

						if (buferCardInt == 49) buferCardInt = 1;
						if (buferCardInt == 50) buferCardInt = 2;
						if (buferCardInt == 51) buferCardInt = 3;
						if (buferCardInt == 52) buferCardInt = 4;
						if (buferCardInt == 53) buferCardInt = 5;
						if (buferCardInt == 54) buferCardInt = 6;
						if (buferCardInt == 55) buferCardInt = 7;
						if (buferCardInt == 56) buferCardInt = 8;
						if (buferCardInt == 57) buferCardInt = 9;
					

						buferCard = Extract(user, buferCardInt);
						if (buferCard.number == 0) {
							cout << "Ваши карты закончились!" << endl;
							Sleep(2000);
							bito = true;
							goto d;
						}
						if (table != NULL) {
							if ((table->info.number < buferCard.number && table->info.suit == buferCard.suit) || buferCard.suit == trumpCardStr) {
								user = deleteFromDeck(user, buferCardInt);
								table = Create(table, buferCard, 0);
								botAttack = true;
								goto g;
							}
							else {
								cout << "Карта не подходит" << endl;
								goto b;
							}
						}
					}
				}
				if (table == NULL && userAttack == true) {   // елси пользователь ходит первый
					cout << "Вы ходите:" << endl << endl;
					View(user);

					buferCardInt = _getch();
					if (buferCardInt == 49) buferCardInt = 1;
					if (buferCardInt == 50) buferCardInt = 2;
					if (buferCardInt == 51) buferCardInt = 3;
					if (buferCardInt == 52) buferCardInt = 4;
					if (buferCardInt == 53) buferCardInt = 5;
					if (buferCardInt == 54) buferCardInt = 6;
					if (buferCardInt == 55) buferCardInt = 7;
					if (buferCardInt == 56) buferCardInt = 8;
					if (buferCardInt == 57) buferCardInt = 9;
			

					buferCard = Extract(user, buferCardInt);
					user = deleteFromDeck(user, buferCardInt);
					table = Create(table, buferCard, 0);
					botAttack = false;
					goto g;
				}
				
			}
			if (motion == false) {
				motion = true;

				if (table != NULL) {
					if (botAttack == false) {
						cout << "Бот отбивается:" << endl;
						Sleep(1000);
						buferCard = MothionBot(bot, table);   // функция принимает карты в рукаве бота и последнюю карту со стола
						if (buferCard.number == -1) {  // если возвращается -1 - бот загребает
							while (table != NULL) {   // чтобы забрать все карты на столе
								bot = Create(bot, table->info, 1);
								table = table->next;
							}
							cout << "Беру!" << endl;
							bito = true;
							botAttack = false;
							userAttack = true;
							Sleep(1000);
							goto d;
						}
						else {   // отбивается
							table = Create(table, buferCard, 0);   // добавляем карту бота на стол
							botAttack = false;
							userAttack = true;
							goto d;
						}
					}
					if (botAttack == true) {
						cout << "Ходит бот:" << endl;
						Sleep(1000);
						buferCard = MothionBotAttack(bot, table);   // в функцию передаются карты в рукаве и карты со стола
						if (buferCard.number == -1) {  // если возвращает -1 - бито
							cout << "Бот: бито" << endl;
							Sleep(2000);
							bito = true;
							botAttack = false;
							userAttack = true;
							goto d;
						}
						else {   // иначе подкидывает карту на стол
							table = Create(table, buferCard, 0); 
							userAttack = false;
							botAttack = true;
							goto d;
						}
					}
				}
				if (table == NULL) {   // если бот ходит первый

					cout << "Ходит бот:" << endl;
					Sleep(1000);
					pointer = bot;

					while (pointer != NULL) {    // узнаем длину стека
						y++;
						pointer = pointer->next;
					}
					pointer = bot;
					int random = rand() % y + 1;   // берём рандомное число от 1 до размера колоды у бота
					y = 0;
					buferCardInt = random;
					buferCard = Extract(bot, buferCardInt);   // рандомная карта
					table = Create(table, buferCard, 0);   // кидаем её на стол
					bot = deleteFromDeck(bot, random);    // удаляем её из колоды бота
					botAttack = true;
					userAttack = false;
				}
			}
			system("cls");
		}
		cout << "Раунд завершён" << endl;
		Delete_deck(&table);
		quantityUser = Quantity(user);
		quantityBot = Quantity(bot);


		if (quantityUser < 6 && size > 0) {          // дополняем недостающие карты
			for (int i = 0; i < 6 - quantityUser; i++) {
				if (size != 0) {
					int random = rand() % size;
					user = Create(user, deck[random], i + 1);
					size = Delete(deck, random, size);
				}
			}
		}
		pointer = user;
		int i = 0;        
		while (pointer != NULL) {           // нумеруем элементы в стеке по очереди
			pointer->numberInStack = i + 1;
			pointer = pointer->next;
			i++;
		}

		if (quantityBot < 6 && size > 0) {    // дополняем недостающие карты
			for (int i = 0; i < 6 - quantityBot; i++) {
				int random = rand() % size;
				bot = Create(bot, deck[random], i + 1);
				size = Delete(deck, random, size);
			}
		}
		pointer = bot;
	    i = 0; 
		while (pointer != NULL) {           // нумеруем элементы в стеке по очереди
			pointer->numberInStack = i + 1;
			pointer = pointer->next;
			i++;
		}
		bito = false;
		system("cls");
	}

	if (quantityUser == 0 && size == 0) {
		cout << "Вы победили!" << endl;
	}
	if (quantityBot == 0 && size == 0) {
		cout << "Бот победил!" << endl;
	}
	Sleep(3000);
	Delete_deck(&user);
	Delete_deck(&bot);
	Delete_deck(&table);
	return 0;
}
