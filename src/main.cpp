// Pataquès et bilboquet.cpp : définit le point d'entrée de l'application.
//

#include "Pataquès et bilboquet.h"

using namespace std;
using namespace chrono_literals;
shared_mutex my_shared_mutex;

// variables importantes

constexpr auto SCREEN_WIDTH = 1600;
constexpr auto SCREEN_HEIGHT = 900;


// declaration class

// subway //

Subway::Subway() {
	id = 999;
	people = 0;
	max_people = 999;
	speed = 0;
	max_speed = 999;
	acceleration = 1;
	direction = 1;
	station_id = 0;
}

Subway::Subway(const int& init_id, const int& init_people, const int& init_max_people, const float& init_speed, const float& init_max_speed, const float& init_acceleration, const bool& init_direction, const int& init_station_id) {
	id = init_id;
	people = init_people;
	max_people = init_max_people;
	speed = init_speed;
	max_speed = init_max_speed;
	acceleration = init_acceleration;
	direction = init_direction;
	station_id = init_station_id;
}


void Subway::set_id(const int& setid) {
	id = setid;
}

void Subway::set_people(const int& setpeople) {
	people = setpeople;
}

void Subway::set_maxpeople(const int& setmaxpeople) {
	max_people = setmaxpeople;
}

void Subway::set_speed(const float& setspeed) {
	speed = setspeed;
}

void Subway::set_maxspeed(const float& setmaxspeed) {
	max_speed = setmaxspeed;
}

void Subway::set_acceleration(const float& setacceleration) {
	acceleration = setacceleration;
}

void Subway::reverse_direction() {
	direction = !direction;
}

void Subway::set_station_id(const int& id) {
	station_id = id;
}

int Subway::get_id() {
	return id;
}

int Subway::get_people() {
	return people;
}

int Subway::get_maxpeople() {
	return max_people;
}


float Subway::get_speed() {
	return speed;
}

float Subway::get_acceleration() {
	return acceleration;
}

float Subway::get_max_speed() {
	return max_speed;
}

bool Subway::get_direction() {
	return direction;
}

int Subway::get_station_id() {
	return station_id;
}

// station //

Station::Station(const string& init_name, const int& init_people_forward, const int& init_people_return, const bool& init_is_subway, const bool& init_is_ready) {
	name = init_name;
	people_forward = init_people_forward;
	people_return = init_people_return;
	is_subway = init_is_subway;
	is_ready = init_is_ready;
}

void Station::set_name(const char& setname) {
	name = setname;
}

void Station::set_id(const int& setid) {
	id = setid;
}

void Station::set_people_forward(const int& setpeopleforward) {
	people_forward = setpeopleforward;
}

void Station::set_people_return(const int& setpeoplereturn) {
	people_return = setpeoplereturn;
}

void Station::set_is_ready(const bool& flag) {
	is_ready = flag;
}

void Station::set_is_subway(const bool& flag) {
	is_subway = flag;
}

void Station::set_sub_in_station_forward(Subway subway_in) {
	sub_in_station_forward = subway_in;
}

void Station::set_sub_in_station_return(Subway subway_in) {
	sub_in_station_return = subway_in;
}

string Station::get_name() {
	return name;
}

int Station::get_id() {
	return id;
}

int Station::get_people_forward() {
	return people_forward;
}

int Station::get_people_return() {
	return people_return;
}

bool Station::get_is_subway() {
	return is_subway;
}

bool Station::get_is_ready() {
	return is_ready;
}

Subway Station::get_sub_in_station_forward() {
	return sub_in_station_forward;
}

Subway Station::get_sub_in_station_return() {
	return sub_in_station_return;
}


void Station::subway_entrance(Subway subway_in) {



	set_is_subway(true);
	if (subway_in.get_direction()) {

		set_sub_in_station_forward(subway_in);
		sub_in_station_forward.set_station_id(this->id);
		cout << sub_in_station_forward.get_id() << " succesfully in " << name << " direction " << sub_in_station_forward.get_direction() << endl;
	}
	else {
		set_sub_in_station_return(subway_in);
		sub_in_station_return.set_station_id(this->id);
		cout << sub_in_station_return.get_id() << " succesfully in " << name << " direction " << sub_in_station_return.get_direction() << endl;
	}

	people_offboarding(subway_in);
	people_onboarding(subway_in);

}


Subway Station::subway_exit(Subway subway_in) {


	if (subway_in.get_direction()) {
		cout << sub_in_station_forward.get_id() << " succesfully exit " << name << " direction " << sub_in_station_forward.get_direction() << endl;
		return sub_in_station_forward;
	}
	else {
		cout << sub_in_station_return.get_id() << " succesfully exit " << name << " direction " << sub_in_station_return.get_direction() << endl;
		return sub_in_station_return;
	}

	set_is_subway(false);
}

void Station::people_offboarding(Subway subway_in) {
	int people_in_subway = (subway_in.get_direction()) ? sub_in_station_forward.get_people() : sub_in_station_return.get_people();
	if (people_in_subway != 0) {
		int off = rand() % people_in_subway + 1;
		//cout << "Tot in subway : " << people_in_subway << ", getting out : " << off << endl;
		for (int i = 1; i <= off; i++) {
			this_thread::sleep_for(10ms); // désactivé pour le déboggage 
		}
		if (subway_in.get_direction()) { sub_in_station_forward.set_people(people_in_subway - off); }
		else { sub_in_station_return.set_people(people_in_subway - off); }

		//cout << sub_in_station.get_people() << " people now in subway" << endl << endl;

	}
}

void Station::people_onboarding(Subway subway_in) {
	int space_left = (subway_in.get_direction()) ? sub_in_station_forward.get_maxpeople() - sub_in_station_forward.get_people() : sub_in_station_return.get_maxpeople() - sub_in_station_return.get_people();
	int people_in_station = (subway_in.get_direction()) ? this->get_people_forward() : this->get_people_return();
	//cout << "Tot in station : " << people_in_station << ", space left : " << space_left << endl;
	int iter = (space_left < people_in_station) ? space_left : people_in_station;
	for (int i = 1; i <= iter; i++) {
		this_thread::sleep_for(10ms); // désactivé pour le deboggage
	}
	//cout << "+" << iter << " in" << endl;
	if (subway_in.get_direction()) {
		this->set_people_forward(people_in_station - iter);
		sub_in_station_forward.set_people(sub_in_station_forward.get_people() + iter);
	}
	else {
		this->set_people_return(people_in_station - iter);
		sub_in_station_return.set_people(sub_in_station_return.get_people() + iter);

	}

	people_in_station = (subway_in.get_direction()) ? this->get_people_forward() : this->get_people_return();
	//cout << "Tot in station : " << people_in_station << ", people now in subway : " << sub_in_station.get_people() << endl;
}

// fonctions 

// déclaration anticipée 



void subway_move(vector<Station*> metro_line, vector<Subway*> metro_subway, int sub_index, jthread* subway_thread) {

	this_thread::sleep_for(100ms);
	auto nb_fois = 1; // nombre de fois que le metro va faire l'aller retour
	int next_id = metro_subway[sub_index]->get_id(); // permettra de démarrer les metros avec un decalage 
	int ratio = ceil((float)(metro_line.size() - 1) * 2 / (float)metro_subway.size()); // ceil permet d'arrondir a l'entier superieur
	bool start = true;
	int down_factor = -2; // pour reset l'aller retour
	for (int j = 0; j < nb_fois; j++) {



		for (int i = 0; i <= (metro_line.size() * 2) - 2; i++) {
			int index = i;
			if (i > metro_line.size() - 1) {
				index += down_factor;
				down_factor -= 2;
			}
			/* {
				std::unique_lock<std::mutex> lock(metro_subway[sub_index]->position_mutex);
				metro_subway[sub_index]->position_condition.wait(lock, [index, metro_subway, sub_index] {
					return index - 1 >= 0 && metro_subway[index - 1]->get_position() > SOME_THRESHOLD;
					});
			}*/ //a tester 

			if (!start && (index == 0 || index == metro_line.size() - 1)) {
				cout << metro_subway[sub_index]->get_id() << " in depot " << index << "\n";
				metro_subway[sub_index]->reverse_direction();
				start = true;
			}
			else if (index != 0 && index != metro_line.size() - 1) {
				if (start) start = false;

				metro_line[index]->subway_entrance(*metro_subway[sub_index]);
				*metro_subway[sub_index] = metro_line[index]->subway_exit(*metro_subway[sub_index]);
				cout << "===============================================" << endl << endl;
			}
			if (i != 0 && i % ratio == 0 && next_id < metro_subway.size()) { // si la distance entre les lignes est bonne + on evite les out of range
				cout << "encore ((((((((((((((((((((((((((((((((((((((((((((((((((((" << endl;
				start_thread(next_id, metro_line, metro_subway, subway_thread);
				next_id = 999;
			}

			// metro_subway[index]->position_condition.notify_one(); // a tester
		}

		down_factor = -2; // pour reset l'aller retour
	}
}


void start_thread(int index, vector<Station*> metro_line, vector<Subway*> metro_subway, jthread* subway_thread) {
	subway_thread[index] = (jthread([=]() {subway_move(metro_line, metro_subway, index, subway_thread); }));

}



int main() {
	srand((int)time(NULL));

	Station Depot("Depot", 0, 0, false, false);
	Station Lille("Lille", 50, 50, false, false);
	Station Berlin("Berlin", 50, 50, false, false);
	Station Moscou("Moscou", 50, 50, false, false);
	Station Madrid("Madrid", 50, 50, false, false);

	Subway Metropolis(1, 10, 40, 0, 1, 1, true, 0);
	Subway Metropompied(2, 10, 40, 0, 1, 1, true, 0);
	Subway Metronome(3, 10, 40, 0, 1, 1, true, 0);
	Subway Metrambulance(4, 10, 40, 0, 1, 1, true, 0);

	vector<Station*> metro_line = { &Depot, &Lille , &Berlin, &Moscou, &Madrid, &Depot };

	vector<Subway*> metro_subway = { &Metropolis };//, & Metropompied};//, &Metronome, &Metrambulance };

	jthread subway_thread[1];

	for (int i = 1; i < metro_line.size(); i++) {
		metro_line[i]->set_id(i);
	}

	start_thread(0, metro_line, metro_subway, subway_thread);

	for (int i = 0; i < metro_subway.size(); ++i) {
		subway_thread[i].join();
	}

	for (int j = 0; j < metro_line.size(); j++) {

		cout << metro_line[j]->get_people_forward() << " : fw || " << metro_line[j]->get_people_return() << " : rtn" << endl;

	}

	return 0;
}


