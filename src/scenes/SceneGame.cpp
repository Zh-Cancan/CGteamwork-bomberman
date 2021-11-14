#include <stdlib.h>
#include <time.h>

#include "../../include/scenes/SceneGame.h"
#include "../../include/bomberman.h"
#include "../../include/utils/FileUtils.h"
//#include "Save.hpp"
//#include "ModelsManager.hpp"
//#include "BoxCollider.hpp"
//
#include "../../include/Player.h"
//#include "Wall.hpp"
//#include "Crispy.hpp"
//#include "Flag.hpp"
//#include "End.hpp"

//#include "EnemyBasic.hpp"
//#include "EnemyFollow.hpp"
//#include "EnemyWithEye.hpp"
//#include "EnemyFly.hpp"
//#include "EnemyCrispy.hpp"
//#include "EnemyFrog.hpp"
//

#include "../../include/scenes/SceneManager.h"

// -- Static members initialisation --------------------------------------------

//std::map<std::string, SceneGame::Entity> SceneGame::entitiesCall = {
	/*{PLAYER_STR, {EntityType::PLAYER, [](SceneGame &game) -> AEntity* {return new Player(game);}}},
	{BOMB_STR, {EntityType::BOARD, [](SceneGame &game) -> AEntity* {return new Bomb(game);}}},
	{WALL_STR, {EntityType::BOARD, [](SceneGame &game) -> AEntity* {return new Wall(game);}}},
	{"block", {EntityType::BOARD, [](SceneGame &game) -> AEntity* {return new Wall(game, Block::BLOCK);}}},
	{CRISPY_STR, {EntityType::BOARD, [](SceneGame &game) -> AEntity* {return new Crispy(game);}}},
	{FLAG_STR, {EntityType::BOARD_FLAG, [](SceneGame &game) -> AEntity* {return new Flag(game);}}},
	{END_STR, {EntityType::BOARD, [](SceneGame &game) -> AEntity* {return new End(game);}}},
	{"safe", {EntityType::BOARD, [](SceneGame &game) -> AEntity* {(void)game; return nullptr;}}},
	{"outside", {EntityType::BOARD, [](SceneGame &game) -> AEntity* {(void)game; return nullptr;}}},
	{"empty", {EntityType::BOARD, [](SceneGame &game) -> AEntity* {(void)game; return nullptr;}}},
	{ENEMY_BASIC_STR, {EntityType::ENEMY, [](SceneGame &game) -> AEntity* {return new EnemyBasic(game);}}},
	{ENEMY_WITH_EYE_STR, {EntityType::ENEMY, [](SceneGame &game) -> AEntity* {return new EnemyWithEye(game);}}},
	{ENEMY_FOLLOW_STR, {EntityType::ENEMY, [](SceneGame &game) -> AEntity* {return new EnemyFollow(game);}}},
	{ENEMY_FLY_STR, {EntityType::ENEMY, [](SceneGame &game) -> AEntity* {return new EnemyFly(game);}}},
	{ENEMY_CRISPY_STR, {EntityType::ENEMY, [](SceneGame &game) -> AEntity* {return new EnemyCrispy(game);}}},
	{ENEMY_FROG_STR, {EntityType::ENEMY, [](SceneGame &game) -> AEntity* {return new EnemyFrog(game);}}},*/
//};

// postProcess face vertices
std::array<float, PP_V_ARRAY_SIZE> const	SceneGame::_ppVertices = {{
	// x, y in normalized device coord, texCoord.u, texCoord.v
	-1.0f, 1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	1.0f, -1.0f,  1.0f, 0.0f,

	-1.0f, 1.0f,  0.0f, 1.0f,
	1.0f, -1.0f,  1.0f, 0.0f,
	1.0f, 1.0f,  1.0f, 1.0f
}};

// -- Constructors -------------------------------------------------------------

/**
 * @brief Construct a new Scene Game:: Scene Game object
 *
 * @param gui A pointer on the gui object
 * @param dtTime A reference to the delta time
 */
SceneGame::SceneGame(Gui * gui, float const &dtTime)
: SceneMenu(gui, dtTime),
  _blurFbo{0, 0},
  _blurTexColor{0, 0} {
	player = nullptr;
	size = {0, 0};
	level = NO_LEVEL;
	state = GameState::PLAY;
	//_terrain = nullptr;
	//_blurShader = 0;
	_rbo = 0;
	_blurMaskTex = 0;
}

/**
 * @brief Destroy the Scene Game:: Scene Game object
 */
SceneGame::~SceneGame() {
	_unloadLevel();  // delete all elements of current level if needed

	if (player != nullptr) {
		delete player;
	}

	//删除模型

	glDeleteFramebuffers(2, _blurFbo);
	glDeleteTextures(2, _blurTexColor);
	glDeleteTextures(1, &_blurMaskTex);
}

/**
 * @brief Construct a new Scene Game:: Scene Game object
 *
 * @param src The object to do the copy
 */
SceneGame::SceneGame(SceneGame const &src)
: SceneMenu(src) {
	*this = src;
}

/**
 * @brief Construct a new Scene Game:: Draw For Menu:: Draw For Menu object
 */
SceneGame::DrawForMenu::DrawForMenu() {
	/*player = nullptr;
	flower = nullptr;
	robot = nullptr;
	fly = nullptr;
	frog = nullptr;
	crispy = nullptr;
	follow = nullptr;*/
}

/**
 * @brief Copy this object
 *
 * @param rhs The object to copy
 * @return SceneGame& A reference to the copied object
 */
SceneGame &SceneGame::operator=(SceneGame const &rhs) {
	if ( this != &rhs ) {
		//board = rhs.board;
		//boardFly = rhs.boardFly;
		player = rhs.player;
		size = rhs.size;
		level = rhs.level;
		state = rhs.state;
	}
	return *this;
}


/**
 * @brief Check if the given pos is in the board.
 *
 * @param pos position
 * @param sz size
 * @return true if the position is in game
 */
bool	SceneGame::positionInGame(glm::vec3 pos, glm::vec3 sz) {
	if (pos.x < 0 || pos.x + sz.x > size.x || pos.z < 0 || pos.z + sz.z > size.y)
		return false;
	return true;
}

/**
 * @brief called when the scene is loaded
 */
void SceneGame::load() {
	/*if (_gui->cam->getMode() == CamMode::FOLLOW_PATH) {
		state = GameState::INTRO;
	}*/
	if (state == GameState::WIN
	|| state == GameState::GAME_OVER) {
		state = GameState::PLAY;
	}
	//player->playPauseAnimation(true);
}
/**
 * @brief called when the scene is unloaded
 */
void SceneGame::unload() {
}








// -- Exceptions errors --------------------------------------------------------

/**
 * @brief Construct a new Scene Game:: Scene Game Exception:: Scene Game Exception object
 */
SceneGame::SceneGameException::SceneGameException()
: std::runtime_error("SceneGame Exception") {}

/**
 * @brief Construct a new Scene Game:: Scene Game Exception:: Scene Game Exception object
 *
 * @param whatArg Error message
 */
SceneGame::SceneGameException::SceneGameException(const char* whatArg)
: std::runtime_error(std::string(std::string("SceneGameError: ") + whatArg).c_str()) {}


/**
 * @brief draw is called each frame to draw the Game Scene.
 *
 * @return true
 * @return false
 */
bool	SceneGame::draw() {
	if (level == NO_LEVEL)
		return drawForMenu();
	return drawGame();
}

/**
 * @brief Draw function if we are in a menu (no level loaded)
 *
 * @return false If failed
 */
bool	SceneGame::drawForMenu() {
	// blur filter settings, enable framebuffer

	/* draw models */
	

	// draw skybox


	 // blur filter postprocess

	return true;
}

/**
 * @brief Draw function if we are in a level
 *
 * @return false If failed
 */
bool	SceneGame::drawGame() {
	//if (state != GameState::INTRO) {
	//	blurFilterBefore();  // blur filter settings, enable framebuffer
	//}

	// draw background terrain
	

	// draw floor
	

	//std::vector<AEntity*>  endBlocs;


	// draw board entities
	

	// draw enemies


	// draw player
	

	// draw skybox


	// draw spawners


	// draw end blocks (last for transparency issue)
	

	// release cubeShader and textures
	
	// draw ui on top
	

	//if (state != GameState::INTRO) {
	//	blurFilterAfter();  // blur filter postprocess
	//}

	return true;
}

/**
 * @brief Draw function if we are in victory menu
 *
 * @return false If failed
 */
bool	SceneGame::drawVictory() {
	// blur filter settings, enable framebuffer

	/* draw models */
	

	// draw skybox
	

	// blur filter postprocess

	return true;
}

/**
 * @brief Draw function if we are in game over menu
 *
 * @return false If failed
 */
bool	SceneGame::drawGameOver() {
	// blur filter settings, enable framebuffer

	/* draw models */
	

	// draw skybox
	

	 // blur filter postprocess

	return true;
}

/**
 * @brief Draw function if we are in endGame menu
 *
 * @return false If failed
 */
bool	SceneGame::drawEndGame() {
	/* draw simple floor */

	/* draw models */

	// draw skybox
	

	return true;
}



/**
 * init game method.
 */
bool			SceneGame::init() {
	//int32_t i = 0;
	//while (_initJsonLevel(i)) {
	//	if (i >= 100000) {  // max level
	//		break;
	//	}
	//	i++;
	//}

	entitiesCount.enemy = -1;
	entitiesCount.staticElements = -1;
	entitiesCount.players = -1;
	entitiesCount.total = -1;

	
	/* load all models for menu */
	

	// init terrain model
	

	/* loading sentence */
	

	_initGameInfos();

	if (!_initPostProcess()) {
		logErr("Failed to init postProcess");
		return false;
	}

	return true;
}

/**
 * @brief init postprocess stuff (framebuffer, texture, ...)
 *
 * @return true on success
 * @return false on failure
 */
bool	SceneGame::_initPostProcess() {
	// create framebuffer for post processing blur effects
	glGenFramebuffers(2, _blurFbo);

	// generate texture for post processing blur effects
	glGenTextures(2, _blurTexColor);

	for (uint32_t i = 0; i < 2; ++i) {
		glBindFramebuffer(GL_FRAMEBUFFER, _blurFbo[i]);
		glBindTexture(GL_TEXTURE_2D, _blurTexColor[i]);
		//
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800,
			600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		// attach it to currently bound framebuffer object
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
			_blurTexColor[i], 0
		);

		if (i == 0) {
			// create render buffer object to store the depth and stencil buffers
			glGenRenderbuffers(1, &_rbo);
			glBindRenderbuffer(GL_RENDERBUFFER, _rbo);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,
				800, 600);
			glBindRenderbuffer(GL_RENDERBUFFER, 0);
		}
		// attach depth and stencil to the render buffer
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
			GL_RENDERBUFFER, _rbo);

		// check framebuffer status
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			logErr("FRAMEBUFFER:: Framebuffer is not complete!");
			return false;
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	// create postProcess shader and fill vao
	//if (!_blurShader) {
	//	_blurShader = new Shader(BLUR_SHADER_VS, BLUR_SHADER_FS);
	//	_blurShader->use();

	//	// create and bind vao and vbo
	//	glGenVertexArrays(1, &_ppShVao);
	//	glBindVertexArray(_ppShVao);
	//	glGenBuffers(1, &_ppShVbo);
	//	glBindBuffer(GL_ARRAY_BUFFER, _ppShVbo);

	//	// fill buffer
	//	glBufferData(GL_ARRAY_BUFFER, _ppVertices.size() * sizeof(float),
	//		&_ppVertices[0], GL_STATIC_DRAW);

	//	// normalized 2d vertex coordinates
	//	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, PP_VAO_WIDTH * sizeof(float),
	//		reinterpret_cast<void*>(0));
	//	glEnableVertexAttribArray(0);
	//	// vertex texture coordinate
	//	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, PP_VAO_WIDTH * sizeof(float),
	//		reinterpret_cast<void*>(2 * sizeof(float)));
	//	glEnableVertexAttribArray(1);

	//	// unbind vao / vbo
	//	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//	glBindVertexArray(0);

	//	_blurShader->unuse();
	//}

	return true;
}

/**
 * @brief Init game informations
 */
void SceneGame::_initGameInfos() {
	//UI
}

/**
 * @brief LoadGameInformations
 */
void		SceneGame::_loadGameInfos() {
	/*try {
		allUI.timeLeftImg->setPos(VOID_SIZE).setSize(VOID_SIZE);
		allUI.timeLeftImgActive->setPos(VOID_SIZE).setSize(VOID_SIZE);
		allUI.timeLeftText->setPos(VOID_SIZE).setSize(VOID_SIZE);
		allUI.scoreImg->setPos(VOID_SIZE).setSize(VOID_SIZE);
		allUI.scoreImgActive->setPos(VOID_SIZE).setSize(VOID_SIZE);
		allUI.scoreText->setPos(VOID_SIZE).setSize(VOID_SIZE);
		allUI.lifeImg->setPos(VOID_SIZE).setSize(VOID_SIZE);
		allUI.lifeImgActive->setPos(VOID_SIZE).setSize(VOID_SIZE);
		allUI.lifeText->setPos(VOID_SIZE).setSize(VOID_SIZE);
		allUI.levelNameText->setPos(VOID_SIZE).setSize(VOID_SIZE);
		allUI.enemiesCounterText->setPos(VOID_SIZE).setSize(VOID_SIZE);
		allUI.speedImg->setPos(VOID_SIZE).setSize(VOID_SIZE);
		allUI.speedImgActive->setPos(VOID_SIZE).setSize(VOID_SIZE);
		allUI.speedText->setPos(VOID_SIZE).setSize(VOID_SIZE);
		allUI.bonusBombImg->setPos(VOID_SIZE).setSize(VOID_SIZE);
		allUI.bonusBombImgActive->setPos(VOID_SIZE).setSize(VOID_SIZE);
		allUI.bonusBombText->setPos(VOID_SIZE).setSize(VOID_SIZE);
		allUI.bonusFlameImg->setPos(VOID_SIZE).setSize(VOID_SIZE);
		allUI.bonusFlameImgActive->setPos(VOID_SIZE).setSize(VOID_SIZE);
		allUI.bonusFlameText->setPos(VOID_SIZE).setSize(VOID_SIZE);
		allUI.bonusFlampassImg->setPos(VOID_SIZE).setSize(VOID_SIZE);
		allUI.bonusFlampassImgActive->setPos(VOID_SIZE).setSize(VOID_SIZE);
		allUI.bonusWallpassImg->setPos(VOID_SIZE).setSize(VOID_SIZE);
		allUI.bonusWallpassImgActive->setPos(VOID_SIZE).setSize(VOID_SIZE);
		allUI.bonusDetonatorImg->setPos(VOID_SIZE).setSize(VOID_SIZE);
		allUI.bonusDetonatorImgActive->setPos(VOID_SIZE).setSize(VOID_SIZE);
		allUI.bonusBombpassImg->setPos(VOID_SIZE).setSize(VOID_SIZE);
		allUI.bonusBombpassImgActive->setPos(VOID_SIZE).setSize(VOID_SIZE);
		allUI.bonusShieldImg->setPos(VOID_SIZE).setSize(VOID_SIZE);
		allUI.bonusShieldImgActive->setPos(VOID_SIZE).setSize(VOID_SIZE);
		allUI.bonusShieldText->setPos(VOID_SIZE).setSize(VOID_SIZE);
	}
	catch (ABaseUI::UIException const& e) {
		logErr(e.what());
	}*/
}



/**
 * @brief load a level by ID
 *
 * @param levelId the level ID
 * @return true if the level loading is a success
 * @return false if the level loading failed
 */
bool SceneGame::loadLevel(int32_t levelId) {
	if (_unloadLevel() == false) {
		level = NO_LEVEL;
		return false;
	}
	std::cout<<"load level " << levelId<<"\n";
	bool result = _loadLevel(levelId);

	// reinit printed values
	_loadGameInfos();

	//_gui->cam->pos = {
	//	size.x / 2,
	//	12.0f + (size.x > size.y ? size.x : size.y) * 0.6,
	//	size.y * 1.3
	//};
	//_gui->cam->lookAt(glm::vec3(
	//	size.x / 2, 1.0f,
	//	size.y / 1.9
	//));
	//_gui->cam->setDefPos();  // set the default position to the current position
	//_gui->cam->setMode(CamMode::FOLLOW_PATH);  // set the default camera mode
	//_gui->cam->setFollowPath(_getIntroAnim());  // set the follow path
	state = GameState::INTRO;
	


	/* load sentence */
	std::ifstream	file(s.s("loadingSentences"));
	std::string		line;
	std::vector<std::string> allSentences;
	if (file.is_open()) {
		for (std::string line; std::getline(file, line); ) {
			if (line.size() > 0)
				allSentences.push_back(line);
		}
		file.close();
	}
	else {
		logWarn("unable to load sentences list for loading menu");
	}
	if (allSentences.empty()) {
		logWarn("No sentences in sentences files (" << s.s("loadingSentences") << ")");
		allSentences.push_back("");
	}
	int sentenceID = rand() % allSentences.size();
	//allUI.introText->setText(allSentences[sentenceID]);

	// get saved values
	/*player->resetParams();
	Save::loadStatesSaved(*this);
	if (!player->init()) {
		return false;
	}

	time = 0;
	levelEnemies = enemies.size();

	levelCrispies = 0;
	for (auto&& box : board) {
		for (auto&& row : box) {
			for (auto&& element : row) {
				if (element->type == Type::CRISPY)
					levelCrispies++;
			}
		}
	}

	score.reset();
	score.setLevelId(levelId);*/

	return result;
}


/**
 * @brief Unload data of level.
 *
 * @return true if succeed
 * @return false
 */
bool	SceneGame::_unloadLevel() {
	if (level == NO_LEVEL)
		return true;
	std::cout<<"Unload level " << level;

	floor.clear();
	
	// Delete old player
	delete player;
	player = nullptr;

	// spawners
	
	level = NO_LEVEL;
	return true;
}

/**
 * @brief Load Level method. Throw a SceneException if the level is incomplete.
 *
 * @param levelId
 * @return true
 * @return false
 */
bool	SceneGame::_loadLevel(int32_t levelId) {
	if (levelId == NO_LEVEL)
		return true;
	if (levelId > 10) {//超过最大关卡数
		std::cout<<"unable to load level " << levelId << ": doesn't exist"<<"\n";
		return false;
	}
	string filepath = homeDir+ MAP_DIR+"//"+"map"+ to_string(levelId)+".txt";
	level = levelId;  // save new level ID
	//SettingsJson& lvl = *(_mapsList[level]);
	player = new Player(filepath);
	// Getting json info
	

	// set camera
	//_gui->cam->lookAt(glm::vec3(size.x / 2 + 0.5f, 1.0f, size.y * 0.7f));

	return true;
}




/**
 * @brief Update function to be called in menu
 *
 * @return false If failed
 */
bool	SceneGame::updateForMenu() {
	/* set camera position for menu */
	/*_gui->cam->setMode(CamMode::STATIC);
	_gui->cam->pos = glm::vec3(0, 1.2, 2.5);
	_gui->cam->lookAt(glm::vec3(0, 0.7, 0));
	*/
	return true;
}
void gotoxy1(int x, int y) {
	COORD pos = { (SHORT)x,(SHORT)y };
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hOut, pos);
}
/**
 * @brief update is called each frame.
 *
 * @return true
 * @return false
 */
bool	SceneGame::update() {
	
	entitiesCount.enemy = -1;
	entitiesCount.staticElements = -1;
	entitiesCount.players = -1;
	entitiesCount.total = -1;

	//if (level == NO_LEVEL)
		//return true;
	
	//if (_gui->cam->getMode() == CamMode::STATIC)
	//	_gui->cam->setMode(CamMode::STATIC_DEFPOS);

	//_gui->cam->update(_dtTime);
	
	//逻辑update
	if (player->Update()) {
		state = GameState::GAME_OVER;
		exit(0);
	}
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	GetConsoleCursorInfo(hOut, &info);
	info.bVisible = false;
	SetConsoleCursorInfo(hOut, &info);
	gotoxy1(0, 0);
	player->Display();
	//allUI.introText->setEnabled(state == GameState::INTRO);

	if (Inputs::getKeyUp(InputType::CANCEL)) {
		_gui->disableExitForThisFrame();
	}

	//if (Inputs::getKeyByScancodeUp(SDL_SCANCODE_H))
	//	_loadHelp = true;

	// set all enemies to Idle on win/loose
	

	 if (state == GameState::INTRO) {
		if ( Inputs::getKeyUp(InputType::CONFIRM) || Inputs::getKeyUp(InputType::ACTION)
			|| Inputs::getKeyUp(InputType::CANCEL)) {//_gui->cam->isFollowFinished() ||
			//_gui->cam->setMode(CamMode::STATIC_DEFPOS);
			state = GameState::PLAY;
		}
		return true;
	}
	else if (state == GameState::WIN) {
		
		// load victory menu on camera anim end
		/*if (_gui->cam->isFollowFinished() || Inputs::getKeyUp(InputType::CONFIRM) || Inputs::getKeyUp(InputType::ACTION)
			|| Inputs::getKeyUp(InputType::CANCEL)) {*/
			delete player;
			player = nullptr;
			SceneManager::loadScene(SceneNames::VICTORY);
			return true;
		/*}*/

		return true;
	}
	else if (state == GameState::GAME_OVER) {
		
		// load loosing menu on camera anim end
		/*if (_gui->cam->isFollowFinished() || Inputs::getKeyUp(InputType::CONFIRM) || Inputs::getKeyUp(InputType::ACTION)
			|| Inputs::getKeyUp(InputType::CANCEL)) {*/
			
			delete player;
			player = nullptr;
			SceneManager::loadScene(SceneNames::GAME_OVER);
		/*}*/

		return true;
	}
	
	_updateGameInfos();

	return true;
}


/**
 * @brief Update game informations
 */
void	SceneGame::_updateGameInfos() {
	//UI 控件
}

/**
 * @brief Update the blur Mask texture
 *
 * @param aMaskData mask texture raw data
 * @return true on success
 * @return false on failure
 */
bool	SceneGame::updateBlurMaskTex(std::vector<uint8_t> const& aMaskData) {
	
	// create the texture the first time
	if (_blurMaskTex == 0) {
		glGenTextures(1, &_blurMaskTex);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, _blurMaskTex);
		// create the texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R8,800, 600, 0, GL_RED,
			GL_UNSIGNED_BYTE, &aMaskData[0]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_2D, 0);
		// tell
		/*_blurShader->use();
		_blurShader->setInt("sceneTex", 0);
		_blurShader->setInt("blurMaskTex", 1);
		_blurShader->unuse();*/
	}
	// else just update the data
	else {
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, _blurMaskTex);
		// update texture data
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 800, 600, GL_RED,
			GL_UNSIGNED_BYTE, &aMaskData[0]);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	glActiveTexture(GL_TEXTURE0);

	return true;
}
