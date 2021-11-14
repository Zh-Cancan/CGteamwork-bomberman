#include "../../include/scenes/SceneLevelSelection.h"
#include "../../include/scenes/SceneGame.h"
#include "../../include/scenes/SceneManager.h"


SceneLevelSelection::SceneLevelSelection(Gui * gui, float const &dtTime)
: SceneMenu(gui, dtTime),
  _currentLvl(-1),
  _transition(0)
{}


SceneLevelSelection::SceneLevelSelection(SceneLevelSelection const & src)
: SceneMenu(src)
{
	*this = src;
}


SceneLevelSelection::~SceneLevelSelection() {}


SceneLevelSelection & SceneLevelSelection::operator=(SceneLevelSelection const & rhs) {
	return *this;
}


bool			SceneLevelSelection::init() {
	
	_transition = 0;  // reset transition

	//init UI element

	setLevel(0);
	return true;
}

/**
 * @brief ���º��� ÿһ֡(frame)����һ��
 *
 * @return �Ƿ���³ɹ�
 */
bool	SceneLevelSelection::update() {
	SceneMenu::update();
	
	//UI element
	//������ͨ���ؼ���õ�ǰ�ؿ���_currentLvl

	SceneGame& scGame = *reinterpret_cast<SceneGame*>(SceneManager::getScene(SceneNames::GAME));
	//ѡ����ĳһ�ؿ�
	if (true) {
	/*if (_states.loadLevel) {*/
		_states.loadLevel = false;
		_transition = 0;
		if (_transition == 0) {  // load only if the transition is over
			try {
				_currentLvl = 1;
				if (!scGame.loadLevel(_currentLvl)) {
					return false;
				}
			} catch (std::exception const &e) {
				std::cout << "Level " << _currentLvl << ": " << e.what()<<"\n";
				SceneManager::loadScene(SceneNames::LEVEL_SELECTION);
				return true;
			}
			//�ɹ�ѡ���˹ؿ�
			SceneManager::loadScene(SceneNames::GAME);
			return true;
		}
	}
	if (_states.menu) {
		_states.menu = false;
		SceneManager::loadScene(SceneNames::MAIN_MENU);
	}
	else if (_states.lastLevel) {
		_states.lastLevel = false;
		setLevel(_currentLvl - 1);
	}
	else if (_states.nextLevel) {
		_states.nextLevel = false;
		setLevel(_currentLvl + 1);
	}
	/* disable arrow buttons if first or last level */
	/*allUI.leftArrow->setEnabled(_currentLvl != 0);
	allUI.rightArrow->setEnabled(_currentLvl < static_cast<int32_t>(_states.nbLevel - 1));*/
	return true;
}

/**
 * @brief called when the scene is loaded
 */
void SceneLevelSelection::load() {
	SceneMenu::load();
	_transition = 1;
	setLevel(1);//�ӵ�һ�ؿ�ʼ
}



/**
 * @brief  ���õ�ǰ�ؿ�
 *
 * @param level �ؿ���
 * @param enableTransition ���Ϊ�棬�ڹؿ�֮��ƽ��ת��
 */
void			SceneLevelSelection::setLevel(int32_t level, bool enableTransition) {
	// set right level ID
	if (level < 0) level = 0;
	if (level >= static_cast<int32_t>(_states.nbLevel)) level = _states.nbLevel - 1;
	if (level == _currentLvl)
		return;

	// set transition
	if (enableTransition) {
		if (level == _currentLvl - 1) _transition = -1;
		else if (level == _currentLvl + 1) _transition = 1;
		else _transition = 0;
	}

	if (_currentLvl >= 0 && _currentLvl < static_cast<int32_t>(_states.nbLevel)) {
		/*getUIElement(_states.firstLevelID + _currentLvl)
			.setKeyLeftClickInput(InputType::NO_KEY)
			.addButtonLeftListener(nullptr);*/
	}

	_currentLvl = level;
	/*if (_currentLvl == 0 || Save::isLevelDone(_currentLvl) || Save::isLevelDone(_currentLvl - 1)
	|| SceneCheatCode::isLevelUnlocked(_currentLvl)) {
		getUIElement(_states.firstLevelID + _currentLvl)
			.setKeyLeftClickInput(InputType::CONFIRM)
			.addButtonLeftListener(&_states.loadLevel)
			.setColor(colorise(0, 0))
			.setMouseHoverColor(colorise(0x000000, 51))
			.setMouseClickColor(UI_DEF_MOUSE_CLICK_COLOR);
	} else {
		getUIElement(_states.firstLevelID + _currentLvl)
			.setColor(colorise(0x000000, 200))
			.setMouseHoverColor(colorise(0x000000, 200))
			.setMouseClickColor(colorise(0x000000, 200));
	}*/
}

/**
 * @brief ��õ�ǰ�ؿ���
 *
 * @return ��ǰ�ؿ���
 */
uint32_t SceneLevelSelection::getCurLevel() const { return _currentLvl; }
/**
 * @brief ������йؿ���
 *
 * @return ���йؿ���
 */
uint32_t SceneLevelSelection::getNbLevel() const { return _states.nbLevel; }
