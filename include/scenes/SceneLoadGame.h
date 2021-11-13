#pragma once

#include "SceneMenu.h"

/**
 * @brief menu for game over
 */
class SceneLoadGame : public SceneMenu {
	public:
		// Constructors
		SceneLoadGame(Gui * gui, float const &dtTime);
		virtual ~SceneLoadGame();
		SceneLoadGame(SceneLoadGame const &src);

		// Operators
		SceneLoadGame &operator=(SceneLoadGame const &rhs);

		// Methods
		virtual bool		init();
		virtual void		load();
		virtual bool		update();

	protected:
		/**
		 * @brief All buttons states
		 */
		struct ButtonsStates {
			int64_t		selectedGame;  /**< Selected game */
			bool		restart;  /**< True if we clicked on the button restart */
			bool		menu;  /**< True if we clicked on the button menu */
			bool		loadGame;  /**< True if we clicked on the button loadGame */
			bool		deleteGame;  /**< True if we clicked on the button deleteGame */
		};
		ButtonsStates	_states;  /**< All buttons states */
		/**
		 * @brief All information & UI of a saved game (from a json file)
		 */
		struct GameSaved {
			uint32_t		gameID;  /**< Game ID */
			SettingsJson	*game;  /**< Game informations (json) */
			std::string		filename;  /**< Filename for game */
			//ABaseUI			*ui;  /**< UI element for a game */
		};
		std::vector<GameSaved *>	_gamesSaved;  /**< All game saved information */
		/**
		 * @brief All UI for preview a saved game
		 */
		
		int64_t						_selectedGame;  /**< Current selected game */
		std::string					_lastSceneName;  /**< Last scene name (to load it when cancel) */

	private:
		SceneLoadGame();
};
