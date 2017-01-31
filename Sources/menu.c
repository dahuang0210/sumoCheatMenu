#include "cheats.h"

char	*builder_name = "AnalogMan",
        version[7] = "v0.4.2",
        formattedVer[23];

int gameVer = -1;

void getVersion(void) {
    protect_region(0x00100000);
    u32 game_check = READU8(0x00100040);
    switch(gamecheck) {
        case 0x30:
            gameVer = 0;
            break;
        case 0x78:
            gameVer = 1;
            break;
    }
}

void    disableOnlineCheats(void) {
    if(getWifiStatus()) {
        disable_entry(MAXBATTLESTATS);
        disable_entry(ZMOVES);
        disable_entry(INFZMOVES);
        disable_entry(SHOWOPPONENTINFO);
    }
}


void    always_run(void) {
    execute_all();
    generateSpawn();
    setRandomID();
    shinyPokemon();
    disableOnlineCheats();
}


void	my_menus(void) {
    getVersion();
    if (gameVer == -1) {
        new_unselectable_entry("Patch version not supported");
        return;
    }
    xsprintf(formattedVer, "%22s", version);
    new_unselectable_entry("Entries w/ an orange background");
    new_unselectable_entry("have notes. Press (Y) to view.");
    new_super_unselectable_entry(formattedVer, always_run);
	new_separator();
    expMenu();
    new_spoiler("Modifiers");
       timeMenu();
       appearanceMenu();
       battleMenu();
       movementMenu();
       pokemonMenu();
    exit_spoiler();
    itemMenu();
    currencyMenu();
    miscMenu();
    new_line();
    new_line();
    new_separator();
    new_unselectable_entry("WARNING: The codes on the next");
    new_unselectable_entry("page may create Pokemon that");
    new_unselectable_entry("cannot be battled or traded");
    new_separator();
    pokemonSpawnMenu();
    illegalMenu();
}
