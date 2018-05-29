#include "Colors.h"

const ColorGroup blue_group_colors = {
	"Blue group", 
	{
		{"Slate Blue", 0x6A5ACD},
		{"Dark Slate Blue", 0x483D8B}
	},
	2,
	0
};

const ColorGroup green_group_colors = {
	"Green group", 
	{
		{"Dark Slate Green", 0x2f4f4f},
		{"Medium Spring Green", 0x00FA9A},
	},
	2,
	0
};

Groups loadGroups() {
	//Groups *new_group = (Groups*) malloc (sizeof(Groups));
	struct Groups groups;
	ColorGroup* new_color_group = (ColorGroup*)malloc(sizeof(ColorGroup)*2);
	
	//groups.colorGroup[BLUE_GROUP] = (const ColorGroup*)blue_group_colors;
	//groups.colorGroup[GREEN_GROUP] = green_group_colors;
	
	//groups.colorGroup = new_color_group;
	
	new_color_group[BLUE_GROUP] = blue_group_colors;
	new_color_group[GREEN_GROUP] = green_group_colors;
	
	groups.colorGroup = new_color_group;
	
	groups.currentGroupIndex = 0;
	groups.numberOfGroups = 2;
	
	return groups;
}

Color* nextColor(ColorGroup* group) {
	assert(group != NULL);
	
	group->currentColorIndex++;
	if (group->currentColorIndex == group->numberOfColors) {
		group->currentColorIndex = 0;
	}
	
	return &(group->color[group->currentColorIndex]);
}

Color* previousColor(ColorGroup* group) {
	uint16_t last_color_index;
	assert(group != NULL);
	
	if (group->currentColorIndex == 0) {
		group->currentColorIndex = group->numberOfColors - 1;
	}
	else {
		group->currentColorIndex--;
	}
	
	return &(group->color[group->currentColorIndex]);
}

ColorGroup* nextColorGroup(Groups* groups) {
	
	uint16_t last_color_index;
	assert(groups != NULL);
	
	groups->currentGroupIndex++;
	if (groups->currentGroupIndex == groups->numberOfGroups) {
		groups->currentGroupIndex = 0;
	}
	
	return &(groups->colorGroup[groups->currentGroupIndex]);
}

ColorGroup* previousColorGroup(Groups* groups) {
	
	uint16_t last_color_index;
	assert(groups != NULL);
	
	if (groups->currentGroupIndex == 0) {
		groups->currentGroupIndex = groups->numberOfGroups - 1;
	}
	else {
		groups->currentGroupIndex--;
	}
	
	return &(groups->colorGroup[groups->currentGroupIndex]);
}