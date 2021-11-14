#pragma once

//һЩ��������һ��ʹ��
#define C_VAO_WIDTH 4
#define C_NB_FACES 6
// C_VAO_WIDTH * C_NB_FACES
#define C_FACE_A_SIZE 24
#define CAM_POS_OFFSET glm::vec3(0.0f, 20.0f, -10.0f)
#define CAM_TARG_OFFSET glm::vec3(0.0f, 1.0f, 0.0f)
#define CAM_SPEED 1.5f
#define VOID_POS glm::ivec2 {-1, -1}
#define VOID_POS3 glm::vec3 {-1, -1, -1}

//SDL��Simple DirectMedia Layer������ֱ��ý��㣩����д������һ����ƽ̨�Ķ�ý���,��Ҫ��װhttps://www.libsdl.org/download-2.0.php
#include "../include/SDL2/SDL.h"
#include <iostream>
#include <array>

#include "bomberman.h"
#include "usingOpengl.h"
//����ͷ�ļ� ����պУ�shader��


/**
 * @brief GUI���ʼ�����ڣ���ͼ
 */
class Gui {
	public:
		explicit	Gui();
		virtual ~Gui();
		Gui(Gui const &src);
		Gui &operator=(Gui const &rhs);

		bool	init();
		void	preDraw();
		void	postDraw();
		void	enableCursor(bool enable);
		void	disableExitForThisFrame(bool disable = true);
	    //������ͼ����

		bool       quit;//�ոĵģ�����GameInfo�е�quit
	//	TextureManager	*textureManager;  /**< TextureManager object */
	//	Shader			*cubeShader;  /**< CubeShader object */
	//	Camera			*cam;  /**< Camera object */

		uint32_t		cubeShVao;  /**< Vertex Array Objects */
		uint32_t		cubeShVbo;  /**< Vertex Buffer Objects */

	private:
		SDL_Window		*_win;  /**< SDL window object */
		SDL_Event		*_event;  /**< SDL event object */
		SDL_GLContext	_context;  /**< SDL gl context object */

	//	Skybox			*_skybox;  /**< Project skybox */

		bool			_exitMenuDisabled;  /**< True if exit menu is disabled */

	//	static std::array<float, C_FACE_A_SIZE> const		_cubeFaces;  /**< All cubes faces */

	//	bool	_init();
	//	bool	_initOpengl();
	//	bool	_initShaders();

	//	static const int									_min_width = 800;  /**< Min screen width */
	//	static const int									_min_height = 600;  /**< Min screen height */
	//	bool	_protect_resolution();
};
