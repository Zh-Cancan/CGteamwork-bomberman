#include "../../include/scenes/SceneDifficulty.h"

SceneDifficulty::SceneDifficulty(Gui * gui, float const &dtTime)
: SceneMenu(gui, dtTime)
{}


SceneDifficulty::SceneDifficulty(SceneDifficulty const & src)
: SceneMenu(src)
{
	*this = src;
}

SceneDifficulty::~SceneDifficulty() {}


SceneDifficulty & SceneDifficulty::operator=(SceneDifficulty const & rhs) {
	if (this != &rhs) {
		logWarn("you are copying SceneDifficulty")
	}
	return *this;
}


/**
 * @brief 初始化menu
 *
 * @return 初始化是否成功
 */
bool			SceneDifficulty::init() {
	glm::vec2 winSz = _gui->gameInfo.windowSize;
	glm::vec2 tmpPos;
	glm::vec2 tmpSize;
	float menuWidth = winSz.x / 2;
	float menuHeight = winSz.y / 14;

	//UI element
	//....

	return true;
}

/**
 * @brief called when the scene is loaded
 */
void SceneDifficulty::load() {
	SceneMenu::load();
	_updateUI();
}

/**
 * @brief this is the update function (called every frames)
 *
 * @return true if the update is a success
 * @return false if there are an error in update
 */
bool	SceneDifficulty::update() {
	SceneMenu::update();
	/*if (_states.beginner) {
		_states.beginner = false;
		Save::newGame();
		Save::setDifficulty(10);
		SceneManager::loadScene(SceneNames::LEVEL_SELECTION);
	}
	else if (_states.easy) {
		_states.easy = false;
		Save::newGame();
		Save::setDifficulty(3);
		SceneManager::loadScene(SceneNames::LEVEL_SELECTION);
	}
	else if (_states.medium) {
		_states.medium = false;
		Save::newGame();
		Save::setDifficulty(2);
		SceneManager::loadScene(SceneNames::LEVEL_SELECTION);
	}
	else if (_states.hardCore) {
		_states.hardCore = false;
		Save::newGame();
		Save::setDifficulty(1);
		SceneManager::loadScene(SceneNames::LEVEL_SELECTION);
	}
	else if (_states.menu || Inputs::getKeyUp(InputType::CANCEL)) {
		_states.menu = false;
		SceneManager::loadScene(SceneNames::MAIN_MENU);
	}*/
	return true;
}

// -- Private methods ----------------------------------------------------------

/**
 * @brief Update UI objects.
 *
 */
void		SceneDifficulty::_updateUI() {
	/*glm::vec2 winSz = _gui->gameInfo.windowSize;
	glm::vec2 tmpPos;
	glm::vec2 tmpSize;
	float menuWidth = winSz.x / 2;
	float menuHeight = winSz.y / 14;
	tmpPos.x = (winSz.x / 2) - (menuWidth / 2);
		tmpPos.y = winSz.y - menuHeight * 2;
		tmpSize.x = menuWidth;
		tmpSize.y = menuHeight;
	tmpPos.y -= menuHeight * 1.8;
	allUI.beginner->setPos(tmpPos).setSize(tmpSize);
	tmpPos.y -= menuHeight * 1.3;
	allUI.easy->setPos(tmpPos).setSize(tmpSize);
	tmpPos.y -= menuHeight * 1.3;
	allUI.medium->setPos(tmpPos).setSize(tmpSize);
	tmpPos.y -= menuHeight * 1.3;
	allUI.hardCore->setPos(tmpPos).setSize(tmpSize);
	tmpSize.x = tmpSize.x * 1.3;
	tmpSize.y = winSz.y - tmpPos.y - menuHeight * 1.8;
	tmpPos.x = (winSz.x / 2) - ((menuWidth * 1.3) / 2);
	tmpPos.y -= menuHeight * 0.5;
	allUI.border->setPos(tmpPos).setSize(tmpSize);*/
}
