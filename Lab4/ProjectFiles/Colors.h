#ifndef COLORS_H
#define COLORS_H

#include "cmsis_os.h"
#include "TM4C129.h"
#include <stdbool.h>
#include "grlib/grlib.h"
#include <stdlib.h>
#include <assert.h>


typedef struct Color {
	char name[50];
	uint32_t rgb;
} Color;

typedef struct ColorGroup {
	char name[50];
	struct Color color[50];
	uint16_t numberOfColors;
	uint16_t currentColorIndex;
} ColorGroup;

typedef struct Groups {
	struct ColorGroup* colorGroup;
	uint16_t numberOfGroups;
	uint16_t currentGroupIndex;
} Groups;

enum COLOR_GROUPS {
	BLUE_GROUP = 0,
	GREEN_GROUP
};

Groups loadGroups();

Color* nextColor(ColorGroup* group);
Color* previousColor(ColorGroup* group);
ColorGroup* nextColorGroup(Groups* groups);
ColorGroup* previousColorGroup(Groups* groups);

#endif