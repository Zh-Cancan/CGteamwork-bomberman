#pragma once


#include <cstdlib>
#include<chrono>
#include "utils/SettingsJson.hpp"
#include "utils/Logging.h"
#include "utils/useGlm.h"

/*---------------------------------路径定义-------------------------------*/

#define CONFIG_DIR				"configs/"
#define SETTINGS_FILE			CONFIG_DIR"settings.json"
#define CONTROLS_FILE			CONFIG_DIR"controls.json"
#define USERDATA_FILE			CONFIG_DIR"userData.json"
#define CHEATCODE_HIST_FILE		CONFIG_DIR"history.cheatcode"
#define SAVE_DIR				"saves/"
#define MAP_DIR				    "assets/map/"

//暂未使用
#define PLAYER_SIZE			glm::vec3(1.2, 1.2, 1.2)
#define ENEMY_BASIC_SIZE	glm::vec3(0.7, 0.7, 0.7)
#define ENEMY_WITH_EYE_SIZE	glm::vec3(1.5, 1.5, 1.5)
#define ENEMY_FLY_SIZE		glm::vec3(0.7, 0.7, 0.7)
#define ENEMY_FROG_SIZE		glm::vec3(1.0, 1.0, 1.0)
#define ENEMY_CRISPY_SIZE	glm::vec3(1.0, 1.0, 1.0)
#define ENEMY_FOLLOW_SIZE	glm::vec3(1.0, 1.0, 1.0)

#define SCR_WIDTH 800
#define SCR_HEIGHT 600

static const std::string	homeDir = "";
extern SettingsJson s;//通用设置

/*---------------------------------函数-------------------------------*/
int						    checkPrgm();
bool						initSettings(std::string const & filename);
bool						saveSettings(std::string const & filename);

glm::vec4					colorise(uint32_t color, uint8_t alpha = 0xff);
std::chrono::milliseconds	getMs();
std::string					timeToString(float time);

int                         bomberman();
int                         start();


