//

#ifndef EXStoSFZ_hpp
#define EXStoSFZ_hpp

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "rmb_main_includes.hpp"

#include <limits.h>

////
////

using namespace std;
using namespace juce;

////
////

namespace rmb
{
	////
	namespace exs {
		const uint32 EXSHeaderSig =		0x00000101;
		const uint32 EXSZoneSig =		0x01000101;
		const uint32 EXSGroupSig =		0x02000101;
		const uint32 EXSSampleSig =		0x03000101;
		const uint32 EXSParamSig =		0x04000101;
		const uint32 ESX04Sig =			0x40000101;
		const uint32 ESX05Sig =			0x50000101;
		const uint32 ESX06Sig =			0x60000101;
		const uint32 ESX07Sig =			0x70000101;
		const uint32 ESX08Sig =			0x80000101;
		////
		const int headerSize =			84;
		////
		inline static int bytes_to_int_big_endian(const char *bytes)
		{
			int i;
			int result;
			
			result = 0;
			for (i = 0; i < sizeof(int); ++i)
			result = (result << CHAR_BIT) + bytes[i];
			return result;
		}
		inline static int bytes_to_int_little_endian(const char *bytes)
		{
			int i;
			int result;
			
			result = 0;
			for (i = 0; i < sizeof(int); ++i)
			result += bytes[i] << (i * CHAR_BIT);
			return result;
		}
		////
		inline unsigned short swap_16bit(unsigned short us)
		{
			return (unsigned short)(((us & 0xFF00) >> 8) |
									((us & 0x00FF) << 8));
		}
		
		inline unsigned long swap_32bit(unsigned long ul)
		{
			return (unsigned long)(((ul & 0xFF000000) >> 24) |
								   ((ul & 0x00FF0000) >>  8) |
								   ((ul & 0x0000FF00) <<  8) |
								   ((ul & 0x000000FF) << 24));
		}
		////
		inline static int parseSize(const char* data, const int offset)
		{
			return 84 + bytes_to_int_little_endian(data+offset+4);
			//__size = 84 + (int)((uint32)pystruct::unpack(PY_STRING('<I'), data.begin()+offset+4));
			//return __size;
		}
		inline static int parseId(const char* data, const int offset)
		{
			return bytes_to_int_little_endian(data+offset+8);
			//__size = 84 + (int)((uint32)pystruct::unpack(PY_STRING('<I'), data.begin()+offset+4));
			//return __size;
		}
		////
		inline static String parseString(const char* data, const int offset, const int byteOffset = 0)
		{
			CharPointer_UTF8 text(data+offset+byteOffset);
			size_t maxChars = 64;
			return String(text, maxChars);
			
		}
		////
		inline static int parseSignedByte(const char* data, const int offset, const int byteOffset = 0)
		{
			return (int)((signed char)(data[offset+byteOffset]));
		}
		inline static int parseUnsignedByte(const char* data, const int offset, const int byteOffset = 0)
		{
			return (int)((unsigned char)(data[offset+byteOffset]));
		}
		inline static int32 parseSignedInt(const char* data, const int offset, const int byteOffset = 0)
		{
			int32 i;
			memcpy(&i, data+offset+byteOffset, sizeof(i));
			return (int32)i;
			//if (JUCE_LITTLE_ENDIAN) {
			//	return (int)i;
			//} else {
			//	return (int)ntohl(i);
			//}
		}
		inline static uint32 parseUnsignedInt(const char* data, const int offset, const int byteOffset = 0)
		{
			uint32 i;
			memcpy(&i, data+offset+byteOffset, sizeof(i));
			return (uint32)i;
			//if (JUCE_LITTLE_ENDIAN) {
			//	return (int)i;
			//} else {
			//	return (int)ntohl(i);
			//}
		}
		////
	}
	////
	class EXSChunk
	{
	public:
		EXSChunk(const int offsetIn = 0, const uint32 sigIn = 0x00000000) :
		sig(sigIn),
		offset(offsetIn)
		{
			
		}
		virtual ~EXSChunk() {}
		//void parse(std::array<char>& data, const int offset)
		//{
			//uint32 sig = (uint32)pystruct::unpack(PY_STRING('<I'), data.begin()+offset);

		//}
		//int size(std::array<char>& data, const int offset)
		//{
			//__size = 84 + (int)((uint32)pystruct::unpack(PY_STRING('<I'), data.begin()+offset+4));
		//	return __size;
		//}
		//void parseSize(const char* data, const int offset)
		//{
		//	__size = 84 + bytes_to_int_little_endian(data+offset+4);
			//__size = 84 + (int)((uint32)pystruct::unpack(PY_STRING('<I'), data.begin()+offset+4));
			//return __size;
		//}
		//void parseId(const char* data, const int offset)
		//{
		//	__id = bytes_to_int_little_endian(data+offset+8);
			//__size = 84 + (int)((uint32)pystruct::unpack(PY_STRING('<I'), data.begin()+offset+4));
			//return __size;
		//}
		//void parseName(const char* data)
		//{
		//	CharPointer_UTF8 text(data);
		//	size_t maxChars = 64;
		//	name = String(text, maxChars);
		//}
		////
		EXSChunk(const EXSChunk& other)
		{
			sig = other.sig;
			offset = other.offset;
			chunkSize = other.chunkSize;
			size = other.size;
			id = other.id;
			name = other.name;
		}
		EXSChunk& operator=(const EXSChunk& other)
		{
			if (this != &other) {
				sig = other.sig;
				offset = other.offset;
				chunkSize = other.chunkSize;
				size = other.size;
				id = other.id;
				name = other.name;
			}
			return *this;
		}
		bool operator<(const EXSChunk& other) const
		{
			if (id < other.id) {
				return true;
			}
			if (name < other.name) {
				return true;
			}
			return false;
		}
		bool operator==(const EXSChunk& other) const
		{
			return //(sig == other.sig)
			//&& (offset == other.offset)
			//&& (chunkSize == other.chunkSize)
			//&& (size == other.size)
			//&&
			(id == other.id)
			&& (name == other.name);
		}
		bool operator!=(const EXSChunk& other) const
		{
			return ! operator==(other);
		}
		////
		virtual void parse(const char* data)
		{
			sig = (uint32)exs::parseUnsignedInt(data, offset, 0);
			size = exs::parseUnsignedInt(data, offset, 4);
			chunkSize = size + 84;
			id = exs::parseUnsignedInt(data, offset, 8);
			name = exs::parseString(data, offset, 20);
		}
		virtual String toString()
		{
			String txt = String("EXSChunk: ")
			+ String::formatted("0x%08jx", sig)
			+ String(" chunkSize: ") + String(chunkSize)
			+ String(" size: ") + String(size)
			+ String(" id: ") + String(id)
			+ String(" name: ") + name;
			return txt;
		}
	public:
		uint32 sig { 0x00000000 };
		int offset { 0 };
		int chunkSize { 0 };
		int size { 0 };
		int id { 0 };
		String name {};
	};
	////
	class EXSHeader :
	public EXSChunk
	{
	public:
		EXSHeader(const int offsetIn = 0, const uint32 sigIn = exs::EXSHeaderSig) :
		EXSChunk(offsetIn, sigIn)
		{
			
		}
		bool operator<(const EXSHeader& other) const
		{
			if (id < other.id) {
				return true;
			}
			if (name < other.name) {
				return true;
			}
			return false;
		}
		bool operator==(const EXSHeader& other) const
		{
			return //(sig == other.sig)
			//&& (offset == other.offset)
			//&& (chunkSize == other.chunkSize)
			//&& (size == other.size)
			//&&
			(id == other.id)
			&& (name == other.name);
		}
		bool operator!=(const EXSHeader& other) const
		{
			return ! operator==(other);
		}
		String toString() override
		{
			String txt = String("EXSHeader: ")
			+ String::formatted("0x%08jx", sig)
			+ String(" chunkSize: ") + String(chunkSize)
			+ String(" size: ") + String(size)
			+ String(" id: ") + String(id)
			+ String(" name: ") + name
			
			;
			return txt;
		}
	public:
	};
	////
	////
	class EXSZone :
	public EXSChunk
	{
	public:
		EXSZone(const int offsetIn = 0, const uint32 sigIn = exs::EXSZoneSig) :
		EXSChunk(offsetIn, sigIn)
		{
		}
		////
		EXSZone(const EXSZone& other)
		{
			sig = other.sig;
			offset = other.offset;
			chunkSize = other.chunkSize;
			size = other.size;
			id = other.id;
			name = other.name;
			////
			rootNote = other.rootNote;
			fineTune = other.fineTune;
			pan = other.pan;
			volumeAdjust = other.volumeAdjust;
			startNote = other.startNote;
			endNote = other.endNote;
			minVel = other.minVel;
			maxVel = other.maxVel;
			
			sampleStart = other.sampleStart;
			sampleEnd = other.sampleEnd;
			
			loopStart = other.loopStart;
			loopEnd = other.loopEnd;
			
			loop = other.loop;
			
			pitchTrack = other.pitchTrack;
			oneShot = other.oneShot;
			
			group = other.group;
			
			sampleIndex = other.sampleIndex;
		}
		EXSZone& operator=(const EXSZone& other)
		{
			if (this != &other) {
				sig = other.sig;
				offset = other.offset;
				chunkSize = other.chunkSize;
				size = other.size;
				id = other.id;
				name = other.name;
				////
				rootNote = other.rootNote;
				fineTune = other.fineTune;
				pan = other.pan;
				volumeAdjust = other.volumeAdjust;
				startNote = other.startNote;
				endNote = other.endNote;
				minVel = other.minVel;
				maxVel = other.maxVel;
				
				sampleStart = other.sampleStart;
				sampleEnd = other.sampleEnd;
				
				loopStart = other.loopStart;
				loopEnd = other.loopEnd;
				
				loop = other.loop;
				
				pitchTrack = other.pitchTrack;
				oneShot = other.oneShot;
				
				group = other.group;
				
				sampleIndex = other.sampleIndex;
			}
			return *this;
		}
		bool operator<(const EXSZone& other) const
		{
			if (startNote < other.startNote) {
				return true;
			}
			if (endNote < other.endNote) {
				return true;
			}
			if (minVel < other.minVel) {
				return true;
			}
			if (maxVel < other.maxVel) {
				return true;
			}
			
			if (rootNote < other.rootNote) {
				return true;
			}

			if (id < other.id) {
				return true;
			}
			if (name < other.name) {
				return true;
			}
			
			if (sampleStart < other.sampleStart) {
				return true;
			}
			if (sampleEnd < other.sampleEnd) {
				return true;
			}
			if (loopStart < other.loopStart) {
				return true;
			}
			if (loopEnd < other.loopEnd) {
				return true;
			}
			if (group < other.group) {
				return true;
			}
			if (sampleIndex < other.sampleIndex) {
				return true;
			}
			return false;
		}
		bool operator==(const EXSZone& other) const
		{
			return //(sig == other.sig)
				//&& (offset == other.offset)
				//&& (chunkSize == other.chunkSize)
				//&& (size == other.size)
				//&&
			(id == other.id)
				&& (name == other.name)
				&& (rootNote == other.rootNote)
				&& (fineTune == other.fineTune)
				&& (pan == other.pan)
				&& (volumeAdjust == other.volumeAdjust)
				&& (startNote == other.startNote)
				&& (endNote == other.endNote)
				&& (minVel == other.minVel)
				&& (maxVel == other.maxVel)
				&& (sampleStart == other.sampleStart)
				&& (sampleEnd == other.sampleEnd)
				&& (loopStart == other.loopStart)
				&& (loopEnd == other.loopEnd)
				&& (loop == other.loop)
				&& (pitchTrack == other.pitchTrack)
				&& (oneShot == other.oneShot)
				&& (group == other.group)
				&& (sampleIndex == other.sampleIndex);
		}
		bool operator!=(const EXSZone& other) const
		{
			return ! operator==(other);
		}
		////
		////
		void parse(const char* data) override
		{
			EXSChunk::parse(data);
			
			rootNote = exs::parseUnsignedByte(data, offset, 85);
			fineTune = exs::parseSignedByte(data, offset, 86);
			pan = exs::parseSignedByte(data, offset, 87);
			volumeAdjust = exs::parseSignedByte(data, offset, 88);
			startNote = exs::parseUnsignedByte(data, offset, 90);
			endNote = exs::parseUnsignedByte(data, offset, 91);
			minVel = exs::parseUnsignedByte(data, offset, 93);
			maxVel = exs::parseUnsignedByte(data, offset, 94);

			sampleStart = exs::parseSignedInt(data, offset, 96);
			sampleEnd = exs::parseSignedInt(data, offset, 100);

			loopStart = exs::parseSignedInt(data, offset, 104);
			loopEnd = exs::parseSignedInt(data, offset, 108);

			loop = exs::parseUnsignedByte(data, offset, 117);

			pitchTrack = (bool)(exs::parseUnsignedByte(data, offset, 84) & 1);
			oneShot = (bool)(exs::parseUnsignedByte(data, offset, 84) & 2);

			group = exs::parseSignedInt(data, offset, 172);

			sampleIndex = exs::parseUnsignedInt(data, offset, 176);

		}
		String toString() override
		{
			String txt = String("EXSZone: ")
			+ String::formatted("0x%08jx", sig)
			+ String(" chunkSize: ") + String(chunkSize)
			+ String(" size: ") + String(size)
			+ String(" id: ") + String(id)
			+ String(" name: ") + name
			+ String(" rootNote: ") + String(rootNote)
			+ String(" fineTune: ") + String(fineTune)
			+ String(" pan: ") + String(pan)
			+ String(" volumeAdjust: ") + String(volumeAdjust)
			+ String(" startNote: ") + String(startNote)
			+ String(" endNote: ") + String(endNote)
			+ String(" minVel: ") + String(minVel)
			+ String(" maxVel: ") + String(maxVel)
			
			+ String(" sampleStart: ") + String(sampleStart)
			+ String(" sampleEnd: ") + String(sampleEnd)
			
			+ String(" loopStart: ") + String(loopStart)
			+ String(" loopEnd: ") + String(loopEnd)
			
			+ String(" loop: ") + String(loop)
			
			+ String(" pitchTrack: ") + String((int)pitchTrack)
			+ String(" oneShot: ") + String((int)oneShot)
			
			+ String(" group: ") + String(group)
			
			+ String(" sampleIndex: ") + String(sampleIndex)

			;
			return txt;
		}
		int getRootNote() const
		{
			if (!pitchTrack) {
				if ((rootNote < startNote) || (rootNote > endNote)) {
					return startNote;
				}
			}
			return rootNote;
		}
	public:
		int rootNote { -1 };
		int fineTune { 0 };
		int pan { 0 };
		int volumeAdjust { 0 };
		int startNote { 0 };
		int endNote { 0 };
		int minVel { 0 };
		int maxVel { 0 };
		int sampleStart { 0 };
		int sampleEnd { 0 };
		int loopStart { 0 };
		int loopEnd { 0 };
		int loop { 0 };
		bool pitchTrack { false };
		bool oneShot { false };
		int group { -1 };
		int sampleIndex { 0 };
	};
	////
	class EXSGroup :
	public EXSChunk
	{
	public:
		EXSGroup(const int offsetIn = 0, const uint32 sigIn = exs::EXSGroupSig) :
		EXSChunk(offsetIn, sigIn)
		{
		}
		////
		EXSGroup(const EXSGroup& other)
		{
			sig = other.sig;
			offset = other.offset;
			chunkSize = other.chunkSize;
			size = other.size;
			id = other.id;
			name = other.name;
			////
			polyphony = other.polyphony;
			trigger = other.trigger;
			output = other.output;
			sequence = other.sequence;
		}
		EXSGroup& operator=(const EXSGroup& other)
		{
			if (this != &other) {
				sig = other.sig;
				offset = other.offset;
				chunkSize = other.chunkSize;
				size = other.size;
				id = other.id;
				name = other.name;
				////
				polyphony = other.polyphony;
				trigger = other.trigger;
				output = other.output;
				sequence = other.sequence;
			}
			return *this;
		}
		bool operator<(const EXSGroup& other) const
		{
			if (id < other.id) {
				return true;
			}
			if (name < other.name) {
				return true;
			}
			if (polyphony < other.polyphony) {
				return true;
			}
			if (trigger < other.trigger) {
				return true;
			}
			if (output < other.output) {
				return true;
			}
			if (sequence < other.sequence) {
				return true;
			}
			return false;
		}
		bool operator==(const EXSGroup& other) const
		{
			return //(sig == other.sig)
			//&& (offset == other.offset)
			//&& (chunkSize == other.chunkSize)
			//&& (size == other.size)
			//&&
			(id == other.id)
			&& (name == other.name)
			&& (polyphony == other.polyphony)
			&& (trigger == other.trigger)
			&& (output == other.output)
			&& (sequence == other.sequence);
		}
		bool operator!=(const EXSGroup& other) const
		{
			return ! operator==(other);
		}
		////
		void parse(const char* data) override
		{
			EXSChunk::parse(data);
			
			polyphony = exs::parseUnsignedByte(data, offset, 86);
			//trigger = exs::parseSignedByte(data, offset, 157);
			//output = exs::parseSignedByte(data, offset, 158);
			trigger = exs::parseUnsignedByte(data, offset, 157);
			output = exs::parseUnsignedByte(data, offset, 158);

			sequence = exs::parseSignedInt(data, offset, 164);
			
		}
		String toString() override
		{
			String txt = String("EXSGroup: ")
			+ String::formatted("0x%08jx", sig)
			+ String(" chunkSize: ") + String(chunkSize)
			+ String(" size: ") + String(size)
			+ String(" id: ") + String(id)
			+ String(" name: ") + name
			+ String(" polyphony: ") + String(polyphony)
			+ String(" trigger: ") + String(trigger)
			+ String(" output: ") + String(output)
			+ String(" sequence: ") + String(sequence)
			
			;
			return txt;
		}
	public:
		int polyphony { 0 };
		int trigger { 0 };
		int output { 0 };
		int sequence { 0 };
	};
	////
	class EXSSample :
	public EXSChunk
	{
	public:
		EXSSample(const int offsetIn = 0, const uint32 sigIn = exs::EXSSampleSig) :
		EXSChunk(offsetIn, sigIn)
		{
		}
		////
		EXSSample(const EXSSample& other)
		{
			sig = other.sig;
			offset = other.offset;
			chunkSize = other.chunkSize;
			size = other.size;
			id = other.id;
			name = other.name;
			////
			length = other.length;
			rate = other.rate;
			bitDepth = other.bitDepth;
			info = other.info;
			sampleFile = other.sampleFile;
		}
		EXSSample& operator=(const EXSSample& other)
		{
			if (this != &other) {
				sig = other.sig;
				offset = other.offset;
				chunkSize = other.chunkSize;
				size = other.size;
				id = other.id;
				name = other.name;
				////
				length = other.length;
				rate = other.rate;
				bitDepth = other.bitDepth;
				info = other.info;
				sampleFile = other.sampleFile;
			}
			return *this;
		}
		bool operator<(const EXSSample& other) const
		{
			if (id < other.id) {
				return true;
			}
			if (name < other.name) {
				return true;
			}
			if (length < other.length) {
				return true;
			}
			if (rate < other.rate) {
				return true;
			}
			if (bitDepth < other.bitDepth) {
				return true;
			}
			if (info < other.info) {
				return true;
			}
			if (sampleFile < other.sampleFile) {
				return true;
			}
			return false;
		}
		bool operator==(const EXSSample& other) const
		{
			return //(sig == other.sig)
			//&& (offset == other.offset)
			//&& (chunkSize == other.chunkSize)
			//&& (size == other.size)
			//&&
			(id == other.id)
			&& (name == other.name)
			&& (length == other.length)
			&& (rate == other.rate)
			&& (bitDepth == other.bitDepth)
			&& (info == other.info)
			&& (sampleFile == other.sampleFile)
			;
		}
		bool operator!=(const EXSSample& other) const
		{
			return ! operator==(other);
		}
		////
		void parse(const char* data) override
		{
			EXSChunk::parse(data);
			
			length = exs::parseSignedInt(data, offset, 88);
			rate = exs::parseSignedInt(data, offset, 92);
			bitDepth = exs::parseSignedByte(data, offset, 96);
			////
			/*
			int lastOffset = offset + chunkSize - 1;
			int endOffset = offset + 96 + 1;
			int startIndex = 0;
			int endIndex = 0;
			for (int i = lastOffset; i >= endOffset; i--) {
				if (data[i] == 0) {
					continue;
				} else {
					endIndex = i;
					break;
				}
			}
			if (endIndex > 0) {
				for (int i = endIndex; i >= endOffset; i--) {
					if (data[i] == 0) {
						startIndex = i + 1;
						break;
					}
				}
				if (startIndex > 0) {
					CharPointer_UTF8 text(data+startIndex);
					size_t maxChars = (size_t)((endIndex + 1) - startIndex);
					info = String(text, maxChars);
					////
					//std::freopen(RMB_APP_LOG_PATH, "a", stdout);
					//std::cout << "EXSSample: idx: " << (startIndex - offset) << std::endl;
				}
			}
			*/
			////
			int lastOffset = offset + chunkSize - 1;
			int endOffset = offset + 96 + 1;
			lastOffset = parseStringFromEnd(data, endOffset, lastOffset, info);
			if ((info == name) && (lastOffset > endOffset)) {
				lastOffset = parseStringFromEnd(data, endOffset, lastOffset, info);
			} else {
				File nameFile(name);
				File infoFile(info);
				String nameFileExt = nameFile.getFileExtension();
				String infoFileExt = infoFile.getFileExtension();
				if (infoFileExt == nameFileExt) {
					name = info;
					if (lastOffset > endOffset) {
						lastOffset = parseStringFromEnd(data, endOffset, lastOffset, info);
					}
				}
			}
		}
		int parseStringFromEnd(const char* data, const int endOffset, const int lastOffset, String& str)
		{
			int startIndex = 0;
			int endIndex = 0;
			for (int i = lastOffset; i >= endOffset; i--) {
				if (data[i] == 0) {
					continue;
				} else {
					endIndex = i;
					break;
				}
			}
			if (endIndex > 0) {
				for (int i = endIndex; i >= endOffset; i--) {
					if (data[i] == 0) {
						startIndex = i + 1;
						break;
					}
				}
				if (startIndex > 0) {
					CharPointer_UTF8 text(data+startIndex);
					size_t maxChars = (size_t)((endIndex + 1) - startIndex);
					str = String(text, maxChars);
					return startIndex - 1;
				}
			}
			return endOffset - 1;
		}
		String toString() override
		{
			String txt = String("EXSSample: ")
			+ String::formatted("0x%08jx", sig)
			+ String(" chunkSize: ") + String(chunkSize)
			+ String(" size: ") + String(size)
			+ String(" id: ") + String(id)
			+ String(" name: ") + name
			+ String(" length: ") + String(length)
			+ String(" rate: ") + String(rate)
			+ String(" bitDepth: ") + String(bitDepth)
			+ String(" info: ") + info
			+ String(" sampleFile: ") + sampleFile.getFullPathName()
			
			;
			return txt;
		}
	public:
		int length { 0 };
		int rate { 0 };
		int bitDepth { 0 };
		String info {};
		File sampleFile {};
	};
	////
	class EXSParam :
	public EXSChunk
	{
	public:
		EXSParam(const int offsetIn = 0, const uint32 sigIn = exs::EXSParamSig) :
		EXSChunk(offsetIn, sigIn)
		{
		}
		bool operator<(const EXSParam& other) const
		{
			if (id < other.id) {
				return true;
			}
			if (name < other.name) {
				return true;
			}
			return false;
		}
		bool operator==(const EXSParam& other) const
		{
			return //(sig == other.sig)
			//&& (offset == other.offset)
			//&& (chunkSize == other.chunkSize)
			//&& (size == other.size)
			//&&
			(id == other.id)
			&& (name == other.name);
		}
		bool operator!=(const EXSParam& other) const
		{
			return ! operator==(other);
		}
		////
		//void parse(const char* data) override
		//{
		//	EXSChunk::parse(data);
		//}
		String toString() override
		{
			String txt = String("EXSParam: ")
			+ String::formatted("0x%08jx", sig)
			+ String(" chunkSize: ") + String(chunkSize)
			+ String(" size: ") + String(size)
			+ String(" id: ") + String(id)
			+ String(" name: ") + name
			
			;
			return txt;
		}
	public:
	};
	////
	typedef std::vector<EXSChunk> EXSChunkVectorType;
	typedef std::vector<EXSHeader> EXSHeaderVectorType;
	typedef std::vector<EXSZone> EXSZoneVectorType;
	typedef std::vector<EXSGroup> EXSGroupVectorType;
	typedef std::vector<EXSSample> EXSSampleVectorType;
	typedef std::vector<EXSParam> EXSParamVectorType;
	////
	class KeyRange
	{
	public:
		KeyRange(const int startNoteIn, const int endNoteIn, const int rootNoteIn, const int panIn, const int sequencePositionIn = 0) :
		startNote(startNoteIn),
		endNote(endNoteIn),
		rootNote(rootNoteIn),
		pan(panIn),
		sequencePosition(sequencePositionIn)
		{
		}
		KeyRange(const EXSZone& zoneIn, const int sequencePositionIn = 0) :
		startNote(zoneIn.startNote),
		endNote(zoneIn.endNote),
		rootNote(zoneIn.getRootNote()),
		pan(zoneIn.pan),
		sequencePosition(sequencePositionIn)
		{
		}
		~KeyRange() {}
		KeyRange(const KeyRange& other)
		{
			startNote = other.startNote;
			endNote = other.endNote;
			rootNote = other.rootNote;
			pan = other.pan;
			sequencePosition = other.sequencePosition;
		}
		KeyRange& operator=(const KeyRange& other)
		{
			if (this != &other) {
				startNote = other.startNote;
				endNote = other.endNote;
				rootNote = other.rootNote;
				pan = other.pan;
				sequencePosition = other.sequencePosition;
			}
			return *this;
		}
		bool operator<(const KeyRange& other) const
		{
			
			if (endNote < other.startNote) {
				return true;
			}
			if (startNote < other.startNote) {
				assert(endNote < other.startNote); // sanity check
				return true;
			}
			if (sequencePosition < other.sequencePosition) {
				return true;
			}
			return false;

			
			//return ((startNote < other.startNote) ? true : ((startNote == other.startNote) ? ((endNote < other.endNote) ? true : (endNote == other.endNote && rootNote < other.rootNote) ? true : false) : false));

			if (startNote < other.startNote) {
				return true;
			}
			if (endNote < other.endNote) {
				return true;
			}
			if (rootNote < other.rootNote) {
				return true;
			}
			if (pan < other.pan) {
				return true;
			}
			if (sequencePosition < other.sequencePosition) {
				return true;
			}
			return false;
		}
		bool operator==(const KeyRange& other) const
		{
			return (startNote == other.startNote)
			&& (endNote == other.endNote)
			&& (rootNote == other.rootNote)
			&& (pan == other.pan)
			&& (sequencePosition == other.sequencePosition);
		}
		bool operator!=(const KeyRange& other) const
		{
			return ! operator==(other);
		}
	public:
		int startNote { 0 };
		int endNote { 0 };
		int rootNote { 0 };
		int pan { 0 };
		int sequencePosition { 0 };
	};
	typedef std::vector<KeyRange> KeyRangeVectorType;
	typedef std::map<KeyRange, EXSZoneVectorType> EXSZoneVectorByKeyRangeMapType;
	////
	////
	/*
	class IntRange
	{
	public:
		IntRange(const int startRangeIn, const int endRangeIn) :
		startRange(startRangeIn > endRangeIn ? endRangeIn : startRangeIn),
		endRange(endRangeIn < startRangeIn ? startRangeIn : endRangeIn)
		{
		}
		~IntRange() {}
		IntRange(const IntRange& other)
		{
			startRange = other.startRange;
			endRange = other.endRange;
		}
		IntRange& operator=(const IntRange& other)
		{
			if (this != &other) {
				startRange = other.startRange;
				endRange = other.endRange;
			}
			return *this;
		}
		bool operator<(const IntRange& other) const
		{
			//return ((startRange < other.startRange) ? true : ((startRange == other.startRange) ? ((endRange < other.endRange) ? true : false) : false));
			//if (startRange <= other.startRange) {
			//	if (endRange < other.endRange) {
			//		return true;
			//	} else if (startRange < other.startRange) {
			//		return true;
			//	}
			//}

			if (startRange < other.startRange) {
				return true;
			}
			if (endRange < other.endRange) {
				return true;
			}
			return false;
		}
		bool operator==(const IntRange& other) const
		{
			return (startRange == other.startRange)
			&& (endRange == other.endRange);
		}
		bool operator!=(const IntRange& other) const
		{
			return ! operator==(other);
		}
	public:
		int startRange { 0 };
		int endRange { 0 };
	};
	*/
	typedef IntNumberRange IntRange;
	typedef std::vector<IntRange> IntRangeVectorType;
	typedef std::map<IntRange, int> IntByIntRangeMapType;
	////
	class EXSInstrument
	{
	public:
		EXSInstrument(const File& fileIn = {}) :
		exsFile(fileIn)
		{
		}
		void clearSampleFileCache()
		{
			cachedSampleSearchDirectories.clear();
			resolvedSampleFiles.clear();
		}
		String getErrorString() { return errorString; }
		String getParseDebugString() { return exsParseDebugString; }
		bool open(const File& fileIn)
		{
			//std::freopen(RMB_APP_LOG_PATH, "a", stdout);
			errorString.clear();
			exsParseDebugString.clear();
			////
			exsFile = fileIn;
			bool result = false;
			////
			if (!exsFile.existsAsFile()) {
				errorString = String("EXS Error: \"") + exsFile.getFullPathName() + String("\" does not exist as a file");
				return result;
			}
			exsFileSize = (int)exsFile.getSize();
			if ((exsFileSize < 84) || (exsFileSize > exsFileSizeMax)) {
				errorString = String("EXS Error: \"") + exsFile.getFullPathName() + String("\" has incorrect size");
				return result;
			}
			MemoryBlock memoryBlock;
			if (!exsFile.loadFileAsData(memoryBlock)) {
				errorString = String("EXS Error: \"") + exsFile.getFullPathName() + String("\" could not be read");
				return result;
			}
			size_t bytesRead = memoryBlock.getSize();
			const char* data = (const char*)memoryBlock.getData();
			exsDataSize = (int)bytesRead;
			//std::unique_ptr<InputStream> input = exsFile.createInputStream();
			//if (input.get() == nullptr) {
			//	return result;
			//}
		//	MemoryBlock destBlock;
		//	ssize_t maxNumBytesToRead = -1;
		//	size_t bytesRead = input->readIntoMemoryBlock(destBlock, maxNumBytesToRead);
		//	const char* data = (const char*)destBlock.getData();
		//	exsDataSize = (int)bytesRead;
			//std::cout << "EXSInstrument: exsDataSize: " << exsDataSize << std::endl;
			////
			uint32 sigIn;
			//if (JUCE_LITTLE_ENDIAN) {
			//	sigIn = (uint32)ntohl((uint32)exs::parseUnsignedInt(data, 0, 0));
			//} else {
				sigIn = (uint32)exs::parseUnsignedInt(data, 0, 0);
			//}
			//String sigStr = String::formatted("0x%08jx", sigIn);
			//std::cout << "EXSInstrument: sig: " << sigStr.toStdString() << std::endl;

			//if (sigIn != exs::EXSHeaderSig) {
			if ((sigIn != exs::EXSHeaderSig) && (sigIn != exs::ESX04Sig) && (sigIn != exs::ESX08Sig)) {
			//if (sigIn != exs::EXSHeaderSig) {
				errorString = String("EXS Error: \"") + exsFile.getFullPathName() + String("\" has incorrect header signature: ") + String::formatted("0x%08jx", sigIn);
				return result;
			}
			String headerString(data+16, 4);
			//std::cout << "EXSInstrument: headerString: " << headerString.toStdString() << std::endl;

			//if (headerString != "SOBT") {
			if ((headerString != "TBOS") && (headerString != "JBOS")) {
			//if (headerString != "TBOS") {
				errorString = String("EXS Error: \"") + exsFile.getFullPathName() + String("\" has incorrect header string: ") + headerString;
				return result;
			}
			////
			headers.clear();
			chunks.clear();
			zones.clear();
			groups.clear();
			samples.clear();
			params.clear();
			////
			int offset = 0;
			//int offset = 84;
			//data = data + offset;
			//offset = 0;
			////
			while (offset < exsDataSize) {
				EXSChunk chunk(offset);
				chunk.parse(data);
				//sigStr = String::formatted("0x%08jx", chunk.sig);
				//std::cout << "sig: " << sigStr.toStdString() << std::endl;
				switch (chunk.sig) {
					case exs::EXSHeaderSig:
					{
						EXSHeader header(offset);
						header.parse(data);
						headers.push_back(header);
					}
						break;
					case exs::ESX08Sig:
					{
						EXSHeader header(offset, exs::ESX08Sig);
						header.parse(data);
						headers.push_back(header);
					}
						break;
					case exs::EXSZoneSig:
					{
						EXSZone zone(offset);
						zone.parse(data);
						zones.push_back(zone);
					}
						break;
					case exs::EXSGroupSig:
					{
						EXSGroup group(offset);
						group.parse(data);
						groups.push_back(group);
					}
						break;
					case exs::EXSSampleSig:
					{
						EXSSample sample(offset);
						sample.parse(data);
						samples.push_back(sample);
					}
						break;
					case exs::EXSParamSig:
					{
						EXSParam param(offset);
						param.parse(data);
						params.push_back(param);
					}
						break;
					default:
					{
						chunks.push_back(chunk);
					}
						break;
				}
				offset += chunk.chunkSize;
			}
			numHeaders = (int)headers.size();
			numZones = (int)zones.size();
			numGroups = (int)groups.size();
			numSamples = (int)samples.size();
			numParams = (int)params.size();
			numChunks = (int)chunks.size();
			if ((numHeaders < 1) || (numSamples < 1) || (numZones < 1)) {
				exsParseDebugString = displayChunkResults();
				errorString = String("EXS Error: \"") + exsFile.getFullPathName() + String("\" has zero headers, zones, or samples");
				return result;
			}
			exsInstrumentName = headers[0].name;
			////
			updateZoneGroups();
			////
			exsParseDebugString = displayChunkResults();
			////
			//resolveSamples();
			//buildKeyRanges();
			//buildSfzFile();
			////
			result = true;
			////
		//	std::freopen(RMB_APP_LOG_PATH, "a", stdout);
		//	std::cout << "EXSInstrument: " << exsInstrumentName.toStdString() << std::endl
		//	<< exsParseDebugString.toStdString() << std::endl
		//	<< buildSfzFile().toStdString() << std::endl
		//	;
			////
			return result;
		}
		String displayChunkResults()
		{
			MemoryOutputStream displayText;
			int cnt = 0;
			displayText << String("HEADERS: ") << String(numHeaders) << String("\n");
			for (auto it = headers.begin(); it != headers.end(); ++it, cnt++) {
				displayText << String(cnt + 1) << String("\t") << it->toString() << String("\n");
			}
			displayText << String("\n");
			displayText << String("CHUNKS: ") << String(numChunks) << String("\n");
			cnt = 0;
			for (auto it = chunks.begin(); it != chunks.end(); ++it, cnt++) {
				displayText << String(cnt + 1) << String("\t") << it->toString() << String("\n");
			}
			displayText << String("\n");
			displayText << String("ZONES: ") << String(numZones) << String("\n");
			cnt = 0;
			for (auto it = zones.begin(); it != zones.end(); ++it, cnt++) {
				displayText << String(cnt + 1) << String("\t") << it->toString() << String("\n");
			}
			displayText << String("\n");
			displayText << String("GROUPS: ") << String(numGroups) << String("\n");
			cnt = 0;
			for (auto it = groups.begin(); it != groups.end(); ++it, cnt++) {
				displayText << String(cnt + 1) << String("\t") << it->toString() << String("\n");
			}
			displayText << String("\n");
			displayText << String("SAMPLES: ") << String(numSamples) << String("\n");
			cnt = 0;
			for (auto it = samples.begin(); it != samples.end(); ++it, cnt++) {
				displayText << String(cnt + 1) << String("\t") << it->toString() << String("\n");
			}
			displayText << String("\n");
			displayText << String("PARAMS: ") << String(numParams) << String("\n");
			cnt = 0;
			for (auto it = params.begin(); it != params.end(); ++it, cnt++) {
				displayText << String(cnt + 1) << String("\t") << it->toString() << String("\n");
			}
			//displayText << String("\n");
			//displayText << String("SAMPLE FILES: ") << String(resolvedSampleFiles.size()) << String("\n");
			//cnt = 0;
			//for (auto it = resolvedSampleFiles.begin(); it != resolvedSampleFiles.end(); ++it, cnt++) {
			//	displayText << String(cnt + 1) << String("\t") << it->getFullPathName() << String("\n");
			//}
			return displayText.toString();
		}
		void buildSequences()
		{
			sequences.clear();
			int groupIndex = 0;
			for (auto it = groups.begin(); it != groups.end(); ++it, groupIndex++) {
				if (it->sequence < 0) {
					continue;
				}
				/////
				//int groupId = it->id;
				int groupId = groupIndex;
				////
				for (int si = 0; si < sequences.size(); ++si) {
					if (sequences[si].contains(groupId)) {
						break;
					}
				}
				////
				Array<int> sequence;
				bool cont = true;
				while (cont) {
					cont = false;
					for (size_t gi = 0; gi < groups.size(); ++gi) {
						EXSGroup g = groups[gi];
						int gIndex = (int)gi;
						//if ((g.sequence == groupId) && (groupIndex != g.sequence) && (!sequence.contains(groupId))) {
						if ((g.sequence == groupId) && (gIndex != g.sequence) && (!sequence.contains(groupId))) {
							sequence.addIfNotAlreadyThere(groupId);
							sequence.sort();
							groupId = (int)gi;
							cont = true;
							break;
						}
					}
				}
				sequence.clear();
				//while ((groupId != -1) && (!sequence.contains(groupId)) && (groupId < groups.size())) {
				while ((groupId != -1) && (!sequence.contains(groupId))) {
					sequence.addIfNotAlreadyThere(groupId);
					sequence.sort();
					groupId = groups[groupId].sequence;
				}
				if (sequence.size() > 1) {
					sequences.add(sequence);
				}
				////
			}
		}
		int getSequencePosition(EXSZone& zone)
		{
			for (int si = 0; si < sequences.size(); ++si) {
				if (sequences[si].contains(zone.group)) {
					return sequences[si].indexOf(zone.group);
				}
			}
			return 0;
			//return -1;
		}
		int getRootNote(EXSZone& zone)
		{
			return zone.getRootNote();
		}
		void updateZoneGroups()
		{
			for (auto it = zones.begin(); it != zones.end(); ++it) {
				// zone group can be -1 but just setting to last group for now
				if ((it->group < 0) && (numGroups > 0)) {
					it->group = numGroups - 1;
				}
			}
		}
		void buildKeyRanges()
		{
			buildSequences();
			////
			keyRangeZoneMap.clear();
			for (auto zone : zones) {
				KeyRange keyRange(zone, getSequencePosition(zone));
				auto search = keyRangeZoneMap.find(keyRange);
				if (search == keyRangeZoneMap.end()) {
					keyRangeZoneMap[keyRange] = {};
				}
				keyRangeZoneMap[keyRange].push_back(zone);
			}
			////
			keySequenceMap.clear();
			int group = 0;
			for (auto it = keyRangeZoneMap.begin(); it != keyRangeZoneMap.end(); ++it) {
				//KeyRange keyRange = it->first;
				group = it->second[0].group;
				for (int si = 0; si < sequences.size(); ++si) {
					if (sequences[si].contains(group)) {
						group = sequences[si][0];
						break;
					}
				}
				keySequenceMap[it->first] = group;
			}
		}
		String buildSfzFile()
		{
			resolveSamples();
			buildKeyRanges();
			////
			MemoryOutputStream sfzFileText;
			sfzFileText << String("// START SFZ FILE\n");
			sfzFileText << String("// SFZ file created from converted EXS instrument file \"") << exsFile.getFileName() << String("\"");
			sfzFileText << String("\n");
			sfzFileText << String("\n");
			for (auto it = keyRangeZoneMap.begin(); it != keyRangeZoneMap.end(); ++it) {
				KeyRange keyRange = it->first;
				sfzFileText << String("<group>");
				////
				//if ((it->second[0].group >= 0) && (it->second[0].group < numGroups)) {
				//	sfzFileText << String(" group_label=") << String(groups[it->second[0].group].name);
				//}
				if ((it->second[0].group >= 0) && (it->second[0].group < numGroups)) {
					sfzFileText << String(" group_label=") << String(it->second[0].group);
				}
				////
				if ((keyRange.startNote == keyRange.endNote) && (keyRange.startNote == keyRange.rootNote)) {
					sfzFileText << String(" key=") << String(keyRange.startNote);
				} else {
					sfzFileText << String(" lokey=") << String(it->second[0].startNote);
					sfzFileText << String(" hikey=") << String(it->second[0].endNote);
					sfzFileText << String(" pitch_keycenter=") << String(it->second[0].rootNote);
				}
				int chokeGroup = 0;
				auto search = keySequenceMap.find(keyRange);
				if (search != keySequenceMap.end()) {
					chokeGroup = keySequenceMap[keyRange];
				} else {
					chokeGroup = it->second[0].group;
				}
				////
				IntByIntRangeMapType chokeVoices;
				//for (auto zit = zones.begin(); zit != zones.end(); ++zit) {
				for (auto zit = it->second.begin(); zit != it->second.end(); ++zit) {
					IntRange velocityRange(zit->minVel, zit->maxVel);
					auto search2 = chokeVoices.find(velocityRange);
					if (search2 != chokeVoices.end()) {
						chokeVoices[velocityRange]++;
					} else {
						chokeVoices[velocityRange] = 1;
					}
				}
				////
				int polyphony = 0;
				IntRange velocityRange(it->second[0].minVel, it->second[0].maxVel);
				auto search3 = chokeVoices.find(velocityRange);
				if (search3 != chokeVoices.end()) {
					polyphony = chokeVoices[velocityRange];
				}
				if ((it->second[0].group >= 0) && (it->second[0].group < numGroups) && (groups[it->second[0].group].polyphony == polyphony)) {
					if (chokeGroup == 0) {
						chokeGroup = numGroups;// - 1;
						//chokeGroup = numGroups - 1;
					}
					sfzFileText << String(" group=") << String(chokeGroup);
					sfzFileText << String(" off_by=") << String(chokeGroup);
					sfzFileText << String(" polyphony=") << String(groups[it->second[0].group].polyphony);
					sfzFileText << String(" off_mode=fast");
				}
				////
				if ((it->second[0].group >= 0) && (it->second[0].group < numGroups) && (groups[it->second[0].group].output > 0)) {
					sfzFileText << String(" output=") << String(groups[it->second[0].group].output);
				}
				////
				if (it->second[0].pan != 0) {
					sfzFileText << String(" pan=") << String(it->second[0].pan);
				}
				////
				if (it->second[0].oneShot) {
					sfzFileText << String(" loop_mode=one_shot");
				}
				////
				for (int si = 0; si < sequences.size(); ++si) {
					if (sequences[si].contains(it->second[0].group)) {
						sfzFileText << String(" seq_length=") << String(sequences[si].size());
						sfzFileText << String(" seq_position=") << String(sequences[si].indexOf(it->second[0].group) + 1);
					}
				}
				////
				sfzFileText << String(" pitch_keytrack=") << String((int)it->second[0].pitchTrack);
				sfzFileText << String("\n");
				////
				////
				for (auto zit = it->second.begin(); zit != it->second.end(); ++zit) {
					sfzFileText << String("\t<region> lovel=") << String(zit->minVel);
					sfzFileText << String(" hivel=") << String(zit->maxVel);
					//sfzFileText << String(" amp_velcurve_") << String(zit->maxVel) << String("=1");
					////
					if (zit->fineTune != 0) {
						sfzFileText << String(" tune=") << String(zit->fineTune);
					}
					////
					if (zit->volumeAdjust != 0) {
						sfzFileText << String(" volume=") << String(zit->volumeAdjust);
					}
					////
					if (zit->sampleStart != 0) {
						sfzFileText << String(" offset=") << String(zit->sampleStart);
					}
					////
					if ((zit->sampleEnd != 0) && (zit->sampleIndex >= 0) && (zit->sampleIndex < numSamples) && (zit->sampleEnd < samples[zit->sampleIndex].length)) {
						sfzFileText << String(" end=") << String(zit->sampleEnd);
					}
					////
					if (zit->loop != 0) {
						sfzFileText << String(" loop_mode=loop_sustain loop_start=") << String(zit->loopStart);
						sfzFileText << String(" loop_end=") << String(zit->loopEnd-1);
					}
					////
					if ((zit->group >= 0) && (zit->group < numGroups) && (groups[zit->group].trigger == 1)) {
						sfzFileText << String(" trigger=release");
					}
					////
					if ((zit->sampleIndex >= 0) && (zit->sampleIndex < numSamples)) {
						sfzFileText << String(" sample=") << samples[zit->sampleIndex].sampleFile.getFullPathName();
					}
					////
					sfzFileText << String("\n");
				}
			}
			sfzFileText << String("\n");
			sfzFileText << String("\n");
			sfzFileText << String("// END SFZ FILE");
			sfzFileText << String("\n");
			return sfzFileText.toString();
		}
		////
		void setSampleSearchDirectory(const File& searchDirectory)
		{
			if ((!searchDirectory.exists()) || (!searchDirectory.isDirectory())) {
				return;
			}
			sampleSearchDirectory = searchDirectory;
			sampleSearchDirectories.addIfNotAlreadyThere(sampleSearchDirectory);
		}
		File getSampleSearchDirectory() { return sampleSearchDirectory; }
		Array<File> getAllSampleSearchDirectories()
		{
			return sampleSearchDirectories;
		}
		void setAllSampleSearchDirectories(const Array<File>& seachDirectories)
		{
			sampleSearchDirectories.clear();
			if ((sampleSearchDirectory.exists()) && (sampleSearchDirectory.isDirectory())) {
				sampleSearchDirectories.addIfNotAlreadyThere(sampleSearchDirectory);
			}
			for (auto dir : seachDirectories) {
				sampleSearchDirectories.addIfNotAlreadyThere(dir);
			}
			if (useDefaultSampleSearchDirectories) {
				for (auto dir : defaultSampleSearchDirectories) {
					sampleSearchDirectories.addIfNotAlreadyThere(dir);
				}
			}
		}
		void setUseDefaultSampleSearchDirectories(bool b)
		{
			useDefaultSampleSearchDirectories = b;
			if (useDefaultSampleSearchDirectories) {
				for (auto dir : defaultSampleSearchDirectories) {
					sampleSearchDirectories.addIfNotAlreadyThere(dir);
				}
			} else {
				for (auto dir : defaultSampleSearchDirectories) {
					sampleSearchDirectories.removeFirstMatchingValue(dir);
				}
			}
		}
		bool getUseDefaultSampleSearchDirectories() { return useDefaultSampleSearchDirectories; }
		////
		void updateSampleSearchDirectories()
		{
			if ((exsFile.getParentDirectory().getChildFile("Samples").exists()) && (exsFile.getParentDirectory().getChildFile("Samples").isDirectory())) {
				sampleSearchDirectories.addIfNotAlreadyThere(exsFile.getParentDirectory().getChildFile("Samples"));
			}
			if ((sampleSearchDirectory.exists()) && (sampleSearchDirectory.isDirectory())) {
				sampleSearchDirectories.addIfNotAlreadyThere(sampleSearchDirectory);
			}
			if (useDefaultSampleSearchDirectories) {
				for (auto dir : defaultSampleSearchDirectories) {
					sampleSearchDirectories.addIfNotAlreadyThere(dir);
				}
			}
		}
		void resolveSamples()
		{
			updateSampleSearchDirectories();
			
			clearSampleFileCache();
			
			bool resolveStatus = false;
			
			//std::freopen(RMB_APP_LOG_PATH, "a", stdout);

			for (auto it = samples.begin(); it != samples.end(); ++it) {
				//std::cout << "EXSInstrument: resolveSample() START \"" << it->name.toStdString() << "\"" << std::endl;
				resolveStatus = resolveSample(*it);
				//if (!resolveStatus) {
				//if (resolveStatus) {
				//	std::cout << "\t\tresolveSample() SUCCESS \"" << it->name.toStdString() << "\"" << std::endl;
				//} else {
				//	std::cout << "\t\tresolveSample() FAIL \"" << it->name.toStdString() << "\"" << std::endl;
				//}
			}
		}
		bool resolveSample(EXSSample& sample)
		{
			String sampleName = sample.name;
			String sampleNameWildcard = String("*") + sample.name;
			String extraInfo = sample.info;
			String pathSeparator = File::getSeparatorString();
			StringArray pathParts;
			String pathPart;
			Array<File> results;
			Array<File> sampleResults;
			File extraInfoFile;
			int resultCount = 0;
			int sampleResultCount = 0;
			int lookForDirectories = (File::TypesOfFileToFind::findDirectories | File::TypesOfFileToFind::ignoreHiddenFiles);
			int lookForFiles = (File::TypesOfFileToFind::findFiles | File::TypesOfFileToFind::ignoreHiddenFiles);
			if (cachedSampleSearchDirectories.size() > 0) {
				for (auto dir : cachedSampleSearchDirectories) {
					results = dir.findChildFiles(lookForFiles, true, sampleNameWildcard);
					resultCount = results.size();
					if (resultCount > 0) {
						sample.sampleFile = results[0];
						resolvedSampleFiles.addIfNotAlreadyThere(results[0]);
						cachedSampleSearchDirectories.addIfNotAlreadyThere(results[0].getParentDirectory());
						cachedSampleSearchDirectories.addIfNotAlreadyThere(results[0].getParentDirectory().getParentDirectory());
						return true;
					}
				}
			}
			if (extraInfo.startsWith(pathSeparator)) {
				extraInfoFile = File(extraInfo);
				if ((extraInfoFile.exists()) && (extraInfoFile.isDirectory())) {
					results = extraInfoFile.findChildFiles(lookForFiles, true, sampleNameWildcard);
					resultCount = results.size();
					if (resultCount > 0) {
						sample.sampleFile = results[0];
						resolvedSampleFiles.addIfNotAlreadyThere(results[0]);
						cachedSampleSearchDirectories.addIfNotAlreadyThere(results[0].getParentDirectory());
						cachedSampleSearchDirectories.addIfNotAlreadyThere(results[0].getParentDirectory().getParentDirectory());
						cachedSampleSearchDirectories.addIfNotAlreadyThere(extraInfoFile);
						return true;
					}
				}
			}
			if ((extraInfo.endsWith(pathSeparator)) || (extraInfo.endsWith(":"))) {
				StringArray extraInfoPathParts;
				if ((extraInfo.startsWith(pathSeparator)) || (extraInfo.endsWith(pathSeparator))) {
					extraInfoPathParts = StringArray::fromTokens(extraInfo, pathSeparator, "");
				} else {
					extraInfoPathParts = StringArray::fromTokens(extraInfo, ":", "");
				}
				extraInfoPathParts.removeEmptyStrings();
				if (extraInfoPathParts.size() > 0) {
					pathPart = extraInfoPathParts[extraInfoPathParts.size()-1];
					resultCount = findFileByName(pathPart, results, lookForDirectories);
					if (resultCount > 0) {
						sampleResults = results[0].findChildFiles(lookForFiles, true, sampleNameWildcard);
						sampleResultCount = sampleResults.size();
						if (sampleResultCount > 0) {
							sample.sampleFile = sampleResults[0];
							resolvedSampleFiles.addIfNotAlreadyThere(sampleResults[0]);
							cachedSampleSearchDirectories.addIfNotAlreadyThere(sampleResults[0].getParentDirectory());
							cachedSampleSearchDirectories.addIfNotAlreadyThere(sampleResults[0].getParentDirectory().getParentDirectory());
							return true;
						}
					}
					////
					if (extraInfoPathParts.size() > 1) {
						pathPart = extraInfoPathParts[extraInfoPathParts.size()-2];
						resultCount = findFileByName(pathPart, results, lookForDirectories);
						if (resultCount > 0) {
							sampleResults = results[0].findChildFiles(lookForFiles, true, sampleNameWildcard);
							sampleResultCount = sampleResults.size();
							if (sampleResultCount > 0) {
								sample.sampleFile = sampleResults[0];
								resolvedSampleFiles.addIfNotAlreadyThere(sampleResults[0]);
								cachedSampleSearchDirectories.addIfNotAlreadyThere(sampleResults[0].getParentDirectory());
								//cachedSampleSearchDirectories.addIfNotAlreadyThere(sampleResults[0].getParentDirectory().getParentDirectory());
								return true;
							}
						}
					}
					////
					if (extraInfoPathParts.size() > 2) {
						pathPart = extraInfoPathParts[extraInfoPathParts.size()-3];
						resultCount = findFileByName(pathPart, results, lookForDirectories);
						if (resultCount > 0) {
							sampleResults = results[0].findChildFiles(lookForFiles, true, sampleNameWildcard);
							sampleResultCount = sampleResults.size();
							if (sampleResultCount > 0) {
								sample.sampleFile = sampleResults[0];
								resolvedSampleFiles.addIfNotAlreadyThere(sampleResults[0]);
								cachedSampleSearchDirectories.addIfNotAlreadyThere(sampleResults[0].getParentDirectory());
								//cachedSampleSearchDirectories.addIfNotAlreadyThere(sampleResults[0].getParentDirectory().getParentDirectory());
								return true;
							}
						}
					}
				}
			}
			if (sampleSearchDirectories.size() > 0) {
				for (auto dir : sampleSearchDirectories) {
					results = dir.findChildFiles(lookForFiles, true, sampleNameWildcard);
					resultCount = results.size();
					if (resultCount > 0) {
						sample.sampleFile = results[0];
						resolvedSampleFiles.addIfNotAlreadyThere(results[0]);
						cachedSampleSearchDirectories.addIfNotAlreadyThere(results[0].getParentDirectory());
						return true;
					}
				}
			}
			return false;
		}
		int findFileByName(const String& f, Array<File>& results, int whatToLookFor)
		{
			int resultCount = 0;
			results.clear();
			for (auto dir : sampleSearchDirectories) {
				results = dir.findChildFiles(whatToLookFor, true, f);
				resultCount = results.size();
				if (resultCount > 0) {
					break;
				}
			}
			return resultCount;
		}
		static StringArray getPathParts(const File& f, const File& root)
		{
			String rootDirPath = root.getFullPathName();
			StringArray rootDirPathParts = StringArray::fromTokens(rootDirPath, File::getSeparatorString(), "");
			rootDirPathParts.removeEmptyStrings();
			StringArray parts;
			File fileDir = f.getParentDirectory();
			String fileDirPath = fileDir.getFullPathName();
			StringArray fileDirPathParts = StringArray::fromTokens(fileDirPath, File::getSeparatorString(), "");
			fileDirPathParts.removeEmptyStrings();
			
			//String partString = fileDirPathParts.joinIntoString(":");
			//std::cout << "getPathParts(): " << partString << std::endl;
			
			for (int i = 0; i < fileDirPathParts.size(); ++i) {
				if (i < rootDirPathParts.size()) {
					if (fileDirPathParts[i] == rootDirPathParts[i]) {
						continue;
					}
				}
				parts.add(fileDirPathParts[i]);
			}
			
			//partString = parts.joinIntoString(":");
			//std::cout << "\t\tparts(): " << partString << std::endl;
			
			return parts;
		}
		////
	public:
		File exsFile {};
		int exsFileSize { 0 };
		int exsDataSize { 0 };
		String exsInstrumentName {};
		EXSHeaderVectorType headers;
		EXSChunkVectorType chunks;
		EXSZoneVectorType zones;
		EXSGroupVectorType groups;
		EXSSampleVectorType samples;
		EXSParamVectorType params;
		////
		int numHeaders { 0 };
		int numChunks { 0 };
		int numZones { 0 };
		int numGroups { 0 };
		int numSamples { 0 };
		int numParams { 0 };
		////
		Array<Array<int>> sequences;
		EXSZoneVectorByKeyRangeMapType keyRangeZoneMap;
		std::map<KeyRange, int> keySequenceMap;
		////
		File sampleSearchDirectory {};
		bool useDefaultSampleSearchDirectories { true };
		Array<File> sampleSearchDirectories {};
		Array<File> cachedSampleSearchDirectories {};
		Array<File> resolvedSampleFiles {};
		String errorString {};
		String exsParseDebugString {};
		////
		//static Array<File> cachedSampleSearchDirectories;
		//static Array<File> resolvedSampleFiles;
		static const int exsFileSizeMax { 1024 * 1024 };
		static const Array<File> defaultSampleSearchDirectories;
	};
	////
}

////
////


#endif /* EXStoSFZ_hpp */

//// END

