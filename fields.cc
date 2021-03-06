#include "fields.h"
#include <iostream>
/* Field */

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

PrizeField::PrizeField(std::string name, int ammount) : name(name), ammount(ammount)
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


IslandField::IslandField() : name("Wyspa")
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


PropertyField::PropertyField(std::string name, int price, int percent)
	: name(name), price(price), percent(percent)
{

}

void PropertyField::action(std::shared_ptr<Player> p)
{
	if (this->owner.expired()){//noninitialized
		if(p->wantBuy(this->name)){
			this->owner = p;
		}
	} else if (auto owner = this->owner.lock()){
		if (owner != p) {
			int fare = (this->getPercent * this->price) / 100;
			p->pay(fare);
			owner->addFishcoins(fare);
		}
	}
	else {
		throw deletedPlayerException;//wysraj wyjatek
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

PublicPropertyField::PublicPropertyField(std::string name, int price)
	: PropertyField(name, price, 40)
{

}

CoralPropertyField::CoralPropertyField(std::string name, int price)
	: PropertyField(name, price, 20)
{

}

DepositField::DepositField(std::string name, int ammount) : name(name), ammount(ammount)
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

FineField::FineField(std::string name. int ammount) : name(name), ammount(ammount)
{

}
void FineField::action(std::shared_ptr<Player> p)
{
	this->fishcoins += p->pay(this->ammount);
}

void FineField::pass(std::shared_ptr<Player> p)
{

}

AquariumField::AquariumField(int turns) : turnsToWait(turns)
{

}
void AquariumField::action(std::shared_ptr<Player> p)
{
	p->setWaitingTime(this->turnsToWait);
}

void AquariumField::pass(std::shared_ptr<Player> p)
{

}

