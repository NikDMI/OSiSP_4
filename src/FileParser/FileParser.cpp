#include "FileParser.h"
#include <fstream>
#include <vector>
#include "../Config.h"
#include "../ThreadPool/ThreadPool.h"
#include <algorithm>

namespace LAB {
	using namespace std;

	void SortFunctionCall(void* str);
	wstring ConvertUtf8ToWstring(char* buffer, int bufLen);
	char* ConvertWstringToUTF8(wstring str, int* bufSize);
	vector<wstring> SliceText(wstring text);
	vector<wstring> SliceLine(const wstring& sortedString);
	bool IsSpace(wchar_t ch);
	wstring MergeText(const vector<vector<wstring>>& textLines);
	static LONG _globalCounter;

	void SortTextFile(std::wstring fileName, std::wstring outFileName) {
		ifstream iFile;
		iFile.open(fileName, ios_base::in);
		if (!iFile) throw Exception(L"Can't open file " + fileName);
		ofstream oFile;
		oFile.open(outFileName, ios_base::out | ios_base::trunc);
		if (!oFile) {
			iFile.close();
			throw Exception(L"Can't create file " + outFileName);
		}
		iFile.seekg(0, ios_base::_Seekend);
		int fileLength = iFile.tellg();
		iFile.seekg(0, ios_base::_Seekbeg);
		unsigned char* buffer = new unsigned char[fileLength];
		iFile.read((char*)buffer, fileLength);
		if (iFile.bad()) {
			delete[] buffer;
			iFile.close();
			oFile.close();
			throw Exception(L"Troubles while reading  " + outFileName);
		}
		iFile.close();
		//Convert from UTF8 to WCHAR
		auto textData = ConvertUtf8ToWstring((char*)buffer, fileLength);
		vector<wstring> slicedText = SliceText(textData);
		vector<vector<wstring>> slicedLines;
		for (int i = 0; i < slicedText.size(); ++i) {
			slicedLines.push_back(SliceLine(slicedText[i]));
		}
		_globalCounter = 0;
		LONG numStrings = slicedText.size();
		ThreadPool threadPool;
		for (int i = 0; i < numStrings; ++i) {
			threadPool.QueueUserWorkItem(SortFunctionCall, &slicedLines[i], ThreadPool::NORMAL_THREAD);
		}
		while (InterlockedCompareExchange(&_globalCounter, numStrings, numStrings) != numStrings) {
			Sleep(400);
		}
		wstring resultText = MergeText(slicedLines);
		int dataLength;
		char* utf8Data = ConvertWstringToUTF8(resultText, &dataLength);
		oFile.write(utf8Data, dataLength);
		if (oFile.fail()) {
			oFile.close();
			throw Exception(L"Can't write output data");
		}
	}


	wstring ConvertUtf8ToWstring(char* buffer, int length) {
		int textSize = MultiByteToWideChar(CP_UTF8, 0, (char*)buffer, length, NULL, 0);
		std::wstring textData; textData.resize(textSize);
		textSize = MultiByteToWideChar(CP_UTF8, 0, (char*)buffer, length, &textData[0], textData.capacity());
		if (textSize == 0) {
			throw Exception(L"File contains invalid data");
		}
		return textData;
	}

	char* ConvertWstringToUTF8(wstring str, int* bufSize) {
		int dataLength = WideCharToMultiByte(CP_UTF8, 0, str.c_str(), str.size(), NULL, 0, NULL, NULL);
		char* data = new char[dataLength];
		dataLength = WideCharToMultiByte(CP_UTF8, 0, str.c_str(), str.size(), data, dataLength, NULL, NULL);
		if (dataLength == 0) {
			delete[] data;
			throw Exception(L"Can't create table data");
		}
		*bufSize = dataLength;
		return data;
	}

	bool IsSpace(wchar_t ch) {
		if (ch == L' ' || ch == '\n' || ch == L'\r') {
			return true;
		}
		return false;
	}


	vector<wstring> SliceText(wstring text) {
		vector<wstring> slicedStrings;
		int iStart = 0;
		int iEnd = 0;
		int lenght = text.length();
		while (iEnd != lenght) {
			if (text[iEnd] == L'\n') {
				slicedStrings.push_back(text.substr(iStart, iEnd - iStart));
				iStart = iEnd + 1;
			}
			iEnd++;
		}
		if (iStart < iEnd) slicedStrings.push_back(text.substr(iStart, iEnd - iStart));
		return slicedStrings;
	}


	vector<wstring> SliceLine(const wstring& sortedString) {
		vector<wstring> words;
		int currentIndex = 0;
		int textLength = sortedString.size();
		while (currentIndex < textLength) {
			wchar_t ch;
			while (IsSpace(sortedString[currentIndex]) && currentIndex < textLength) {
				++currentIndex;
			}
			wstring addString;
			while (!IsSpace((ch = sortedString[currentIndex])) && currentIndex < textLength) {
				addString.push_back(ch);
				++currentIndex;
			}
			words.push_back(addString);
		}
		return words;
	}


	wstring MergeText(const vector<vector<wstring>>& textLines) {
		wstring resultString;
		int totalWordsCount = 0;
		vector<pair<vector<wstring>::const_iterator, vector<wstring>::const_iterator>> iterators;
		for (int i = 0; i < textLines.size(); ++i) {
			iterators.push_back({ textLines[i].cbegin(), textLines[i].cend() });
			totalWordsCount += textLines[i].size();
		}
		int addedWords = 0;
		wstring nullWord = L"";
		while (addedWords < totalWordsCount) {
			const wstring* nextWord = &nullWord;
			pair<vector<wstring>::const_iterator, vector<wstring>::const_iterator>* choosedIterLine = nullptr;
			for (int i = 0; i < iterators.size(); ++i) {
				if (iterators[i].first != iterators[i].second && ((*iterators[i].first < *nextWord) || nextWord == &nullWord)) {
					choosedIterLine = &iterators[i];
					nextWord = &(*(iterators[i].first));
				}
			}
			++(*choosedIterLine).first;
			resultString += *nextWord + L" ";
			++addedWords;
		}
		return resultString;
	}


	void SortFunctionCall(void* vec) {
		vector<wstring>* sortedString = (vector<wstring>*)vec;
		sort(sortedString->begin(), sortedString->end(), [](const wstring& str1, const wstring str2) {
			return str1 < str2;
			});
		InterlockedAdd(&_globalCounter, 1);
	}
}
