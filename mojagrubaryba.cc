#include "mojagrubaryba.h"
#include <iostream>

/* MojaGrubaRyba */
//static bool debug = false;
void MojaGrubaRyba::setDie(std::shared_ptr<Die> die)
{
	this->die = die;
}

void MojaGrubaRyba::initBoard()
{

	std::vector< std::shared_ptr< Field > > fields;

	fields.push_back(std::shared_ptr<Field>(new StartField(50)));
	fields.push_back(std::shared_ptr<Field>(new CoralPropertyField("Anemonia", 160)));
	fields.push_back(std::shared_ptr<Field>(new IslandField));
	fields.push_back(std::shared_ptr<Field>(new CoralPropertyField("Aporina", 220)));
	fields.push_back(std::shared_ptr<Field>(new AquariumField(3)));
	fields.push_back(std::shared_ptr<Field>(new PublicPropertyField("Grota", 300)));
	fields.push_back(std::shared_ptr<Field>(new CoralPropertyField("Menella", 280)));
	fields.push_back(std::shared_ptr<Field>(new DepositField("Laguna", 15)));
	fields.push_back(std::shared_ptr<Field>(new PublicPropertyField("Statek", 250)));
	fields.push_back(std::shared_ptr<Field>(new PrizeField("Blazenki", 120)));
	fields.push_back(std::shared_ptr<Field>(new CoralPropertyField("Pennatula", 400)));
	fields.push_back(std::shared_ptr<Field>(new FineField("Rekin", 180)));
	board.reset(new Board(std::move(fields)));
}

void MojaGrubaRyba::play(unsigned int rounds)
{
	initBoard();
	for(auto player : players)
		player -> reset();
	playersInGame = players.size();
	for(int r = 1; r <= rounds && playersInGame > 1; r++)
	{
		std::cout << "Runda: " << r << '\n';
		for(auto player : players)
		{
			if(player->inGame() == false){
				continue;
			}
			if(player->getWaitingTime() > 0){
				player->setWaitingTime(player->getWaitingTime()-1);
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
				if (player->inGame() == false){
					playersInGame--;
					if (playersInGame == 1)
						break;
				}
			}
		}
		// statystyki:
		for(auto player : players)
		{
			std::cout << player->getName();
			if(player -> inGame() == false)
				std::cout << " *** bankrut *** \n";
			else
			{
				std::cout << " pole: " << board->field(player->getPos())->getName();
				if(player->getWaitingTime() == 0)
					std::cout << " gotowka: " << player->getFishcoins() << '\n';
				else
					std::cout << " *** czekanie: " << player->getWaitingTime() << " *** \n";
			//	if(r == 70){
			//		debug = true;
			//	}
			}
		}
	}
}

MojaGrubaRyba::MojaGrubaRyba()
{
}

void MojaGrubaRyba::addComputerPlayer(ComputerLevel level)
{
	std::string name = "Gracz"+std::to_string(players.size()+1);
	switch(level)
	{
		case ComputerLevel::DUMB:
			players.push_back(std::shared_ptr<Player>(new DumbComputerPlayer(name, fishcoinsOnStart)));
			break;
		case ComputerLevel::SMARTASS:
			players.push_back(std::shared_ptr<Player>(new SmartassComputerPlayer(name, fishcoinsOnStart)));
			break;
	}
}

void MojaGrubaRyba::addHumanPlayer(std::shared_ptr<Human> human)
{
	players.push_back(std::shared_ptr<Player>(new HumanPlayer(human, fishcoinsOnStart)));
}

/* Player */

Player::Player(const std::string &&cname, int fishcoins) : name(std::move(cname)), fishcoins(fishcoins), fishcoinsOnStart(fishcoins), pos(0), waitingTime(0), bankrupt(false)
{
}

void Player::reset()
{
	setFishcoins(fishcoinsOnStart);
	setPos(0);
	setWaitingTime(0);
	bankrupt = false;
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
	//std::vector< weak_ptr< PropertyField >> newProperties;
	for (auto wprop : this->ownedProperties){
		if (auto prop = wprop.lock()){
			if (wantSell(prop->getName())){
				fishcoins += prop->getPrice();
				prop->sold();
			}
	//		else {
	//			newProperties.push_back(wprop);
	//		}
		}
	}
	//this->ownedProperties.clear();
	//this->ownedProperties = std::move(newProperties);
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
	/*std::cerr << "BANKRUT " <<this->getName() << " " << this->ownedProperties.size() << '\n';
	for (auto wprop : this->ownedProperties){
		if (auto prop = wprop.lock()){
			prop->sold();
			std::cerr << "SOLD\n";
		}
	}
	this->ownedProperties.clear();*/
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

void Player::setFishcoins(unsigned int fishcoins)
{
	this -> fishcoins = fishcoins;
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
void Player::addProperty(weak_ptr< PropertyField > property)
{
	this->ownedProperties.push_back(property);
	std::cerr << this->ownedProperties.size() << " " << this->getName() << std::endl;
}
DumbComputerPlayer::DumbComputerPlayer(const std::string cname, unsigned int fishcoins) : Player(std::move(cname), fishcoins)
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

SmartassComputerPlayer::SmartassComputerPlayer(const std::string cname, unsigned int fishcoins) : Player(std::move(cname), fishcoins)
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

HumanPlayer::HumanPlayer(std::shared_ptr<Human> hInterface, unsigned int fishcoins) : Player(std::move(hInterface->getName()), fishcoins), humanInterface(hInterface)
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

Board::Board()
{
}

Board::Board(std::vector < std::shared_ptr < Field > > &&_fields) : fields(std::move(_fields))
{
}

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
	
	if (this->owner.expired() || !this->owner.lock()->inGame()){
		if(p->wantBuy(this->name)){
			if (p->pay(this->price) == this->price){
				this->owner = p;
			}
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
