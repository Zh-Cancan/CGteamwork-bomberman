#include <windows.h>
#include <chrono>

#include "../../include/scenes/SceneManager.h"
#include "../../include/bomberman.h"
//#include "ABaseUI.hpp"
//#include "ModelsManager.hpp"
//
/* import all scenes */
#include "../../include/scenes/SceneMainMenu.h"
#include "../../include/scenes/SceneLevelSelection.h"
#include "../../include/scenes/SceneGame.h"
#include "../../include/scenes/SceneGameOver.h"
#include "../../include/scenes/SceneVictory.h"
#include "../../include/scenes/SceneExit.h"
#include "../../include/scenes/SceneLoadGame.h"
#include "../../include/scenes/SceneEndGame.h"
//#include "SceneDebug.hpp"
#include "../../include/scenes/SceneLoading.h"
//#include "SceneHelp.hpp"

/**
 * @brief Construct a new Scene Manager:: Scene Manager object
 */
SceneManager::SceneManager()
	:_isInit(false),
	_gameInfo(),
	_gui(nullptr),
	_dtTime(0.0f),
	_scene(SceneNames::GAME),
	//  _isInCheatCode(false),
	//  _showCheatCodeTextTime(0),
	_fps(60)
	//  _sceneLoadedCurrentFrame(false)
{}


SceneManager::SceneManager(SceneManager const& src) {
	*this = src;
}


SceneManager::~SceneManager() {

}

/**
 * @brief Copy this object
 *
 * @param rhs The object to copy
 * @return SceneManager& A reference to the copied object
 */
SceneManager& SceneManager::operator=(SceneManager const& rhs) {
	(void)rhs;
	logErr("You should never call copy operator for SceneManager");
	return *this;
}

/**
 * @brief get the SceneManager
 *
 * @return SceneManager& the instance of the SceneManager
 */
SceneManager& SceneManager::get() {
	static SceneManager	instance;
	return instance;
}

/**
 * @brief init the SceneManager (load the first scene& load gui)
 *
 * @return false if failure
 */
bool SceneManager::init() {
	return SceneManager::get()._init();
}
/**
 * @brief init the SceneManager (load the first scene& load gui)
 *
 * @return false if failure
 */
bool SceneManager::_init() {
	if (_sceneMap.find(SceneNames::MAIN_MENU) != _sceneMap.end()) {
		std::cout << "SceneManager::init already called";
		return false;
	}

	// GUI创建
	_gui = new Gui(_gameInfo);
	if (!_gui->init()) {
		return false;
	}

	/* 创建loading scene */
	_scene = SceneNames::GAME;
	_sceneMap.insert(std::pair<std::string, Scene*>(SceneNames::GAME, new SceneGame(_gui, _dtTime))); 
	/*_scene = SceneNames::LOADING;
	_sceneMap.insert(std::pair<std::string, Scene*>(SceneNames::LOADING, new SceneLoading(_gui, _dtTime)));*/
	try {
		if (_sceneMap[_scene]->init() == false) {
			std::cout << "failed to init scene: loading";
			return false;
		}
	}
	catch (std::exception const& e) {
		std::cout << "Error : " << e.what();
		return false;
	}

	//try {
	//	_sceneMap[_scene]->load();  // load first scene
	//}
	//catch (std::exception const& e) {
	//	std::cout << "Error : " << e.what();
	//	return false;
	//}

	/* draw */
	Inputs::update();
	_gui->preDraw();
	// draw debug menu scene
	if (_sceneMap[_scene]->draw() == false) {
		return false;
	}
	_gui->postDraw();

	// load and init 3d models


	// create and init all scene
	/*_sceneMap.insert(std::pair<std::string, Scene*>(SceneNames::MAIN_MENU, new SceneMainMenu(_gui, _dtTime)));
	_sceneMap.insert(std::pair<std::string, Scene*>(SceneNames::LEVEL_SELECTION,
		new SceneLevelSelection(_gui, _dtTime)));
	_sceneMap.insert(std::pair<std::string, Scene*>(SceneNames::GAME, new SceneGame(_gui, _dtTime)));
	_sceneMap.insert(std::pair<std::string, Scene*>(SceneNames::GAME_OVER, new SceneGameOver(_gui, _dtTime)));
	_sceneMap.insert(std::pair<std::string, Scene*>(SceneNames::VICTORY, new SceneVictory(_gui, _dtTime)));
	_sceneMap.insert(std::pair<std::string, Scene*>(SceneNames::EXIT, new SceneExit(_gui, _dtTime)));
	_sceneMap.insert(std::pair<std::string, Scene*>(SceneNames::LOADGAME, new SceneLoadGame(_gui, _dtTime)));
	_sceneMap.insert(std::pair<std::string, Scene*>(SceneNames::END_GAME, new SceneEndGame(_gui, _dtTime)));*/
	_sceneMap.insert(std::pair<std::string, Scene*>(SceneNames::GAME, new SceneGame(_gui, _dtTime)));

	for (auto it = _sceneMap.begin(); it != _sceneMap.end(); it++) {
		try {
			if (it->first == SceneNames::LOADING)
				continue;
			if (it->second->init() == false) {
				std::cout << "failed to init scene: " << it->first;
				return false;
			}
		}
		catch (std::exception const& e) {
			std::cout << "Error : " << e.what();
			return false;
		}
	}
	_isInit = true;
	return true;
}

/**
 * @brief this is the main game loop to update & draw everythings
 *
 * @return false if failure
 */
bool SceneManager::run() {
	//读取screen的帧率
	float		maxFrameDuration = 1000 / s.j("screen").u("fps");

	return SceneManager::get()._run(maxFrameDuration);
}
/**
 * @brief this is the main game loop to update & draw everythings
 *
 * @return false if failure
 */
bool SceneManager::_run(float maxFrameDuration) {
	std::chrono::milliseconds	lastLoopMs = getMs();

	while (true) {
		/* reset variables */
		_sceneLoadedCurrentFrame = false;
		_dtTime = (getMs().count() - lastLoopMs.count()) / 1000.0;
		lastLoopMs = getMs();
		_fps = 1 / _dtTime;

		/*if (_sceneMap.find(_scene) == _sceneMap.end()) {
			std::cout << "invalid scene name: " << _scene << "\n";
		}*/
		/*else {*/
			/* update & draw scene */
			if (_update() == false)
				return false;
			if (_draw() == false)
				return false;
		/*}*/

		/* quit if it's the end of the game */
		if (_gameInfo.quit) {
			break;
		}

		/* fps control */
		std::chrono::milliseconds loopDuration = getMs() - lastLoopMs;
		float	frameDuration = loopDuration.count();

		if (frameDuration <= maxFrameDuration) {
			Sleep(maxFrameDuration - frameDuration);
		}

	}

	return true;
}

/**
 * @brief Update the current scene更新scene
 *
 * @return false if error
 */
bool SceneManager::_update() {
	/* get inputs */
	//Inputs::update();


	/* update */
	//ABaseUI::staticUpdate();
	//_gui->preUpdate(_dtTime);
	//bool cheatCodeClosed = false;

	/* scene */
	
	if (_sceneMap[_scene]->update() == false) {
		std::cout<<"Unexpected error when updating scene"<<"\n";
		return false;
	}

	//防止更新的时候退出
	if (isSceneChangedInCurFrame())
		_gui->disableExitForThisFrame();

	/*_gui->postUpdate(_dtTime);*/

	return true;
}

/**
 * @brief Draw the current scene
 *
 * @return false if error
 */
bool SceneManager::_draw() {
	/* draw */
	_gui->preDraw();

	// draw the scene
	if (_sceneMap[_scene]->draw() == false) {
		std::cout<<"Unexpected error when drawing scene"<<"\n";
		return false;
	}

	// draw cheatcode scene


	// draw debug menu scene


	_gui->postDraw();

	return true;
}

/**
 * @brief load a scene from his name
 *
 * @param name the scene name
 * @return AScene* a pointer to the scene loaded
 */
Scene* SceneManager::loadScene(std::string const& name) {
	return SceneManager::get()._loadScene(name);
}
/**
 * @brief load a scene from his name
 *
 * @param name the scene name
 * @return AScene* a pointer to the scene loaded
 */
Scene* SceneManager::_loadScene(std::string const& name) {
	if (get()._sceneMap.find(name) == get()._sceneMap.end()) {
		logErr("invalid scene name: " << name << " in loadScene");
		return _sceneMap[_scene];
	}
	if (_scene == name) {
		return _sceneMap[_scene];
	}
	// scene changed, load the new
	_sceneLoadedCurrentFrame = true;
	_sceneMap[_scene]->unload();  // unload last scene
	try {
		_sceneMap[name]->load();  // load new scene (getScene return the name of the last scene)
	}
	catch (std::exception const& e) {
		logErr("Error: " << e.what());
	}
	_scene = name;
	return _sceneMap[_scene];
}

/**
 * @brief get a scene
 *
 * @param name the name of the scene to get
 * @return AScene* a pointer to the scene
 */
Scene* SceneManager::getScene(std::string const& name) {
	return SceneManager::get()._getScene(name);
}
/**
 * @brief get a scene
 *
 * @param name the name of the scene to get
 * @return AScene* a pointer to the scene
 */
Scene* SceneManager::_getScene(std::string const& name) {
	if (get()._sceneMap.find(name) == get()._sceneMap.end()) {
		logErr("invalid scene name: " << name << " in getScene");
		return _sceneMap[_scene];
	}
	return _sceneMap[name];
}

/**
 * @brief get the current scene name
 *
 * @return std::string const& the current scene name
 */
std::string const& SceneManager::getSceneName() {
	return SceneManager::get()._getSceneName();
}
/**
 * @brief get the current scene name
 *
 * @return std::string const& the current scene name
 */
std::string const& SceneManager::_getSceneName() const {
	return _scene;
}

///**
// * @brief get the current fps count
// *
// * @return uint16_t the current fps count
// */
//uint16_t	SceneManager::getFps() {
//	uint16_t clampVal = s.j("screen").u("fps");
//	uint16_t clampFps = SceneManager::get()._fps;
//	clampFps = (clampFps < clampVal) ? clampFps : clampVal;
//	return clampFps;
//}
//
/**
 * @brief Return if the scene has changed in the current frame
 *
 * @return true If the scene changed in the current frame
 */
bool SceneManager::isSceneChangedInCurFrame() {
	return SceneManager::get()._isSceneChangedInCurFrame();
}
/**
 * @brief Return if the scene has changed in the current frame
 *
 * @return true If the scene changed in the current frame
 */
bool SceneManager::_isSceneChangedInCurFrame() const {
	return _sceneLoadedCurrentFrame;
}

///**
// * @brief Open or force close cheat code command line
// *
// * @param open True to open cheat code command line
// */
//void SceneManager::openCheatCode(bool open) {
//	SceneManager::get()._openCheatCode(open);
//}
///**
// * @brief Open or force close cheat code command line
// *
// * @param open True to open cheat code command line
// */
//void SceneManager::_openCheatCode(bool open) {
//	if (_isInCheatCode == open)  // if state didn't changed
//		return;
//	if (open) {
//		_showCheatCodeTextTime = 0;
//		if (reinterpret_cast<SceneCheatCode *>(_sceneMap[SceneNames::CHEAT_CODE])->getText().size() == 0)
//			reinterpret_cast<SceneCheatCode *>(_sceneMap[SceneNames::CHEAT_CODE])->setText(CHEATCODE_DEF_TXT);
//		_sceneMap[SceneNames::CHEAT_CODE]->load();
//	}
//	else {
//		_sceneMap[SceneNames::CHEAT_CODE]->unload();
//	}
//	_isInCheatCode = open;
//}
//
///**
// * @brief Open cheatcode text for a certain time
// *
// * @param ms The time to open cheatcode text
// */
//void SceneManager::openCheatCodeForTime(uint64_t ms) {
//	SceneManager::get()._openCheatCodeForTime(ms);
//}
///**
// * @brief Open cheatcode text for a certain time
// *
// * @param ms The time to open cheatcode text
// */
//void SceneManager::_openCheatCodeForTime(uint64_t ms) {
//	if (ms == 0 || static_cast<int64_t>(ms) > _showCheatCodeTextTime) {
//		_showCheatCodeTextTime = ms;
//	}
//}
//
///**
// * @brief Know if we are in cheat code mode
// *
// * @return true If cheat code command line is open
// */
//bool SceneManager::isCheatCodeOpen() {
//	return SceneManager::get()._isCheatCodeOpen();
//}
///**
// * @brief Know if we are in cheat code mode
// *
// * @return true If cheat code command line is open
// */
//bool SceneManager::_isCheatCodeOpen() const {
//	return _isInCheatCode;
//}
//
/**
 * @brief quit the game
 */
void SceneManager::quit() {
	SceneManager::get()._quit();
}
/**
 * @brief quit the game
 */
void SceneManager::_quit() {
	_gameInfo.quit = true;
}

///* exception */
///**
// * @brief Construct a new Scene Manager:: Scene Manager Exception:: Scene Manager Exception object
// */
//SceneManager::SceneManagerException::SceneManagerException()
//: std::runtime_error("SceneManager Exception") {}
//
///**
// * @brief Construct a new Scene Manager:: Scene Manager Exception:: Scene Manager Exception object
// *
// * @param whatArg Error message
// */
//SceneManager::SceneManagerException::SceneManagerException(const char* whatArg)
//: std::runtime_error(std::string(std::string("SceneManagerException: ") + whatArg).c_str()) {}
