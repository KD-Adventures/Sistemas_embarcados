#include "Graphics.h"


InterfaceMenu initMenu(Groups* groups) {
	InterfaceMenu new_menu;
	
	assert(groups != NULL);
	
	new_menu.groups = groups;
	new_menu.currentEntrySelected = COLOR_GROUP_ENTRY;
	new_menu.numberOfEntries = 2;
	new_menu.currentColorGroup = &(groups->colorGroup[groups->currentGroupIndex]);
	new_menu.currentColor = &(new_menu.currentColorGroup->color[new_menu.currentColorGroup->currentColorIndex]);
	
	return new_menu;
}


void nextEntry(InterfaceMenu  *menu) {
	uint16_t last_entry_index;
	assert(menu != NULL);
	
	last_entry_index = menu->numberOfEntries - 1;
	
	if (menu->currentEntrySelected < last_entry_index) {
		menu->currentEntrySelected++;
	}
	
	return;
}

void previousEntry(InterfaceMenu  *menu) {
	assert(menu != NULL);
	
	if (menu->currentEntrySelected > 0) {
		menu->currentEntrySelected--;
	}
	
	return;
}

void setColorGroup(InterfaceMenu  *menu, ColorGroup* group) {
	assert(group != NULL);
	
	menu->currentColorGroup = group;
	menu->currentColor = menu->currentColorGroup->color;
}

void setColor(InterfaceMenu  *menu, Color* color) {
	assert(color != NULL);
	
	menu->currentColor = color;
}

void previousValue(InterfaceMenu  *menu) {
	assert(menu != NULL);
	
	switch (menu->currentEntrySelected) {
		case COLOR_GROUP_ENTRY:
			setColorGroup(menu, previousColorGroup(menu->groups));
			break;
		case COLOR_ENTRY:
			setColor(menu, previousColor(menu->currentColorGroup));
			break;
	}
}

void nextValue(InterfaceMenu  *menu) {
	assert(menu != NULL);
	
	switch (menu->currentEntrySelected) {
		case COLOR_GROUP_ENTRY:
			setColorGroup(menu, nextColorGroup(menu->groups));
			break;
		case COLOR_ENTRY:
			setColor(menu, nextColor(menu->currentColorGroup));
			break;
	}
}

void drawConsoleMenu(InterfaceMenu* menu) {
	terminal_clear();
	goto_xy(5, 0);
	sendString("Escolha o Grupo");
	goto_xy(5, 2);
	sendString(menu->currentColorGroup->name);
	goto_xy(5, 4);
	sendString("Escolha a Cor");
	goto_xy(5, 5);
	sendString(menu->currentColor->name);
	
	switch (menu->currentEntrySelected) {
		case COLOR_GROUP_ENTRY:
			goto_xy(0, 2);
			background_color(0xffffff);
			sendString("  ");
			break;
		case COLOR_ENTRY:
			goto_xy(0, 5);
			background_color(0xffffff);
			sendString("  ");
			break;
	}
	
	draw_square(0, 20, 10, 20, menu->currentColor->rgb);
}

void drawDisplayMenu(InterfaceMenu* menu, tContext* context) {
	tRectangle rect;
	
	GrContextBackgroundSet(context, ClrBlack);	
	GrContextForegroundSet(context, ClrBlack);	
	// nojeira
	GrStringDraw(context, "                    ", -1, (context->psFont->ui8MaxWidth)*4,  (context->psFont->ui8Height+2)*2, true);
	GrStringDraw(context, "                    ", -1, (context->psFont->ui8MaxWidth)*4,  (context->psFont->ui8Height+2)*5, true);
	
	GrContextForegroundSet(context, ClrWhite);	
	
	GrStringDraw(context, "Escolha o Grupo", -1, (context->psFont->ui8MaxWidth)*4,  (context->psFont->ui8Height+2)*1, true);
	GrStringDraw(context, menu->currentColorGroup->name, -1, (context->psFont->ui8MaxWidth)*4,  (context->psFont->ui8Height+2)*2, true);
	
	GrStringDraw(context, "Escolha a Cor", -1, (context->psFont->ui8MaxWidth)*4,  (context->psFont->ui8Height+2)*4, true);
	GrStringDraw(context, menu->currentColor->name, -1, (context->psFont->ui8MaxWidth)*4,  (context->psFont->ui8Height+2)*5, true);
	
	// Nojeira
	GrContextBackgroundSet(context, ClrBlack);
	GrStringDraw(context, "  ", -1, (context->psFont->ui8MaxWidth)*0,  (context->psFont->ui8Height+2)*2, true);
	GrStringDraw(context, "  ", -1, (context->psFont->ui8MaxWidth)*0,  (context->psFont->ui8Height+2)*5, true);
	GrContextBackgroundSet(context, ClrWhite);
	
	switch (menu->currentEntrySelected) {
		case COLOR_GROUP_ENTRY:
			GrStringDraw(context, " ", -1, (context->psFont->ui8MaxWidth)*0,  (context->psFont->ui8Height+2)*2, true);
			break;
		case COLOR_ENTRY:
			GrStringDraw(context, " ", -1, (context->psFont->ui8MaxWidth)*0,  (context->psFont->ui8Height+2)*5, true);
			break;
	}
	
	GrContextBackgroundSet(context, menu->currentColor->rgb);
	GrContextForegroundSet(context, menu->currentColor->rgb);
	rect.i16XMin = 0;
	rect.i16XMax = 128;
	rect.i16YMin = 70;
	rect.i16YMax = 128;
	GrRectFill(context, &rect);
}