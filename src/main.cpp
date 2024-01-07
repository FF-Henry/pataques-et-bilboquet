// Pataquès et bilboquet.cpp : définit le point d'entrée de l'application.

#include "Pataquès et bilboquet.h"

using namespace std;
using namespace chrono_literals; // pour mettre les threads en pause

// variables importantes

constexpr auto SCREEN_WIDTH = 1600; // largueur ecran 
constexpr auto SCREEN_HEIGHT = 900; // hauteur ecran 
#define NUM_FRAMES  3 

// declaration class //

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

void Subway::set_id(const int& setid) {	id = setid; }

void Subway::set_people(const int& setpeople) {	people = setpeople; }

void Subway::set_maxpeople(const int& setmaxpeople) { max_people = setmaxpeople; }

void Subway::set_speed(const int& setspeed) { speed = setspeed; }

void Subway::set_maxspeed(const int& setmaxspeed) {	max_speed = setmaxspeed;}

void Subway::set_acceleration(const int& setacceleration) {	acceleration = setacceleration; }

void Subway::reverse_direction() { direction = !direction; }

void Subway::set_station_id(const int& id) { station_id = id; }

int Subway::get_id() { return id; }

int Subway::get_people() { return people; }

int Subway::get_maxpeople() { return max_people; }

int Subway::get_speed() { return speed; }

int Subway::get_acceleration() { return acceleration; }

int Subway::get_max_speed() { return max_speed; }

bool Subway::get_direction() { return direction; }

int Subway::get_station_id() { return station_id; }

int Subway::get_safe_distance() { return safe_distance; }

void Subway::move_to_station(Vector2 target_position, Subway& previous_subway) {

	int distance = static_cast<int>(target_position.x) - static_cast<int>(this->coordinates.x);
	int distance_to_previous = abs(previous_subway.coordinates.x - this->coordinates.x);

	bool delay = true;

	while (abs(distance) > 0) {

		this_thread::sleep_for(10ms);

		distance = static_cast<int>(target_position.x) - static_cast<int>(this->coordinates.x);
		distance_to_previous = abs(previous_subway.coordinates.x - this->coordinates.x);

		int direction_indicator = (distance > 0) ? 1 : -1;

		if (distance_to_previous < this->get_safe_distance() && previous_subway.direction == this->get_direction()) {
			// Arrêt progressif en fonction de la proximité avec le métro précédent
			float brake_factor = distance_to_previous / safe_distance;
			this->set_speed(ceil((float)this->get_speed() * (float)brake_factor));
		}
		else {
			if (abs(distance) < this->get_speed()*2) {
				this->set_speed(this->get_speed() - this->get_acceleration());
			}
			else { 
				this->set_speed(this->get_speed() + this->get_acceleration()); 
				if (this->get_speed() > this->get_max_speed()) {
					this->set_speed(this->get_max_speed());
				}
			}
		}
		if (target_position.x == 1500 && coordinates.x == 1500) {
			coordinates.y = 615;
		}
		if (target_position.x == 100 && coordinates.x == 100) {
			coordinates.y = 400;
		}
		coordinates.x += direction_indicator * static_cast<int>(this->get_speed());
	}
	this->set_speed(0);
}

// station //

Station::Station(const string& init_name, const int& init_people_forward, const int& init_people_return, const bool& init_is_subway, const Vector2& init_station_location) {
	name = init_name;
	people_forward = init_people_forward;
	people_return = init_people_return;
	is_subway = init_is_subway;
	station_location = init_station_location;
}

void Station::set_name(const char& setname) { name = setname; }

void Station::set_id(const int& setid) { id = setid; }

void Station::set_people_forward(const int& setpeopleforward) {	people_forward = setpeopleforward; }

void Station::set_people_return(const int& setpeoplereturn) { people_return = setpeoplereturn; }

void Station::set_is_subway(const bool& flag) {	is_subway = flag; }

void Station::set_sub_in_station_forward(Subway subway_in) { sub_in_station_forward = subway_in; }

void Station::set_sub_in_station_return(Subway subway_in) {	sub_in_station_return = subway_in; }

string Station::get_name() { return name; }

int Station::get_id() {	return id; }

int Station::get_people_forward() {	return people_forward; }

int Station::get_people_return() { return people_return; }

bool Station::get_is_subway() {	return is_subway; }

Subway Station::get_sub_in_station_forward() { return sub_in_station_forward; }

Subway Station::get_sub_in_station_return() {  return sub_in_station_return; }

void Station::subway_entrance(Subway subway_in) {

	set_is_subway(true); // on met a jour l'etat de la station
	if (subway_in.get_direction()) { // pour savoir le sens dans lequel on est (rappel : true = aller, false = retour)
		set_sub_in_station_forward(subway_in); // on ajoute notre metro dans le bon sens de la station
		sub_in_station_forward.set_station_id(this->id); // on recupère l'id de la station
		cout << sub_in_station_forward.get_id() << " succesfully in " << name << " direction " << sub_in_station_forward.get_direction() << endl; // debug
	}
	else {
		set_sub_in_station_return(subway_in);
		sub_in_station_return.set_station_id(this->id);
		cout << sub_in_station_return.get_id() << " succesfully in " << name << " direction " << sub_in_station_return.get_direction() << endl; // debug
	}
	people_offboarding(subway_in);
	people_onboarding(subway_in);
}

Subway Station::subway_exit(Subway subway_in) {

	if (subway_in.get_direction()) { // pour savoir lequel des metros il faut renvoyer en fonction du sens
		cout << sub_in_station_forward.get_id() << " succesfully exit " << name << " direction " << sub_in_station_forward.get_direction() << endl; // debug
		return sub_in_station_forward;
	}
	else {
		cout << sub_in_station_return.get_id() << " succesfully exit " << name << " direction " << sub_in_station_return.get_direction() << endl; // debug
		return sub_in_station_return;
	}
	set_is_subway(false); // on libère la station
}

void Station::people_offboarding(Subway subway_in) {

	int people_in_subway = (subway_in.get_direction()) ? sub_in_station_forward.get_people() : sub_in_station_return.get_people(); // on récupère le nombre de personne dans le sens aller ou retour 
	if (people_in_subway > 0) { // si le metro dans la station n'est pas vide
		random_device rd; // generation aleatoire
		mt19937 generator(rd()); // on recupère l'heure syteme
		uniform_int_distribution<int> distribution(1, people_in_subway); // range du rand
		int off = distribution(generator); // quantité aléatoire de personne qui sortent
		//cout << "Tot in subway : " << people_in_subway << ", getting out : " << off << endl; // debug
		for (int i = 1; i < off; i++) { 
			this_thread::sleep_for(10ms); // on attends Xms pour simuler les personnes qui sortent (désactivé pour le déboggage) 
		}
		if (subway_in.get_direction()) {
			sub_in_station_forward.set_people(people_in_subway - off);
			// this->set_people_forward(this->get_people_forward() + off); // boucle de passager fermée, à commenter pour demande initiale
		}
		else {
			sub_in_station_return.set_people(people_in_subway - off);
			// this->set_people_return(this->get_people_return() + off); // boucle de passager fermée, à commenter pour demande initiale
		}
		//cout << sub_in_station.get_people() << " people now in subway" << endl << endl;
	}
}

void Station::people_onboarding(Subway subway_in) {

	int space_left = (subway_in.get_direction()) ? sub_in_station_forward.get_maxpeople() - sub_in_station_forward.get_people() : sub_in_station_return.get_maxpeople() - sub_in_station_return.get_people(); // on regarde la place qu'il reste dans le metro en fonction de la place maximale et du nombre de personne deja dedans
	int people_atm = (subway_in.get_direction() ? people_forward : people_return); // on regarde le nombre de personne dans la station dans le sens du metro
	int qty_entrance = (space_left >= people_atm) ? people_atm : space_left;
	if (space_left != 0 && people_atm > 0) { // si le metro est deja plein alors on passe
		int people_in_station = (subway_in.get_direction()) ? this->get_people_forward() : this->get_people_return();
		random_device rd; // generation aleatoire
		mt19937 generator(rd()); // on recupère l'heure systeme 
		uniform_int_distribution<int> distribution(1, qty_entrance); // range du rand
		int iter = distribution(generator); // quantité aléatoire de personne qui entrent
		//cout << "Tot in station : " << people_in_station << ", space left : " << space_left << endl;
		for (int i = 0; i < iter; i++) {
			this_thread::sleep_for(10ms);  // on attends Xms pour simuler les personnes qui entrent (désactivé pour le déboggage) 
		}
		//cout << "+" << iter << " in" << endl;
		if (subway_in.get_direction()) {
			this->set_people_forward(people_in_station - iter);
			sub_in_station_forward.set_people(sub_in_station_forward.get_people() + iter); // on met a jour
		}
		else {
			this->set_people_return(people_in_station - iter);
			sub_in_station_return.set_people(sub_in_station_return.get_people() + iter); // on met a jour
		}
		//people_in_station = (subway_in.get_direction()) ? this->get_people_forward() : this->get_people_return();
		//cout << "Tot in station : " << people_in_station << ", people now in subway : " << sub_in_station.get_people() << endl;
	}
}

void core_gameplay(vector<Station*> metro_line, vector<Subway*> metro_subway, int sub_index, jthread* subway_thread) {

	this_thread::sleep_for(100ms);
	auto nb_fois = 5; // nombre de fois que le metro va faire l'aller retour
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

			if (!start && (index == 0 || index == metro_line.size() - 1)) { // si on est a l'un des 2 dépot (start permet d'eviter de remplir la condition lors du démarrage)

				if (sub_index > 0) {
					// Appel de move_to_station avec le métro précédent
					metro_subway[sub_index]->move_to_station(metro_line[index]->station_location, *metro_subway[sub_index - 1]);
				}
				else {
					// Premier métro sans métro précédent
					metro_subway[sub_index]->move_to_station(metro_line[index]->station_location, *metro_subway[metro_subway.size() - 1]);
				}

				cout << metro_subway[sub_index]->get_id() << " in depot " << index << "\n"; // debug
				metro_subway[sub_index]->reverse_direction();
				start = true;
			}
			else if (index != 0 && index != metro_line.size() - 1) { // quand on est pas aux dépots

				if (start) start = false;

				if (sub_index > 0) {
					// Appel de move_to_station avec le métro précédent
					metro_subway[sub_index]->move_to_station(metro_line[index]->station_location, *metro_subway[sub_index - 1]);
				}
				else {
					// Premier métro sans métro précédent
					metro_subway[sub_index]->move_to_station(metro_line[index]->station_location, *metro_subway[metro_subway.size() - 1]);
				}
				metro_line[index]->subway_entrance(*metro_subway[sub_index]);
				*metro_subway[sub_index] = metro_line[index]->subway_exit(*metro_subway[sub_index]);
				cout << "===============================================" << endl << endl; // debug
			}
			if (i != 0 && i % ratio == 0 && next_id < metro_subway.size()) { // si la distance entre les lignes est bonne + on evite les out of range
				cout << "A new metro just spawned ---------------------<" << endl; // debug
				start_thread(next_id, metro_line, metro_subway, subway_thread);
				next_id = 999;
			}
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
	//srand((int)time(NULL));

	
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pataques et bilboquet");
	SetTargetFPS(60);

	// déclaration des class stations //
	Station Depot1("Depot1", 0, 0, false, { 100, 400 });
	Station Lille("Lille", 100, 100, false, { 450, 400 });
	Station Berlin("Berlin", 100, 100, false, { 800, 400 });
	Station Moscou("Moscou", 100, 100, false, { 1150, 400 });
	Station Depot2("Depot2", 0, 0, false, { 1500, 400 });

	// déclaration des class metro //
	Subway Metropolis(1, 10, 40, 0, 2, 1, true, 0);
	Subway Metropompied(2, 10, 40, 0, 2, 1, true, 0);
	Subway Metrambulance(3, 10, 40, 0, 2, 1, true, 0);
	//Subway Metronome(4, 10, 40, 0, 10, 3, true, 0);

	vector<Station*> metro_line = { &Depot1, &Lille , &Berlin, &Moscou, &Depot2 }; // stations du metro
	vector<Subway*> metro_subway = { &Metropolis, &Metropompied, &Metrambulance }; // liste des metros 

	jthread subway_thread[10]; // permet de creer des threads en fonction du nombre de metro /!\ ce n'est pas automatique il faut changer manuellement la taille du tableau /!\

	// STATION ID GIVING
	for (int i = 1; i < metro_line.size() - 1; i++) {
		metro_line[i]->set_id(i);
	}

	// TEXTURE LOADING
	Metropolis.sub_texture = LoadTexture(ASSETS_PATH"sub1_asset.png");
	Metropompied.sub_texture = LoadTexture(ASSETS_PATH"sub2_asset.png");
	Metrambulance.sub_texture = LoadTexture(ASSETS_PATH"sub3_asset.png");

	Depot1.station_texture = LoadTexture(ASSETS_PATH"depot_asset.png");
	Depot2.station_texture = LoadTexture(ASSETS_PATH"depot_asset.png");

	Texture2D background = LoadTexture(ASSETS_PATH"bg.png");

	Texture2D stop_button = LoadTexture(ASSETS_PATH"button.png");

	// 
	float frameHeight = (float)stop_button.height / NUM_FRAMES;
	vector<Rectangle> sourceRec = {};
	sourceRec.push_back({ 0, 0, (float)stop_button.width, frameHeight });
	sourceRec.push_back({ 0, 0, (float)stop_button.width, frameHeight });
	sourceRec.push_back({ 0, 0, (float)stop_button.width, frameHeight });


	vector<int> btnState = { 0,0,0 };
	vector<bool> btnAction = {false,false,false};
	vector<Rectangle> btnBounds = { { 750, 42, (float)stop_button.width, frameHeight } , { 750, 42 + static_cast<float>(1) * 60.0f, (float)stop_button.width, frameHeight } , { 750, 42 + static_cast<float>(2) * 60.0f, (float)stop_button.width, frameHeight } };
	
		
	// Button state: 0-NORMAL, 1-MOUSE_HOVER, 2-PRESSED


	Vector2 mousePoint = { 0.0f, 0.0f };





	// DEFAULT STATION TEXTURE LOADING
	for (int i = 1; i < metro_line.size() - 1; i++) {
		metro_line[i]->station_texture = LoadTexture(ASSETS_PATH"station_asset.png");
	}

	start_thread(0, metro_line, metro_subway, subway_thread);

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(WHITE);

		mousePoint = GetMousePosition();
		sourceRec[0].y = btnState[0] * frameHeight;
		sourceRec[1].y = btnState[1] * frameHeight;
		sourceRec[2].y = btnState[2] * frameHeight;


		DrawTextureEx(background, { 0,0 }, 0, 1, WHITE); // draw background

		// DRAWING STATION FORWARD
		for (int i = 0; i < metro_line.size(); i++) {
			DrawTextureEx(metro_line[i]->station_texture, { metro_line[i]->station_location.x - ceil((float)metro_line[i]->station_texture.width / (float)2), 300 }, 0, 1, WHITE);
			if (i != 0 && i != metro_line.size() - 1) {
				string text = "People in station fw -> " + to_string(metro_line[i]->get_people_forward());
				const Vector2 text_size = MeasureTextEx(GetFontDefault(), text.c_str(), 20, 1);
				DrawText(text.c_str(), metro_line[i]->station_location.x - ceil((float)text_size.x / (float)2), 425 + text_size.y, 20, BLACK);
			}
		}

		// DRAWING STATION RETURN
		for (int i = 0; i < metro_line.size(); i++) {
			DrawTextureEx(metro_line[i]->station_texture, { metro_line[i]->station_location.x - ceil((float)metro_line[i]->station_texture.width / (float)2), 510 }, 0, 1, WHITE);
			if (i != 0 && i != metro_line.size() - 1) {
				string text = "People in station rtn -> " + to_string(metro_line[i]->get_people_return());
				const Vector2 text_size = MeasureTextEx(GetFontDefault(), text.c_str(), 20, 1);
				DrawText(text.c_str(), metro_line[i]->station_location.x - ceil((float)text_size.x / (float)2), 640 + text_size.y, 20, BLACK);
			}
		}

		// DRAWING SUBWAY
		


		for (int i = 0; i < metro_subway_active.size(); ++i) {
			DrawTextureEx(metro_subway_active[i]->sub_texture, { metro_subway_active[i]->coordinates.x - ceil((float)metro_subway_active[i]->sub_texture.width / (float)2), metro_subway_active[i]->coordinates.y }, 0, 1, WHITE);
			string text = "Metro " + to_string(metro_subway_active[i]->get_id()) +
				" | People: " + to_string(metro_subway_active[i]->get_people()) +
				" | Speed: " + to_string(metro_subway_active[i]->get_speed()) +
				" | Direction: " + (metro_subway_active[i]->get_direction() ? "Forward" : "Return") +
				" | Station ID: " + to_string(metro_subway_active[i]->get_station_id());
			DrawText(text.c_str(), 50, 50 + i*60, 20, BLACK);
			DrawTextureRec(stop_button, sourceRec[i], Vector2{ 750.0f, 42 + static_cast<float>(i) * 60.0f }, WHITE);
			
			

			if (CheckCollisionPointRec(mousePoint, btnBounds[0]))
			{
				if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) btnState[0] = 2;
				else btnState[0] = 1;

				if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
					btnAction[0] = true;
					// TODO: Trigger the action corresponding to the clicked button
					// For example, you can use 'i' to identify which button was clicked
				}
			}
			else btnState[0] = 0;

			if (btnAction[0])
			{
				// TODO: Any desired action
			}

			if (CheckCollisionPointRec(mousePoint, btnBounds[1]))
			{
				if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) btnState[1] = 2;
				else btnState[1] = 1;

				if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
					btnAction[1] = true;
					// TODO: Trigger the action corresponding to the clicked button
					// For example, you can use 'i' to identify which button was clicked
				}
			}
			else btnState[1] = 0;

			if (btnAction[1])
			{
				// TODO: Any desired action
			}

			if (CheckCollisionPointRec(mousePoint, btnBounds[2]))
			{
				if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) btnState[2] = 2;
				else btnState[2] = 1;

				if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
					btnAction[2] = true;
					// TODO: Trigger the action corresponding to the clicked button
					// For example, you can use 'i' to identify which button was clicked
				}
			}
			else btnState[2] = 0;

			if (btnAction[2])
			{
				// TODO: Any desired action
			}
		}


		
		EndDrawing();
	}

	CloseWindow();

	for(int i = 0; i < metro_subway.size(); ++i) { subway_thread[i].join(); } // on attends que les threads aie tous terminés

	for(int j = 0; j < metro_line.size(); j++) { cout << metro_line[j]->get_people_forward() << " : fw || " << metro_line[j]->get_people_return() << " : rtn" << endl;	} // debug

	// UNLOADING TEXTURES //

	UnloadTexture(stop_button);
	UnloadTexture(background);
	for (int i = 0; i < metro_line.size(); i++) { UnloadTexture(metro_line[i]->station_texture); }
	for (int i = 0; i < metro_subway.size(); ++i) {	UnloadTexture(metro_subway[i]->sub_texture); }

	return 0;
}

