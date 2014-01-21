#ifndef _MOJAGRUBARYBA_H_
#define _MOJAGRUBARYBA_H_

#include "grubaryba.h"
#include <string>
#include <vector.

class Player;
class Board;
class Field;

class MojaGrubaRyba : public GrubaRyba
{
private:
	static const int minPlayers = 2;
	static const int maxPlayers = 8;
	
	std::vector < Player > players;
	std::shared_ptr < Die > die;
	std::shared_ptr < Board > board;
}

class Player
{
}

class HumanPlayer : public Player
{
}

class DumbComputerPlayer : public Player
{
}

class SmartassComputerPlayer : public Player
{
}

class Board
{
}

class Field
{
}

class Property : public Field
{
private:
	const std::string name;
}

#endif