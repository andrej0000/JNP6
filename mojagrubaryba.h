#ifndef _MOJAGRUBARYBA_H_
#define _MOJAGRUBARYBA_H_
#include <string>
#include <vector>
#include <exception>

#include "grubaryba.h"

using std::shared_ptr;
using std::exception;
using std::weak_ptr;

class Player;
class Board;
class Field;
class PropertyField;



class MojaGrubaRyba : public GrubaRyba
{
public:
	void play(unsigned int rounds);
	void setDie(std::shared_ptr<Die> die);
	void addComputerPlayer(ComputerLevel level);
	void addHumanPlayer(std::shared_ptr<Human> human);


	void bankrupcy(shared_ptr< Player > p); //plejer zglasza bankructwo
	MojaGrubaRyba();
private:
	static const int minPlayers = 2;
	static const int maxPlayers = 8;
	static const int fishcoinsOnStart = 1000;

	std::vector < std::shared_ptr < Player > > players;
	std::shared_ptr < Die > die;
	std::unique_ptr < Board > board;

	unsigned playersInGame;
	void initBoard();
};

class Player
{
private:
	int pos;
	int fishcoins;
	int fishcoinsOnStart;
	unsigned int waitingTime;
	std::vector< weak_ptr< PropertyField > > ownedProperties;
	bool bankrupt;
	std::string name;
	void bankrupcy();
public:
	Player(const std::string&& name, int fishcoins);
	int pay(int ammount);
	void addFishcoins(int ammount);
	void setFishcoins(unsigned int ammount);
	int getFishcoins();
	void setWaitingTime(unsigned int wt);
	unsigned int getWaitingTime();
	int getPos();
	void setPos(int newPos);
	bool inGame();
	std::string getName();
	void reset();
	void addProperty(weak_ptr< PropertyField > property);
	virtual bool wantBuy(std::string const& propertyName) = 0;
	virtual bool wantSell(std::string const& propertyName) = 0;
};

class deletedPlayerException : public exception
{
	virtual const char* what() const throw(){
		return "Player deleted prematurly";
	}
};

class HumanPlayer : public Player
{
	std::shared_ptr < Human > humanInterface;
public:
	HumanPlayer(std::shared_ptr<Human> hInterface, unsigned int fc);
	virtual bool wantBuy(std::string const& propertyName);
	virtual bool wantSell(std::string const& propertyName);
};

class DumbComputerPlayer : public Player
{
public:
	DumbComputerPlayer(const std::string cname, unsigned int fc);
	virtual bool wantBuy(std::string const& propertyName);
	virtual bool wantSell(std::string const& propertyName);
};

class SmartassComputerPlayer : public Player
{
public:
	SmartassComputerPlayer(const std::string cname, unsigned int fc);
	virtual bool wantBuy(std::string const& propertyName);
	virtual bool wantSell(std::string const& propertyName);
};

class Board
{
public:
	Board(std::vector < std::shared_ptr <Field > > &&_fields);
	Board();
	int getSize();
	std::shared_ptr < Field >  field(int pos);
	std::vector < std::shared_ptr < Field > > fields; // TODO: to powinno byc private i uzupelniane w konstruktorze!!!
private:
};

class Field
{
public:
	Field(const std::string &&name);
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
	virtual void action(shared_ptr<Player> p);
	virtual void pass(shared_ptr<Player> p);
private:
	int cash;
};

class IslandField : public Field
{
public:
	IslandField();
	virtual void action(shared_ptr<Player> p);
	virtual void pass(shared_ptr<Player> p);
};

class PrizeField : public Field
{
public:
	PrizeField(std::string &&name, int ammount);
	virtual void action(shared_ptr<Player> p);
	virtual void pass(shared_ptr<Player> p);
private:
	int ammount;
};

class FineField : public Field
{
public:
	FineField(std::string &&name, int ammount);
	virtual void action(shared_ptr<Player> p);
	virtual void pass(shared_ptr<Player> p);
private:
	int fishcoins;
	int ammount;
};

class DepositField : public Field
{
public:
	DepositField(std::string &&name, int ammount);
	virtual void action(shared_ptr<Player> p);
	virtual void pass(shared_ptr<Player> p);
private:
	const int ammount;
	int fishcoins;
};

class AquariumField : public Field
{
public:
	AquariumField(int turns);
	virtual void action(shared_ptr<Player> p);
	virtual void pass(shared_ptr<Player> p);
private:
	const int turnsToWait;
};

class PropertyField : public Field
{
public:
	PropertyField(std::string &&name, int price, int percent);
	virtual void action(shared_ptr< Player > p);
	virtual void pass(shared_ptr< Player > p);
	void sold();
	int getPrice();
	virtual int getPercent();
private:
	std::weak_ptr < Player > owner;
	int price;
	int percent;
};
class PublicPropertyField : public PropertyField
{
public:
	PublicPropertyField(std::string &&name, int price);
};
class CoralPropertyField : public PropertyField
{
public:
	CoralPropertyField(std::string &&name, int price);
};

#endif
