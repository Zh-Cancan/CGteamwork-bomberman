#pragma once

#include <vector>
#include "../utils/useGlm.h"

#include "Scene.h"
#include "SceneManager.h"

/* import all UI objects */
//...

#define VOID_COLOR glm::vec4 {-1 , -1, -1, -1}

/**
 * @brief Struct TransparentBox, store transparent ui pos/size for blur effect
 */
struct	TransparentBox {
	glm::vec2	pos;  /**< The box position */
	glm::vec2	size;  /**< The box size */
};

/**
 * @brief Scene object to re-implement in all scenes for menu
 */
class SceneMenu : public Scene {
	public:
		// Members

		// Constructors
		SceneMenu(Gui * gui, float const &dtTime);
		virtual ~SceneMenu();
		SceneMenu(SceneMenu const &src);

		// Operators
		SceneMenu &operator=(SceneMenu const &rhs);
		friend std::ostream& operator<<(std::ostream& os, const SceneMenu& myClass);

		// Scene methods
		virtual bool		init() = 0;
		virtual bool		update();
		virtual bool		draw();
		virtual void		load();
		virtual void		unload();

		/* add element */
		

		/* get element */
		/*ABaseUI &			getUIElement(uint32_t id);
		uint32_t			getNbUIElements() const;*/

	protected:
		
		/* blur effect ?transparentBox实现 需要吗？*/
		bool	_blurEnabled;  /**< If enabled blur transparent ui 模糊效果*/
		static	std::vector<uint8_t>		_aMaskData;  /**< Blur mask texture raw data */
		static	std::vector<TransparentBox>	_transparentBoxs;  /**< Transparents box to be blured */

		bool	_initBG();
		
};
