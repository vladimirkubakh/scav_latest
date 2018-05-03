#include "scav_game.hpp"
#include "Box2D/Box2D.h"
#include <thread>
#include <unistd.h>
#include <mutex>
#include "textures.hpp"
bool online = true;
Textures* textures;

void get(Connector* connector, GameField* field) {
	while (online) {
		sf::Packet* packet = connector->get();

		while (!packet->endOfPacket()) {
			ActionConstructor::execute_action(field, *packet, textures);
		}
		delete packet;
	}
}

void render(GameField* field) {
	b2World* world = field->get_physics_world();
	std::mutex& mtx = field->get_mutex();
	

	ContactListener l;
	world->SetContactListener(&l);
  		
	while (field->render()) {
		mtx.lock();
        l.execute_actions(*field);
        world->Step(1/60.f, 8, 3);
        mtx.unlock();
	}

	online = false;
}

void send(Connector* connector, GameField* field) {
	while (online) {
		sf::Packet packet;
		if (field->get_action(packet)) {
			connector->send(&packet);
		}
		usleep(20000);
	}
}

int main(int argc, char const *argv[])
{
	textures = new Textures("textures.txt");
    GameField field;
    std::string name;
    std::string ip;
    int port = 55503;
	if (argc < 3) {
		std::cout << "Input ip and port" << std::endl;
		Menu menu(field.get_window(), "scav_bg.jpg", "minecraft.otf");
		menu.draw();
		name = menu.get_name();
		ip = menu.get_ip();
		port = menu.get_port();
	}
	else {
		ip = argv[1];
		port = std::stoi(argv[2]);
	}

	Connector connector(ip, port);

	std::thread get_thread(get, &connector, &field);
	std::thread send_thread(send, &connector, &field);
	std::thread render_thread(render, &field);

	get_thread.join();
	send_thread.join();
	render_thread.join();
	delete textures;
	return 0;
}
