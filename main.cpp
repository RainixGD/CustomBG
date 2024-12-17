#include "./includes.h"

class CustomBGManager {

	enum DataLoadingResult {
		OK,
		FileNotFound,
		ParsingError
	};

	std::unordered_map<std::string, std::string> bgs;
	DataLoadingResult loadingStatus;
	static CustomBGManager* instance;
	bool noGroundInLSL = false;
	
	void init() {
		loadingStatus = loadData();
	}

	DataLoadingResult loadData() {
		std::ifstream file("Resources/customBG.json");
		if (!file) return FileNotFound;
		std::ostringstream buffer;
		buffer << file.rdbuf();
		std::string fileContent = buffer.str();

		file.close();
		try {
			auto root = nlohmann::json::parse(fileContent);
			if (!root.contains("settings") || !root["settings"].is_object()
				|| !root.contains("backgrounds") || !root["backgrounds"].is_object()) return ParsingError;

			if (root["settings"].contains("NoGroundInLevelSearchLayer") && root["settings"]["NoGroundInLevelSearchLayer"].is_boolean() && root["settings"]["NoGroundInLevelSearchLayer"].get<bool>())
				noGroundInLSL = true;
			for (const auto& [key, value] : root["backgrounds"].items()) {
				if (!value.is_string()) return ParsingError;

				auto bg = value.get<std::string>();
				if (bg.empty()) continue;

				bgs[key] = std::move(bg);
			}
		}
		catch (...) {
			return ParsingError;
		}
		return OK;
	}

	void createErrorLabel(CCLayer* layer) {
		std::string errorText;
		switch (loadingStatus) {
		case CustomBGManager::FileNotFound:
			errorText = "Can't find 'customBG.json' in ./Resources";
			break;
		case CustomBGManager::ParsingError:
			errorText = "Can't parse 'customBG.json'";
			break;
		}

		auto size = CCDirector::sharedDirector()->getWinSize();

		auto errorLabel = CCLabelBMFont::create(errorText.c_str(), "bigFont.fnt");
		errorLabel->setColor({ 255, 0, 0 });
		errorLabel->setScale(0.6);
		errorLabel->setPosition({ size.width / 2, size.height - 10 });
		layer->addChild(errorLabel);
	}

	CustomBGManager() {};
public:

	void createBGNode(CCLayer* layer, std::string layerName) {
		if (loadingStatus != OK) {
			createErrorLabel(layer);
			return;
		}

		if (bgs.find(layerName) == bgs.end()) return;

		layer->sortAllChildren();
		auto oldBG = dynamic_cast<CCNode*>(layer->getChildren()->objectAtIndex(0));
		oldBG->setVisible(0);

		if (noGroundInLSL && layerName == "LevelSelectLayer") {
			auto ground = dynamic_cast<CCNode*>(layer->getChildren()->objectAtIndex(1));
			ground->setVisible(0);
		}

		CCSprite* newBG = CCSprite::create(bgs[layerName].c_str());
		if (newBG != NULL) {
			auto size = CCDirector::sharedDirector()->getWinSize();
			newBG->setAnchorPoint({ 0, 0 });
			newBG->setScaleX(size.width / newBG->getContentSize().width);
			newBG->setScaleY(size.height / newBG->getContentSize().height);
			layer->addChild(newBG, -10);
		}
	}

	static CustomBGManager* getInstance() {
		if (!instance) {
			instance = new CustomBGManager();
			instance->init();
		}
		return instance;
	}

};
CustomBGManager* CustomBGManager::instance = nullptr;


bool(__thiscall* MenuLayer_init)(MenuLayer* self);
bool __fastcall MenuLayer_init_H(MenuLayer* self, void*) {
	if (!MenuLayer_init(self)) return false;
	CustomBGManager::getInstance()->createBGNode(self, "MenuLayer");
	return true;
}

bool(__thiscall* CreatorLayer_init)(CCLayer* self);
bool __fastcall CreatorLayer_init_H(CCLayer* self, void*) {
	if (!CreatorLayer_init(self)) return false;
	CustomBGManager::getInstance()->createBGNode(self, "CreatorLayer");
	return true;
}

bool(__thiscall* GarageLayer_init)(CCLayer* self);
bool __fastcall GarageLayer_init_H(CCLayer* self, void*) {
	if (!GarageLayer_init(self)) return false;
	CustomBGManager::getInstance()->createBGNode(self, "GarageLayer");
	return true;
}

bool(__thiscall* LevelSearchLayer_init)(CCLayer* self);
bool __fastcall LevelSearchLayer_init_H(CCLayer* self, void*) {
	if (!LevelSearchLayer_init(self)) return false;
	CustomBGManager::getInstance()->createBGNode(self, "LevelSearchLayer");
	return true;
}

bool(__thiscall* LevelBrowserLayer_init)(CCLayer* self, GJSearchObject* pSearch);
bool __fastcall LevelBrowserLayer_init_H(CCLayer* self, void*, GJSearchObject* pSearch) {
	if (!LevelBrowserLayer_init(self, pSearch)) return false;
	CustomBGManager::getInstance()->createBGNode(self, "LevelBrowserLayer");
	return true;
}

bool(__thiscall* LevelSelectLayer_init)(CCLayer* self, int lvl);
bool __fastcall LevelSelectLayer_init_H(CCLayer* self, void*, int lvl) {
	if (!LevelSelectLayer_init(self, lvl)) return false;
	CustomBGManager::getInstance()->createBGNode(self, "LevelSelectLayer");
	return true;
}

bool(__thiscall* LevelInfoLayer_init)(CCLayer* self, GJGameLevel* level);
bool __fastcall LevelInfoLayer_init_H(CCLayer* self, void*, GJGameLevel* level) {
	if (!LevelInfoLayer_init(self, level)) return false;
	CustomBGManager::getInstance()->createBGNode(self, "LevelInfoLayer");
	return true;
}

bool(__thiscall* LeaderLayer_init)(CCLayer* self, int idk);// idk = LeaderboardState
bool __fastcall LeaderLayer_init_H(CCLayer* self, void*, int idk) {
	if (!LeaderLayer_init(self, idk)) return false;
	CustomBGManager::getInstance()->createBGNode(self, "LeaderLayer");
	return true;
}

bool(__thiscall* GauntletLayer_init)(CCLayer* self, int idk);// int can be sus
bool __fastcall GauntletLayer_init_H(CCLayer* self, void*, int idk) {
	if (!GauntletLayer_init(self, idk)) return false;
	CustomBGManager::getInstance()->createBGNode(self, "GauntletLayer");
	return true;
}

bool(__thiscall* GauntletSelectLayer_init)(CCLayer* self, int idk);// int can be sus
bool __fastcall GauntletSelectLayer_init_H(CCLayer* self, void*, int idk) {
	if (!GauntletSelectLayer_init(self, idk)) return false;
	CustomBGManager::getInstance()->createBGNode(self, "GauntletSelectLayer");
	return true;
}

bool(__thiscall* EditLevelLayer_init)(CCLayer* self, GJGameLevel* level);
bool __fastcall EditLevelLayer_init_H(CCLayer* self, void*, GJGameLevel* level) {
	if (!EditLevelLayer_init(self, level)) return false;
	CustomBGManager::getInstance()->createBGNode(self, "EditLevelLayer");
	return true;
}

void inject() {
#if _WIN32
	auto base = reinterpret_cast<uintptr_t>(GetModuleHandle(0));

	MH_CreateHook(
		reinterpret_cast<void*>(base + 0x1907b0),
		reinterpret_cast<void*>(&MenuLayer_init_H),
		reinterpret_cast<void**>(&MenuLayer_init)
	);

	MH_CreateHook(
		reinterpret_cast<void*>(base + 0x4DE40),
		reinterpret_cast<void*>(&CreatorLayer_init_H),
		reinterpret_cast<void**>(&CreatorLayer_init)
	);

	MH_CreateHook(
		reinterpret_cast<void*>(base + 0x1255D0),
		reinterpret_cast<void*>(&GarageLayer_init_H),
		reinterpret_cast<void**>(&GarageLayer_init)
	);

	MH_CreateHook(
		reinterpret_cast<void*>(base + 0x17DA60),
		reinterpret_cast<void*>(&LevelSearchLayer_init_H),
		reinterpret_cast<void**>(&LevelSearchLayer_init)
	);

	MH_CreateHook(
		reinterpret_cast<void*>(base + 0x15A040),
		reinterpret_cast<void*>(&LevelBrowserLayer_init_H),
		reinterpret_cast<void**>(&LevelBrowserLayer_init)
	);

	MH_CreateHook(
		reinterpret_cast<void*>(base + 0x1855A0),
		reinterpret_cast<void*>(&LevelSelectLayer_init_H),
		reinterpret_cast<void**>(&LevelSelectLayer_init)
	);

	MH_CreateHook(
		reinterpret_cast<void*>(base + 0x175DF0),
		reinterpret_cast<void*>(&LevelInfoLayer_init_H),
		reinterpret_cast<void**>(&LevelInfoLayer_init)
	);

	MH_CreateHook(
		reinterpret_cast<void*>(base + 0x1587B0),
		reinterpret_cast<void*>(&LeaderLayer_init_H),
		reinterpret_cast<void**>(&LeaderLayer_init)
	);

	MH_CreateHook(
		reinterpret_cast<void*>(base + 0x102E70),
		reinterpret_cast<void*>(&GauntletLayer_init_H),
		reinterpret_cast<void**>(&GauntletLayer_init)
	);

	MH_CreateHook(
		reinterpret_cast<void*>(base + 0x1051C0),
		reinterpret_cast<void*>(&GauntletSelectLayer_init_H),
		reinterpret_cast<void**>(&GauntletSelectLayer_init)
	);

	MH_CreateHook(
		reinterpret_cast<void*>(base + 0x6F5D0),
		reinterpret_cast<void*>(&EditLevelLayer_init_H),
		reinterpret_cast<void**>(&EditLevelLayer_init)
	);

	MH_EnableHook(MH_ALL_HOOKS);
#endif
}

#if _WIN32
WIN32CAC_ENTRY(inject)
#endif