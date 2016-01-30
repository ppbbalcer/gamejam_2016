#include "Character.h"
#include "GlobalData.h"
#include "Engine/Engine.h"
#include "fireball.h"

using namespace std;

Character::Character(RTexture* texture, IMap * map)
{
	_texture = texture;
	power_level = DEFAULT_POWER_LEVEL;
	_map = map;
	_health = MAX_HEALTH;
	_state = ALIVE;
	_pos_after_x =
		_pos_before_x =
			_pos_after_y =
				_pos_before_y = 0;
	last_dir_x = 1;
	last_dir_y = 0;
}

Character::~Character()
{
	delete _texture;
	_texture = NULL;
}

void Character::setPosTiles(int x, int y)
{
	_map->GetFieldAt(_pos_before_x, _pos_before_y)->LeftField();
	_pos_after_x = x;
	_pos_before_x = x;
	_pos_after_y = y;
	_pos_before_y = y;
	_map->GetFieldAt(_pos_after_x, _pos_before_y)->SteppedOver(this);
	setPos((float)x * EngineInst->getTileSize(), (float)y * EngineInst->getTileSize());
}

int Character::getHealth()
{
	return _health;
}

int Character::crucio(int howMuchCrucio)
{
	_health = max<int>(_health - howMuchCrucio, 0);
	printf("crucio! %d\n", _health);
	if (_health == 0) {
		_state = DEAD;
		_map->GetFieldAt(_pos_before_x, _pos_before_y)->LeftField();
		_map->GetFieldAt(_pos_after_x, _pos_after_y)->LeftField();
	}
	return _health;
}

int Character::GetState()
{
	return _state;
}

void Character::heal(int howMuchHeal)
{
	_health = min<int>(_health + howMuchHeal, MAX_HEALTH);
}

int Character::GetPowerLevel()
{
	return power_level;
}
int Character::SetPowerLevel(int x)
{
	return (power_level = x);
}


void Character::OnRenderCircle()
{
	// precalc
}

bool Character::canSee(int x0, int y0)
{
	int x1 = getPosAfterX();
	int y1 = getPosAfterY();
	bool landscape = abs(x1 - x0) > abs(y1 - y0);
	int num_fields = std::max<int>(abs(x1 - x0) , abs(y1 - y0));
	bool negative = landscape ? ( x0 > x1) : (y0 > y1);
	int sign = negative ? -1 : +1;
	int x, y;
	// iterate from (x0, y0 to x1, y1)
	for (int i = 0 ; i != num_fields; ++i) {
		if (landscape) {
			x = x0 + i * sign;
			y = y0 + i * sign * ( y1 - y0 ) / (x1 - x0);
		} else {
			y = y0 + i * sign;
			x = x0 + i * sign * ( x1 - x0 ) / (y1 - y0);
		}
		if (x<0 || y<0 || x>= _map->GetWidth()
		    || y>= _map->GetHeight()) {

		} else {
			IField * f = _map->GetFieldAt(x,y);
			if (!f) return false;
			if (f->IsObstacle() && ! f->WhoIsHere()) return false;
		}

	}
	return true;
}


void Character::OnRender(SDL_Renderer *renderer)
{
	if (GetState() == DEAD) {
		_texture->renderTile(renderer, (int)getPosX(), (int)getPosY(), 30, SDL_FLIP_NONE);
	} else if (GetState() == ALIVE) {
		_texture->renderTile(renderer, (int)getPosX(), (int)getPosY());
	}
}

void Character::setPos(float x, float y)
{
	_posX = x;
	_posY = y;
}

float Character::getPosX()
{
	return _posX;
}
float Character::getPosY()
{
	return _posY;
}

int Character::getPosBeforeX()
{
	return _pos_before_x;
}

int Character::getPosAfterX()
{
	return _pos_after_x;
}

int Character::getPosBeforeY()
{
	return _pos_before_y;
}

int Character::getPosAfterY()
{
	return _pos_after_y;
}

void Character::updateDirection(DIRECT directMove)
{
	if (_state != ALIVE)
		return;

	int prev_target_x = _pos_after_x;
	int prev_target_y = _pos_after_y;
	float tileSize = (float)EngineInst->getTileSize();

	int closeTileX = (_posX + tileSize*0.5f)/tileSize;
	int closeTileY = (_posY + tileSize*0.5f)/tileSize;

	//Protect move two tiles
	float distX = _posX - closeTileX*tileSize;
	float distY = _posY - closeTileY*tileSize;
	if (getType() == TYPE_ENEMY) {
		distX = 0;
		distY = 0;
	}



	switch (directMove) {
	case DIRECT_DOWN:
		if ( (closeTileY <  _map->GetHeight() - 1) 
			&& (distY >= 0) 
			&& (!_map->GetFieldAt(closeTileX, closeTileY + 1)->IsObstacle()) )
			//&& (!_map->GetFieldAt(_pos_before_x, _pos_before_y + 1)->IsObstacle())
		{
			_pos_after_x = closeTileX;
			_pos_after_y = closeTileY + 1;
		}
		break;
	case DIRECT_UP:
		if ((closeTileY > 0) 
			&& (distY <= 0) 
			&& (!_map->GetFieldAt(closeTileX, closeTileY - 1)->IsObstacle()) )
		// && (!_map->GetFieldAt(_pos_before_x, _pos_before_y - 1)->IsObstacle())
		{
			
			_pos_after_x = closeTileX;
			_pos_after_y = closeTileY - 1;
		}
		break;

	case DIRECT_RIGHT:
		if ((closeTileX <  _map->GetWidth() - 1) 
			&& (distX >= 0) 
			&& (!_map->GetFieldAt(closeTileX + 1, closeTileY)->IsObstacle()) )
		 //&& (!_map->GetFieldAt(_pos_before_x + 1, _pos_before_y)->IsObstacle())
		{
			_pos_after_y = closeTileY;
			_pos_after_x = closeTileX + 1;
		}
		_texture->setFlip(SDL_FLIP_NONE);
		break;

	case DIRECT_LEFT:
		if ((closeTileX  > 0) 
			&& (distX <= 0) 
			&&(!_map->GetFieldAt(closeTileX - 1,closeTileY)->IsObstacle()) )
		//&&  (!_map->GetFieldAt(_pos_before_x - 1, _pos_before_y)->IsObstacle())
		{
			_pos_after_y = closeTileY;
			_pos_after_x = closeTileX - 1;
		}
		_texture->setFlip(SDL_FLIP_HORIZONTAL);
		break;

	default:
		printf("Unrecognized direction %d given\n", directMove);
		break;
	}

	if  ( (prev_target_y!=_pos_after_y ||
	       prev_target_x!=_pos_after_x) ) {
		//! if leaving old movement target

		// if original target was NOT a origin
		if (!(prev_target_y==_pos_before_y &&
		      prev_target_x==_pos_before_x))
			_map->GetFieldAt(prev_target_x, prev_target_y)
				->LeftField();
		if (!_map->GetFieldAt(_pos_after_x, _pos_after_y)
		    ->IsOccupied())
			
			_map->GetFieldAt(_pos_after_x, _pos_after_y)
				->SteppedOver(this);
	}

}

void Character::OnUpdate(int time_ms)
{
	int tile_size = EngineInst->getTileSize();
	int target_y = _pos_after_y * tile_size;
	int target_x = _pos_after_x * tile_size;
	float dist = (float)tile_size * (float)time_ms * 0.007f;
	bool finishX = false;
	bool finishY = false;


	if (getType() == TYPE_ENEMY) {
		dist /= 5.0f;
	}

	float pos_x = getPosX();
	float pos_y = getPosY();

	if (_state != ALIVE)
		return;

	/* Update direction for fireball */
	if (pos_y > target_y) {
		last_dir_y = -1;
		last_dir_x = 0;
	} else if (pos_y < target_y) {
		last_dir_y = 1;
		last_dir_x = 0;
	}
	if (pos_x > target_x) {
		_texture->setFlip(SDL_FLIP_HORIZONTAL);
		last_dir_y = 0;
		last_dir_x = -1;
	} else if (pos_x < target_x) {
		_texture->setFlip(SDL_FLIP_NONE);
		last_dir_y = 0;
		last_dir_x = +1;
	}


	if (pos_x > target_x) {
		pos_x -=  dist;
		if(pos_x <= target_x) {
			pos_x = target_x;
			//Event
			finishX = true;
		}
	} else if (pos_x < target_x) {
		pos_x +=  dist;
		if(pos_x >= target_x) {
			pos_x = target_x;
			//Event
			finishX = true;
		}
	}

	if (pos_y > target_y) {
		pos_y -= dist;
		if(pos_y <= target_y) {
			pos_y = target_y;
			//Event
			finishY = true;
		}
	}  else if (pos_y < target_y) {
		pos_y += dist;
		if(pos_y >= target_y) {
			pos_y = target_y;
			//Event
			finishY = true;
		}
	}


	if ( target_x == pos_x || finishX) {
		if (_pos_before_x != _pos_after_x) {
			
			_map->GetFieldAt(_pos_before_x, _pos_before_y)->LeftField();
			if (!_map->GetFieldAt(_pos_after_x, _pos_before_y)
			    ->IsOccupied())
				_map->GetFieldAt(_pos_after_x, _pos_before_y)->SteppedOver(this);
			_pos_before_x = _pos_after_x;
		}
	}

	if ( target_y == pos_y || finishY) {
		if (_pos_before_y != _pos_after_y) {
			
			_map->GetFieldAt(_pos_before_x, _pos_before_y)
			->LeftField();
			if (!_map->GetFieldAt(_pos_before_x, _pos_after_y)
			    ->IsOccupied())
				_map->GetFieldAt(_pos_before_x, _pos_after_y)
					->SteppedOver(this);
			_pos_before_y = _pos_after_y;
		}
	}

	if (_pos_before_y == 0 || _pos_before_y == _map->GetHeight() - 1 ||
	                _pos_before_x == 0 || _pos_before_x == _map->GetWidth() - 1) {
		Win();
	}
	setPos(pos_x, pos_y);
}
void Character::Win() {
		
}

void Character::renderAvatar(SDL_Renderer *renderer, int x, int y, SDL_RendererFlip flip)
{
	if (GetState() == DEAD) {
		_texture->renderTile(renderer, x, y, 30, SDL_FLIP_NONE);
	} else {
		_texture->renderTile(renderer, x, y, _texture->getTileIdx(), flip);
	}
}


