// Pataquès et bilboquet.h : fichier Include pour les fichiers Include système standard,
// ou les fichiers Include spécifiques aux projets.

#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <random>
#include <time.h>
#include <thread>
#include <chrono>
#include <mutex>
#include <shared_mutex>
#include "raylib.h"


using namespace std;

class Subway {
private:
	int id;
	int people;
	int max_people;
	float speed;
	float max_speed;
	float acceleration;
	bool direction = true; // True est le sens croissant (pour l'aller) des stations et false le sens décroissant (pour le retour)
	int station_id;
	
	//mutex position_mutex;
	//condition_variable position_condition;


public:
	
	Subway();
	Subway(const int& init_id,const int& init_people,const int& init_max_people,const float& init_speed,const float& init_max_speed,const float& init_acceleration, const bool& init_direction, const int& init_station_id);
	
	//Subway operator=(Subway&);

	Vector2 coordinates;
	Texture2D sub_texture;

	void set_id(const int& setid);
	void set_people(const int& setpeople);
	void set_maxpeople(const int& setmaxpeople);
	void set_speed(const float& setspeed);
	void set_maxspeed(const float& setmaxspeed);
	void set_acceleration(const float& setacceleration);
	void reverse_direction();
	void set_station_id(const int& id);


	int get_id();
	int get_people();
	int get_maxpeople();
	float get_speed();
	float get_acceleration();
	float get_max_speed();
	bool get_direction();
	int get_station_id();


	void move_to_station(Vector2 target_position);
};


class Station {
private:
	string name;
	int id;
	int people_forward;
	int people_return;
	bool is_subway;
	bool is_ready;
	Subway sub_in_station_forward;
	Subway sub_in_station_return;

public:

	Vector2 station_location;
	Texture2D station_texture;

	Station(const string& init_name, const int& init_people_forward, const int& init_people_return, const bool& init_is_subway, const bool& init_is_ready, const Vector2& init_station_location);
	void set_name(const char& setname);
	void set_id(const int& setid);
	void set_people_forward(const int& setpeopleforward);
	void set_people_return(const int& setpeoplereturn);
	void set_is_ready(const bool& flag);
	void set_is_subway(const bool& flag);
	void set_sub_in_station_forward(Subway subway_in);
	void set_sub_in_station_return(Subway subway_in);

	string get_name();
	int get_id();
	int get_people_forward();
	int get_people_return();
	bool get_is_subway();
	bool get_is_ready();
	Subway get_sub_in_station_forward();
	Subway get_sub_in_station_return();


	// Méthodes autres que get / set
	void subway_entrance(Subway subway_in);
	Subway subway_exit(Subway subway_in);

	void people_offboarding(Subway subway_in);
	void people_onboarding(Subway subway_in);
};

void start_thread(int index, vector<Station*> metro_line, vector<Subway*> metro_subway, jthread* subway_thread);


void subway_move(vector<Station*> metro_line, vector<Subway*> metro_subway, int sub_index, jthread* subway_thread);


//Subway Subway::operator=(Subway &subway) {
//	Subway temp;
//	temp.set_id(subway.get_id());
//	temp.set_people(subway.get_people());
//	temp.set_maxpeople(subway.get_maxpeople());
//	temp.set_speed(subway.get_speed());
//	temp.set_maxspeed(subway.get_max_speed());
//	temp.set_acceleration(subway.get_acceleration());
//	temp.set_direction(subway.get_direction());
//	temp.set_station_id(subway.get_station_id());
//	return temp;
//}
