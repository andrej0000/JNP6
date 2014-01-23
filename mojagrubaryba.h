#ifndef _MOJAGRUBARYBA_H_
#define _MOJAGRUBARYBA_H_

#include "grubaryba.h"
#include <string>
#include <vector>
#include <exception>
using std::shared_ptr;
using std::exception;
using std::weak_ptr;

class Player;
class Board;
class Field;



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
	int pay(int ammount); //bankrutuje jesli nie ma kasy i cale konekwencje z tego wynikajace
	void addFishcoins(int ammount);
	void setWaitingTime(int wt);
	int getWaitingTime();
	int getPos();
	void setPos(int newPos);
	bool inGame(); // czy juz zbankrutowalismy czy jeszcze gramy
	std::string getName();
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
//DONE
class Field
{
public:
	virtual void action(shared_ptr<Player> p) = 0;
	virtual void pass(shared_ptr<Player> p) = 0;
	const std::string getName();
protected:
	const std::string name;
};

class StartField : public Field
{
public:
	StartField(int cash);
	virtual void action(shared_ptr<Player> p) = 0;
	virtual void pass(shared_ptr<Player> p) = 0;
private:
	int cash;
};

class IslandField : public Field
{
public:
	IslandField()
	virtual void action(shared_ptr<Player> p);
	virtual void pass(shared_ptr<Player> p);
}

class PrizeField : public Field
{
	PrizeField(std::string name, int ammount);
	virtual void action(shared_ptr<Player> p) = 0;
	virtual void pass(shared_ptr<Player> p) = 0;
private:
	int ammount;
};
class FineField : public Field
{
	FineField(std::string name. int ammount);
	virtual void action(shared_ptr<Player> p) = 0;
	virtual void pass(shared_ptr<Player> p) = 0;
private:
	int ammount;
};

class DepositField : public Field
{
	DepositField(std::string name, int ammount);
	virtual void action(shared_ptr<Player> p) = 0;
	virtual void pass(shared_ptr<Player> p) = 0;
private:
	const int ammount;
	int fishcoins;
};

class AquariumField : public Field
{
	AquariumField(int turns);
	virtual void action(shared_ptr<Player> p) = 0;
	virtual void pass(shared_ptr<Player> p) = 0;
private:
	const int turnsToWait;
};

class PropertyField : public Field
{
public:
	PropertyField(std::string name, int price, int percent);
	virtual void action(shared_ptr< Player > p) = 0;
	virtual void pass(shared_ptr< Player > p) = 0;
	void sold();
	int getPrice();
private:
	std::weak_ptr < Player > owner;
	const std::string name;
	int price;
	int percent;
	virtual int getPercent();
};
class PublicPropertyField : public PropertyField
{
public:
	PublicPropertyField(std::string name, int price);
};
class CoralPropertyField : public PropertyField
{
public:
	CoralPropertyField(std::string name, int price);
};
#endif
