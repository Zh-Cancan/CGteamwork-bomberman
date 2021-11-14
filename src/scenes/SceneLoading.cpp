#define _CRT_SECURE_NO_WARNINGS
#include "../../include/scenes/SceneLoading.h"
//#include "Save.hpp"
#include "../../include/utils/FileUtils.h"


SceneLoading::SceneLoading(Gui * gui, float const &dtTime)
: SceneMenu(gui, dtTime)
{
	_blurEnabled = false;
}


SceneLoading::SceneLoading(SceneLoading const & src)
: SceneMenu(src)
{
	*this = src;
}


SceneLoading::~SceneLoading() {}


SceneLoading & SceneLoading::operator=(SceneLoading const & rhs) {
	return *this;
}


bool			SceneLoading::init() {
	glm::vec2 winSz = _gui->gameInfo.windowSize;
	glm::vec2 tmpPos;
	glm::vec2 tmpSize;
	float menuWidth = winSz.x * 0.8;
	float menuHeight = winSz.y / 14;

	//UI init

	return true;
}

/**
 * @brief called when the scene is loaded
 */
void SceneLoading::load() {
	SceneMenu::load();
}

/**
 * @brief this is the update function (called every frames)
 *
 * @return true if the update is a success
 * @return false if there are an error in update
 */
bool	SceneLoading::update() {
	SceneMenu::update();
	SceneManager::loadScene(SceneNames::MAIN_MENU);
	return true;
}

/**
 * @brief this is the draw function (called every frames)
 *
 * @return true if the draw is a success
 * @return false if there are an error in draw
 */
bool SceneLoading::draw() {
	bool ret = true;
	ret = SceneMenu::draw();
	return ret & true;
}
