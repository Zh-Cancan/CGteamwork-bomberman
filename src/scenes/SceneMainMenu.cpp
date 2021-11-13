#include "../../include/scenes/SceneMainMenu.h"


SceneMainMenu::SceneMainMenu(Gui * gui, float const &dtTime)
: SceneMenu(gui, dtTime)
{}


SceneMainMenu::SceneMainMenu(SceneMainMenu const & src)
: SceneMenu(src)
{
	*this = src;
}

SceneMainMenu::~SceneMainMenu() {}


SceneMainMenu & SceneMainMenu::operator=(SceneMainMenu const & rhs) {
	return *this;
}


/**
 * @brief 初始化menu
 *
 * @return 初始化是否成功
 */
bool			SceneMainMenu::init() {
	glm::vec2 winSz = _gui->gameInfo.windowSize;
	glm::vec2 tmpPos;
	glm::vec2 tmpSize;
	float menuWidth = winSz.x / 2;
	float menuHeight = winSz.y / 14;

	//UI

	return true;
}

/**
 * @brief called when the scene is loaded
 */
void SceneMainMenu::load() {
	SceneMenu::load();
	_updateUI();
}

/**
 * @brief 更新函数 每一帧(frame)更新一次
 *
 * @return 是否更新成功
 */
bool	SceneMainMenu::update() {
	SceneMenu::update();
	if (_states.continueGame) {
		_states.continueGame = false;
		SceneManager::loadScene(SceneNames::LEVEL_SELECTION);
	}
	if (_states.newGame) {
		_states.newGame = false;
	}
	else if (_states.loadGame) {
		_states.loadGame = false;
		SceneManager::loadScene(SceneNames::LOADGAME);
	}
	else if (_states.exit) {
		_states.exit = false;
		SceneManager::loadScene(SceneNames::EXIT);
	}
	return true;
}



/**
 * @brief 更新UI
 */
void		SceneMainMenu::_updateUI() {
	//...
}
