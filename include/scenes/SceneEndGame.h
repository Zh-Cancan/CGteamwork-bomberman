#pragma once

#include "SceneMenu.h"
#include "SceneGame.h"
//#include "ABaseUI.hpp"

/**
 * @brief End of the game (after all levels finished)
 */
class SceneEndGame : public SceneMenu {
	public:
		// Constructors
		SceneEndGame(Gui * gui, float const &dtTime);
		virtual ~SceneEndGame();
		SceneEndGame(SceneEndGame const &src);

		// Operators
		SceneEndGame &operator=(SceneEndGame const &rhs);

		// Methods
		virtual bool		init();
		virtual void		load();
		virtual bool		update();
		virtual bool		draw();

	protected:
		/**
		 * @brief All buttons states
		 */
		struct ButtonsStates {
			bool	exit;  /**< True if we clicked on the exit button */
		};
		ButtonsStates	_states;  /**< All buttons states */
		

	private:
		SceneEndGame();
		void		_updateUI();
};
