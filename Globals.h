#pragma once

enum EAIBC2HaxCellType
{
	Goal,
	Forbidden,
	Default
};

enum EHexCellDirection
{
	W, NW, NE, E, SE, SW
};

struct SMoveOrder
{
	int npcUID;
	EHexCellDirection direction;
};

struct STileInfo
{
	int q;
	int r;
	EAIBC2HaxCellType type;
};

struct SNPCInfo
{
	int uid;
	int q;
	int r;
};
