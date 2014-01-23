#ifndef _MOJAGRUBARYBA_H_
#define _MOJAGRUBARYBA_H_
#include <string>
#include <vector>
#include <exception>

#include "grubaryba.h"
#include "fields.h"

using std::shared_ptr;
using std::exception;
using std::weak_ptr;

class Player;
class Board;



class MojaGrubaRyba : public GrubaRyba
{
public:
	void bankrupcy(shared_ptr< Player > p); //plejer zglasza bankructwo
private:
	static const int minPlayers = 2;
	static const int maxPlayers = 8;

	std::vector < Player > players;
	std::unique_ptr < Die > die;
	std::unique_ptr < Board > board;

	unsigned playersInGame;
};

class Player
{
private:
	int pos;
	int fishcoins;
	int waitingTime;
	std::vector< PropertyField > ownedProperties;
	bool bankrupt;
	std::string name;
public:
	int pay(int ammount); //bankrutuje jesli nie ma kasy i cale konekwencje z tego wynikajace//TODO
	void addFishcoins(int ammount);//TODO
	void setWaitingTime(int wt);//TODO
	int getWaitingTime();//TODO
	int getPos();
	void setPos(int newPos);
	bool inGame(); // czy juz zbankrutowalismy czy jeszcze gramy//TODO
	std::string getName();//TODO
	virtual bool wantBuy(std::string const& propertyName) = 0;
	virtual bool wantSell(std::string const& propertyName) = 0;
};

class deletedPlayerException : public Exception
{
	virtual const char* what() const throw(){
		return "Player deleted prematurly";
	}
} deletedPlaterEx;

class HumanPlayer : public Player
{
};

class DumbComputerPlayer : public Player
{
};

class SmartassComputerPlayer : public Player
{
};

class Board
{
	Board(std::vector < Field > &&_fields);
	Board();
	int getSize();
private:
	std::vector < std::unique_ptr < Field > > fields;
};
#endif
