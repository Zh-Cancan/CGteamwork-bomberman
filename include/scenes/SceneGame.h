#ifndef SCENEGAME_HPP_
#define SCENEGAME_HPP_

#include <iostream>
#include <stdexcept>
#include <vector>
#include <map>
#include "../utils/useGlm.h"

#include "SceneMenu.h"


#define NO_LEVEL -1  // value is no level loaded
#define LEVEL_INTRO_DURATION 2
#define BLUR_SHADER_VS "shaders/blur_vs.glsl"
#define BLUR_SHADER_FS "shaders/blur_fs.glsl"
#define PP_VAO_WIDTH 4
#define PP_V_ARRAY_SIZE 24

class Player;

namespace GameState {
	/**
	 * @brief All possible game states (INTRO, PLAY, PAUSE, GAME_OVER, ...)
	 */
	enum Enum {
		INTRO,
		PLAY,
		PAUSE,
		GAME_OVER,
		WIN,
	};
}  // namespace GameState

/**
 * @brief This is the game Scene. In this scene, you can play to the game and load levels
 */
class SceneGame : public SceneMenu {
private:
	SceneGame();
	
	/**
	 * @brief All 3D models of enemies that can be drew on different menus
	 */
	struct DrawForMenu {
		//Model *	player;  /**< 3D model to draw player */
		//Model *	flower;  /**< 3D model to draw flower */
		//Model *	robot;  /**< 3D model to draw robot */
		//Model * fly;  /**< 3D model to draw fly */
		//Model * frog;  /**< 3D model to draw frog */
		//Model * crispy;  /**< 3D model to draw crispy */
		//Model * follow;  /**< 3D model to draw follow */

		DrawForMenu();
	};

	DrawForMenu _menuModels;  /**< All 3D elements to draw */

	//Model		*_terrain;  /**< The terrain element */
	
	// post processing stuff
	//Shader		*_blurShader;  /**< PostProcess blur shader */
	static std::array<float, PP_V_ARRAY_SIZE> const	_ppVertices;  /**< Vertices data */
	uint32_t	_ppShVbo;  /**< PostProcess vbo */
	uint32_t	_ppShVao;  /**< PostProcess vao */
	uint32_t	_blurFbo[2];  /**< PostProcess blur framebuffer */
	uint32_t	_blurTexColor[2];  /**< PostProcess blur texture */
	uint32_t	_rbo;  /**< renderBufferObject to store depth and stencil buffers */
	uint32_t	_blurMaskTex;  /**< The blur mask texture */

	// Methods
	bool	_loadLevel(int32_t levelId);
	bool	_unloadLevel();
	bool	_initPostProcess();
	void	_drawBoard();

protected:
	/**
	 * @brief All UI elements (bonus, text, ...)
	 */

	void			_initGameInfos();
	void			_loadGameInfos();
	void			_updateGameInfos();

	/*std::vector<CamPoint>	_getIntroAnim() const;
	std::vector<CamPoint>	_getGameOverAnim() const;
	std::vector<CamPoint>	_getVictoryAnim() const;*/

public:
	// Members
	//static std::map<std::string, Entity>					entitiesCall;  /**< All entity type & functions */
	std::vector< std::vector< bool > >						floor;  /**< True if there is a floor here */
	//std::vector< std::vector< std::vector<AEntity *> > >	board;  /**< The base board with all static elements */
	//std::vector< std::vector< std::vector<AEntity *> > >	boardFly;  /**< The fly board with all static flying elements */
	Player						*player;  /**< The player */
	
	glm::uvec2					size;  /**< Level size */
	int32_t						level;  /**< The current level ID (-1 for no level) */
	GameState::Enum				state;  /**< Actual game state (PLAY, PAUSE, GAME_OVER, ...) */
	/**
	 * @brief Stats about number of entites, bombs, ...
	 */
	struct EntitiesCount {
		int		enemy;  /**< Number of enemy on the game */
		int		staticElements;  /**< Number of staticElements on the game */
		int		players;  /**< Number of players on the game */
		int		total;  /**< Number of total on the game */
	};
	EntitiesCount	entitiesCount;  /**< Stats about number of entites, bombs, ... */

	// Constructors
	SceneGame(Gui * gui, float const &dtTime);
	virtual ~SceneGame();
	SceneGame(SceneGame const &src);

	// Operators
	SceneGame &operator=(SceneGame const &rhs);

	// Methods
	bool			positionInGame(glm::vec3 pos, glm::vec3 sz = glm::vec3(1, 1, 1));
	bool			updateBlurMaskTex(std::vector<uint8_t> const &aMaskData);
	//void			blurFilterBefore();
	//void			blurFilterAfter();

	// SceneGame methods
	virtual bool	init();
	virtual bool	update();
	//virtual bool	postUpdate();
	virtual void	load();
	virtual void	unload();
	virtual bool	draw();
	bool			updateForMenu();
	bool			drawGame();
	bool			drawForMenu();
	bool			drawVictory();
	bool			drawGameOver();
	bool			drawEndGame();
	bool			loadLevel(int32_t levelId);
	//bool			insertEntity(std::string const & name, glm::ivec2 pos, bool isFly = false, uint64_t wallGenPercent = 0);


	// Exceptions
	/**
	 * @brief SceneGame exception
	 */
	class SceneGameException : public std::runtime_error {
	public:
		SceneGameException();
		/**
		 * @brief Construct a new Scene Game Exception object
		 *
		 * @param whatArg Error message
		 */
		explicit SceneGameException(const char* whatArg);
	};
};

#endif  // SCENEGAME_HPP_
