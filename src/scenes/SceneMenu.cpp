#include "../../include/scenes/SceneMenu.h"
#include "../../include/scenes/SceneGame.h"


SceneMenu::SceneMenu(Gui * gui, float const &dtTime)
: Scene(gui, dtTime),
  _blurEnabled(true) {}

/**
 * @brief Destroy the ASceneMenu::ASceneMenu object
 */
SceneMenu::~SceneMenu() {
	//ɾ��UI ����button
}

SceneMenu::SceneMenu(SceneMenu const &src)
: Scene(src) {
	*this = src;
}

/* -------------------------------- Operators ----------------------------------*/

/**
 * @brief ����rhs
 */
SceneMenu &SceneMenu::operator=(SceneMenu const &rhs) {
	return *this;
}

/**
 * @brief Cout operator
 *
 * @param os The ostream object
 * @param myClass The class to cout
 * @return std::ostream& the ostream object
 */
std::ostream &	operator<<(std::ostream & os, const SceneMenu& myClass) {
	(void)myClass;
	os << "<ASceneMenu object>";
	return os;
}

/* -------------------------------- Methods----------------------------------*/
/**
 * @brief ���º��� ÿһ֡(frame)����һ��
 *
 * @return �Ƿ���³ɹ�
 */
bool	SceneMenu::update() {
	if (s.j("debug").b("3d-menu")) {
		SceneGame & scGame = *reinterpret_cast<SceneGame *>(SceneManager::getScene(SceneNames::GAME));
		scGame.updateForMenu();
	}
	/* UI elements */
	return true;
}

/**
 * @brief ��ͼ���� ÿһ֡(frame)����һ��
 *
 * @return �Ƿ���³ɹ�
 */
bool	SceneMenu::draw() {

	//std::cout << "I'm drawing --sceneMenu\n";
	/* update alpha mask used for blur effect */

	/* 3d background */
	
	/* UI elements */
	
	return  true;
}
/**
 * @brief called when the scene is loaded
 */
void SceneMenu::load() {
	_gui->enableCursor(true);
}
/**
 * @brief called when the scene is unloaded
 */
void SceneMenu::unload() {
}


//add UI elements




/**
 * @brief ��ʼ������
 *
 * @return �Ƿ��ʼ���ɹ�
 */
bool SceneMenu::_initBG() {
	//UI

	return true;
}


//TransparentBox
