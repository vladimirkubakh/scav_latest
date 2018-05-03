#include "game_object.hpp"

//GameObject
GameObject::GameObject(int _id): id(_id) {
}

GameObject::~GameObject() {
}

void GameObject::set_id(int _id) {
	id = _id;
}

int GameObject::get_id() {
	return id;
}

int GameObject::texture() {
	return 0;
}

//Player
Player::Player(int _id, b2World* _world, const b2Vec2& size,const b2Vec2& pos) 
				: StaticObject(_id, _world, size, pos) {
}

Player::~Player() {
	//body->SetActive(false);
	//this->body->GetWorld()->DestroyBody(body);
}


AidKit::AidKit(int _id, b2World* _world, const b2Vec2& size,const b2Vec2& pos) :
			Entity(_id, _world, size, pos) {
	

}

AidKit::~AidKit() {

}

int AidKit::object_type() {
	return 3;
}

void AidKit::interact(PhysicsObject* object, sf::Packet* packet) {
	object->set_hp(100);
	*packet << 5 << object->get_id() << 100;
}

int AidKit::texture() {
	return 9;
}

LandingMine::LandingMine(int _id, b2World* _world, const b2Vec2& size,const b2Vec2& pos) :
			Entity(_id, _world, size, pos) {
	

}

LandingMine::~LandingMine() {

}

int LandingMine::texture() {
	return 10;
}

int LandingMine::object_type() {
	return 3;
}

void LandingMine::interact(PhysicsObject* object, sf::Packet* packet) {
	object->set_hp(object->get_hp() - 50);
	*packet << 5 << object->get_id() << object->get_hp();
}
