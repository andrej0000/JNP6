#include "mojagrubaryba.h"

void MojaGrubaRyba::setDie(std::shared_ptr<Die> die)
{
	this->die.reset(die);
}

MojaGrubaRyba::MojaGrubaRyba()
{
	vector < Field > fields;
	fields.push_back(new StartField);
	fields.push_back(new PropertyField("Anemonia", 160));
	fields.push_back(new IslandField);
	fields.push_back(new PropertyField("Aporina", 220));
	fields.push_back(new AquariumField(3));
	fields.push_back(new PropertyField("Hotel w grocie", 300));
	fields.push_back(new PropertyField("Menella", 280));
	fields.push_back(new DeopsitField("Laguna", 15));
	fields.push_back(new PropertyField("Statek", 250));
	fields.push_back(new PrizeField("Blazenki", 120));
	fields.push_back(new PropertyField("Pennatula", 400));
	fields.push_back(new FineField("Rekin", 180));
	board.fields = std::move(fields);
}
