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
	enemies = std::vector<AEnemy *>();
	flags = 0;
	size = {0, 0};
	level = NO_LEVEL;
	state = GameState::PLAY;
	levelTime = 0;
	time = 0;
	levelEnemies = 0;
	levelCrispies = 0;
	//_terrain = nullptr;
	//_blurShader = 0;
	_rbo = 0;
	_blurMaskTex = 0;
	enemiesToKill = 0;
	enemiesKilled = 0;
	_alarm = false;
	//_loadHelp = false;
	
}

/**
 * @brief Destroy the Scene Game:: Scene Game object
 */
SceneGame::~SceneGame() {
	_unloadLevel();  // delete all elements of current level if needed

	if (player != nullptr) {
		delete player;
	}

	for (auto it = _mapsList.begin(); it != _mapsList.end(); it++) {
		delete *it;
	}
	_mapsList.clear();

	//É¾³ýÄ£ÐÍ

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
		enemies = rhs.enemies;
		flags = rhs.flags;
		size = rhs.size;
		level = rhs.level;
		state = rhs.state;
		levelTime = rhs.levelTime;
		time = rhs.time;
		levelEnemies = rhs.levelEnemies;
		levelCrispies = rhs.levelCrispies;
		enemiesToKill = rhs.enemiesToKill;
		enemiesKilled = rhs.enemiesKilled;
	}
	return *this;
}

/**
 * @brief Cout operator
 *
 * @param os The ostream object
 * @param myClass The class to cout
 * @return std::ostream& The ostream obj
 */
std::ostream &	operator<<(std::ostream & os, const SceneGame& myClass) {
	os << myClass.print();
	return (os);
}

/**
 * print params.
 */
std::string		SceneGame::print() const {
	std::string		str;
	str = "SceneGame info: [" + std::to_string(size.x) + ", "
	+ std::to_string(size.y) + "]";

	return str;
}

/**
 * @brief clear entity at position pos from board.
 *
 * @param entity
 * @param pos
 * @return true if cleared
 * @return false if not found
 */
//bool			SceneGame::clearFromBoard(AEntity *entity, glm::vec2 pos) {
//	std::vector<AEntity *> &box = board[pos.x][pos.y];
//	std::vector<AEntity *>::iterator find = std::find(box.begin(), box.end(), entity);
//	if (find == box.end())
//		return false;
//	box.erase(find);
//	return true;
//}

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
	_alarm = false;
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

/**
 * @brief Insert an entity in the game
 *
 * @param name Entity name
 * @param pos Entity position
 * @param isFly If entity is flying
 * @param wallGenPercent Percentage of wall generation
 * @return false If failed
 */
bool SceneGame::insertEntity(std::string const & name, glm::ivec2 pos, bool isFly, uint64_t wallGenPercent) {
	//AEntity * entity;

	///* if out of board */
	//if (!positionInGame({pos.x, 0, pos.y})) {
	//	return false;
	//}

	///* if invalid entity name */
	//if (entitiesCall.find(name) == entitiesCall.end()) {
	//	logErr("invalid entity name " << name << " in SceneGame::insertEntity");
	//	return false;
	//}

	///* if already an entity */
	//if (isFly) {
	//	if (boardFly[pos.x][pos.y].size() > 0)
	//		return false;
	//}
	//else {
	//	if (board[pos.x][pos.y].size() > 0)
	//		return false;
	//}

	//// if it's empty, generate crispy wall with a certain probability
	//if (name == "empty" && !isFly)
	//	entity = Crispy::generateCrispy(*this, wallGenPercent);
	//else
	//	entity = entitiesCall[name].entity(*this);

	//// do nothing on empty block
	//if (entity == nullptr)
	//	return true;

	//if (isFly) {
	//	if (entity->type == Type::WALL) {
	//		reinterpret_cast<AObject *>(entity)->isInFlyBoard = true;
	//		boardFly[pos.x][pos.y].push_back(entity);
	//	}
	//	else if (entitiesCall[name].entityType == EntityType::ENEMY) {
	//		enemies.push_back(reinterpret_cast<AEnemy *>(entity));
	//		enemies.back()->setPosition({pos.x, 1, pos.y});
	//	}
	//	else {
	//		logWarn("board fly can only contains walls and enemy");
	//	}
	//}
	//else {  // if not fly
	//	switch (entitiesCall[name].entityType) {
	//		case EntityType::PLAYER:
	//			if (player == nullptr) {
	//				player = reinterpret_cast<Player *>(entity);
	//				player->init();
	//				entity = nullptr;  // to avoid to call init a second time
	//			}
	//			else {
	//				delete entity;
	//				entity = nullptr;
	//			}
	//			player->setPosition({pos.x, 0, pos.y});
	//			break;
	//		case EntityType::BOARD_FLAG:
	//			flags++;
	//			board[pos.x][pos.y].push_back(entity);
	//			break;
	//		case EntityType::BOARD:
	//			if (entity->type == Type::BOMB) {
	//				if (player != nullptr) {
	//					reinterpret_cast<Bomb*>(entity)->setPropagation(player->bombProgation);
	//				}
	//				if (board[pos.x][pos.y].size() > 0) {
	//					delete entity;
	//					return false;
	//				}
	//			}
	//			board[pos.x][pos.y].push_back(entity);
	//			break;
	//		case EntityType::ENEMY:
	//			if (board[pos.x][pos.y].size() > 0) {
	//				logWarn("Cannot insert enemy on a block");
	//				delete entity;
	//				return false;
	//			}
	//			if (reinterpret_cast<AEnemy *>(entity)->getCollision({pos.x, 0, pos.y}).size()) {
	//				// don't create if we have a other Enemy at the same place
	//				delete entity;
	//				return false;
	//			}
	//			enemies.push_back(reinterpret_cast<AEnemy *>(entity));
	//			enemies.back()->setPosition({pos.x, 0, pos.y});
	//			break;
	//		default:
	//			delete entity;
	//	}
	//}

	//// init entity
	//if (entity && !entity->init()) {
	//	return false;
	//}

	return true;
}

// -- getter -------------------------------------------------------------------

/**
 * @brief Get the total number of levels
 *
 * @return uint32_t The number of levels
 */
uint32_t	SceneGame::getNbLevel() const { return _mapsList.size(); }
/**
 * @brief Get a level name
 *
 * @param levelId The level ID
 * @return std::string The level name
 */
std::string	SceneGame::getLevelName(int32_t levelId) const {
	if (levelId == NO_LEVEL)
		return "NO_LEVEL";
	if (static_cast<int32_t>(_mapsList.size()) <= levelId)
		throw SceneGameException(("Level " + std::to_string(levelId) + " do not exist.").c_str());
	return _mapsList[levelId]->s("name");
}
/**
 * @brief Get a level image
 *
 * @param levelId The level ID
 * @return std::string The level image path
 */
std::string	SceneGame::getLevelImg(int32_t levelId) const {
	if (levelId == NO_LEVEL) {
		logErr("can't get image for level 'NO_LEVEL'");
		return "";
	}
	return _mapsList[levelId]->s("img");
}

/**
 * @brief Return JSON Settings of level.
 *
 * @return SettingsJson& JSON Settings of level
 * @throw SceneGameException if error
 */
SettingsJson	&SceneGame::getSettingsLevel() const {
	if (level == NO_LEVEL)
		throw SceneGameException("no level set");
	if (level > (int32_t)_mapsList.size())
		throw SceneGameException(("unable to load level " + std::to_string(level)
		+ ": doesn't exist").c_str());
	return *(_mapsList[level]);
}

/**
 * @brief Get the name of all entity
 *
 * @return std::vector<std::string> The names
 */
std::vector<std::string> SceneGame::getAllEntityNames() {
	std::vector<std::string> res;
	/*for (auto && it : entitiesCall) {
		res.push_back(it.first);
	}*/
	return res;
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
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _gui->gameInfo.windowSize.x,
			_gui->gameInfo.windowSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
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
				_gui->gameInfo.windowSize.x, _gui->gameInfo.windowSize.y);
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
 * @brief Init the json for a level
 *
 * @param levelId The level ID
 * @return false If failed
 */
bool	SceneGame::_initJsonLevel(int32_t levelId) {
	// level$(levelID)
	std::stringstream ss;
	ss << "level" << std::setw(2) << std::setfill('0') << levelId;
	std::string		levelName = ss.str();
	// $(mapsPath)/$(levelName).json
	std::string		filename = s.s("mapsPath") + "/" + levelName + ".json";
	std::cout << filename;
	if (file::isFile(filename) == false) {
		return false;  // file does not exist
	}

	SettingsJson* lvl = new SettingsJson();

	lvl->name(levelName).description("Level map");
	lvl->add<std::string>(levelName + "Filename", filename);

	// File json definition:
	lvl->add<std::string>("name");
	lvl->add<std::string>("img", "bomberman-assets/img/icon_level1");
	lvl->add<std::string>("music", "");
	lvl->add<uint64_t>("height", 0).setMin(0).setMax(100);
	lvl->add<uint64_t>("width", 0).setMin(0).setMax(100);
	lvl->add<int64_t>("time", 0).setMin(-1).setMax(86400);
	lvl->add<int64_t>("enemiesToKill", 0).setMin(0).setMax(9999);

	// foreach empty zone, chance to create a wall
	lvl->add<uint64_t>("wallGenPercent", 40).setMin(0).setMax(100);

	lvl->add<SettingsJson>("objects");
	// this is outside of the platform (no floor)
	lvl->j("objects").add<std::string>("outside", ".");
	// replaced by a crispy wall or nothing (random)
	lvl->j("objects").add<std::string>("empty", " ");
	// unique player on game.
	//lvl->j("objects").add<std::string>(PLAYER_STR, "p");
	//// destructing element dropped by the player.
	//lvl->j("objects").add<std::string>(BOMB_STR, "x");
	//// indestructible element outside the board
	//lvl->j("objects").add<std::string>(WALL_STR, "w");
	//// indestructible element of the board
	//lvl->j("objects").add<std::string>("block", "b");
	//// destructable element, who can give bonuses randomly
	//lvl->j("objects").add<std::string>(CRISPY_STR, "c");
	//// flag to get end
	//lvl->j("objects").add<std::string>(FLAG_STR, "f");
	//// end of level when all flag
	//lvl->j("objects").add<std::string>(END_STR, "e");
	//// no spawn zone
	//lvl->j("objects").add<std::string>("safe", "_");
	///* enemies */
	//lvl->j("objects").add<std::string>(ENEMY_BASIC_STR, "0");
	//lvl->j("objects").add<std::string>(ENEMY_WITH_EYE_STR, "1");
	//lvl->j("objects").add<std::string>(ENEMY_FOLLOW_STR, "2");
	//lvl->j("objects").add<std::string>(ENEMY_FLY_STR, "3");
	//lvl->j("objects").add<std::string>(ENEMY_CRISPY_STR, "4");
	//lvl->j("objects").add<std::string>(ENEMY_FROG_STR, "5");

	SettingsJson* spawnerPattern = new SettingsJson();
	spawnerPattern->add<std::string>("typeEnemy", "");
	spawnerPattern->add<uint64_t>("frequency", 5);
	spawnerPattern->add<SettingsJson>("position");
	spawnerPattern->j("position").add<uint64_t>("x");
	spawnerPattern->j("position").add<uint64_t>("y");
	lvl->addList<SettingsJson>("spawner", spawnerPattern);

	SettingsJson* mapPattern = new SettingsJson();
	mapPattern->add<std::string>("0", "");
	mapPattern->add<std::string>("1", "");
	lvl->addList<SettingsJson>("map", mapPattern);

	lvl->add<SettingsJson>("bonus");
	//for (auto&& pair : Bonus::bonus) {
	//	lvl->j("bonus").add<SettingsJson>(pair.first);
	//	lvl->j("bonus").j(pair.first).add<int64_t>("chance", 0).setMin(0).setMax(100);
	//	lvl->j("bonus").j(pair.first).add<int64_t>("nb", -1).setMin(-1).setMax(100);
	//}
	//try {
	//	if (lvl->loadFile(filename) == false) {
	//		// warning when loading settings
	//		return true;
	//	}
	//}
	//catch (SettingsJson::SettingsException const& e) {
	//	logErr(e.what());
	//	return false;
	//}

	_mapsList.push_back(lvl);

	return true;
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
	/*for (auto&& box : board) {
		for (auto&& row : box) {
			std::vector<AEntity*>::iterator element = row.begin();
			AEntity* entity;
			while (element != row.end()) {
				entity = *element;
				row.erase(element);
				delete entity;
				element = row.begin();
			}
		}
	}
	board.clear();
	for (auto&& box : boardFly) {
		for (auto&& row : box) {
			std::vector<AEntity*>::iterator element = row.begin();
			AEntity* entity;
			while (element != row.end()) {
				entity = *element;
				row.erase(element);
				delete entity;
				element = row.begin();
			}
		}
	}
	boardFly.clear();
	std::vector<AEnemy*>::iterator it = enemies.begin();
	AEnemy* enemy;
	while (it != enemies.end()) {
		enemy = *it;
		enemies.erase(it);
		delete enemy;
		it = enemies.begin();
	}
	enemies.clear();*/


	// Delete old player
	delete player;
	player = nullptr;

	// spawners
	enemiesToKill = 0;
	enemiesKilled = 0;
	
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
	if (levelId > (int32_t)_mapsList.size()) {
		std::cout<<"unable to load level " << levelId << ": doesn't exist"<<"\n";
		return false;
	}
	string filepath = "C:\\Users\\1234\\Desktop\\bomberGameLogical\\map.txt";
	level = levelId;  // save new level ID
	SettingsJson& lvl = *(_mapsList[level]);
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
	
	//Âß¼­update
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
	//UI ¿Ø¼þ
}

/**
 * @brief Update the blur Mask texture
 *
 * @param aMaskData mask texture raw data
 * @return true on success
 * @return false on failure
 */
bool	SceneGame::updateBlurMaskTex(std::vector<uint8_t> const& aMaskData) {
	glm::vec2 winSize = _gui->gameInfo.windowSize;

	// create the texture the first time
	if (_blurMaskTex == 0) {
		glGenTextures(1, &_blurMaskTex);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, _blurMaskTex);
		// create the texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, winSize.x, winSize.y, 0, GL_RED,
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
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, winSize.x, winSize.y, GL_RED,
			GL_UNSIGNED_BYTE, &aMaskData[0]);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	glActiveTexture(GL_TEXTURE0);

	return true;
}
