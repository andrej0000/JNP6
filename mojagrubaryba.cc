#include "mojagrubaryba.h"
#include <iostream>

/* MojaGrubaRyba */

void MojaGrubaRyba::setDie(std::shared_ptr<Die> die)
{
	this->die = die;
}

void MojaGrubaRyba::play(unsigned int rounds)
{
	playersInGame = players.size();
	for(int r = 1; r <= rounds && playersInGame > 1; r++)
	{
		std::cout << "Runda " << r << '\n';
		for(auto player : players)
		{
			std::cout << player->getName();
			if(player->inGame() == false)
			{
				std::cout << " *** bankrut ***\n";
				continue;
			}
			if(player->getWaitingTime() == 0)
			{
				int moves = 0;
				moves += die->roll();
				moves += die->roll();
				for(int m = 1; m < moves; m++)
				{
					int fieldNum = (player->getPos() + m) % board->getSize();
					board->field(fieldNum)->pass(player);
				}
				int newPos = (player->getPos() + moves) % board->getSize();
				player->setPos(newPos);
				board->field(newPos)->action(player);
				std::cout << " pole: " << board->field(newPos)->getName() << " gotowka: " << player->getFishcoins() << '\n';
			}
			else
			{
				std::cout << " pole: " << board->field(player->getPos())->getName() << " *** czekanie: " << player->getWaitingTime() << " ***\n";
				player->setWaitingTime(player->getWaitingTime()-1);
			}
		}
	}
}

MojaGrubaRyba::MojaGrubaRyba()
{
	//TODO chyba trzeba przerobic na factory jakies 
	std::vector < std::shared_ptr < Field > > fields;
	fields.push_back(std::shared_ptr<Field>(new StartField(50)));
	fields.push_back(std::shared_ptr<Field>(new PropertyField("Anemonia", 160, 20)));
	fields.push_back(std::shared_ptr<Field>(new IslandField));
	fields.push_back(std::shared_ptr<Field>(new PropertyField("Aporina", 220, 20)));
	fields.push_back(std::shared_ptr<Field>(new AquariumField(3)));
	fields.push_back(std::shared_ptr<Field>(new PropertyField("Hotel w grocie", 300, 40)));
	fields.push_back(std::shared_ptr<Field>(new PropertyField("Menella", 280, 20)));
	fields.push_back(std::shared_ptr<Field>(new DepositField("Laguna", 15)));
	fields.push_back(std::shared_ptr<Field>(new PropertyField("Statek", 250, 40)));
	fields.push_back(std::shared_ptr<Field>(new PrizeField("Blazenki", 120)));
	fields.push_back(std::shared_ptr<Field>(new PropertyField("Pennatula", 400, 20)));
	fields.push_back(std::shared_ptr<Field>(new FineField("Rekin", 180)));
	board->fields = std::move(fields);
}

void MojaGrubaRyba::addComputerPlayer(ComputerLevel level)
{
	std::string name = "Gracz"+std::to_string(board->getSize()+1);
	switch(level)
	{
		case ComputerLevel::DUMB:
			players.push_back(std::shared_ptr<Player>(new DumbComputerPlayer(name)));
			break;
		case ComputerLevel::SMARTASS:
			players.push_back(std::shared_ptr<Player>(new SmartassComputerPlayer(name)));
			break;
	}
}

void MojaGrubaRyba::addHumanPlayer(std::shared_ptr<Human> human)
{
	players.push_back(std::shared_ptr<Player>(new HumanPlayer(human)));
}

/* Player */

Player::Player(const std::string &&cname) : name(std::move(cname))
{
}

int Player::getPos()
{
	return pos;
}

void Player::setPos(int newPos)
{
	pos = newPos;
}

int Player::pay(int ammount)
{
	if (this->fishcoins >= ammount){
		fishcoins -= ammount;
		return ammount;
	}
	for (auto wprop : this->ownedProperties){
		if (auto prop = wprop.lock()){
			if (wantSell(prop->getName())){
				fishcoins += prop->getPrice();
				prop->sold();
			}
		}
	}
	if (this->fishcoins >= ammount){
		fishcoins -= ammount;
		return ammount;
	}
	else {
		this->bankrupcy();
		return fishcoins;
	}
}

void Player::bankrupcy()
{
	bankrupt = true;
}

bool Player::inGame()
{
	return !this->bankrupt;
}

void Player::addFishcoins(int ammount)
{
	this->fishcoins += ammount;
}

int Player::getFishcoins()
{
	return this->fishcoins;
}

void Player::setWaitingTime(unsigned int wt)
{
	this->waitingTime = wt;
}

unsigned int Player::getWaitingTime()
{
	return this->waitingTime;
}
std::string Player::getName()
{
	return this->name;
}

DumbComputerPlayer::DumbComputerPlayer(const std::string cname) : Player(std::move(cname))
{
}

bool DumbComputerPlayer::wantBuy(std::string const& propertyName)
{
	static int it = 1;
	if(it == 3)
	{
		it = 1;
		return true;
	}
	it++;
	return false;
}

bool DumbComputerPlayer::wantSell(std::string const& propertyName)
{
	return false;
}

SmartassComputerPlayer::SmartassComputerPlayer(const std::string cname) : Player(std::move(cname))
{
}

bool SmartassComputerPlayer::wantBuy(std::string const& propertyName)
{
	return true;
}

bool SmartassComputerPlayer::wantSell(std::string const& propertyName)
{
	return false;
}

HumanPlayer::HumanPlayer(std::shared_ptr<Human> hInterface) : Player(std::move(hInterface->getName())), humanInterface(hInterface)
{
}

bool HumanPlayer::wantBuy(std::string const& propertyName)
{
	return humanInterface->wantBuy(propertyName);
}

bool HumanPlayer::wantSell(std::string const& propertyName)
{
	return humanInterface->wantSell(propertyName);
}

/* Board */

int Board::getSize()
{
	return fields.size();
}

std::shared_ptr < Field > Board::field(int pos)
{
	return fields[pos];
}

/* Fields */

Field::Field(const std::string &&cname) : name(std::move(cname))
{
}

const std::string Field::getName()
{
	return this->name;
}

StartField::StartField(int cash) : cash(cash), Field("Start")
{
}

void StartField::action(std::shared_ptr< Player > p)
{
	p->addFishcoins(this->cash);
}

void StartField::pass(std::shared_ptr< Player > p)
{
	p->addFishcoins(this->cash);
}

PrizeField::PrizeField(std::string &&name, int ammount) : Field(std::move(name)), ammount(ammount)
{

}

void PrizeField::action(std::shared_ptr<Player> p)
{
	p->addFishcoins(this->ammount);
}

void PrizeField::pass(std::shared_ptr<Player> p)
{
	//nothing
}


IslandField::IslandField() : Field("Wyspa")
{

}


void IslandField::action(std::shared_ptr<Player> p)
{
	//nothing
}

void IslandField::pass(std::shared_ptr<Player> p)
{
	//nothing
}


PropertyField::PropertyField(std::string &&name, int price, int percent)
	: Field(std::move(name)), price(price), percent(percent)
{

}

int PropertyField::getPercent()
{
	return percent;
}

void PropertyField::action(std::shared_ptr<Player> p)
{
	if (this->owner.expired()){//noninitialized
		if(p->wantBuy(this->name)){
			this->owner = p;
		}
	} else if (auto owner = this->owner.lock()){
		if (owner != p) {
			int fare = (this->getPercent() * this->price) / 100;
			p->pay(fare);
			owner->addFishcoins(fare);
		}
	}
	else {
		throw deletedPlayerException();//wysraj wyjatek
	}
}

void PropertyField::pass(std::shared_ptr<Player> p)
{

}

int PropertyField::getPrice(){
	return this->price;
}

void PropertyField::sold(){
	this->owner.reset();
}

PublicPropertyField::PublicPropertyField(std::string &&name, int price)
	: PropertyField(std::move(name)	, price, 40)
{

}

CoralPropertyField::CoralPropertyField(std::string &&name, int price)
	: PropertyField(std::move(name), price, 20)
{

}

DepositField::DepositField(std::string &&name, int ammount) : Field(std::move(name)), ammount(ammount)
{
	
}
void DepositField::action(std::shared_ptr<Player> p)
{
	p->addFishcoins(this->fishcoins);
	this->fishcoins = 0;
}

void DepositField::pass(std::shared_ptr<Player> p)
{
	this->fishcoins += p->pay(this->ammount);
}

FineField::FineField(std::string &&name, int ammount) : Field(std::move(name)), ammount(ammount), fishcoins(0)
{

}
void FineField::action(std::shared_ptr<Player> p)
{
	this->fishcoins += p->pay(this->ammount);
}

void FineField::pass(std::shared_ptr<Player> p)
{

}

AquariumField::AquariumField(int turns) : Field("Akwarium"), turnsToWait(turns)
{

}
void AquariumField::action(std::shared_ptr<Player> p)
{
	p->setWaitingTime(this->turnsToWait);
}

void AquariumField::pass(std::shared_ptr<Player> p)
{

}
