#include "lib.h"
#include "natives.h"
#include "enums.h"

#ifndef NULL
#define NULL 0
#endif

bool init = false;
int frameCount = 0;
bool selfoption[50];
int intTest[50];
float floatTest[50];

int menuColours[25];

typedef union RGBA
{
	struct
	{
		union{int red; int r;};
		union{int green; int g;};
		union{int blue; int b;};
		union{int opacity; int alpha; int a;};
	};
	int col[4];
} RGBA;
typedef int rgba;


float menuXCoord = 0.845f;

int submenu = 0;
int submenuLevel;
int lastSubmenu[20];
int lastOption[20];
int currentOption;
int optionCount;
bool optionPress = false;
bool rightPress = false;
bool leftPress = false;
bool fastRightPress = false;
bool fastLeftPress = false;
bool squarePress = false;
int selectedPlayer;
bool menuSounds = true;
bool keyboardActive = false;
int keyboardAction;
int *keyboardVar = 0;
char *infoText;

bool newTimerTick = true;
int maxTimerCount;
bool newTimerTick2 = true;
int maxTimerCount2;

bool instructions;
int instructCount;
int mov;
bool instructionsSetupThisFrame;
bool xInstruction;
bool squareInstruction;
bool lrInstruction;

int bannerTextRed = 255;
int bannerTextGreen = 255;
int bannerTextBlue = 255;
int bannerTextOpacity = 255;
int bannerTextFont = 1;
int bannerRectRed = 255;
int bannerRectGreen = 195;
int bannerRectBlue = 0;
int bannerRectOpacity = 150;
int backgroundRed = 16;
int backgroundGreen = 16;
int backgroundBlue = 16;
int backgroundOpacity = 150;
int optionsRed = 255;
int optionsGreen = 255;
int optionsBlue = 255;
int optionsOpacity = 255;
int optionsFont = 0;
int scrollerRed = 255;
int scrollerGreen = 195;
int scrollerBlue = 0;
int scrollerOpacity = 160;
int indicatorRed = 255;
int indicatorGreen = 255;
int indicatorBlue = 255;
int indicatorOpacity = 255;
float textXCoord = 0.74f;
int maxOptions = 20;
int newSubmenu = 0;
int hours = 12;
int Std = TIME::GET_CLOCK_HOURS();
int Minute = TIME::GET_CLOCK_MINUTES();
int Sekunde = TIME::GET_CLOCK_SECONDS();
int fam;

bool cstrcmp(const char* s1, const char* s2) {
	while (*s1 && (*s1 == *s2))s1++, s2++;
	if (*(const unsigned char*)s1 - *(const unsigned char*)s2 == 0)return true;
	else return false;
}
namespace Sprites
{
	enum BadgeStyle
	{
		BGNone,
		BGBronzeMedal,
		BGGoldMedal,
		BGSilverMedal,
		BGAlert,
		BGCrown,
		BGAmmo,
		BGArmour,
		BGBarber,
		BGClothes,
		BGFranklin,
		BGBike,
		BGCar,
		BGGun,
		BGHeart,
		BGMakeup,
		BGMask,
		BGMichael,
		BGStar,
		BGTatoo,
		BGTrevor,
		BGLock,
		BGTick,
	};
	static void DRAW_SPRITE(char* Streamedtexture, char * textureName, float X, float Y, float width, float height, float rotation, int r, int g, int b, int a)
	{
		GRAPHICS::REQUEST_STREAMED_TEXTURE_DICT(Streamedtexture, 0);
		GRAPHICS::HAS_STREAMED_TEXTURE_DICT_LOADED(Streamedtexture);
		GRAPHICS::DRAW_SPRITE(Streamedtexture, textureName, X, Y, width, height, rotation, r, g, b, a);
	}
};

void set_text_component(char *text)
{
	GRAPHICS::BEGIN_TEXT_COMMAND_SCALEFORM_STRING("STRING");
	UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(text);
	GRAPHICS::END_TEXT_COMMAND_SCALEFORM_STRING();
}

void instructionsSetup()
{
	mov = GRAPHICS::REQUEST_SCALEFORM_MOVIE("instructional_buttons");
	GRAPHICS::DRAW_SCALEFORM_MOVIE_FULLSCREEN(mov, 255, 255, 255, 0, 0);
	GRAPHICS::BEGIN_SCALEFORM_MOVIE_METHOD(mov, "CLEAR_ALL");
	GRAPHICS::END_SCALEFORM_MOVIE_METHOD();
	GRAPHICS::BEGIN_SCALEFORM_MOVIE_METHOD(mov, "SET_CLEAR_SPACE");
	GRAPHICS::_ADD_SCALEFORM_MOVIE_METHOD_PARAMETER_INT(200);
	GRAPHICS::END_SCALEFORM_MOVIE_METHOD();
	instructCount = 0;
}
void addInstruction(int button, char *text)
{
	if (!instructionsSetupThisFrame)
	{
		instructionsSetup();
		instructionsSetupThisFrame = true;
	}
	GRAPHICS::BEGIN_SCALEFORM_MOVIE_METHOD(mov, "SET_DATA_SLOT");
	GRAPHICS::_ADD_SCALEFORM_MOVIE_METHOD_PARAMETER_INT(instructCount);
	GRAPHICS::_ADD_SCALEFORM_MOVIE_METHOD_PARAMETER_INT(button);
	set_text_component(text);
	GRAPHICS::END_SCALEFORM_MOVIE_METHOD();
	instructCount++;
}
void instructionsClose()
{
	GRAPHICS::BEGIN_SCALEFORM_MOVIE_METHOD(mov, "DRAW_INSTRUCTIONAL_BUTTONS");
	GRAPHICS::END_SCALEFORM_MOVIE_METHOD();
	GRAPHICS::BEGIN_SCALEFORM_MOVIE_METHOD(mov, "SET_BACKGROUND_COLOUR");
	GRAPHICS::_ADD_SCALEFORM_MOVIE_METHOD_PARAMETER_INT(0);
	GRAPHICS::_ADD_SCALEFORM_MOVIE_METHOD_PARAMETER_INT(0);
	GRAPHICS::_ADD_SCALEFORM_MOVIE_METHOD_PARAMETER_INT(0);
	GRAPHICS::_ADD_SCALEFORM_MOVIE_METHOD_PARAMETER_INT(80);
	GRAPHICS::END_SCALEFORM_MOVIE_METHOD();
}

void DRAW_TEXTURE(char *Streamedtexture, char *textureName, float x, float y, float width, float height, float rotation, int r, int g, int b, int a)
{
	if (!GRAPHICS::HAS_STREAMED_TEXTURE_DICT_LOADED((char*)Streamedtexture))
	{
		GRAPHICS::REQUEST_STREAMED_TEXTURE_DICT((char*)Streamedtexture, false);
	}
	else
	{
		GRAPHICS::DRAW_SPRITE((char*)Streamedtexture, (char*)textureName, x, y, width, height, rotation, r, g, b, a);
	}
}
void drawText(char * text, int font, float x, float y, float scalex, float scaley, int r, int b, int g, int a, bool center)
{
		UI::SET_TEXT_FONT(font);
		UI::SET_TEXT_SCALE(scalex, scaley);
		UI::SET_TEXT_COLOUR(r, g, b, a);
		UI::SET_TEXT_WRAP(0.0f, 1.0f);
		UI::SET_TEXT_CENTRE(center);
		UI::BEGIN_TEXT_COMMAND_DISPLAY_TEXT("STRING");
		UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(text);
		UI::END_TEXT_COMMAND_DISPLAY_TEXT(x, y);
}

void drawText1(char * text, int font, float x, float y, float scalex, float scaley, int r, int b, int g, int a, bool center)
{
		UI::SET_TEXT_FONT(font);
		UI::SET_TEXT_SCALE(scalex, scaley);
		UI::SET_TEXT_COLOUR(r, g, b, a);
		UI::SET_TEXT_WRAP(0.0f, 1.0f);
		//UI::SET_TEXT_CENTRE(center);
		UI::BEGIN_TEXT_COMMAND_DISPLAY_TEXT("STRING");
		UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(text);
		UI::END_TEXT_COMMAND_DISPLAY_TEXT(x, y);
}
void drawNotification(char* msg)
{
	if (menuXCoord == 0.845f)
	{
		UI::BEGIN_TEXT_COMMAND_DISPLAY_TEXT("STRING");
		UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(msg);
		UI::_SET_NOTIFICATION_MESSAGE_2("CHAR_SOCIAL_CLUB", "CHAR_SOCIAL_CLUB", true, 4, "Menu Base", "");
		UI::_DRAW_NOTIFICATION(false, true);
	}
	else
	{
		UI::BEGIN_TEXT_COMMAND_DISPLAY_TEXT("STRING");
		UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(msg);
		UI::END_TEXT_COMMAND_PRINT(3000, 1);
	}
}

void playSound(char* sound)
{
	if (menuSounds)
		AUDIO::PLAY_SOUND_FRONTEND(-1, sound, "HUD_FRONTEND_DEFAULT_SOUNDSET", false);
}
bool isNumericChar(char x)
{
	return (x >= '0' && x <= '9') ? true : false;
}
int StoI(char *str)
{
	if (*str == NULL)
		return 0;

	int res = 0,
		sign = 1,
		i = 0;

	if (str[0] == '-')
	{
		sign = -1;
		i++;
	}
	for (; str[i] != '\0'; ++i)
	{
		if (isNumericChar(str[i]) == false)
			return 0;
		res = res * 10 + str[i] - '0';
	}
	return sign*res;
}

char *ItoS(int num)
{
	char buf[30];
	snprintf(buf, sizeof(buf), "%i", num);
	return buf;
}
char* FtoS(float input)
{
	char returnvalue[64];
	int wholenumber = (int)input;
	input -= wholenumber;
	input *= 100;
	sprintf(returnvalue, "%d.%d", wholenumber, (int)input);
	return returnvalue;
}

void startKeyboard(int action, char *defaultText, int maxLength)
{
	GAMEPLAY::DISPLAY_ONSCREEN_KEYBOARD(0, "FMMC_KEY_TIP8", "", defaultText, "", "", "", maxLength);
	keyboardAction = action;
	keyboardActive = true;
}
void changeSubmenu(int newSubmenu)
{
	lastSubmenu[submenuLevel] = submenu;
	lastOption[submenuLevel] = currentOption;
	currentOption = 1;
	submenu = newSubmenu;
	submenuLevel++;
}

void SetupTimer(int tickTime)
{
	if (newTimerTick) {
		maxTimerCount = GAMEPLAY::GET_GAME_TIMER() + tickTime;
		newTimerTick = false;
	}
}
bool IsTimerReady()
{
	return GAMEPLAY::GET_GAME_TIMER() > maxTimerCount;
}
void ResetTimer()
{
	newTimerTick = true;
}
void SetupTimer2(int tickTime)
{
	if (newTimerTick2) {
		maxTimerCount2 = GAMEPLAY::GET_GAME_TIMER() + tickTime;
		newTimerTick2 = false;
	}
}
bool IsTimerReady2()
{
	return GAMEPLAY::GET_GAME_TIMER() > maxTimerCount2;
}
void ResetTimer2()
{
	newTimerTick2 = true;
}

float GlobeX = 1.16424269f;
float GlobeY = 0.4180556f;
float GlobeH = 0.93f;
float GlobeG = 0.825f;
void drawGlare(float pX, float pY, float sX = 1, float sY = 1, int r = 255, int g = 255, int b = 255) {
	int gGlareHandle = GRAPHICS::REQUEST_SCALEFORM_MOVIE("MP_MENU_GLARE");
	GRAPHICS::DRAW_SCALEFORM_MOVIE(gGlareHandle, pX, pY, sX, sY, 255, 255, 255, 255, 0);
}
void addTitle(char *title)
{
	drawText1(title, 1, textXCoord, 0.063f, 1.0f, 1.0f, 0, 0, 0,255, true);
	GRAPHICS::DRAW_RECT(menuXCoord, 0.0850f, 0.22f, 0.080f, bannerRectRed, bannerRectGreen, bannerRectBlue, bannerRectOpacity);
	drawGlare(GlobeX, GlobeY, GlobeH, GlobeG, 255, 0, 93);
}

float rectVar1 = 0.775;
float rectVar2 = 0.1005f;
float rectVar3 = 0.075f;
float rectVar4 = 0.040f;
float rectVar5 = 0.0;
void addTitle2(char *title)
{
	GRAPHICS::DRAW_RECT(menuXCoord, 0.0850f, 0.22f, 0.0775f, bannerRectRed, bannerRectGreen, bannerRectBlue, bannerRectOpacity);
	drawText1(title, 4, textXCoord, 0.063f, 1.0f, 1.0f, 0, 0, 0, 255, true);
	drawGlare(GlobeX, GlobeY, GlobeH, GlobeG, 255, 0, 93);
}
float menuXCoord1 = 0.754;
float menuXCoord3 = 0.915f;
char opt[100];
void addSubTitle2(char *texting, char *Max_Case)
{
	char buffer[30];
	//sprintf(buffer, "%i/%i", currentOption, optionCount);
	char Maxcases[100];
	snprintf(Maxcases, sizeof(Maxcases), " ~w~%i ~y~/ ~w~%s", currentOption, Max_Case);
	drawText(Maxcases, 4, menuXCoord3, 0.127, 0.45, 0.45, bannerTextRed, bannerTextGreen, bannerTextBlue, bannerTextOpacity, false);
	drawText1(texting, 4, textXCoord, 0.127, 0.40, 0.40f, bannerRectRed, bannerRectBlue, bannerRectGreen, bannerTextOpacity, false);//0.845f
	GRAPHICS::DRAW_RECT(menuXCoord, 0.1415, 0.22, 0.035f, 0, 0, 0, 190);
}

bool x;
bool y;
bool width;
bool height;
bool rotation;
void DrawSprite(char  *Streamedtexture, char  *textureName, float x, float y, float width, float height, float rotation, int r, int g, int b, int a)
{
	if (!GRAPHICS::HAS_STREAMED_TEXTURE_DICT_LOADED(Streamedtexture))
		GRAPHICS::REQUEST_STREAMED_TEXTURE_DICT(Streamedtexture, false);
	else
		GRAPHICS::DRAW_SPRITE(Streamedtexture, textureName, x, y, width, height, rotation, r, g, b, a);
}
float
UpDown = .1135f, //1680
MenuWidth = .22f,
SpriteWidht = .3420f;
float checkboxshit = 0.875f;

int optionsRed2 = 0;
int optionsGreen2 = 0;
int optionsBlue2 = 0;
void addOption(char *option, char *info = "")
{
	char buf[50];
	snprintf(buf, sizeof(buf), "> %s", option);
	infoText = info;
	optionCount++;
	if (currentOption == optionCount)
	{
		if (currentOption <= maxOptions && optionCount <= maxOptions)
			drawText(buf, optionsFont, textXCoord, (optionCount * 0.035f + 0.125f), 0.45f, 0.45f, optionsRed, optionsBlue, optionsGreen, optionsOpacity, false);
		else if ((optionCount > (currentOption - maxOptions)) && optionCount <= currentOption)
			drawText(buf, optionsFont, textXCoord, ((optionCount - (currentOption - maxOptions)) * 0.035f + 0.125f), 0.45f, 0.45f, optionsRed, optionsBlue, optionsGreen, optionsOpacity, false);
	}
	else {
		if (currentOption <= maxOptions && optionCount <= maxOptions)
			drawText(option, optionsFont, textXCoord, (optionCount * 0.035f + 0.125f), 0.4f, 0.4f, optionsRed, optionsGreen, optionsBlue, optionsOpacity, false);
		else if ((optionCount > (currentOption - maxOptions)) && optionCount <= currentOption)
			drawText(option, optionsFont, textXCoord, ((optionCount - (currentOption - maxOptions)) * 0.035f + 0.125f), 0.4f, 0.4f, optionsRed, optionsGreen, optionsBlue, optionsOpacity, false);
	}
}

void addSubmenuOption(char *option, int newSubmenu, char* sprite = "", char *info = "")
{
	float xx = 0.945;
	float yy = (optionCount * 0.035f + 0.18f);
	float we = 0.02f;
	float he = 0.02f;
	addOption(option, info);
	drawText(">>", 4, textXCoord + .190, (optionCount * 0.035f + 0.123f), .57, .52, 255, 255, 255, 255, false);
	if (currentOption == optionCount)
	{
		xInstruction = true;
		if (optionPress)
			changeSubmenu(newSubmenu);
	}
}

void addBoolOption(char *option, bool *b00l, char *info = "")
{
	char on[30];
	char off[30];
 	addOption(option, info);
 	if(currentOption == optionCount && optionPress){
 		*b00l = !*b00l;
 	}
 	if (*b00l){
		int onr = 0;
		int ong = 255;
		int onb = 0;
		snprintf(on, sizeof(on), "O N", option);
		drawText(on, 4, textXCoord + .190, (optionCount * 0.035f + 0.123f), .57, .52, 0, 0, 255, 255, false);
 	}else{
		int offr = 255;
		int offg = 0;
		int offb = 0;
		snprintf(off, sizeof(off), "O F F", option);
		drawText(off, 4, textXCoord + .190, (optionCount * 0.035f + 0.123f), .57, .52, 255, 0, 0, 255, false);
 	}
}

void addIntOption(char *option, int *var, int min, int max, bool fastScroll, char *info = "")
{
	char buf[30];
	snprintf(buf, sizeof(buf), "%s: < %i >", option, *var);
	addOption(buf, info);
	if (currentOption == optionCount)
	{
		lrInstruction = true;
	
		if(fastScroll){
			if (fastRightPress)
		{
			playSound("NAV_UP_DOWN");
			if (*var >= max)
				*var = min;
			else
				*var = *var += 1;
		}
		else if (fastLeftPress)
		{
			playSound("NAV_UP_DOWN");
			if (*var <= min)
				*var = max;
			else
				*var = *var -= 1;
		}
		}else{
			if (rightPress)
		{
			playSound("NAV_UP_DOWN");
			if (*var >= max)
				*var = min;
			else
				*var = *var + 1;
		}
		else if (leftPress)
		{
			playSound("NAV_UP_DOWN");
			if (*var <= min)
				*var = max;
			else
				*var = *var - 1;
		}
		}
	}
}
int getOption()
{
	xInstruction = true;
	if (optionPress)
		return currentOption;
	else return 0;
}
void monitorButtons()
{
	if (submenu == Closed)
	{
		if (IsTimerReady())
		{
			if (CONTROLS::IS_DISABLED_CONTROL_PRESSED(0, Button_Square) && CONTROLS::IS_DISABLED_CONTROL_PRESSED(0, Button_Dpad_Right_2))
			{
				submenu = Main_Menu;
				submenuLevel = 0;
				currentOption = 1;
				playSound("YES");
			}
		}
	}
	else {
			if (CONTROLS::IS_DISABLED_CONTROL_JUST_PRESSED(0, Button_Circle))
			{
				if (submenu == Main_Menu)
				{
					submenu = Closed;
				}
				else
				{
					submenu = lastSubmenu[submenuLevel - 1];
					newSubmenu = lastSubmenu[submenuLevel - 1];
					currentOption = lastOption[submenuLevel - 1];
					submenuLevel--;
				}
				playSound("Back");
			}
			else if (CONTROLS::IS_DISABLED_CONTROL_JUST_PRESSED(0, Button_X))
			{
				optionPress = true;
				playSound("SELECT");
			}
			else if (CONTROLS::IS_DISABLED_CONTROL_JUST_PRESSED(0, Button_Dpad_Up))
			{
				currentOption--;
				if (currentOption < 1)
				{
					currentOption = optionCount;
				}
				playSound("NAV_UP_DOWN");
			}
			else if (CONTROLS::IS_DISABLED_CONTROL_JUST_PRESSED(0, Button_Dpad_Down))
			{
				currentOption++;
				if (currentOption > optionCount)
				{
					currentOption = 1;
				}
				playSound("NAV_UP_DOWN");
			}
			else if (CONTROLS::IS_DISABLED_CONTROL_JUST_PRESSED(0, Button_Dpad_Right_2))
			{
				rightPress = true;
			}
			else if (CONTROLS::IS_DISABLED_CONTROL_JUST_PRESSED(0, Button_Dpad_Left_2))
			{
				leftPress = true;
			}
			else if (CONTROLS::IS_DISABLED_CONTROL_PRESSED(0, Button_Dpad_Right_2))
			{
				fastRightPress = true;
			}
			else if (CONTROLS::IS_DISABLED_CONTROL_PRESSED(0, Button_Dpad_Left_2))
			{
				fastLeftPress = true;
			}
			else if (CONTROLS::IS_DISABLED_CONTROL_JUST_PRESSED(0, Button_X))
			{
				squarePress = true;
			}
	}
}
void otherLoop(){
	if (selfoption[1]){
			ENTITY::SET_ENTITY_VISIBLE(PLAYER::PLAYER_PED_ID(), 0,0);
    }else{
	       ENTITY:: SET_ENTITY_VISIBLE(PLAYER::PLAYER_PED_ID(), 1,1);
	}
}
void normalMenuActions()
{
	rgba rr = bannerRectRed;
	rgba rg = bannerRectGreen;
	rgba rb = bannerRectBlue;
	rgba ro = bannerRectOpacity;
	UI::HIDE_HELP_TEXT_THIS_FRAME();
	CAM::SET_CINEMATIC_BUTTON_ACTIVE(0);
    CONTROLS::DISABLE_CONTROL_ACTION(0, Button_Dpad_Up, true);
    CONTROLS::DISABLE_CONTROL_ACTION(0, Button_Dpad_Down, true);
    CONTROLS::DISABLE_CONTROL_ACTION(0, Button_Dpad_Right, true);
    CONTROLS::DISABLE_CONTROL_ACTION(0, Button_Dpad_Left, true);
    CONTROLS::DISABLE_CONTROL_ACTION(0, Button_Dpad_Right_2, true);
    CONTROLS::DISABLE_CONTROL_ACTION(0, Button_Dpad_Left_2, true);
    CONTROLS::DISABLE_CONTROL_ACTION(0, Button_Circle, true);
    CONTROLS::DISABLE_CONTROL_ACTION(0, Button_X, true);
    CONTROLS::SET_INPUT_EXCLUSIVE(0, Button_Cirlce_2);
    CONTROLS::SET_INPUT_EXCLUSIVE(0, Button_Dpad_Right);
    CONTROLS::SET_INPUT_EXCLUSIVE(0, Button_Dpad_Left);


	if (optionCount > maxOptions)
	{
		if (currentOption > maxOptions)
		{
			GRAPHICS::DRAW_RECT(menuXCoord, ((maxOptions * 0.035f) + 0.1415f), 0.22f, 0.035f, scrollerRed, scrollerGreen, scrollerBlue, scrollerOpacity);//scroller background
		}
		else
		{
			GRAPHICS::DRAW_RECT(menuXCoord, ((currentOption * 0.035f) + 0.1415f), 0.22f, 0.035f, scrollerRed, scrollerGreen, scrollerBlue, scrollerOpacity);//scroller background
		}
		if (currentOption != optionCount)
		{
			GRAPHICS::DRAW_RECT(menuXCoord, ((maxOptions * 0.035f) + 0.161f), 0.23f, 0.005f, indicatorRed, indicatorGreen, indicatorBlue, indicatorOpacity);//Down Indicator
		}
		GRAPHICS::DRAW_RECT(menuXCoord, (((maxOptions * 0.035f) / 2) + 0.159f), 0.22f, (maxOptions * 0.035f), backgroundRed, backgroundGreen, backgroundBlue, backgroundOpacity);//Background

	}
	else
	{
		GRAPHICS::DRAW_RECT(0.8455f, (((optionCount * 0.035f) / 2) + 0.159f), 0.22f, (optionCount * 0.035f), backgroundRed, backgroundGreen, backgroundBlue, backgroundOpacity);//background
		GRAPHICS::DRAW_RECT(menuXCoord, ((currentOption * 0.035f) + 0.1415f), 0.22f, 0.035f, scrollerRed, scrollerGreen, scrollerBlue, scrollerOpacity);//scroller background
		drawText(infoText, optionsFont, menuXCoord, ((optionCount + 1) * 0.035f + 0.125f), 0.4f, 0.4f, bannerTextRed, bannerTextGreen, bannerTextBlue, bannerTextOpacity, true);//bottom info
		GRAPHICS::DRAW_RECT(menuXCoord, (((optionCount + 1) * 0.035f) + 0.1415f), 0.22f, 0.035f,  rr, rg, rb, ro);//bottom rec
	}
	if (!GAMEPLAY::IS_STRING_NULL_OR_EMPTY(infoText))
	{
		if (optionCount > maxOptions)
		{
			drawText(infoText, optionsFont, menuXCoord, ((maxOptions + 1) * 0.035f + 0.125f), 0.4f, 0.4f, bannerTextRed, bannerTextGreen, bannerTextBlue, bannerTextOpacity, true);
			GRAPHICS::DRAW_RECT(menuXCoord, (((maxOptions + 1) * 0.035f) + 0.1415f), 0.22f, 0.035f,  rr, rg, rb, ro);
		}
		else
		{
			drawText(infoText, optionsFont, menuXCoord, ((optionCount + 1) * 0.035f + 0.125f), 0.4f, 0.4f, bannerTextRed, bannerTextGreen, bannerTextBlue, bannerTextOpacity, true);
			GRAPHICS::DRAW_RECT(menuXCoord, (((optionCount + 1) * 0.035f) + 0.1415f), 0.22f, 0.035f, rr, rg, rb, ro);
		}
	}
}
void resetVars()
{
	if (submenu != Closed)
		normalMenuActions();
	optionPress = false;
	rightPress = false;
	leftPress = false;
	fastRightPress = false;
	fastLeftPress = false;
	squarePress = false;
	infoText = NULL;
	instructionsSetupThisFrame = false;
	squareInstruction = false;
	xInstruction = false;
	lrInstruction = false;
}
int Hook()
{
	monitorButtons();
	optionCount = 0;
	switch (submenu)
	{
#pragma endregion

#pragma region Main Menu
	case Main_Menu:
		addTitle("MENU BASE V1");
		addSubTitle2("M A I N   M E N U", "3");
        addSubmenuOption("Self", Self_Options);
        addSubmenuOption("Examples", Example_Menu);
        addSubmenuOption("Settings", Settings_Menu);
   		break;
   case Self_Options:
		addTitle("MENU BASE V1");
		addSubTitle2("S E L F   O P T I O N S", "2");
		addBoolOption("God Mode", &selfoption[0]);
		addBoolOption("Invisiblity", &selfoption[1]);
		break;
   
      case Example_Menu:
		addTitle("MENU BASE V1");
		addSubTitle2("T E S T   O P T I O N S", "2");
		addIntOption("Int Option", &intTest[0], 1, 10, false);
		addBoolOption("Example Bool", &selfoption[0]);
		break;
      case Settings_Menu:
		addTitle("MENU BASE V1");
		addSubTitle2("S E T T I N G S", "3");
		addSubmenuOption("Scroller Colour", Scroller_Colour);
		addSubmenuOption("Banner Colour", Banner_Colour);
		addSubmenuOption("Background Colour", Background_Colour);
		break;
      case Scroller_Colour:
		addTitle("MENU BASE V1");
		addSubTitle2("S C R O L L E R   C O L O U R", "5");
		addIntOption("Red", &scrollerRed, 0, 255, true);
		addIntOption("Green", &scrollerGreen, 0, 255, true);
		addIntOption("Blue", &scrollerBlue, 0, 255, true);
		addIntOption("Alpha", &scrollerOpacity, 0, 255, true);
		addOption("Reset Scroller Colour");
		switch (getOption())
		{
			case 5:
				scrollerRed = 255;
				scrollerGreen = 195;
				scrollerBlue = 00;
				scrollerOpacity = 160;
			break;
		}
		break;
      case Banner_Colour:
		addTitle("MENU BASE V1");
		addSubTitle2("B A N N E R   C O L O U R", "5");
		addIntOption("Red", &bannerRectRed, 0, 255, true);
		addIntOption("Green", &bannerRectGreen, 0, 255, true);
		addIntOption("Blue", &bannerRectBlue, 0, 255, true);
		addIntOption("Alpha", &bannerRectOpacity, 0, 255, true);
		addOption("Reset Banner Colour");
		switch (getOption())
		{
			case 5:
				bannerRectRed = 255;
				bannerRectGreen = 195;
				bannerRectBlue = 0;
				bannerRectOpacity = 150;
			break;
		}
		break;
      case Background_Colour:
		addTitle("MENU BASE V1");
		addSubTitle2("B A C K G R O U N D   C O L O U R", "5");
		addIntOption("Red", &backgroundRed, 0, 255, true);
		addIntOption("Green", &backgroundGreen, 0, 255, true);
		addIntOption("Blue", &backgroundBlue, 0, 255, true);
		addIntOption("Alpha", &backgroundOpacity, 0, 255, true);
		addOption("Reset Background Colour");
		switch (getOption())
		{
			case 5:
				backgroundRed = 16;
				backgroundGreen = 16;
				backgroundBlue = 16;
				backgroundOpacity = 150;
			break;
		}
		break;
   }
   	resetVars();
	return true;
}


extern "C" void _main(void) {
	if (!init) {
		initLibs();
		init = true;
	}

	int newFrameCount = GAMEPLAY::GET_FRAME_COUNT();
	if (newFrameCount > frameCount) {
		frameCount = newFrameCount;

		Hook();
		otherLoop();		
	}
}