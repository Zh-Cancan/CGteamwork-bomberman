#include "../../include/scenes/SceneVictory.h"
#include "../../include/scenes/SceneGame.h"

SceneVictory::SceneVictory(Gui * gui, float const &dtTime)
: SceneMenu(gui, dtTime),
  _lastSceneName(SceneNames::MAIN_MENU)
{
}

SceneVictory::SceneVictory(SceneVictory const & src)
: SceneMenu(src)
{
	*this = src;
}

SceneVictory::~SceneVictory() {}


SceneVictory & SceneVictory::operator=(SceneVictory const & rhs) {
	return *this;
}


bool			SceneVictory::init() {

	//UI
	
		
	return true;
}


bool	SceneVictory::update() {
	SceneMenu::update();
	SceneGame & scGame = *reinterpret_cast<SceneGame *>(SceneManager::getScene(SceneNames::GAME));
	
	scGame.updateForMenu();

	
	//¸üÐÂUI

	if (_states.nextLevel) {
		
		_states.nextLevel = false;
		if (scGame.level + 1 < 10) {
			try {
				// reload the current level
				if (!scGame.loadLevel(scGame.level + 1)) {
					return false;
				}
			} catch (std::exception const &e) {
				SceneManager::loadScene(SceneNames::LEVEL_SELECTION);
				return true;
			}
			SceneManager::loadScene(_lastSceneName);
		}
		else {
			SceneManager::loadScene(SceneNames::END_GAME);
		}
	}
	else if (_states.restart) {
		_states.restart = false;
		// reload the current level
		if (!scGame.loadLevel(scGame.level)) {
			return false;
		}
		SceneManager::loadScene(_lastSceneName);
	}
	else if (_states.menu) {
		
		_states.menu = false;
		SceneManager::loadScene(SceneNames::MAIN_MENU);
	}
	else if (_states.exit) {
		
		_states.exit = false;
		SceneManager::loadScene(SceneNames::EXIT);
	}
	return true;
}


void SceneVictory::load() {
	SceneMenu::load();
	if (SceneManager::getSceneName() != SceneNames::EXIT) {
		_lastSceneName = SceneManager::getSceneName();
	}
}


bool SceneVictory::draw() {
	bool ret = true;

	/* 3d background */
	SceneGame & scGame = *reinterpret_cast<SceneGame *>(SceneManager::getScene(SceneNames::GAME));
	ret = scGame.drawVictory();  
	
	ret = SceneMenu::draw();
	return ret & true;
}
