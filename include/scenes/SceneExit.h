#pragma once

#include "SceneMenu.h"

/**
 * @brief menu for exit
 */
class SceneExit : public SceneMenu {
	public:
		// Constructors
		SceneExit(Gui * gui, float const &dtTime);
		virtual ~SceneExit();
		SceneExit(SceneExit const &src);

		// Operators
		SceneExit &operator=(SceneExit const &rhs);

		// Methods
		virtual bool		init();
		virtual bool		update();
		virtual void		load();
		virtual void		unload();
		virtual bool		draw();

	protected:
		/**
		 * @brief All buttons states
		 */
		struct ButtonsStates {
			bool	exit;  /**< True if we clicked on exit button */
			bool	cancel;  /**< True if we clicked on cancel button */
		};
		ButtonsStates	_states;  /**< All buttons states */

		/**
		 * @brief All UI elements
		 */
		
		std::string		_lastSceneName;  /**< The last scene name (used if we cancel) */

	private:
		SceneExit();
		void			_updateUI();
};
