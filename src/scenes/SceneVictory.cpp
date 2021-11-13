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

/**
 * @brief init the menu
 *
 * @return true if the init succeed
 * @return false if the init failed
 */
bool			SceneVictory::init() {
	glm::vec2 winSz = _gui->gameInfo.windowSize;
	glm::vec2 tmpPos;
	glm::vec2 tmpSize;
	float menuWidth = winSz.x / 2;
	float menuHeight = winSz.y / 14;
	float statisticHeight = menuHeight * 0.6;

	//UI
	
		
	return true;
}

/**
 * @brief this is the update function (called every frames)
 *
 * @return true if the update is a success
 * @return false if there are an error in update
 */
bool	SceneVictory::update() {
	SceneMenu::update();
	SceneGame & scGame = *reinterpret_cast<SceneGame *>(SceneManager::getScene(SceneNames::GAME));
	if (s.j("debug").b("3d-menu")) {
		scGame.updateForMenu();
	}
	
	//¸üÐÂUI

	if (_states.nextLevel) {
		
		_states.nextLevel = false;
		if (scGame.level + 1 < static_cast<int32_t>(scGame.getNbLevel())) {
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

/**
 * @brief called when the scene is loaded
 */
void SceneVictory::load() {
	SceneMenu::load();
	if (SceneManager::getSceneName() != SceneNames::EXIT) {
		_lastSceneName = SceneManager::getSceneName();
	}
}

/**
 * @brief this is the draw function (called every frames)
 *
 * @return true if the draw is a success
 * @return false if there are an error in draw
 */
bool SceneVictory::draw() {
	bool ret = true;

	/* 3d background */
	if (s.j("debug").b("3d-menu")) {
		SceneGame & scGame = *reinterpret_cast<SceneGame *>(SceneManager::getScene(SceneNames::GAME));
		ret = scGame.drawVictory();  // draw the game if possible
	}
	ret = SceneMenu::draw();
	return ret & true;
}
