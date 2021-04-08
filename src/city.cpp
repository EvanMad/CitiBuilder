#include <cmath>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <list>
#include <algorithm>
#include <random>
#include <string>

#include "city.hpp"
#include "tile.hpp"
#include "map.hpp"
#include <FastNoiseLite.h>

double City::distributePool(double& pool, Tile& tile, double rate = 0.0)
{
	const static int moveRate = 4;
	unsigned int maxPop = tile.maxPopPerLevel * (tile.tileVariant + 1);
	if (pool > 0)
	{
		float moving = maxPop - tile.population;
		if (moving > moveRate) moving = moveRate;
		if (pool - moving < 0) moving = pool;
		pool -= moving;
		tile.population += moving;
	}

	tile.population += tile.population * rate;

	if (tile.population > maxPop)
	{
		pool += tile.population - maxPop;
		tile.population = maxPop;
	}
	return tile.population;
}

void City::bulldoze(const Tile& tile)
{
	/* Replace the selected tiles on the map with the tile and
	 * update populations etc accordingly */
	for (int pos = 0; pos < this->map.width * this->map.height; ++pos)
	{
		int test = this->map.selected[pos];
		if (this->map.selected[pos] != 0)
		{
			this->map.tiles[pos] = tile;
		}
	}
	return;
}

void City::shuffleTiles()
{
	while (this->shuffledTiles.size() < this->map.tiles.size())
	{
		this->shuffledTiles.push_back(0);
		//this->shuffledTiles.push_back(this->map.tiles);
	}
	//std::iota(shuffledTiles.begin(), shuffledTiles.end(), 1);
	//std::random_shuffle(shuffledTiles.begin(), shuffledTiles.end());

	return;
}

void City::tileChanged()
{
	this->map.updateDirection(17);
	this->map.updateDirection(18);
	this->map.findConnectedRegions({
		17,
		18,
		}, 0);
	return;
}

void City::load(std::string cityName, std::vector<Tile>& tileAtlas)
{
	int width = 0;
	int height = 0;
	std::fstream inputFile;
	inputFile.open(cityName, std::ios::in | std::ios::binary);
	
	int dump;
	CitySaveStruct cityLoad;
	inputFile.read((char*)&dump, sizeof(int));
	inputFile.read((char*)&cityLoad.width, sizeof(int));
	inputFile.read((char*)&cityLoad.height, sizeof(int));
	inputFile.read((char*)&cityLoad.day, sizeof(int));
	inputFile.read((char*)&cityLoad.populationPool, sizeof(int));
	inputFile.read((char*)&cityLoad.employmentPool, sizeof(int));
	inputFile.read((char*)&cityLoad.population, sizeof(int));
	inputFile.read((char*)&cityLoad.employable, sizeof(int));
	inputFile.read((char*)&cityLoad.birthRate, sizeof(int));
	inputFile.read((char*)&cityLoad.deathRate, sizeof(int));
	inputFile.read((char*)&cityLoad.residentialTax, sizeof(int));
	inputFile.read((char*)&cityLoad.commercialTax, sizeof(int));
	inputFile.read((char*)&cityLoad.industrialTax, sizeof(int));
	inputFile.read((char*)&cityLoad.funds, sizeof(int));
	inputFile.read((char*)&cityLoad.earnings, sizeof(int));

	int total = cityLoad.width * cityLoad.height;
	std::vector<int> test;
	std::vector<TileSaveStruct> tileStructs;
	for (int x = 0; x < total; x++)
	{
		TileSaveStruct tile;
		inputFile.read((char*)(&tile), sizeof(TileSaveStruct));
		test.push_back(tile.tiletype);
		test.push_back(tile.tileVariant);
		test.push_back(tile.population);
		test.push_back(tile.regions);
		test.push_back(tile.storedGoods);

		tileStructs.push_back(tile);
	}

	this->map.LoadFromCitiFile(tileStructs, cityLoad.width, cityLoad.height, tileAtlas);

	inputFile.close();
	tileChanged();
	return;
}

void City::save(std::string cityName)
{
	std::ofstream outputFile;
	outputFile.open("saves/" + cityName + ".citi", std::ios::out | std::ios::binary);
	outputFile << "citi";
	CitySaveStruct city_out;
	city_out.height = this->map.width;
	city_out.width = this->map.height;
	city_out.day = this->day;
	city_out.populationPool = this->populationPool;
	city_out.employmentPool = this->employmentPool;
	city_out.population = this->population;
	city_out.employable = this->employable;
	city_out.birthRate = this->birthRate;
	city_out.deathRate = this->deathRate;
	city_out.residentialTax = this->residentialTax;
	city_out.industrialTax = this->industrialTax;
	city_out.commercialTax = this->commercialTax;
	city_out.funds = this->funds;
	city_out.earnings = this->earnings;

	outputFile.write(reinterpret_cast<char*>(&city_out), sizeof(city_out));
	
	std::vector<int> test = this->map.save();
	for (auto tile : test)
	{
		outputFile.write(reinterpret_cast<char*>(&tile), sizeof(int));
	}

	std::cout << "Save" << std::endl;

	//outputFile.open("saves/debug.AHHH", std::ios::out | std::ios::binary);
	//outputFile << "Test";
	//outputFile.close();

	outputFile.close();

	return;
}

void City::update(float dt)
{
	double popTotal = 0;
	double commercialRevenue = 0;
	double industrialRevenue = 0;

	this->currentTime += dt;
	if (this->currentTime < this->timePerDay) return;
	++day;
	this->currentTime = 0.0;
	for (int i = 0; i < this->map.tiles.size(); ++i)
	{
		Tile& tile = this->map.tiles[this->shuffledTiles[i]];
		tile.update();
	}
	return;
}

void City::RandomCity(int seed, float freq, std::vector<Tile>& tileAtlas, int world_size)
{
	FastNoiseLite noise;
	noise.SetSeed(seed);
	noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
	noise.SetFrequency(freq);
	float noiseData[256][256];
	int index = 0;

	//std::ofstream outputFile;
	//outputFile.open("maps/randomworld.dat", std::ios::out | std::ios::binary);

	std::vector<TileSaveStruct> tileStructs;
	for (int y = 0; y < world_size; y++)
	{
		for (int x = 0; x < world_size; x++)
		{
			TileSaveStruct tile;
			tile.tileVariant = 0;
			tile.regions = 0;
			tile.population = 0;
			tile.storedGoods = 0;
			noiseData[x][y] = noise.GetNoise((float)x, (float)y);
			if(noiseData[x][y] > 0.5) { tile.tiletype = 16; }
			else if (noiseData[x][y] > 0) { tile.tiletype = 14; }
			else if (noiseData[x][y] < 0) { tile.tiletype = 21; }
			else if (noiseData[x][y] < -0.5) { tile.tiletype = 16; }
			else { tile.tiletype = 16; }
			tileStructs.push_back(tile);

			//outputFile.write(reinterpret_cast<char*>(&tile), sizeof(tile));
		}
	}
	this->map.LoadFromCitiFile(tileStructs, world_size, world_size, tileAtlas);
	//outputFile.close();
	//delete[] noiseData;
}

