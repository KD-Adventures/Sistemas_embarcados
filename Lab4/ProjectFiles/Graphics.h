#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "cmsis_os.h"
#include <stdbool.h>
#include "grlib/grlib.h"
#include <stdlib.h>
#include "Colors.h"
#include "cfaf128x128x16.h"
#include "Comunication.h"
#include "Terminal.h"

typedef struct InterfaceMenu {
	struct Groups* groups;
	struct ColorGroup* currentColorGroup;
	struct Color* currentColor;
	uint16_t numberOfEntries;
	uint16_t currentEntrySelected;
} InterfaceMenu ;

enum ENTRY_NAME {
	COLOR_GROUP_ENTRY = 0,
	COLOR_ENTRY,
};

InterfaceMenu initMenu(Groups* groups);
void nextEntry(InterfaceMenu  *menu);
void previousEntry(InterfaceMenu  *menu);
void setColorGroup(InterfaceMenu  *menu, ColorGroup* group);
void setColor(InterfaceMenu  *menu, Color* color);
void previousValue(InterfaceMenu  *menu);
void previousValue(InterfaceMenu  *menu);
void nextValue(InterfaceMenu  *menu);

void drawConsoleMenu(InterfaceMenu* menu);
void drawDisplayMenu(InterfaceMenu* menu, tContext* context);

#endif