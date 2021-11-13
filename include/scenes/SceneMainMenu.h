#pragma once

#include "SceneMenu.h"
//#include "ABaseUI.hpp"

/**
 * @brief ���˵� ��Ϸ��ʼʱչʾ
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
		//button UIԪ��
		struct ButtonsStates {
			bool	continueGame;  /**< True if we clicked on the button continueGame */
			bool	newGame;  /**< True if we clicked on the button newGame */
			bool	loadGame;  /**< True if we clicked on the button loadGame */
			bool	exit;  /**< True if we clicked on the button exit */
		};
		ButtonsStates	_states;  /**< All buttons states */

	private:
		SceneMainMenu();
		void		_updateUI();
};
