/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "gargoyle/gargoyle.h"

#include "base/plugins.h"
#include "common/md5.h"
#include "common/memstream.h"
#include "common/savefile.h"
#include "common/str-array.h"
#include "common/system.h"
#include "engines/advancedDetector.h"
#include "graphics/colormasks.h"
#include "graphics/surface.h"

#define MAX_SAVES 99

namespace Gargoyle {

struct GargoyleGameDescription {
	ADGameDescription _desc;
	Common::String _filename;
	InterpreterType _interpType;
	Common::String _md5;
};

const Common::String &GargoyleEngine::getFilename() const {
	return _gameDescription->_filename;
}
uint32 GargoyleEngine::getFeatures() const {
	return _gameDescription->_desc.flags;
}

bool GargoyleEngine::isDemo() const {
	return (bool)(_gameDescription->_desc.flags & ADGF_DEMO);
}

Common::Language GargoyleEngine::getLanguage() const {
	return _gameDescription->_desc.language;
}

InterpreterType GargoyleEngine::getInterpreterType() const {
	return _gameDescription->_interpType;
}

const Common::String &GargoyleEngine::getGameMD5() const {
	return _gameDescription->_md5;
}

} // End of namespace Gargoyle

static const PlainGameDescriptor gargoyleGames[] = {
	{"scott", "Scott Adams Games"},
	{0, 0}
};

#include "common/config-manager.h"
#include "common/file.h"
#include "gargoyle/detection_tables.h"
#include "gargoyle/scott/detection.h"
#include "gargoyle/scott/scott.h"

class GargoyleMetaEngine : public AdvancedMetaEngine {
public:
	GargoyleMetaEngine() : AdvancedMetaEngine(Gargoyle::gameDescriptions, sizeof(Gargoyle::GargoyleGameDescription), gargoyleGames) {
		_maxScanDepth = 3;
	}

	virtual const char *getName() const {
		return "Gargoyle Engine";
	}

	virtual const char *getOriginalCopyright() const {
		return "Gargoyle Engine (c)";
	}

	virtual bool hasFeature(MetaEngineFeature f) const override;
	virtual bool createInstance(OSystem *syst, Engine **engine, const ADGameDescription *desc) const override;
	virtual SaveStateList listSaves(const char *target) const;
	virtual int getMaximumSaveSlot() const;
	virtual void removeSaveState(const char *target, int slot) const;
	SaveStateDescriptor querySaveMetaInfos(const char *target, int slot) const;

	virtual DetectedGames detectGames(const Common::FSList &fslist) const override;

	virtual ADDetectedGames detectGame(const Common::FSNode &parent, const FileMap &allFiles, Common::Language language, Common::Platform platform, const Common::String &extra) const override;
};

bool GargoyleMetaEngine::hasFeature(MetaEngineFeature f) const {
	return
	    (f == kSupportsListSaves) ||
		(f == kSupportsLoadingDuringStartup) ||
		(f == kSupportsDeleteSave) ||
		(f == kSavesSupportMetaInfo) ||
		(f == kSavesSupportThumbnail);
}

bool Gargoyle::GargoyleEngine::hasFeature(EngineFeature f) const {
	return
		(f == kSupportsRTL) ||
		(f == kSupportsLoadingDuringRuntime) ||
		(f == kSupportsSavingDuringRuntime);
}

bool GargoyleMetaEngine::createInstance(OSystem *syst, Engine **engine, const ADGameDescription *desc) const {
	Gargoyle::GargoyleGameDescription *gd = (Gargoyle::GargoyleGameDescription *)desc;
	gd->_filename = ConfMan.get("filename");

	switch (gd->_interpType) {
	case Gargoyle::INTERPRETER_SCOTT:
		*engine = new Gargoyle::Scott::Scott(syst, gd);
		break;
	default:
		error("Unknown interpreter");
	}

	return gd != 0;
}

SaveStateList GargoyleMetaEngine::listSaves(const char *target) const {
	SaveStateList saveList;
	return saveList;
}

int GargoyleMetaEngine::getMaximumSaveSlot() const {
	return MAX_SAVES;
}

void GargoyleMetaEngine::removeSaveState(const char *target, int slot) const {
}

SaveStateDescriptor GargoyleMetaEngine::querySaveMetaInfos(const char *target, int slot) const {
	return SaveStateDescriptor();
}

DetectedGames GargoyleMetaEngine::detectGames(const Common::FSList &fslist) const {
	DetectedGames detectedGames;
	Gargoyle::Scott::ScottMetaEngine::detectGames(fslist, detectedGames);

	return detectedGames;
}

static Gargoyle::GargoyleGameDescription gameDescription;

ADDetectedGames GargoyleMetaEngine::detectGame(const Common::FSNode &parent, const FileMap &allFiles, Common::Language language, Common::Platform platform, const Common::String &extra) const {
	static char gameId[100];
	strcpy(gameId, ConfMan.get("gameid").c_str());
	Common::String filename = ConfMan.get("filename");

	Common::FSList fslist;
	DetectedGames detectedGames;
	fslist.push_back(parent.getChild(filename));
	ADDetectedGames results;
	Common::File f;

	Gargoyle::Scott::ScottMetaEngine::detectGames(fslist, detectedGames);
	if (detectedGames.size() > 0 && f.open(parent.getChild(filename))) {
		DetectedGame gd = detectedGames.front();

		gameDescription._interpType = Gargoyle::INTERPRETER_SCOTT;
		gameDescription._desc.gameId = gameId;
		gameDescription._desc.language = gd.language;
		gameDescription._desc.platform = gd.platform;
		gameDescription._filename = filename;
		gameDescription._md5 = Common::computeStreamMD5AsString(f, 5000);

		ADDetectedGame dg((ADGameDescription *)&gameDescription);
		results.push_back(dg);
	}

	return results;
}

#if PLUGIN_ENABLED_DYNAMIC(GARGOYLE)
	REGISTER_PLUGIN_DYNAMIC(Gargoyle, PLUGIN_TYPE_ENGINE, GargoyleMetaEngine);
#else
	REGISTER_PLUGIN_STATIC(GARGOYLE, PLUGIN_TYPE_ENGINE, GargoyleMetaEngine);
#endif