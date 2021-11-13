#include "../../include/scenes/SceneEndGame.h"



SceneEndGame::SceneEndGame(Gui * gui, float const &dtTime)
: SceneMenu(gui, dtTime)
{
}


SceneEndGame::SceneEndGame(SceneEndGame const & src)
: SceneMenu(src)
{
	*this = src;
}


SceneEndGame::~SceneEndGame() {}


SceneEndGame & SceneEndGame::operator=(SceneEndGame const & rhs) {
	
	return *this;
}



bool			SceneEndGame::init() {
	glm::vec2 winSz = _gui->gameInfo.windowSize;
	glm::vec2 tmpPos;
	glm::vec2 tmpSize;
	float menuWidth = winSz.x * 0.8;
	float menuHeight = winSz.y / 14;

	//UI init

	return true;
}


void SceneEndGame::load() {
	SceneMenu::load();
}


bool	SceneEndGame::update() {
	SceneMenu::update();
	SceneGame & scGame = *reinterpret_cast<SceneGame *>(SceneManager::getScene(SceneNames::GAME));
	if (s.j("debug").b("3d-menu")) {
		scGame.updateForMenu();
	}

	if (_states.exit || Inputs::getKeyUp(InputType::ACTION)) {
		_states.exit = false;
		SceneManager::loadScene(SceneNames::MAIN_MENU);
	}
	return true;
}

bool SceneEndGame::draw() {
	bool ret = true;

	/* 3d background */
	if (s.j("debug").b("3d-menu")) {
		SceneGame & scGame = *reinterpret_cast<SceneGame *>(SceneManager::getScene(SceneNames::GAME));
		ret = scGame.drawEndGame();  // draw the game if possible
	}
	ret = SceneMenu::draw();
	return ret & true;
}
