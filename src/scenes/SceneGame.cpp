//#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#include "../../include/scenes/SceneGame.h"
#include "../../include/bomberman.h"
#include "../../include/utils/FileUtils.h"
//#include "Save.hpp"
//#include "ModelsManager.hpp"
//#include "BoxCollider.hpp"
//
//#include "Player.hpp"
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
	//score.reset();
	levelEnemies = 0;
	levelCrispies = 0;
	//_terrain = nullptr;
	//_blurShader = 0;
	_rbo = 0;
	_blurMaskTex = 0;
	enemiesToKill = 0;
	enemiesKilled = 0;
	_alarm = false;
	_loadHelp = false;
	
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

	/*delete _menuModels.player;
	delete _menuModels.robot;
	delete _menuModels.flower;
	delete _menuModels.fly;
	delete _menuModels.frog;
	delete _menuModels.crispy;
	delete _menuModels.follow;
	delete _terrain;*/

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
		/*logWarn("SceneGame object copied");
		board = rhs.board;
		boardFly = rhs.boardFly;
		player = rhs.player;
		enemies = rhs.enemies;
		flags = rhs.flags;
		size = rhs.size;
		level = rhs.level;
		state = rhs.state;
		levelTime = rhs.levelTime;
		time = rhs.time;
		score = rhs.score;
		levelEnemies = rhs.levelEnemies;
		levelCrispies = rhs.levelCrispies;
		enemiesToKill = rhs.enemiesToKill;
		enemiesKilled = rhs.enemiesKilled;*/
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
	}
	else if (state == GameState::PAUSE
	|| state == GameState::WIN
	|| state == GameState::GAME_OVER) {
		state = GameState::PLAY;
	}
	player->playPauseAnimation(true);*/
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
	blurFilterBefore();  // blur filter settings, enable framebuffer

	/* draw models */
	/*try {
		_menuModels.player->transform.setRot(0);
		_menuModels.player->transform.setPos({ -0.9, 0, 0 });
		if (_menuModels.player->getCurrentAnimationName() != "Armature|idle")
			_menuModels.player->setAnimation("Armature|idle");
		_menuModels.player->draw();

		_menuModels.flower->transform.setRot(0);
		_menuModels.flower->transform.setPos({ 0.9, 0, 0 });
		if (_menuModels.flower->getCurrentAnimationName() != "Armature|idle")
			_menuModels.flower->setAnimation("Armature|idle");
		_menuModels.flower->draw();
	}
	catch (OpenGLModel::ModelException const& e) {
		logErr(e.what());
		return false;
	}*/

	// draw skybox
	/*glm::mat4	view = _gui->cam->getViewMatrix();
	_gui->drawSkybox(view);*/

	blurFilterAfter();  // blur filter postprocess

	return true;
}

/**
 * @brief Draw function if we are in a level
 *
 * @return false If failed
 */
bool	SceneGame::drawGame() {
	if (state != GameState::INTRO) {
		blurFilterBefore();  // blur filter settings, enable framebuffer
	}

	// draw background terrain
	//if (s.j("debug").j("show").b("terrain")) {
	//	try {
	//		_terrain->draw();
	//	}
	//	catch (OpenGLModel::ModelException const& e) {
	//		logErr(e.what());
	//		return false;
	//	}
	//}

	//// draw floor
	//if (s.j("debug").j("show").b("baseBoard")) {
	//	for (uint32_t j = 0; j < size.y; j++) {
	//		for (uint32_t i = 0; i < size.x; i++) {
	//			if (floor[i][j])
	//				_gui->drawCube(Block::FLOOR, { i, -1.5f, j }, { 1, 1.5f, 1 });
	//		}
	//	}
	//}

	//std::vector<AEntity*>  endBlocs;

	//// draw board entities
	//for (auto&& board_it0 : board) {
	//	for (auto&& board_it1 : board_it0) {
	//		for (AEntity* board_it2 : board_it1) {
	//			if (board_it2->type == Type::END) {
	//				endBlocs.push_back(board_it2);
	//			}
	//			else {
	//				if (s.j("debug").j("show").b("baseBoard") && !board_it2->draw(*_gui))
	//					return false;
	//				if (s.j("debug").j("show").b("staticCollider") && !board_it2->drawCollider())
	//					return false;
	//			}
	//		}
	//	}
	//}

	//// draw flyng obstacles
	//for (auto&& board_it0 : boardFly) {
	//	for (auto&& board_it1 : board_it0) {
	//		for (AEntity* board_it2 : board_it1) {
	//			if (s.j("debug").j("show").b("flyHeight") && !board_it2->draw(*_gui))
	//				return false;
	//			if (s.j("debug").j("show").b("staticCollider") && !board_it2->drawCollider())
	//				return false;
	//		}
	//	}
	//}

	//// draw enemies
	//for (auto&& enemy : enemies) {
	//	if (s.j("debug").j("show").b("entity") && !enemy->draw(*_gui))
	//		return false;
	//	if (s.j("debug").j("show").b("movingCollider") && !enemy->drawCollider())
	//		return false;
	//}

	//// draw player
	//if (s.j("debug").j("show").b("entity") && !player->draw(*_gui))
	//	return false;
	//if (s.j("debug").j("show").b("movingCollider") && !player->drawCollider())
	//	return false;

	//// draw skybox
	//glm::mat4	view = _gui->cam->getViewMatrix();
	//_gui->drawSkybox(view);

	//// draw spawners
	//for (auto&& spawner : spawners) {
	//	if (!spawner->draw(*_gui))
	//		return false;
	//}

	//// draw end blocks (last for transparency issue)
	//for (AEntity* end : endBlocs) {
	//	if (s.j("debug").j("show").b("baseBoard") && !end->draw(*_gui))
	//		return false;
	//	if (s.j("debug").j("show").b("staticCollider") && !end->drawCollider())
	//		return false;
	//}
	//endBlocs.clear();

	//// release cubeShader and textures
	//_gui->cubeShader->use();
	//_gui->textureManager->disableTextures();
	//_gui->cubeShader->unuse();

	//// draw ui on top
	//if (state == GameState::PLAY
	//	&& (allUI.timeLeftImg->getPos() != VOID_SIZE || allUI.timeLeftImgActive->getPos() != VOID_SIZE))
	//{
	//	ASceneMenu::draw();
	//}
	//else if (state == GameState::INTRO) {
	//	allUI.introText->draw();
	//}

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
	blurFilterBefore();  // blur filter settings, enable framebuffer

	/* draw models */
	/*try {
		_menuModels.player->transform.setRot(0);
		_menuModels.player->transform.setPos({ -1, 0, 0 });
		if (_menuModels.player->getCurrentAnimationName() != "Armature|dance")
			_menuModels.player->setAnimation("Armature|dance");
		_menuModels.player->draw();

		_menuModels.flower->transform.setRot(0);
		_menuModels.flower->transform.setPos({ 1, 0, 0 });
		if (_menuModels.flower->getCurrentAnimationName() != "Armature|loose")
			_menuModels.flower->setAnimation("Armature|loose");
		_menuModels.flower->draw();
	}
	catch (OpenGLModel::ModelException const& e) {
		logErr(e.what());
		return false;
	}*/

	// draw skybox
	/*glm::mat4	view = _gui->cam->getViewMatrix();
	_gui->drawSkybox(view);*/

	blurFilterAfter();  // blur filter postprocess

	return true;
}

/**
 * @brief Draw function if we are in game over menu
 *
 * @return false If failed
 */
bool	SceneGame::drawGameOver() {
	blurFilterBefore();  // blur filter settings, enable framebuffer

	/* draw models */
	/*try {
		_menuModels.player->transform.setRot(0);
		_menuModels.player->transform.setPos({ -1, 0, 0 });
		if (_menuModels.player->getCurrentAnimationName() != "Armature|loose")
			_menuModels.player->setAnimation("Armature|loose");
		_menuModels.player->draw();

		_menuModels.flower->transform.setRot(0);
		_menuModels.flower->transform.setPos({ 1, 0, 0 });
		if (_menuModels.flower->getCurrentAnimationName() != "Armature|dance")
			_menuModels.flower->setAnimation("Armature|dance");
		_menuModels.flower->draw();
	}
	catch (OpenGLModel::ModelException const& e) {
		logErr(e.what());
		return false;
	}*/

	// draw skybox
	/*glm::mat4	view = _gui->cam->getViewMatrix();
	_gui->drawSkybox(view);*/

	blurFilterAfter();  // blur filter postprocess

	return true;
}

/**
 * @brief Draw function if we are in endGame menu
 *
 * @return false If failed
 */
bool	SceneGame::drawEndGame() {
	/* draw simple floor */
	//_gui->drawCube(Block::FLOOR, { -10.0, -1.5, -5.0 }, { 20.0, 0.5, 5.0 });

	/* draw models */
	//try {
	//	float tmpX = _menuModels.player->transform.getPos().x;
	//	if (tmpX < -3 || tmpX > 10)
	//		tmpX = -3;
	//	tmpX += 0.03;

	//	_menuModels.player->animationSpeed = 0.8;
	//	_menuModels.player->transform.setPos({ tmpX, -1, -2 });
	//	_menuModels.player->transform.setRot(glm::radians(90.0));
	//	if (_menuModels.player->getCurrentAnimationName() != "Armature|run")
	//		_menuModels.player->setAnimation("Armature|run");
	//	_menuModels.player->draw();

	//	tmpX -= 1.3;
	//	_menuModels.robot->transform.setPos({ tmpX, -1, -2 });
	//	_menuModels.robot->transform.setRot(glm::radians(90.0));
	//	if (_menuModels.robot->getCurrentAnimationName() != "Armature|run")
	//		_menuModels.robot->setAnimation("Armature|run");
	//	_menuModels.robot->draw();

	//	tmpX -= 1.3;
	//	_menuModels.flower->transform.setPos({ tmpX, -1, -2 });
	//	_menuModels.flower->transform.setRot(glm::radians(90.0));
	//	if (_menuModels.flower->getCurrentAnimationName() != "Armature|run")
	//		_menuModels.flower->setAnimation("Armature|run");
	//	_menuModels.flower->draw();

	//	tmpX -= 1.3;
	//	_menuModels.crispy->transform.setPos({ tmpX, -1, -2 });
	//	_menuModels.crispy->transform.setRot(glm::radians(90.0));
	//	if (_menuModels.crispy->getCurrentAnimationName() != "Armature|run")
	//		_menuModels.crispy->setAnimation("Armature|run");
	//	_menuModels.crispy->draw();

	//	tmpX -= 1.3;
	//	_menuModels.follow->transform.setPos({ tmpX, -1, -2 });
	//	_menuModels.follow->transform.setRot(glm::radians(90.0));
	//	if (_menuModels.follow->getCurrentAnimationName() != "Armature|run")
	//		_menuModels.follow->setAnimation("Armature|run");
	//	_menuModels.follow->draw();

	//	tmpX -= 1.3;
	//	_menuModels.fly->transform.setPos({ tmpX, .2, -2 });
	//	_menuModels.fly->transform.setRot(glm::radians(90.0));
	//	if (_menuModels.fly->getCurrentAnimationName() != "Armature|run")
	//		_menuModels.fly->setAnimation("Armature|run");
	//	_menuModels.fly->draw();

	//	_menuModels.frog->draw();
	//}
	//catch (OpenGLModel::ModelException const& e) {
	//	logErr(e.what());
	//	return false;
	//}

	//// draw skybox
	//glm::mat4	view = _gui->cam->getViewMatrix();
	//_gui->drawSkybox(view);

	return true;
}

/**
 * @brief get intro animation
 *
 * @return std::vector<CamPoint> Animation
 */
//std::vector<CamPoint>	SceneGame::_getIntroAnim() const {
//	return {
//		{
//			{80, 80, 150},  // pos
//			CamMovement::NoDirection,  // lookDir
//			{size.x / 2, 1, size.y / 2},  // lookAt
//			true,  // tpTo
//			-1,  // speed
//		},
//		{
//			{size.x / 2, 32, size.y / 2},  // pos
//			CamMovement::NoDirection,  // lookDir
//			{size.x / 2, 1, size.y / 2},  // lookAt
//			false,  // tpTo
//			_gui->cam->movementSpeed * 5,  // speed
//		},
//		{
//			{size.x / 2 + 5, 45, size.y / 2 + 5},  // pos
//			CamMovement::NoDirection,  // lookDir
//			{size.x / 2, 1, size.y / 2},  // lookAt
//			false,  // tpTo
//			_gui->cam->movementSpeed / 3,  // speed
//		},
//
//		{
//			{-15, 20, -15},  // pos
//			CamMovement::NoDirection,  // lookDir
//			{size.x / 2, 1, size.y / 2},  // lookAt
//			true,  // tpTo
//			-1,  // speed
//		},
//		{
//			{-15, 20, size.y + 15},  // pos
//			CamMovement::NoDirection,  // lookDir
//			{size.x / 2, 1, size.y / 2},  // lookAt
//			false,  // tpTo
//			-1,  // speed
//		},
//
//		{
//			_gui->cam->getDefPos(),  // pos
//			CamMovement::NoDirection,  // lookDir
//			{size.x / 2, 1, size.y / 2},  // lookAt
//			false,  // tpTo
//			-1,  // speed
//		},
//	};
//}

/**
 * @brief get game over animation
 *
 * @return std::vector<CamPoint> Animation
 */
//std::vector<CamPoint>	SceneGame::_getGameOverAnim() const {
//	return {
//		{
//			_gui->cam->getDefPos(),  // pos
//			CamMovement::NoDirection,  // lookDir
//			player->position,  // lookAt
//			true,  // tpTo
//			-1,  // speed
//		},
//		{
//			{player->position.x, player->position.y + 7, player->position.z},  // pos
//			CamMovement::NoDirection,  // lookDir
//			player->position,  // lookAt
//			false,  // tpTo
//			-1,  // speed
//		},
//		{
//			{player->position.x, player->position.y + 20, player->position.z + 5},  // pos
//			CamMovement::NoDirection,  // lookDir
//			player->position,  // lookAt
//			false,  // tpTo
//			_gui->cam->movementSpeed / 5,  // speed
//		},
//		{
//			{player->position.x, player->position.y + 1, player->position.z},  // pos
//			CamMovement::NoDirection,  // lookDir
//			player->position,  // lookAt
//			false,  // tpTo
//			_gui->cam->movementSpeed * 5,  // speed
//		},
//	};
//}

/**
 * @brief get victory animation
 *
 * @return std::vector<CamPoint> Animation
 */
//std::vector<CamPoint>	SceneGame::_getVictoryAnim() const {
//	return {
//		{
//			_gui->cam->getDefPos(),  // pos
//			CamMovement::NoDirection,  // lookDir
//			player->position,  // lookAt
//			true,  // tpTo
//			-1,  // speed
//		},
//		{
//			{player->position.x, player->position.y + 7, player->position.z},  // pos
//			CamMovement::NoDirection,  // lookDir
//			player->position,  // lookAt
//			false,  // tpTo
//			-1,  // speed
//		},
//		{
//			{player->position.x, player->position.y + 16, player->position.z + 8},  // pos
//			CamMovement::NoDirection,  // lookDir
//			player->position,  // lookAt
//			false,  // tpTo
//			_gui->cam->movementSpeed / 5,  // speed
//		},
//	};
//}

/**
 * @brief Call this function before drawing operation you want the ui to blur
 */
void	SceneGame::blurFilterBefore() {
	// first pass, draw the scene in framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, _blurFbo[0]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
}
/**
 * @brief Call this function after drawing operation you want the ui to blur
 */
void	SceneGame::blurFilterAfter() {
	// Second pass the apply blur effect
	uint32_t	nbPass = 6;
	uint32_t	totalPass = nbPass * 2;
	bool		horizontal = true;

	glDisable(GL_DEPTH_TEST);
	for (uint32_t i = 0; i < totalPass; ++i) {
		if (i < totalPass - 1) {
			glBindFramebuffer(GL_FRAMEBUFFER, _blurFbo[(i + 1) % 2]);
		}
		else {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
		glClear(GL_COLOR_BUFFER_BIT);

		/*_blurShader->use();
		_blurShader->setBool("horizontal", horizontal);*/
		glBindVertexArray(_ppShVao);
		// fbo texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _blurTexColor[i % 2]);
		// mask texture
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, _blurMaskTex);
		// draw the quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glActiveTexture(GL_TEXTURE0);
		/*_blurShader->unuse();*/

		horizontal = !horizontal;
	}
	glEnable(GL_DEPTH_TEST);
}



/**
 * init game method.
 */
bool			SceneGame::init() {
	int32_t i = 0;
	while (_initJsonLevel(i)) {
		if (i >= 100000) {  // max level
			break;
		}
		i++;
	}

	entitiesCount.enemy = -1;
	entitiesCount.staticElements = -1;
	entitiesCount.players = -1;
	entitiesCount.total = -1;

	/*try {*/
		/* load all models for menu */
	//	_menuModels.player = new Model(ModelsManager::getModel("white"), getDtTime(),
	//		ETransform({ 0, 0, 0 }, PLAYER_SIZE));
	//	_menuModels.player->play = true;
	//	_menuModels.player->loopAnimation = true;
	//	_menuModels.player->setAnimation("Armature|idle");

	//	_menuModels.flower = new Model(ModelsManager::getModel("flower"), getDtTime(),
	//		ETransform({ 0, 0, 0 }, ENEMY_WITH_EYE_SIZE));
	//	_menuModels.flower->play = true;
	//	_menuModels.flower->loopAnimation = true;
	//	_menuModels.flower->setAnimation("Armature|idle");

	//	_menuModels.robot = new Model(ModelsManager::getModel("robot"), getDtTime(),
	//		ETransform({ 0, 0, 0 }, ENEMY_BASIC_SIZE));
	//	_menuModels.robot->play = true;
	//	_menuModels.robot->animationSpeed = 1.6;
	//	_menuModels.robot->loopAnimation = true;
	//	_menuModels.robot->setAnimation("Armature|idle");

	//	_menuModels.fly = new Model(ModelsManager::getModel("flyingBot"), getDtTime(),
	//		ETransform({ 0, 0, 0 }, ENEMY_FLY_SIZE));
	//	_menuModels.fly->play = true;
	//	_menuModels.fly->loopAnimation = true;
	//	_menuModels.fly->setAnimation("Armature|run");

	//	_menuModels.frog = new Model(ModelsManager::getModel("frog"), getDtTime(),
	//		ETransform({ 0, 0, 0 }, ENEMY_FROG_SIZE));
	//	_menuModels.frog->play = true;
	//	_menuModels.frog->loopAnimation = true;
	//	_menuModels.frog->setAnimation("Armature|idle");
	//	_menuModels.frog->transform.setPos({ -2, -1, -4 });
	//	_menuModels.frog->transform.setRot(glm::radians(30.0));

	//	_menuModels.crispy = new Model(ModelsManager::getModel("crispyEnemy"), getDtTime(),
	//		ETransform({ 0, 0, 0 }, ENEMY_CRISPY_SIZE));
	//	_menuModels.crispy->play = true;
	//	_menuModels.crispy->animationSpeed = 1.6;
	//	_menuModels.crispy->loopAnimation = true;
	//	_menuModels.crispy->setAnimation("Armature|run");

	//	_menuModels.follow = new Model(ModelsManager::getModel("follow"), getDtTime(),
	//		ETransform({ 0, 0, 0 }, ENEMY_FOLLOW_SIZE));
	//	_menuModels.follow->play = true;
	//	_menuModels.follow->animationSpeed = 1.4;
	//	_menuModels.follow->loopAnimation = true;
	//	_menuModels.follow->setAnimation("Armature|idle");

	//	// init terrain model
	//	if (!_terrain) {
	//		_terrain = new Model(ModelsManager::getModel("terrain"), getDtTime());
	//	}

	//	/* loading sentence */
	//	allUI.introText = &addText({ 0, 0 }, { _gui->gameInfo.windowSize.x, 70 }, "")
	//		.setTextAlign(TextAlign::CENTER)
	//		.setColor(colorise(s.j("colors").j("bg-rect").u("color"), s.j("colors").j("bg-rect").u("alpha")))
	//		.setZ(0.5);
	//}
	//catch (ModelsManager::ModelsManagerException const& e) {
	//	logErr(e.what());
	//	return false;
	//}
	//catch (OpenGLModel::ModelException const& e) {
	//	logErr(e.what());
	//	return false;
	//}

	//_initGameInfos();

	//if (!_initPostProcess()) {
	//	logErr("Failed to init postProcess");
	//	return false;
	//}

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
	/*try {
		allUI.timeLeftImg = &addImage(VOID_SIZE, VOID_SIZE, "bomberman-assets/textures/bonus/time.png");
		allUI.timeLeftImgActive = &addImage(VOID_SIZE, VOID_SIZE, "bomberman-assets/textures/bonus/time_active.png");
		allUI.timeLeftText = &addText(VOID_SIZE, VOID_SIZE, "time-left").setTextAlign(TextAlign::RIGHT);
		allUI.scoreImg = &addImage(VOID_SIZE, VOID_SIZE, "bomberman-assets/textures/bonus/score.png");
		allUI.scoreImgActive = &addImage(VOID_SIZE, VOID_SIZE, "bomberman-assets/textures/bonus/score_active.png");
		allUI.scoreText = &addText(VOID_SIZE, VOID_SIZE, "score").setTextAlign(TextAlign::RIGHT);
		allUI.lifeImg = &addImage(VOID_SIZE, VOID_SIZE, "bomberman-assets/textures/bonus/life.png");
		allUI.lifeImgActive = &addImage(VOID_SIZE, VOID_SIZE, "bomberman-assets/textures/bonus/life_active.png");
		allUI.lifeText = &addText(VOID_SIZE, VOID_SIZE, "nb-player-lives").setTextAlign(TextAlign::RIGHT);
		allUI.levelNameText = &addText(VOID_SIZE, VOID_SIZE, "level-name").setTextAlign(TextAlign::RIGHT);
		allUI.enemiesCounterText = &addText(VOID_SIZE, VOID_SIZE, "nb-enemies").setTextAlign(TextAlign::RIGHT);
		allUI.speedImg = &addImage(VOID_SIZE, VOID_SIZE, "bomberman-assets/textures/bonus/speed.png");
		allUI.speedImgActive = &addImage(VOID_SIZE, VOID_SIZE, "bomberman-assets/textures/bonus/speed_active.png");
		allUI.speedText = &addText(VOID_SIZE, VOID_SIZE, "speed").setTextAlign(TextAlign::RIGHT);
		allUI.bonusBombImg = &addImage(VOID_SIZE, VOID_SIZE, "bomberman-assets/textures/bonus/bomb.png");
		allUI.bonusBombImgActive = &addImage(VOID_SIZE, VOID_SIZE, "bomberman-assets/textures/bonus/bomb_active.png");
		allUI.bonusBombText = &addText(VOID_SIZE, VOID_SIZE, "total-bombs").setTextAlign(TextAlign::RIGHT);
		allUI.bonusFlameImg = &addImage(VOID_SIZE, VOID_SIZE, "bomberman-assets/textures/bonus/flame.png");
		allUI.bonusFlameImgActive = &addImage(VOID_SIZE, VOID_SIZE, "bomberman-assets/textures/bonus/flame_active.png");
		allUI.bonusFlameText = &addText(VOID_SIZE, VOID_SIZE, "bomb-propagation").setTextAlign(TextAlign::RIGHT);
		allUI.bonusFlampassImg = &addImage(VOID_SIZE, VOID_SIZE, "bomberman-assets/textures/bonus/flampass.png");
		allUI.bonusFlampassImgActive = &addImage(VOID_SIZE, VOID_SIZE,
			"bomberman-assets/textures/bonus/flampass_active.png");
		allUI.bonusWallpassImg = &addImage(VOID_SIZE, VOID_SIZE, "bomberman-assets/textures/bonus/wallpass.png");
		allUI.bonusWallpassImgActive = &addImage(VOID_SIZE, VOID_SIZE,
			"bomberman-assets/textures/bonus/wallpass_active.png");
		allUI.bonusDetonatorImg = &addImage(VOID_SIZE, VOID_SIZE, "bomberman-assets/textures/bonus/detonator.png");
		allUI.bonusDetonatorImgActive = &addImage(VOID_SIZE, VOID_SIZE,
			"bomberman-assets/textures/bonus/detonator_active.png");
		allUI.bonusBombpassImg = &addImage(VOID_SIZE, VOID_SIZE, "bomberman-assets/textures/bonus/bombpass.png");
		allUI.bonusBombpassImgActive = &addImage(VOID_SIZE, VOID_SIZE,
			"bomberman-assets/textures/bonus/bombpass_active.png");
		allUI.bonusShieldImg = &addImage(VOID_SIZE, VOID_SIZE, "bomberman-assets/textures/bonus/shield.png");
		allUI.bonusShieldImgActive = &addImage(VOID_SIZE, VOID_SIZE,
			"bomberman-assets/textures/bonus/shield_active.png");
		allUI.bonusShieldText = &addText(VOID_SIZE, VOID_SIZE, "invulnerable").setTextAlign(TextAlign::RIGHT);
	}
	catch (ABaseUI::UIException const& e) {
		logErr(e.what());
	}*/
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
	logDebug("load level " << levelId);
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
	//state = GameState::INTRO;
	//AudioManager::pauseMusic();


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
	logDebug("Unload level " << level);

	/*floor.clear();
	for (auto&& box : board) {
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
	boardFly.clear();*/
	std::vector<AEnemy*>::iterator it = enemies.begin();
	AEnemy* enemy;
	while (it != enemies.end()) {
		enemy = *it;
		enemies.erase(it);
		delete enemy;
		it = enemies.begin();
	}
	enemies.clear();

	for (auto spawner : spawners) {
		delete spawner;
	}
	spawners.clear();

	// Delete old player
	delete player;
	player = nullptr;

	// spawners
	enemiesToKill = 0;
	enemiesKilled = 0;
	musicLevel = "";

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
		logErr("unable to load level " << levelId << ": doesn't exist");
		return false;
	}

	level = levelId;  // save new level ID
	SettingsJson& lvl = *(_mapsList[level]);

	// Getting json info
	/*size = { lvl.u("width"), lvl.u("height") };
	floor = std::vector< std::vector< bool > >(size.x,
		std::vector< bool >(size.y, true));
	board = std::vector< std::vector<std::vector<AEntity*>> >(size.x,
		std::vector< std::vector<AEntity*> >(size.y,
			std::vector< AEntity* >()));
	boardFly = std::vector< std::vector<std::vector<AEntity*>> >(size.x,
		std::vector< std::vector<AEntity*> >(size.y,
			std::vector< AEntity* >()));

	if (lvl.lj("map").list.size() != size.y)
		throw SceneException("Map height error");*/

	levelTime = lvl.i("time");

	enemiesToKill = lvl.i("enemiesToKill");

	
	//_terrain->transform.setPos({ 5, -2, 0 });

	flags = 0;
	// Get map informations
	//for (uint32_t j = 0; j < size.y; j++) {
	//	// base board creation
	//	std::string line = lvl.lj("map").list[j]->s("0");

	//	// throw on bad line width
	//	if (line.length() != size.x)
	//		throw SceneException(("Map width error on line " + std::to_string(j)).c_str());

	//	// loop through line to create the entities
	//	for (uint32_t i = 0; i < size.x; i++) {
	//		std::string name;
	//		for (auto&& entitYCall : entitiesCall) {
	//			if (line[i] == lvl.j("objects").s(entitYCall.first)[0])
	//				name = entitYCall.first;
	//		}
	//		if (name != "") {
	//			if (name == "outside") {
	//				floor[i][j] = false;  // mo floor -> outside the platform
	//			}
	//			else if (insertEntity(name, { i, j }, false, lvl.u("wallGenPercent")) == false)
	//				throw SceneException("Unexpected error in map loading");
	//		}
	//		else {
	//			throw SceneException(("Invalid element in map (" + std::to_string(i) + ", "
	//				+ std::to_string(j) + "): " + line[i]).c_str());
	//		}
	//	}

	//	/* fly board creation */
	//	line = lvl.lj("map").list[j]->s("1");
	//	if (line.length() != size.x)
	//		throw SceneException(("Map fly width error on line " + std::to_string(j)).c_str());
	//	for (uint32_t i = 0; i < size.x; i++) {
	//		std::string name;
	//		for (auto&& entitYCall : entitiesCall) {
	//			if (line[i] == lvl.j("objects").s(entitYCall.first)[0])
	//				name = entitYCall.first;
	//		}
	//		if (name != "") {
	//			if (insertEntity(name, { i, j }, true, lvl.u("wallGenPercent")) == false)
	//				throw SceneException("Unexpected error in map loading");
	//		}
	//		else {
	//			throw SceneException(("Invalid element in map (" + std::to_string(i) + ", "
	//				+ std::to_string(j) + "): " + line[i]).c_str());
	//		}
	//	}
	//}

	//for (auto spawner : lvl.lj("spawner").list) {
	//	std::vector<std::string> typeEnemy;
	//	if (spawner->s("typeEnemy").size() == 0)
	//		throw SceneException("No type enemies defined in spawner");
	//	glm::uvec2 spawnerPos = { spawner->j("position").u("x"), spawner->j("position").u("y") };
	//	if (spawnerPos.x >= size.x || spawnerPos.y >= size.y) {
	//		throw SceneException("Wrong position for spawner");
	//	}
	//	bool	found = false;
	//	for (auto character : spawner->s("typeEnemy")) {
	//		found = false;
	//		for (auto&& entityCall : entitiesCall) {
	//			if (character == lvl.j("objects").s(entityCall.first)[0]) {
	//				if (entityCall.second.entityType != EntityType::ENEMY)
	//					throw SceneException("Spawner only for enemies.");
	//				typeEnemy.push_back(entityCall.first);
	//				found = true;
	//			}
	//		}
	//		if (!found)
	//			throw SceneException("Spawner only for enemies.");
	//	}
	//	Spawner* spawnerEntity = new Spawner(*this);
	//	spawnerEntity->setFrequency(spawner->u("frequency"))
	//		.setTypeEnemy(typeEnemy)
	//		.setPos({ spawnerPos.x, 0, spawnerPos.y });
	//	spawnerEntity->init();
	//	spawners.push_back(spawnerEntity);
	//}

	/*if (player == nullptr)
		throw SceneException("No player on this level.");

	bool	end = false;
	for (uint32_t j = 0; j < size.y; j++) {
		for (uint32_t i = 0; i < size.x; i++) {
			for (auto entity : board[i][j]) {
				if (entity->type == Type::END)
					end = true;
			}
		}
	}
	if (!end)
		throw SceneException("No end on this level.");*/


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

	if (level == NO_LEVEL)
		return true;

	//if (_gui->cam->getMode() == CamMode::STATIC)
	//	_gui->cam->setMode(CamMode::STATIC_DEFPOS);

	//_gui->cam->update(_dtTime);

	//if (player->getState() == EntityState::DYING)
	//	state = GameState::GAME_OVER;

	//allUI.introText->setEnabled(state == GameState::INTRO);

	//if (Inputs::getKeyUp(InputType::CANCEL)) {
	//	if (state != GameState::INTRO && state != GameState::WIN && state != GameState::GAME_OVER)
	//		state = GameState::PAUSE;
	//	else
	//		_gui->disableExitForThisFrame();
	//}

	//if (Inputs::getKeyByScancodeUp(SDL_SCANCODE_H))
	//	_loadHelp = true;

	//// set all enemies to Idle on win/loose
	//if (state == GameState::WIN || state == GameState::GAME_OVER) {
	//	for (auto&& enemy : enemies) {
	//		enemy->alive = false;

	//		if (enemy->getState() != EntityState::IDLE) {
	//			enemy->setState(EntityState::IDLE);
	//			enemy->update();

	//			// reset EnemyCrispy rot/pos
	//			if (enemy->name == ENEMY_CRISPY_STR) {
	//				reinterpret_cast<EnemyCrispy*>(enemy)->setIdlePos();
	//			}
	//		}
	//	}
	//}

	//if (state == GameState::PAUSE || _loadHelp) {
	//	AudioManager::pauseAllSounds();
	//	if (_loadHelp) {
	//		AudioManager::resumeAllSounds();
	//		_loadHelp = false;
	//		state = GameState::PAUSE;
	//		// open help menu
	//		SceneManager::loadScene(SceneNames::HELP);
	//	}
	//	else {
	//		// open pause menu
	//		SceneManager::loadScene(SceneNames::PAUSE);
	//	}
	//	player->playPauseAnimation(false);
	//	return true;
	//}
	//else if (state == GameState::INTRO) {
	//	if (_gui->cam->isFollowFinished() || Inputs::getKeyUp(InputType::CONFIRM) || Inputs::getKeyUp(InputType::ACTION)
	//		|| Inputs::getKeyUp(InputType::CANCEL)) {
	//		_gui->cam->setMode(CamMode::STATIC_DEFPOS);
	//		AudioManager::playMusic(musicLevel, 0.3f, true);
	//		AudioManager::stopSound(INTROLEVEL_SOUND, 500);
	//		state = GameState::PLAY;
	//		if (level == 0)
	//			_loadHelp = true;
	//	}
	//	return true;
	//}
	//else if (state == GameState::WIN) {
	//	if (_gui->cam->getMode() != CamMode::FOLLOW_PATH) {
	//		// calculate score and save it
	//		int32_t	crispiesLast = 0;
	//		for (auto&& box : board) {
	//			for (auto&& row : box) {
	//				for (auto&& element : row) {
	//					if (element->type == Type::CRISPY)
	//						crispiesLast++;
	//				}
	//			}
	//		}
	//		score.addBonusTime(levelTime, time);
	//		score.addBonusEnemies(levelEnemies, levelEnemies - enemiesKilled, levelCrispies, crispiesLast);
	//		Save::updateSavedFile(*this, true);
	//		Save::save(true);

	//		// manage winning sounds
	//		AudioManager::stopAllSounds();
	//		try {
	//			AudioManager::playSound(WIN_SOUND, 1.0f, false, true, 2000);
	//		}
	//		catch (Sound::SoundException const& e) {
	//			logErr(e.what());
	//		}

	//		player->startWinAnim();

	//		// start win camera animation
	//		_gui->cam->setMode(CamMode::FOLLOW_PATH);
	//		_gui->cam->setFollowPath(_getVictoryAnim());
	//	}

	//	player->update();

	//	// load victory menu on camera anim end
	//	if (_gui->cam->isFollowFinished() || Inputs::getKeyUp(InputType::CONFIRM) || Inputs::getKeyUp(InputType::ACTION)
	//		|| Inputs::getKeyUp(InputType::CANCEL)) {
	//		delete player;
	//		player = nullptr;
	//		SceneManager::loadScene(SceneNames::VICTORY);
	//		return true;
	//	}

	//	return true;
	//}
	else if (state == GameState::GAME_OVER) {
		//if (_gui->cam->getMode() != CamMode::FOLLOW_PATH) {
		//	
		//	

		//	// start loose camera animation
		//	_gui->cam->setMode(CamMode::FOLLOW_PATH);
		//	_gui->cam->setFollowPath(_getGameOverAnim());

		//	if (player->getState() != EntityState::DYING) {
		//		player->setState(EntityState::LOOSE_EMOTE);
		//		player->update();
		//	}
		//}

		//// load loosing menu on camera anim end
		//if (_gui->cam->isFollowFinished() || Inputs::getKeyUp(InputType::CONFIRM) || Inputs::getKeyUp(InputType::ACTION)
		//	|| Inputs::getKeyUp(InputType::CANCEL)) {
		//	// clear game infos.
		//	player->resetParams();
		//	Save::updateSavedFile(*this, false);
		//	Save::save(true);
		//	delete player;
		//	player = nullptr;
		//	SceneManager::loadScene(SceneNames::GAME_OVER);
		//}

		return true;
	}

	// manage level time limit
	time += _dtTime;
	if ((levelTime - time) < 0) {
		state = GameState::GAME_OVER;
		return true;
	}
	// play alarm_time sound when there is not much time left
	if ((levelTime - time) < 20) {
		if (!_alarm) {
			
			_alarm = true;
		}
	}

	// switch to fps camera on key press
#if DEBUG
	if (Inputs::getKeyByScancodeUp(SDL_SCANCODE_C)) {
		if (_gui->cam->getMode() == CamMode::FPS) {
			_gui->cam->setMode(CamMode::STATIC_DEFPOS);
		}
		else if (_gui->cam->getMode() == CamMode::STATIC_DEFPOS) {
			_gui->cam->setMode(CamMode::FPS);
		}
	}
	_gui->enableCursor(_gui->cam->getMode() != CamMode::FPS);  // disable cursor in FPS mode
#endif

// update board entities
	/*entitiesCount.staticElements = 0;
	for (auto&& board_it0 : board) {
		for (auto&& board_it1 : board_it0) {
			for (AEntity* board_it2 : board_it1) {
				entitiesCount.staticElements++;
				if (!board_it2->update())
					return false;
			}
		}
	}
	for (auto&& board_it0 : boardFly) {
		for (auto&& board_it1 : board_it0) {
			entitiesCount.staticElements += board_it1.size();
		}
	}
	entitiesCount.enemy = enemies.size();
	for (auto&& enemy : enemies) {
		if (!enemy->update())
			return false;
	}
	entitiesCount.staticElements += spawners.size();
	for (auto&& spawner : spawners) {
		if (!spawner->update())
			return false;
	}
	entitiesCount.players = 1;
	if (!player->update()) {
		return false;
	}

	entitiesCount.total =
		entitiesCount.enemy +
		entitiesCount.staticElements +
		entitiesCount.players;*/

	_updateGameInfos();

	return postUpdate();
}

/**
 * @brief postUpdate is called each frame, after update.
 *
 * @return true
 * @return false
 */
bool	SceneGame::postUpdate() {
	/*player->postUpdate();
	auto enemy = enemies.begin();
	while (enemy != enemies.end()) {
		if (!(*enemy)->postUpdate()) {
			enemy = enemies.begin();
			continue;
		}
		enemy++;
	}*/

	/*for (auto&& board_it0 : board) {
		for (auto&& board_it1 : board_it0) {
			std::vector<AEntity*>::iterator it = board_it1.begin();
			AEntity* copy;
			while (it != board_it1.end()) {
				copy = *it;
				if (!(*it)->postUpdate()) {
					it = board_it1.begin();
					continue;
				}
				if (it == board_it1.end())
					continue;
				if (copy == *it)
					it++;
			}
		}
	}*/

	return true;
}

/**
 * @brief Update game informations
 */
void	SceneGame::_updateGameInfos() {
	//glm::vec2	winSz = _gui->gameInfo.windowSize;
	//glm::vec2	tmpPos;
	//float		imgY;
	//float		textY;
	//glm::vec2	tmpSize;
	//uint32_t	padding = 5;
	//float		menuWidth = winSz.x / 2;
	//float		menuHeight = menuWidth / 8;

	//try {
	//	tmpPos.x = (winSz.x / 2) - (menuWidth / 2);
	//	tmpPos.y = winSz.y - menuHeight * 2;
	//	imgY = tmpPos.y;
	//	textY = tmpPos.y + 2;
	//	tmpSize.x = menuWidth;
	//	tmpSize.y = menuHeight;
	//	tmpSize = { 32, 32 };

	//	// -- Title ---------
	//	/* level name */
	//	allUI.levelNameText->setText(getLevelName(level))
	//		.setSize(VOID_POS).setCalculatedSize();
	//	allUI.levelNameText->setPos({ (winSz.x / 2) - allUI.levelNameText->getSize().x / 2, tmpPos.y + 1.5 * tmpSize.y });

	//	tmpPos.x = (winSz.x / 2) - (menuWidth / 2);
	//	tmpPos.y = winSz.y - menuHeight * 2;

	//	// -- Top -----------
	//	/* time left */
	//	if (player->activeBonus.time > 0) {
	//		allUI.timeLeftImgActive->setPos({ tmpPos.x, imgY }).setSize(tmpSize);
	//		allUI.timeLeftImg->setPos(VOID_POS).setSize(VOID_SIZE);
	//		tmpPos.x += allUI.timeLeftImgActive->getSize().x;
	//	}
	//	else {
	//		allUI.timeLeftImg->setPos({ tmpPos.x, imgY }).setSize(tmpSize);
	//		allUI.timeLeftImgActive->setPos(VOID_POS).setSize(VOID_SIZE);
	//		tmpPos.x += allUI.timeLeftImg->getSize().x;
	//	}
	//	allUI.timeLeftText->setPos({ tmpPos.x, textY }).setText(timeToString(levelTime - time))
	//		.setSize(VOID_POS).setCalculatedSize();
	//	if (!_alarm)
	//		allUI.timeLeftText->setTextColor(colorise(s.j("colors").j("font").u("color")));
	//	else
	//		allUI.timeLeftText->setTextColor(colorise(s.j("colors").j("orange").u("color")));
	//	tmpPos.x += allUI.timeLeftText->getSize().x;

	//	/* life */
	//	tmpPos.x += padding;
	//	if (player->activeBonus.life > 0) {
	//		allUI.lifeImgActive->setPos({ tmpPos.x, imgY }).setSize(tmpSize);
	//		allUI.lifeImg->setPos(VOID_POS).setSize(VOID_SIZE);
	//		tmpPos.x += allUI.lifeImgActive->getSize().x;
	//	}
	//	else {
	//		allUI.lifeImg->setPos({ tmpPos.x, imgY }).setSize(tmpSize);
	//		allUI.lifeImgActive->setPos(VOID_POS).setSize(VOID_SIZE);
	//		tmpPos.x += allUI.lifeImg->getSize().x;
	//	}
	//	allUI.lifeText->setPos({ tmpPos.x, textY }).setText(std::to_string(player->lives))
	//		.setSize(VOID_SIZE).setCalculatedSize();
	//	tmpPos.x += allUI.lifeText->getSize().x;

	//	/* enemies counter */
	//	if (enemiesToKill > 0) {
	//		tmpPos.x += padding;
	//		std::string enemiesStr = std::to_string(enemiesKilled) + "/" + std::to_string(enemiesToKill);
	//		glm::vec4 color = enemiesKilled >= enemiesToKill
	//			? colorise(0x39A0DD)
	//			: colorise(0xBF001E);
	//		allUI.enemiesCounterText->setPos({ tmpPos.x, textY }).setText(enemiesStr)
	//			.setSize(VOID_POS).setCalculatedSize().setTextColor(color);
	//		tmpPos.x += allUI.enemiesCounterText->getSize().x;
	//	}

	//	/* score */
	//	tmpPos.x = (winSz.x / 2) + (menuWidth / 2);
	//	allUI.scoreText->setText(score.toString()).setSize(VOID_POS).setCalculatedSize();
	//	tmpPos.x -= allUI.scoreText->getSize().x;
	//	allUI.scoreText->setPos({ tmpPos.x, textY });
	//	if (player->activeBonus.score > 0) {
	//		allUI.scoreImgActive->setSize(tmpSize);
	//		tmpPos.x -= allUI.scoreImgActive->getSize().x;
	//		allUI.scoreImgActive->setPos({ tmpPos.x, imgY });
	//		allUI.scoreImg->setPos(VOID_POS).setSize(VOID_SIZE);
	//	}
	//	else {
	//		allUI.scoreImg->setSize(tmpSize);
	//		tmpPos.x -= allUI.scoreImg->getSize().x;
	//		allUI.scoreImg->setPos({ tmpPos.x, imgY });
	//		allUI.scoreImgActive->setPos(VOID_POS).setSize(VOID_SIZE);
	//	}

	//	// -- Bonus -----------
	//	glm::vec2 pos = { winSz.x / 10, winSz.y - menuHeight * 4 };
	//	tmpPos = pos;

	//	/* speed */
	//	// tmpPos.x += padding;
	//	if (player->activeBonus.speed > 0) {
	//		allUI.speedImgActive->setPos(tmpPos).setSize(tmpSize);
	//		allUI.speedImg->setPos(VOID_POS).setSize(VOID_SIZE);
	//		tmpPos.x += allUI.speedImgActive->getSize().x;
	//	}
	//	else {
	//		allUI.speedImg->setPos(tmpPos).setSize(tmpSize);
	//		allUI.speedImgActive->setPos(VOID_POS).setSize(VOID_SIZE);
	//		tmpPos.x += allUI.speedImg->getSize().x;
	//	}
	//	std::string	speed = std::to_string(player->speed);
	//	speed = speed.substr(0, speed.find("."));
	//	allUI.speedText->setPos({ tmpPos.x, tmpPos.y + 2 }).setText(speed)
	//		.setSize(VOID_SIZE).setCalculatedSize();
	//	pos.y -= menuHeight * 0.8;
	//	tmpPos = pos;

	//	/* bonus bomb */
	//	if (player->activeBonus.bombs > 0) {
	//		allUI.bonusBombImgActive->setPos(tmpPos).setSize(tmpSize);
	//		allUI.bonusBombImg->setPos(VOID_POS).setSize(VOID_SIZE);
	//		tmpPos.x += allUI.bonusBombImgActive->getSize().x;
	//	}
	//	else {
	//		allUI.bonusBombImg->setPos(tmpPos).setSize(tmpSize);
	//		allUI.bonusBombImgActive->setPos(VOID_POS).setSize(VOID_SIZE);
	//		tmpPos.x += allUI.bonusBombImg->getSize().x;
	//	}
	//	allUI.bonusBombText->setPos({ tmpPos.x, tmpPos.y + 2 }).setText(std::to_string(player->totalBombs))
	//		.setSize(VOID_SIZE).setCalculatedSize();
	//	pos.y -= menuHeight * 0.8;
	//	tmpPos = pos;

	//	/* bonus flame */
	//	if (player->activeBonus.flames > 0) {
	//		allUI.bonusFlameImgActive->setPos(tmpPos).setSize(tmpSize);
	//		allUI.bonusFlameImg->setPos(VOID_POS).setSize(VOID_SIZE);
	//		tmpPos.x += allUI.bonusFlameImgActive->getSize().x;
	//	}
	//	else {
	//		allUI.bonusFlameImg->setPos(tmpPos).setSize(tmpSize);
	//		allUI.bonusFlameImgActive->setPos(VOID_POS).setSize(VOID_SIZE);
	//		tmpPos.x += allUI.bonusFlameImg->getSize().x;
	//	}
	//	allUI.bonusFlameText->setPos({ tmpPos.x, tmpPos.y + 2 }).setText(std::to_string(player->bombProgation))
	//		.setSize(VOID_SIZE).setCalculatedSize();
	//	pos.y -= menuHeight * 0.8;
	//	tmpPos = pos;

	//	/* bonus flampass */
	//	if (player->passFire) {
	//		if (player->activeBonus.flampass > 0) {
	//			allUI.bonusFlampassImgActive->setPos(tmpPos).setSize(tmpSize);
	//			allUI.bonusFlampassImg->setPos(VOID_POS).setSize(VOID_SIZE);
	//		}
	//		else {
	//			allUI.bonusFlampassImg->setPos(tmpPos).setSize(tmpSize);
	//			allUI.bonusFlampassImgActive->setPos(VOID_POS).setSize(VOID_SIZE);
	//		}
	//		pos.y -= menuHeight * 0.8;
	//		tmpPos = pos;
	//	}
	//	else {
	//		allUI.bonusFlampassImg->setPos(VOID_POS).setSize(VOID_SIZE);
	//	}
	//	/* bonus wallpass */
	//	if (player->passWall) {
	//		if (player->activeBonus.wallpass > 0) {
	//			allUI.bonusWallpassImgActive->setPos(tmpPos).setSize(tmpSize);
	//			allUI.bonusWallpassImg->setPos(VOID_POS).setSize(VOID_SIZE);
	//		}
	//		else {
	//			allUI.bonusWallpassImg->setPos(tmpPos).setSize(tmpSize);
	//			allUI.bonusWallpassImgActive->setPos(VOID_POS).setSize(VOID_SIZE);
	//		}
	//		pos.y -= menuHeight * 0.8;
	//		tmpPos = pos;
	//	}
	//	else {
	//		allUI.bonusWallpassImg->setPos(VOID_POS).setSize(VOID_SIZE);
	//	}
	//	/* bonus detonator */
	//	if (player->detonator) {
	//		if (player->activeBonus.detonator > 0) {
	//			allUI.bonusDetonatorImgActive->setPos(tmpPos).setSize(tmpSize);
	//			allUI.bonusDetonatorImg->setPos(VOID_POS).setSize(VOID_SIZE);
	//		}
	//		else {
	//			allUI.bonusDetonatorImg->setPos(tmpPos).setSize(tmpSize);
	//			allUI.bonusDetonatorImgActive->setPos(VOID_POS).setSize(VOID_SIZE);
	//		}
	//		pos.y -= menuHeight * 0.8;
	//		tmpPos = pos;
	//	}
	//	else {
	//		allUI.bonusDetonatorImg->setPos(VOID_POS).setSize(VOID_SIZE);
	//	}
	//	/* bonus passBomb */
	//	if (player->passBomb) {
	//		if (player->activeBonus.bombpass > 0) {
	//			allUI.bonusBombpassImgActive->setPos(tmpPos).setSize(tmpSize);
	//			allUI.bonusBombpassImg->setPos(VOID_POS).setSize(VOID_SIZE);
	//		}
	//		else {
	//			allUI.bonusBombpassImg->setPos(tmpPos).setSize(tmpSize);
	//			allUI.bonusBombpassImgActive->setPos(VOID_POS).setSize(VOID_SIZE);
	//		}
	//		pos.y -= menuHeight * 0.8;
	//		tmpPos = pos;
	//	}
	//	else {
	//		allUI.bonusBombpassImg->setPos(VOID_POS).setSize(VOID_SIZE);
	//	}
	//	/* bonus invulnerable */
	//	if (player->invulnerable) {
	//		if (player->activeBonus.shield > 0) {
	//			allUI.bonusShieldImgActive->setPos(tmpPos).setSize(tmpSize);
	//			allUI.bonusShieldImg->setPos(VOID_POS).setSize(VOID_SIZE);
	//			tmpPos.x += allUI.bonusShieldImgActive->getSize().x;
	//		}
	//		else {
	//			allUI.bonusShieldImg->setPos(tmpPos).setSize(tmpSize);
	//			allUI.bonusShieldImgActive->setPos(VOID_POS).setSize(VOID_SIZE);
	//			tmpPos.x += allUI.bonusShieldImg->getSize().x;
	//		}
	//		std::string	invulnerable = std::to_string(player->invulnerable);
	//		invulnerable = invulnerable.substr(0, invulnerable.find(".") + 2);
	//		allUI.bonusShieldText->setPos({ tmpPos.x, tmpPos.y + 2 }).setText(timeToString(player->invulnerable))
	//			.setSize(VOID_SIZE).setCalculatedSize();
	//	}
	//	else {
	//		allUI.bonusShieldImg->setPos(VOID_POS).setSize(VOID_SIZE);
	//		allUI.bonusShieldText->setPos(VOID_POS).setSize(VOID_SIZE);
	//	}
	//}
	//catch (ABaseUI::UIException const& e) {
	//	logErr(e.what());
	//}
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