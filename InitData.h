#pragma once

struct STileInfo;

struct SInitData
{
	int initDelay; //in ms

	int rowCount;
	int colCount;

	int turnDelay; //in ms
	int maxTurnNb;
	int nbNPCs;

	STileInfo* tileInfoArray;
	int tileInfoArraySize;
};
