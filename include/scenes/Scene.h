#pragma once

#include "../Gui.h"

/**
 * @brief base object to implement in all scenes
 */
class Scene {
	public:
		Scene(Gui * gui, float const& dtTime);
		Scene(Scene const & src);
		virtual ~Scene();

		/**
		 * @brief Init the scene
		 *
		 * @return false If failed
		 */
		virtual bool init() = 0;
		/**
		 * @brief Update the scene (called every frames)
		 *
		 * @return false If failed
		 */
		virtual bool update() = 0;
		/**
		 * @brief Draw the scene (called every frames)
		 *
		 * @return false If failed
		 */
		virtual bool draw() = 0;
		/**
		 * @brief Load the scene (called on every scene loading)
		 */
		virtual void load() = 0;
		/**
		 * @brief Unload the scene (called on every scene unloading)
		 */
		virtual void unload() = 0;

		float const& getDtTime() const;

		Scene & operator=(Scene const & rhs);


	protected:
		Scene();

		Gui *	_gui;  /**< A reference to the gui object */
		float const& _dtTime;  /**< A reference to the delta time */
};
