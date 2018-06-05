#include "Colors.h"

const ColorGroup blue_group_colors = {
	"Blue group", 
	{
		{"Slate Blue", 0x6A5ACD},
		{"Dark Slate Blue", 0x483D8B},
		{"Midnight Blue", 0x191970},
		{"Navy", 0x000080},
		{"Dark Blue", 0x00008B},
		{"Medium Blue", 0x0000CD},
		{"Blue", 0x0000FF},
		{"Corn Flower Blue", 0x6495ed},
		{"Royal Blue", 0x4169E1},
		{"Dodger Blue", 0x1E90FF},
		{"Deep Sky Blue", 0x00BFFF},
		{"Light Sky Blue", 0x87CEFA},
		{"Sky Blue", 0x87CEEB},
		{"Light Blue", 0xADD8E6},
		{"Steel Blue", 0x4682B4},
		{"Light Steel Blue", 0xB0C4DE},
		{"Slate Gray", 0x708090},
		{"Light Slate Gray", 0x778899}
	},
	18,
	0
};

const ColorGroup green_group_colors = {
	"Green group", 
	{
		{"Dark Slate Green", 0x2f4f4f},
		{"Medium Spring Green", 0x00FA9A},
		{"Spring Green", 0x00FF7F},
		{"Pale Green", 0x98F898},
		{"Light Green", 0x90EE90},
		{"Dark Sea Green", 0x8FBC8F},
		{"Medium Sea Green", 0x3CB371},
		{"Sea Green", 0x2E8B57},
		{"Dark Green", 0x006400},
		{"Green", 0x008000},
		{"Forest Green", 0x228B22},
		{"Lime Green", 0x32CD32},
		{"Lime", 0x00FF00},
		{"Lawn Green", 0x7CFC00},
		{"Cartreuse", 0x7FFF00},
		{"Green Yello", 0xADFF2F},
		{"Yellow Green", 0x9ACD32},
		{"Olive Drab", 0x6B8E23},
		{"Dark Olive Green", 0x556B2F},
		{"Olive", 0x808000}
	},
	20,
	0
};

const ColorGroup brown_group_colors = {
	"Brown group", 
	{
		{"Dark Khaki", 0xBDB76B},
		{"Goldenrod", 0xDAA520},
		{"Dark Golden Rod", 0xB8860B},
		{"Saddle Brown", 0x8B4513},
		{"Sienna", 0xA0522D},
		{"Rosy Brown", 0xBC8F8F},
		{"Peru", 0xCD853F},
		{"Chocolate", 0xD2691E},
		{"Sandy Brown", 0xF4A460},
		{"Navajo White", 0xFFDEAD},
		{"Wheat", 0xF5DEB3},
		{"Burly Wood", 0xDEB887},
		{"Tan", 0xD2B48C}
	},
	13,
	0
};

const ColorGroup purple_group_colors = {
	"Purple group", 
	{
		{"Medium Slate Blue", 0x7B68EE},
		{"Medium Purple",0x9370DB},
		{"Blue Violet",0x8A2BE2},
		{"Indigo", 0x4B0082},
		{"Dark Violet",0x9400D3},
		{"Dark Orchid",0x9932CC},
		{"Medium Orchid", 0xBA55D3},
		{"Purple",0x800080},
		{"Dark Magenta",0x8B008B},
		{"Fuchsia", 0xFF00FF},
		{"Violet",0xEE82EE},
		{"Orchid",0xDA70D6},
		{"Plum", 0xDDA0DD}
	},
	13,
	0
};

const ColorGroup cyan_group_colors = {
	"Cyan group", 
	{
		{"Aqua/Cyan", 0x00FFFF},
		{"Dark Turquoise",0x00CED1},
		{"Turquoise",0x40E0D0},
		{"Medium Turquoise", 0x48D1CC},
		{"Light Sea Green",0x20B2AA},
		{"Dark Cyan",0x008B8B},
		{"Teal", 0x008080},
		{"Aquamarine",0x7FFFD4},
		{"Medium Aquamarine",0x66CDAA},
		{"Cadet Blue", 0x5F9EA0}
	},
	10,
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
		{"Pale Violet Red", 0xDB7093},
		{"Light Pink",0xFFB6C1},
		{"Pink",0xFFC0CB},
		{"Light Coral", 0xF08080},
		{"Indian Red",0xCD5C5C},
		{"Crimson",0xDC143C},
	},
	9,
	0
};

const ColorGroup red_group_colors = {
	"Red group", 
	{
		{"Maroon", 0x800000},
		{"Dark Red",0x8b0000},
		{"Fire Brick", 0x822222},
		{"Brown",0xA52A2A},
		{"Salmon", 0xFA8072},
		{"Dark Salmo",0xE9967A},
		{"Light Salmon", 0xFFA07A},
		{"Coral",0xFF7F50},
		{"Tomato", 0xFF6347},
		{"Red",0xFF0000},
	},
	10,
	0
};



Groups loadGroups() {
	struct Groups groups;
	ColorGroup* new_color_group;
	
	groups.numberOfGroups = 9;
	
	new_color_group = (ColorGroup*)malloc(sizeof(ColorGroup)*groups.numberOfGroups);
	new_color_group[BLUE_GROUP] = blue_group_colors;
	new_color_group[GREEN_GROUP] = green_group_colors;
	new_color_group[BROWN_GROUP] = brown_group_colors;
	new_color_group[PURPLE_GROUP] = purple_group_colors;
	new_color_group[CYAN_GROUP] = cyan_group_colors;
	new_color_group[ORANGE_GROUP] = orange_group_colors;
	new_color_group[YELLOW_GROUP] = yellow_group_colors;
	new_color_group[PINK_GROUP] = pink_group_colors;
	new_color_group[RED_GROUP] = red_group_colors;
	
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