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

const ColorGroup brown_group_colors = {
	"Brown group", 
	{
		{"Dark Khaki", 0xbdb76b},
		{"Goldenrod", 0xdaa520},
	},
	2,
	0
};

const ColorGroup purple_group_colors = {
	"Purple group", 
	{
		{"Medium Slate Blue", 0x7B68EE},
		{"Medium Purple",0x9370DB},
		{"Blue Violet",0x8A2BE2},
	},
	3,
	0
};

const ColorGroup cyan_group_colors = {
	"Cyan group", 
	{
		{"Aqua/Cyan", 0x00FFFF},
		{"Dark Turquoise",0x00CED1},
		{"Turquoise",0x40E0D0},
	},
	3,
	0
};

const ColorGroup orange_group_colors = {
	"Orange group", 
	{
		{"Orange Red", 0xFF4500},
		{"Dark Orange",0xFF8C00},
		{"Orange",0xFFA500},
	},
	3,
	0
};

const ColorGroup yellow_group_colors = {
	"Yellow group", 
	{
		{"Gold", 0xFFD700},
		{"Yellow",0xFFFF00},
		{"Khakl",0xF0E68C},
	},
	3,
	0
};

const ColorGroup pink_group_colors = {
	"Pink group", 
	{
		{"Medium Violet Red", 0xC71585},
		{"Deep Pink",0xFF1193},
		{"Hot Pink",0xFf69B4},
	},
	3,
	0
};

const ColorGroup red_group_colors = {
	"Red group", 
	{
		{"Maroon", 0x800000},
		{"Dark Red",0x8b0000},
	},
	2,
	0
};



Groups loadGroups() {
	struct Groups groups;
	ColorGroup* new_color_group;
	
	groups.numberOfGroups = 4;
	
	new_color_group = (ColorGroup*)malloc(sizeof(ColorGroup)*groups.numberOfGroups);
	new_color_group[BLUE_GROUP] = blue_group_colors;
	new_color_group[GREEN_GROUP] = green_group_colors;
	new_color_group[BROWN_GROUP] = brown_group_colors;
	new_color_group[PURPLE_GROUP] = purple_group_colors;
	//new_color_group[CYAN_GROUP] = cyan_group_colors;
	//new_color_group[ORANGE_GROUP] = orange_group_colors;
	//new_color_group[YELLOW_GROUP] = yellow_group_colors;
	//new_color_group[PINK_GROUP] = pink_group_colors;
	//new_color_group[RED_GROUP] = red_group_colors;
	
	groups.colorGroup = new_color_group;
	
	groups.currentGroupIndex = 0;
	
	
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