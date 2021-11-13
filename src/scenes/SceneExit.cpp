#include "../../include/scenes/SceneExit.h"
//#include "Save.hpp"


SceneExit::SceneExit(Gui * gui, float const &dtTime)
: SceneMenu(gui, dtTime),
  _lastSceneName(SceneNames::MAIN_MENU)
{}

SceneExit::SceneExit(SceneExit const & src)
: SceneMenu(src)
{
	*this = src;
}

SceneExit::~SceneExit() {}

SceneExit & SceneExit::operator=(SceneExit const & rhs) {
	return *this;
}


bool			SceneExit::init() {
	glm::vec2 winSz = _gui->gameInfo.windowSize;
	glm::vec2 tmpPos;
	glm::vec2 tmpSize;
	float menuWidth = winSz.x / 2;
	float menuHeight = winSz.y / 14;

	//UI init

	return true;
}


bool	SceneExit::update() {
	_updateUI();
	SceneMenu::update();

	if (_states.exit) {
		_states.exit = false;
		SceneManager::quit();
	}
	else if (_states.cancel) {
		_states.cancel = false;
		SceneManager::loadScene(_lastSceneName);
	}
	return true;
}


void SceneExit::load() {
	SceneMenu::load();
	if (SceneManager::getSceneName() != SceneNames::EXIT) {
		_lastSceneName = SceneManager::getSceneName();
	}
}


void SceneExit::unload() {
	SceneMenu::unload();
}


bool SceneExit::draw() {
	bool ret = true;

	/* 3d background */
	
	return ret & true;
}


void	SceneExit::_updateUI() {
	glm::vec2 winSz = _gui->gameInfo.windowSize;
	glm::vec2 tmpPos;
	glm::vec2 tmpSize;
	float menuWidth = winSz.x / 2;
	float menuHeight = winSz.y / 14;

	//UI update
}
