//
//

#include "EXStoSFZ.hpp"


using namespace std;
using namespace juce;
using namespace rmb;

////
////

////
////

//Array<File> rmb::EXSInstrument::cachedSampleSearchDirectories = {};
//Array<File> rmb::EXSInstrument::resolvedSampleFiles = {};

const Array<File> rmb::EXSInstrument::defaultSampleSearchDirectories = {
#if JUCE_MAC

	File::getSpecialLocation(File::userMusicDirectory).getChildFile("Audio Music Apps").getChildFile("Sampler Instruments"),

	File::getSpecialLocation(File::commonApplicationDataDirectory).getChildFile("Application Support").getChildFile("Logic").getChildFile("EXS Factory Samples"),

	File::getSpecialLocation(File::commonApplicationDataDirectory).getChildFile("Application Support").getChildFile("Logic").getChildFile("Sampler Instruments"),

//	File::getSpecialLocation(File::commonApplicationDataDirectory).getChildFile("Application Support").getChildFile("GarageBand").getChildFile("Instrument Library").getChildFile("Sampler"),

	
//	File::getSpecialLocation(File::commonApplicationDataDirectory).getChildFile("Audio").getChildFile("Apple Loops"),
//	File::getSpecialLocation(File::commonApplicationDataDirectory).getChildFile("Audio").getChildFile("Sounds"),
//	File::getSpecialLocation(File::commonApplicationDataDirectory).getChildFile("Audio").getChildFile("Presets"),

	
	
//	File::getSpecialLocation(File::userApplicationDataDirectory).getChildFile("Audio").getChildFile("Sounds"),
//	File::getSpecialLocation(File::userApplicationDataDirectory).getChildFile("Audio").getChildFile("Apple Loops"),
//	File::getSpecialLocation(File::userApplicationDataDirectory).getChildFile("Audio").getChildFile("Presets"),
	
#endif
	
	//File::getSpecialLocation(File::userHomeDirectory),
	//File::getSpecialLocation(File::userMusicDirectory),
	//File::getSpecialLocation(File::userDocumentsDirectory),

};



////
////


//// END

