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

Subway::Subway(const int& init_id, const int& init_people, const int& init_max_people, const int& init_speed, const int& init_max_speed, const int& init_acceleration, const bool& init_direction, const int& init_station_id) {
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

void Subway::set_speed(const int& setspeed) {
	speed = setspeed;
}

void Subway::set_maxspeed(const int& setmaxspeed) {
	max_speed = setmaxspeed;
}

void Subway::set_acceleration(const int& setacceleration) {
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


int Subway::get_speed() {
	return speed;
}

int Subway::get_acceleration() {
	return acceleration;
}

int Subway::get_max_speed() {
	return max_speed;
}

bool Subway::get_direction() {
	return direction;
}

int Subway::get_station_id() {
	return station_id;
}

int Subway::get_safe_distance(){
	return safe_distance;
}

void Subway::move_to_station(Vector2 target_position, Subway& previous_subway){

	int distance = static_cast<int>(target_position.x) - static_cast<int>(this->coordinates.x);
	int distance_to_previous = abs(previous_subway.coordinates.x - this->coordinates.x);

	while (abs(distance) > 0) {

		this_thread::sleep_for(10ms);

		distance = static_cast<int>(target_position.x) - static_cast<int>(this->coordinates.x);
		distance_to_previous = abs(previous_subway.coordinates.x - this->coordinates.x);

		int direction = (distance > 0) ? 1 : -1;

		if (distance_to_previous < this->get_safe_distance() && previous_subway.direction == direction) {
			// Arrêt progressif en fonction de la proximité avec le métro précédent
			float brake_factor = distance_to_previous / safe_distance;
			this->set_speed(ceil((float)this->get_speed() * (float)brake_factor));
		}
		else{
			if (abs(distance) < this->get_speed()) {
				this->set_speed(this->get_speed() - this->get_acceleration());
			}
			else {
				this->set_speed(this->get_speed() + this->get_acceleration());
				if (this->get_speed() > this->get_max_speed()) {
					this->set_speed(this->get_max_speed());
				}

			}
		}
		coordinates.x += direction * static_cast<int>(this->get_speed());
	}
	this->set_speed(0);
}

// station //

Station::Station(const string& init_name, const int& init_people_forward, const int& init_people_return, const bool& init_is_subway, const bool& init_is_ready, const Vector2& init_station_location) {
	name = init_name;
	people_forward = init_people_forward;
	people_return = init_people_return;
	is_subway = init_is_subway;
	is_ready = init_is_ready;
	station_location = init_station_location;
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

void core_gameplay(vector<Station*> metro_line, vector<Subway*> metro_subway, int sub_index, jthread* subway_thread) {

	this_thread::sleep_for(100ms);
	auto nb_fois = 2; // nombre de fois que le metro va faire l'aller retour
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

				if (sub_index > 0) {
					// Appel de move_to_station avec le métro précédent
					metro_subway[sub_index]->move_to_station(metro_line[index]->station_location, *metro_subway[sub_index - 1]);
				}
				else {
					// Premier métro sans métro précédent
					metro_subway[sub_index]->move_to_station(metro_line[index]->station_location, *metro_subway[metro_subway.size() - 1]);
				}

				cout << metro_subway[sub_index]->get_id() << " in depot " << index << "\n";
				metro_subway[sub_index]->reverse_direction();
				start = true;
			}
			else if (index != 0 && index != metro_line.size() - 1) {
				if (start) start = false;

				if(sub_index > 0) {
					// Appel de move_to_station avec le métro précédent
					metro_subway[sub_index]->move_to_station(metro_line[index]->station_location, *metro_subway[sub_index - 1]);
				}
				else {
					// Premier métro sans métro précédent
					metro_subway[sub_index]->move_to_station(metro_line[index]->station_location, *metro_subway[metro_subway.size()-1]);
				}

				metro_line[index]->subway_entrance(*metro_subway[sub_index]);
				*metro_subway[sub_index] = metro_line[index]->subway_exit(*metro_subway[sub_index]);
				cout << "===============================================" << endl << endl;
			}
			if (i != 0 && i % ratio == 0 && next_id < metro_subway.size()) { // si la distance entre les lignes est bonne + on evite les out of range
				cout << "encore ((((((((((((((((((((((((((((((((((((((((((((" << endl;
				start_thread(next_id, metro_line, metro_subway, subway_thread);
				next_id = 999;
			}

			// metro_subway[index]->position_condition.notify_one(); // a tester
		}

		down_factor = -2; // pour reset l'aller retour
	}
}


void start_thread(int index, vector<Station*> metro_line, vector<Subway*> metro_subway, jthread* subway_thread) {
	metro_subway_active.push_back(metro_subway[index]);
	metro_subway[index]->coordinates = { metro_line[0]->station_location.x, metro_line[0]->station_location.y };
	subway_thread[index] = (jthread([=]() {core_gameplay(metro_line, metro_subway, index, subway_thread); }));
}


int main() {
	srand((int)time(NULL));

	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pataprout et bilboquet");
	SetTargetFPS(20);

	Station Depot1("Depot1", 0, 0, false, false, { 100, 400 });
	Station Lille("Lille", 50, 50, false, false, { 300, 400 });
	Station Berlin("Berlin", 50, 50, false, false, { 500, 400 });
	Station Moscou("Moscou", 50, 50, false, false, { 1000, 400 });
	Station Madrid("Madrid", 50, 50, false, false, { 1200, 400 });
	Station Depot2("Depot2", 0, 0, false, false, { 1500, 400 });

	Subway Metropolis(1, 10, 40, 0, 3, 1, true, 0);
	Subway Metropompied(2, 10, 40, 0, 5, 1, true, 0);
	Subway Metrambulance(3, 10, 40, 0, 3, 1, true, 0);
	Subway Metronome(4, 10, 40, 0, 10, 3, true, 0);

	vector<Station*> metro_line = { &Depot1, &Lille , &Berlin, &Moscou, &Madrid, &Depot2 };
	vector<Subway*> metro_subway = { &Metropolis, &Metropompied }; //, &Metrambulance };//, &Metronome };

	jthread subway_thread[2];

	// STATION ID GIVING
	for (int i = 1; i < metro_line.size() - 1; i++) {
		metro_line[i]->set_id(i);
	}

	// POS AND TEXTURE LOADING
	/*Metropolis.coordinates = { 100, 400 };
	Metropompied.coordinates = { 100, 400 };*/
	//Metrambulance.coordinates = { 100, 400 };

	Metropolis.sub_texture = LoadTexture(ASSETS_PATH"sub1_asset.png");
	Metropompied.sub_texture = LoadTexture(ASSETS_PATH"sub2_asset.png");
	//Metrambulance.sub_texture = LoadTexture(ASSETS_PATH"sub3_asset.png");

	Depot1.station_texture = LoadTexture(ASSETS_PATH"depot_asset.png");
	Depot2.station_texture = LoadTexture(ASSETS_PATH"depot_asset.png");

	// DEFAULT STATION TEXTURE LOADING
	for (int i = 1; i < metro_line.size() - 1; i++) {
		metro_line[i]->station_texture = LoadTexture(ASSETS_PATH"station_asset.png");
	}

	start_thread(0, metro_line, metro_subway, subway_thread);

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(WHITE);

		// DRAWING STATION
		for (int i = 0; i < metro_line.size(); i++) {
			DrawTextureEx(metro_line[i]->station_texture, { metro_line[i]->station_location.x - ceil((float)metro_line[i]->station_texture.width / (float)2), 300 }, 0, 1, WHITE);
		}

		// DRAWING SUBWAY
		for (int i = 0; i < metro_subway_active.size(); ++i) {
			DrawTextureEx(metro_subway_active[i]->sub_texture, { metro_subway_active[i]->coordinates.x - ceil((float)metro_subway_active[i]->sub_texture.width / (float)2), metro_subway_active[i]->coordinates.y }, 0, 1, WHITE);
		}

		EndDrawing();
	}

	CloseWindow();

	for (int i = 0; i < metro_subway.size(); ++i) {
		subway_thread[i].join();
	}

	for (int j = 0; j < metro_line.size(); j++) {
		cout << metro_line[j]->get_people_forward() << " : fw || " << metro_line[j]->get_people_return() << " : rtn" << endl;
	}

	// UNLOADING TEXTURES
	for (int i = 0; i < metro_line.size(); i++) {
		UnloadTexture(metro_line[i]->station_texture);
	}
	
	for (int i = 0; i < metro_subway.size(); ++i) {
		UnloadTexture(metro_subway[i]->sub_texture);
	}

	return 0;
}




/*const char* text = "PATAPROUT";d
const Vector2 text_size = MeasureTextEx(GetFontDefault(), text, 20, 1);
DrawText(text, SCREEN_WIDTH / 2 - text_size.x / 2, 500 + text_size.y + 10, 20, BLACK);*/
