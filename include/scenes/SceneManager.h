#pragma once

#include <stdexcept>
#include <map>
#include "../utils/opengl/Inputs.h"
#include "../Gui.h"
class SceneMenu;
/**
 * @brief this is the list of all existing scenes
 */
namespace SceneNames {
	static std::string const MAIN_MENU = "mainMenu";
	static std::string const LEVEL_SELECTION = "levelSelection";
	static std::string const GAME = "game";
	static std::string const GAME_OVER = "gameOver";
	static std::string const VICTORY = "victory";
	static std::string const EXIT = "exit";
	static std::string const LOADGAME = "loadGame";
	static std::string const END_GAME = "endGame";
	static std::string const DEBUG_MENU = "debugMenu";
	static std::string const LOADING = "loading";
}  // namespace SceneNames

/**
 * @brief this object can manage all scenes (AScene objects)
 *
 * you can load a scene (SceneManager::loadScene) and his update & draw will be automatically updated
 */
class SceneManager {
	public:
		SceneManager();
		virtual ~SceneManager();

		static SceneManager &		get();
		static bool					init();
		static bool					run();
		static SceneMenu*			loadScene(std::string const & name);
		static SceneMenu*			getScene(std::string const & name);
		static std::string const &	getSceneName();
	//	static uint16_t				getFps();
		static bool					isSceneChangedInCurFrame();
		static void					quit();
		


	protected:
		bool		_isInit;  /**< True if SceneManager was init */
		Gui *		_gui;  /**< Gui object */
		float		_dtTime;  /**< Delta time btw 2 loop calls */
		std::string	_scene;  /**< The name of the current scene */
	    std::map<std::string, SceneMenu*>	_sceneMap;  /**< All scenes (in a map) */
		uint16_t	_fps;  /**< Max FPS of the game */

		bool		_sceneLoadedCurrentFrame;  /**< True if a scene was loaded in the current frame */

		bool				_init();
		bool				_run(float maxFrameDuration);
		bool				_update();
		bool				_draw();
		SceneMenu*			_loadScene(std::string const & name);
		SceneMenu*			_getScene(std::string const & name);
		std::string const &	_getSceneName() const;
		bool				_isSceneChangedInCurFrame() const;
	//	void				_openCheatCode(bool open);
	//	void				_openCheatCodeForTime(uint64_t ms);
	//	bool				_isCheatCodeOpen() const;
		void				_quit();

	private:
		SceneManager(SceneManager const & src);
		SceneManager & operator=(SceneManager const & rhs);
};
