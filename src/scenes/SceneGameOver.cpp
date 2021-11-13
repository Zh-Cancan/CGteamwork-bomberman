#include "../../include/scenes/SceneGameOver.h"
#include "../../include/scenes/SceneGame.h"


SceneGameOver::SceneGameOver(Gui * gui, float const &dtTime)
: SceneMenu(gui, dtTime),
  _lastSceneName(SceneNames::MAIN_MENU)
{
}


SceneGameOver::SceneGameOver(SceneGameOver const & src)
: SceneMenu(src)
{
	*this = src;
}


SceneGameOver::~SceneGameOver() {}


SceneGameOver & SceneGameOver::operator=(SceneGameOver const & rhs) {
	return *this;
}


bool			SceneGameOver::init() {
	glm::vec2 winSz = _gui->gameInfo.windowSize;
	glm::vec2 tmpPos;
	glm::vec2 tmpSize;
	float menuWidth = winSz.x / 2;
	float menuHeight = winSz.y / 14;

	//UI

	return true;
}


bool	SceneGameOver::update() {
	SceneMenu::update();
	SceneGame & scGame = *reinterpret_cast<SceneGame *>(SceneManager::getScene(SceneNames::GAME));
	if (s.j("debug").b("3d-menu")) {
		scGame.updateForMenu();
	}

	if (_states.restart) {
		
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

void SceneGameOver::load() {
	SceneMenu::load();
	if (SceneManager::getSceneName() != SceneNames::EXIT) {
		_lastSceneName = SceneManager::getSceneName();
	}
}


bool SceneGameOver::draw() {
	bool ret = true;

	/* 3d background */
	if (s.j("debug").b("3d-menu")) {
		SceneGame & scGame = *reinterpret_cast<SceneGame *>(SceneManager::getScene(SceneNames::GAME));
		ret = scGame.drawGameOver();  // draw the game if possible
	}
	ret = SceneMenu::draw();
	return ret & true;
}
