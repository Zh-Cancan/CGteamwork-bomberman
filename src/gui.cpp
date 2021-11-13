#include "../include/Gui.h"
#include "../include/utils/Logging.h"
#include "../include/utils/opengl/Inputs.h"
#include "../include/scenes/SceneManager.h"

/* ------------------------------ GameInfo struct ---------------------------*/
GameInfo::GameInfo() {
	title = "bomberman";
	windowSize = {
		s.j("graphics").i("width"),
		s.j("graphics").i("height")
	};
	quit = false;
}

/* ----------------------------------- Gui ----------------------------------*/
Gui::Gui(GameInfo &gameInfo)
: gameInfo(gameInfo)
{}

Gui::~Gui() {

	// free vao / vbo
	
}

Gui::Gui(Gui const &src)
: gameInfo(src.gameInfo) {
	*this = src;
}

Gui &Gui::operator=(Gui const &rhs) {
	return *this;
}

/**
 * @brief 初始化GUI
 *
 * @return 是否初始化成功
 */
bool	Gui::init() {
	std::cout<<"create gui\n";

	//加载

	// 初始化openGL设置
	

	// 初始化Shader，UI等
	

	return true;
}

// -- enableCursor -------------------------------------------------------------
/**
 * @brief enable or disable the cursor visibility
 *
 * @param enable enable boolean
 */
void Gui::enableCursor(bool enable) {
	if (enable) {
		SDL_ShowCursor(SDL_ENABLE);
		SDL_SetRelativeMouseMode(SDL_FALSE);
	}
	else {
		SDL_ShowCursor(SDL_DISABLE);
		SDL_SetRelativeMouseMode(SDL_TRUE);
	}
}

// -- draw ---------------------------------------------------------------------
/**
 * @brief call this function to do stuff before drawing scene
 */
void Gui::preDraw() {
	// clear buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, gameInfo.windowSize.x, gameInfo.windowSize.y);
	glClearColor(0.2, 0.5, 0.95, 1.0);
}

/**
 * @brief call this function to do stuff after drawing scene
 */
void Gui::postDraw() {
	// swap buffer and check errors
	SDL_GL_SwapWindow(_win);
}