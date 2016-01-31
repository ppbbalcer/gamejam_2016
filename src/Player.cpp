#include "Player.h"
#include "GlobalData.h"
#include "Engine/RTexture.h"
#include "Equipment.h"
#include <algorithm>

using namespace std;

Player::Player(RTexture * texture, IMap * map, int hp, int mana, int ammo, int traps) : Character(texture, map)
{
	equipment = Equipment();
	_type = TYPE_PLAYER;
	_mana = mana;
	_health = hp;
	printf("PLAYER: %u %u\n", _health, _mana);
	_time_to_shot = 0;
	_time_to_arm = 0;
	arming = 0;
	viewangle = M_PI;

	inventory[ITEM_AMMO].available = ammo;
	inventory[ITEM_AMMO].capacity = ammo;
	inventory[ITEM_TRAP].available = traps;
	inventory[ITEM_TRAP].capacity = traps;
};

Player::~Player(void) {};

void Player::onDirectionUpdate()
{
	arming = 0;
	_time_to_arm = 0;
}

void Player::placeTrap()
{
	if (inventory[ITEM_TRAP].available <= 0)
		return;

	if (arming == 0) {
		arming = 1;
		_time_to_arm = TRAP_ARM_TIME;
	}
}

void Player::modInventoryItemCount(inventory_item item, int mod)
{
	inventory[item].available += mod;

	inventory[item].available = max(inventory[item].available, 0);
	inventory[item].available = min(inventory[item].available, inventory[item].capacity);
}

int Player::getInvetoryInfo(inventory_item item, inventory_value_type type)
{
	return type == ITEM_CURRENT ? inventory[item].available : inventory[item].capacity;
}

int Player::getMana()
{
	return _mana;
}
void Player::Win()
{
	printf("WIN");
	SetState(WON);
	_map->GetFieldAt(getPosBeforeX(), getPosBeforeY())->LeftField();
}

void Player::restoreMana(int howMuchMana)
{
	_mana += howMuchMana;
	if (_mana > MAX_MANA) {
		_mana = MAX_MANA;
	}
}
void Player::OnUpdate(int time_ms)
{
	_time_to_shot=max<int>(_time_to_shot-time_ms,
			       0);

	if (arming) {
		_time_to_arm -= time_ms;
		if (_time_to_arm <= 0) {
			_time_to_arm = 0;
			modInventoryItemCount(ITEM_TRAP, -1);
			_map->placeObstruction(getPosAfterX(), getPosAfterY(), OBSTRUCTION_TRAP);
			arming = 0;
		}
	}

	if (getPosAfterX() == getPosBeforeX()
	    && getPosAfterY() == getPosBeforeY()) {
		_texture->setTileIdx(3);
			
	} else {
		std::vector<int > w;
		w.push_back(0);
		w.push_back(1);
		w.push_back(2);
		w.push_back(1);
		AnimateFrames(time_ms, w);
	}

	Character::OnUpdate(time_ms);
}

Fireball * Player::Shoot()
{
	clock_t now = clock();

	if (GetState() != ALIVE)
		return NULL;

	if (_time_to_shot)
		return NULL;

	if (inventory[ITEM_AMMO].available <= 0)
		return NULL;

	modInventoryItemCount(ITEM_AMMO, -1);

	_time_to_shot=FIREBALL_MIN_DIFF_MS;

	globalAudios[GameSounds::FIREBALL].res.sound->play();

	return new Fireball(getPosBeforeX(),
	                    getPosBeforeY(),
	                    last_dir_x, last_dir_y,
			    GetPowerLevel(),this);
}

void drawBar(SDL_Renderer *renderer, float pct, int x, int  y, int r, int g, int b) {
	SDL_Rect rect = { x, y, 100*pct, 10 };

	SDL_SetRenderDrawColor(renderer, r, g, b, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRect(renderer, &rect);
	//Frame
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
//	rect.x = defaultX - 200;
	rect.w = 100;
	SDL_RenderDrawRect(renderer, &rect);
	rect.x++;
	rect.y++;
	rect.w -= 2;
	rect.h -= 2;
	SDL_RenderDrawRect(renderer, &rect);
}

void Player::OnRender(SDL_Renderer *renderer, SDL_Point *camera)
{
	Character::OnRender(renderer, camera);

	if (arming && _time_to_arm > 0) {
		drawBar(renderer, (float)_time_to_arm / (float)TRAP_ARM_TIME, getPosX() - camera->x - 30, getPosY() - camera->y - 10, 200, 200, 200);
	}

}

int Player::crucio(int howMuchCrucio)
{
	int hpRem = Character::crucio(howMuchCrucio);
	if (hpRem <= 0) {
		globalAudios[PLAYER_DEATH].res.sound->play();
	} else {
		globalAudios[PLAYER_PAIN].res.sound->play();
	}
	return hpRem;
}
