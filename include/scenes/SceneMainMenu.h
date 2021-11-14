#pragma once

#include "SceneMenu.h"
//#include "ABaseUI.hpp"

/**
 * @brief 主菜单 游戏开始时展示
 */
class SceneMainMenu : public SceneMenu {
	public:
		// Constructors
		SceneMainMenu(Gui * gui, float const &dtTime);
		virtual ~SceneMainMenu();
		SceneMainMenu(SceneMainMenu const &src);

		// Operators
		SceneMainMenu &operator=(SceneMainMenu const &rhs);

		// Methods
		virtual bool		init();
		virtual void		load();
		virtual bool		update();

	protected:
		//button UI元素
		struct ButtonsStates {
			bool	newGame;  /**< True if we clicked on the button newGame 选择newGame则进入关卡选择菜单*/
			bool	exit;  /**< True if we clicked on the button exit */
		};
		ButtonsStates	_states;  /**< All buttons states */
		//UI 控件

	private:
		SceneMainMenu();
		void		_updateUI();//控件实现
};
