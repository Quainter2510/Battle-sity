#ifndef CONFIG_H
#define CONFIG_H
#include <map>

enum class direction {
    UP = 0,
    RIGHT,
    DOWN,
    LEFT
};

enum class types {
    HERO = 0,
    ENEMY,
    BLOCK,
    BONUS,
    BUSHES
};

enum class bonusTypes {
    LEVELBONUS = 0,
    TIMEBONUS,
    BASEBONUS
};

inline int WIDTH = 1050;
inline int HEIGHT = 800;
inline int IMAGE_SIZE = 50;
inline int PERCENTAGE_DESTR_BLOCK = 20;
inline int PERCENTAGE_INDESRT_BLOCK = 12;
inline int PERCENTAGE_WATER_BLOCK = 5;
inline int PERCENTAGE_BUSHES_BLOCK = 5;
inline int FPS = 30;

inline std::map<std::string, std::string> blockImages = {
    {"destr", ":/bigimages/bigImages/block_brick.png"},
    {"indestr", ":/bigimages\\bigImages\\block_armor.png"},
    {"water", ":/bigimages\\bigImages\\block_water.png"},
    {"bushes", ":/bigimages\\bigImages\\block_bushes.png"},
    {"base", ":/bigimages\\bigImages\\base.png"},
};

inline std::map<std::string, std::string> bonusImages = {
    {"level", ":/bigimages\\bigImages\\bonus_star.png"},
    {"base", ":/bigimages\\bigImages\\bonus_shovel.png"},
    {"time", ":/bigimages\\bigImages\\bonus_time.png"}
};

#endif // CONFIG_H
