#pragma once

#include "SceneMenu.h"

/**
 * @brief this is the victory menu
 */
class SceneVictory : public SceneMenu {
	public:
		// Constructors
		SceneVictory(Gui * gui, float const &dtTime);
		virtual ~SceneVictory();
		SceneVictory(SceneVictory const &src);

		// Operators
		SceneVictory &operator=(SceneVictory const &rhs);

		// Methods
		virtual bool		init();
		virtual bool		update();
		virtual void		load();
		virtual bool		draw();

	protected:
		/**
		 * @brief All buttons states
		 */
		struct ButtonsStates {
			bool	nextLevel;  /**< True if we clicked on the button nextLevel */
			bool	restart;  /**< True if we clicked on the button restart */
			bool	menu;  /**< True if we clicked on the button menu */
			bool	exit;  /**< True if we clicked on the button exit */
		};
		ButtonsStates	_states;  /**< All buttons states */
		std::string		_lastSceneName;  /**< The last loaded scene name */

		/**
		 * @brief Stats about game to draw on the screen
		 */
		

	private:
		SceneVictory();
};
