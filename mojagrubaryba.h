#ifndef _MOJAGRUBARYBA_H_
#define _MOJAGRUBARYBA_H_

#include "grubaryba.h"
#include <string>
#include <vector>

class Player;
class Board;
class Field;

class MojaGrubaRyba : public GrubaRyba
{
private:
	static const int minPlayers = 2;
	static const int maxPlayers = 8;
	
	std::vector < Player > players;
	std::unique_ptr < Die > die;
	std::unique_ptr < Board > board;
};

class Player
{
private:
	int pos;
	int fishcoins;
};

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
private:
	std::vector < std::unique_ptr < Field > > fields;
};

class Field
{
	virtual void action() = 0;
	virtual void pass() = 0;
protected:
	const std::string name;
};

class StartField : public Field
{
};

class PrizeField : public Field
{
private:
	int ammount;
};

class FineField : public Field
{
private:
	int ammount;
};

class DepositField : public Field
{
private:
	const int ammount;
	int fishcoins;
};

class Aquarium : public Field
{
private:
	const int turnsToWait;
};


class PropertyField : public Field
{
private:
	std::weak_ptr < Player > owner;
	const std::string name;
};

#endif
