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

class Station;

class Subway {
private:
	int id;					 // id du metro
	int people;				 // nombre de personne lors de la creation du métro
	int max_people;			 // nomre de personne maximum
	int speed;				 // vitesse du metro
	int max_speed;			 // vitesse maximale
	int acceleration;		 // acceleration // attention ce n'est pas un coefficient 
	bool direction = true;	 // True est le sens croissant (pour l'aller) des stations et false le sens décroissant (pour le retour) // true par défaut
	int station_id;			 // id de la station dans laquelle est le métro
	int safe_distance = 200; // distance de sécurité (en px) entre les métro // 200px par défaut

public:

	Vector2 coordinates; // coordonnées du métro 
	Texture2D sub_texture; // sprite du métro

	Subway();
	Subway(const int& init_id, const int& init_people, const int& init_max_people, const int& init_speed, const int& init_max_speed, const int& init_acceleration, const bool& init_direction, const int& init_station_id);

	// setters //
	void set_id(const int& setid);
	void set_people(const int& setpeople);
	void set_maxpeople(const int& setmaxpeople);
	void set_speed(const int& setspeed);
	void set_maxspeed(const int& setmaxspeed);
	void set_acceleration(const int& setacceleration);
	void set_station_id(const int& id);

	// getters //
	int get_id();
	int get_people();
	int get_maxpeople();
	int get_speed();
	int get_acceleration();
	int get_max_speed();
	bool get_direction();
	int get_station_id();
	int get_safe_distance();

	// méthode //

	/*reverse_direction
	- inverse la direction du métro*/
	void reverse_direction();

	/*move_to_station
	- déplace le métro vers la station donnée en parametre et faisant attention a respecter les distances de sécurité avec le metro dévant lui (metro donnée en parametre)*/
	void move_to_station(Vector2 target_position, Subway& previous_subway);
};


class Station {
private:
	string name;					// nom de la station
	int id;							// id de la station
	int people_forward;				// nombre de personne dans la station dans le sens aller 
	int people_return;				// nombre de personne dans la station dans le sens retour
	bool is_subway;					// permet de savoir si un métro est dans la station
	Subway sub_in_station_forward;	// permet de savoir quel métro est dans le sens aller
	Subway sub_in_station_return;	// permet de savoir quel métro est dans le sens retour

public:

	Vector2 station_location;		// coordonnée de la station
	Texture2D station_texture;		// texture de la station

	Station(const string& init_name, const int& init_people_forward, const int& init_people_return, const bool& init_is_subway, const Vector2& init_station_location);

	// setters //
	void set_name(const char& setname);
	void set_id(const int& setid);
	void set_people_forward(const int& setpeopleforward);
	void set_people_return(const int& setpeoplereturn);
	void set_is_subway(const bool& flag);
	void set_sub_in_station_forward(Subway subway_in);
	void set_sub_in_station_return(Subway subway_in);

	// getters //
	string get_name();
	int get_id();
	int get_people_forward();
	int get_people_return();
	bool get_is_subway();
	Subway get_sub_in_station_forward();
	Subway get_sub_in_station_return();


	// méthodes //

	/*subway_entrance
	- entre dans une station dans le bon sens et execute people_offboarding et people_onboarding*/
	void subway_entrance(Subway subway_in);

	/*subway_exit
	- return le métro qui est dans la station dans le bon sens et libère la station*/
	Subway subway_exit(Subway subway_in);

	void people_offboarding(Subway subway_in);
	void people_onboarding(Subway subway_in);
};

/*start_thread
- initialise les coordonées du metro et l'ajoute dans la liste des metro actif puis demarre un thread de la fonction core_gameplay avec les paramètre spécifiés*/
void start_thread(int index, vector<Station*> metro_line, vector<Subway*> metro_subway, jthread* subway_thread);

void core_gameplay(vector<Station*> metro_line, vector<Subway*> metro_subway, int sub_index, jthread* subway_thread);

vector<Subway*> metro_subway_active; // variables globales qui nous indiquent les métros actif ( permet d'eviter d'avoir des metros en "attente de demarrage")
