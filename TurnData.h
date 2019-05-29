#pragma once

struct SNPCInfo;

struct STurnData
{
	int turnNb;

	SNPCInfo* npcInfoArray;
	int npcInfoArraySize;

	int turnDelay; //in ms
};
