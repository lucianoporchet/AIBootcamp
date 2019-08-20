#pragma once

enum EHexCellType
{
	Goal,
	Forbidden,
	Default
};

enum EHexCellDirection
{
	W, NW, NE, E, SE, SW, CENTER
};

enum EObjectType
{
	Wall,
	Window,
	Door,
	PressurePlate
};

enum EObjectState
{
	Opened,
	Closed
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
	EHexCellType type;
};

struct SObjectInfo
{
	int uid;
	int q;
	int r;
	EHexCellDirection cellPosition;

	int* types;
	int typesSize;
};

struct SNPCInfo
{
	int uid;
	int q;
	int r;
};
