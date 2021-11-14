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
 * @brief ��ʼ��menu
 *
 * @return ��ʼ���Ƿ�ɹ�
 */
bool			SceneMainMenu::init() {
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
 * @brief ���º��� ÿһ֡(frame)����һ��
 *
 * @return �Ƿ���³ɹ�
 */
bool	SceneMainMenu::update() {
	SceneMenu::update();
	if(true){
	/*if (_states.newGame) {*/
		_states.newGame = false;
		SceneManager::loadScene(SceneNames::LEVEL_SELECTION);
	}
	else if (_states.exit) {
		_states.exit = false;
		SceneManager::loadScene(SceneNames::EXIT);
	}
	return true;
}



/**
 * @brief ����UI
 */
void		SceneMainMenu::_updateUI() {
	//...
}
