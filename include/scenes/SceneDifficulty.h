#pragma once

#include "SceneMenu.h"
//#include "ABaseUI.hpp"

/**
 * @brief 难度选择 （在游戏开始时显示）
 */
class SceneDifficulty : public SceneMenu {
	public:
		// Constructors
		SceneDifficulty(Gui * gui, float const &dtTime);
		virtual ~SceneDifficulty();
		SceneDifficulty(SceneDifficulty const &src);

		// Operators
		SceneDifficulty &operator=(SceneDifficulty const &rhs);

		// Methods
		virtual bool		init();
		virtual void		load();
		virtual bool		update();

	protected:
		/**
		 * @brief All buttons states
		 */
		struct ButtonsStates {
			bool	beginner;  /**< True if we clicked on the button beginner */
			bool	easy;  /**< True if we clicked on the button easy */
			bool	medium;  /**< True if we clicked on the button medium */
			bool	hardCore;  /**< True if we clicked on the button hardCore */
			bool	menu;  /**< True if we clicked on the menu button */
		};
		ButtonsStates	_states;  /**< All buttons states */
		
        /* UI elements*/
		
	private:
		SceneDifficulty();
		void		_updateUI();
};
