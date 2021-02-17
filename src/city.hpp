#ifndef CITY_HPP
#define CITY_HPP

#include <vector>
#include <map>
#include <string>

#include "map.hpp"

struct CitySaveStruct
{
	int width;
	int height;
	int day;
	int populationPool;
	int employmentPool;
	int population;
	int employable;
	int birthRate;
	int deathRate;
	int residentialTax;
	int commercialTax;
	int industrialTax;
	int funds;
	int earnings;
};

class City
{
private:
	float currentTime;
	float timePerDay;

	std::vector<int> shuffledTiles;
	double populationPool;
	double employmentPool;
	float propCanWork;

	double birthRate;
	double deathRate;

	double distributePool(double& pool, Tile& tile, double rate);

public:
	Map map;
	std::string name;
	double population;
	double employable;

	double residentialTax;
	double commercialTax;
	double industrialTax;

	double earnings;
	double funds;

	int day;

	City()
	{
		this->birthRate = 0.00155;
		this->deathRate = 0.00023;
		this->propCanWork = 0.50;
		this->populationPool = 50;
		this->population = populationPool;
		this->employmentPool = 25;
		this->employable = employmentPool;
		this->residentialTax = 0.05;
		this->commercialTax = 0.05;
		this->industrialTax = 0.05;
		this->earnings = 0;
		this->funds = 10000;
		this->currentTime = 0.0;
		this->timePerDay = 1.0;
		this->day = 0;
	}

	City(std::string cityName, int tileSize, std::map<std::string, Tile>& tileAtlas) : City()
	{
		this->map.tileSize = tileSize;
		load(cityName, tileAtlas);
	}

	void load(std::string cityName, std::map<std::string, Tile>& tileAtlas);
	void save(std::string cityName);
	void update(float dt);
	void bulldoze(const Tile& tile);
	void shuffleTiles();
	void tileChanged();
	double getHomeless() { return this->populationPool; }
	double getUnemployed() { return this->employmentPool; }
	void RandomCity(int seed, float freq);
};

#endif // CITY_HPP