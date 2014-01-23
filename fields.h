#ifndef _FIELDS_H
#define _FIELDS_H

#include <string>
using std::shared_ptr;

class Player;
class Board;


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
