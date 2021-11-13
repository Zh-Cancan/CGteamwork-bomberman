#include "../../include/scenes/Scene.h"

/**
 * @brief Construct a new AScene::AScene object
 *
 * @param gui A pointer on the gui object
 * @param dtTime A reference to the delta time
 */
Scene::Scene(Gui *gui, float const& dtTime)
: _gui(gui),
  _dtTime(dtTime) {}

/**
 * @brief Construct a new AScene::AScene object
 *
 * @param src The object to do the copy
 */
Scene::Scene(Scene const & src)
: _gui(src._gui) ,
  _dtTime(src._dtTime) {
	*this = src;
}

/**
 * @brief Destroy the AScene::AScene object
 */
Scene::~Scene() {
	_gui->enableCursor(true);
}

/**
 * @brief Copy this object
 *
 * @param rhs The object to copy
 * @return AScene& A reference to the copied object
 */
Scene & Scene::operator=(Scene const & rhs) {
	if (this != &rhs) {
		std::cout<<"scene object copied";
		_gui = rhs._gui;
	}
	return *this;
}

/**
 * @brief Get the delta time
 *
 * @return float const& The delta time
 */
float const& Scene::getDtTime() const { return _dtTime; }


