// Filename: battle_modifiers.c

#include "cheats.h"
#include "hid.h"

/********************************
 *                              *
 *      Battle Modifiers        *
 *                              *
 ********************************/

int shinyChanceValue = 4096;

// Battle menu entry
void    battleMenu(void) {

    // Creates spoiler and cheat entries

    new_spoiler("Battle");
        new_entry_managed_note("No Wild Encounters", "Hold START to temporarily enable encounters", noEncounters, NOENCOUNTERS, 0);
        new_entry("100% Capture Rate", catch100);
        new_entry_managed("Shiny Chance: XXXXXX", decreaseShinyChance, DECREASESHINYCHANCE, AUTO_DISABLE);
        new_entry_managed_note("View Opponent's Info", "Tap Opponent's icon on battle screen to see HP, Ability & Held Item", showOpponentInfo, SHOWOPPONENTINFO, 0);
        // new_entry("Always Critical Hit", alwaysCritical);
        new_entry_arg("Stat Stages +6", maxBattleStats, 0, MAXBATTLESTATS, TOGGLE);
        new_entry_arg("Use Z-Moves w/o Z-Crystal", zMoves, 0, ZMOVES, TOGGLE);
        new_entry_managed("Infinite Z-Moves", infZMoves, INFZMOVES, 0);
        new_line();
    exit_spoiler();
    updateShiny();
}

// No wild encounters unless START is held
void    noEncounters(void) {
    u32 offset[] =
    {
        0x0807A28C,
        0x0807A5E8,
    };
    if (!checkAddress(offset[gameVer]))
        return;
    if (READU32(offset[gameVer]) == 0xE3A00064 || READU32(offset[gameVer]) == 0xE3A09000) {
        if (is_pressed(BUTTON_ST))
            WRITEU32(offset[gameVer], 0xE3A00064);
        else
            WRITEU32(offset[gameVer], 0xE3A09000);
    }
}


// Always Critical Hit
void    alwaysCritical(void) {
    u32 offset[][2] =
    {
        {0x00595AD0, 0x08085D1C},
        {0x005979D0, 0x080860D8},
    };
    u32 data[] = {0xEBF9B36B, 0xEBF9B23C};

    static const u8    buffer[] =
    {
        0x00, 0x00, 0x9D, 0xE5, 0x1E, 0x40, 0x2D, 0xE9,
        0x08, 0x10, 0x9A, 0xE5, 0x04, 0x30, 0x81, 0xE2,
        0x1C, 0x40, 0x81, 0xE2, 0x04, 0x20, 0x93, 0xE4,
        0x02, 0x00, 0x50, 0xE1, 0x01, 0x10, 0xA0, 0x03,
        0x18, 0x10, 0x8D, 0x05, 0x04, 0x00, 0x53, 0xE1,
        0xF9, 0xFF, 0xFF, 0x1A, 0x1E, 0x80, 0xBD, 0xE8
    };
    memcpy((void *)(offset[gameVer][0]), buffer, 0x30);
    if (!checkAddress(offset[gameVer][1]))
        return;
    if (READU32(offset[gameVer][1]) == 0xE59D0000);
        WRITEU32(offset[gameVer][1], data[gameVer]);
}


// Shows opponent Pokémon's info during battle on bottom screen when icon is tapped
void    showOpponentInfo(void) {
    u32 offset[] =
    {
        0x080AE178,
        0x080AE5F8,
    };
    u32 data = 0xEAFFFEE7;
    if (!checkAddress(offset[gameVer]))
        return;
    if (READU32(offset[gameVer]) == 0xE92D40F8)
        WRITEU32(offset[gameVer], data);
}


// Sets all in-battle stats to +6 ranks
void    maxBattleStats(u32 state) {
    u32 offset[][2] =
    {
        {0x00595A00, 0x0029A048},
        {0x00597900, 0x0029A168},
    };

    u32 data[] =
    {
        0xEA0BEE6C,
        0xEA0BF5E4,
    };
    static u32  original;

    // Checks if cheat is enabled from menu and executes if it is
    if (state) {

        // Stores original value in memory
        original = READU32(offset[gameVer][1]);

        static const u8    buffer[] =
        {
            0x80, 0x20, 0x9F, 0xE5, 0x0E, 0x00, 0x52, 0xE1,
            0x1D, 0x00, 0x00, 0x1A, 0x1F, 0x00, 0x2D, 0xE9,
            0x74, 0x20, 0x9F, 0xE5, 0xD0, 0x00, 0x94, 0xE5,
            0x04, 0x30, 0x80, 0xE2, 0x1C, 0x40, 0x80, 0xE2,
            0x04, 0x10, 0x93, 0xE4, 0x00, 0x00, 0x51, 0xE3,
            0xEA, 0x21, 0xC1, 0x15, 0xEB, 0x21, 0xC1, 0x15,
            0xEC, 0x21, 0x81, 0x15, 0xF0, 0x21, 0xC1, 0x15,
            0x04, 0x00, 0x53, 0xE1, 0xF7, 0xFF, 0xFF, 0x1A,
            0x2C, 0x00, 0x9D, 0xE5, 0x88, 0x0A, 0x90, 0xE5,
            0x01, 0x0A, 0x80, 0xE2, 0x08, 0x08, 0x90, 0xE5,
            0x0C, 0x00, 0x90, 0xE5, 0x08, 0x00, 0x90, 0xE5,
            0x04, 0x30, 0x80, 0xE2, 0x1C, 0x40, 0x80, 0xE2,
            0x04, 0x10, 0x93, 0xE4, 0x00, 0x00, 0x51, 0xE3,
            0xEA, 0x21, 0xC1, 0x15, 0xEB, 0x21, 0xC1, 0x15,
            0xEC, 0x21, 0x81, 0x15, 0xF0, 0x21, 0xC1, 0x15,
            0x04, 0x00, 0x53, 0xE1, 0xF7, 0xFF, 0xFF, 0x1A,
            0x1F, 0x00, 0xBD, 0xE8, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x6E, 0x00, 0x0C, 0x0C, 0x0C, 0x0C
        };
        static const u8    buffer2[][6] =
        {
            {0x70, 0x11, 0xF4, 0xEA, 0xA4, 0x77},
            {0xF8, 0x09, 0xF4, 0xEA, 0x80, 0x96},
        };

        memcpy((void *)(offset[gameVer][0]), buffer, 0x90);
        memcpy((void *)(offset[gameVer][0] + 0x84), buffer2[gameVer], 0x06);
        WRITEU32(offset[gameVer][1], data[gameVer]);
    } else {

        // Sets value back to original when cheat is disabled
        WRITEU32(offset[gameVer][1], original);
    }
}


// 100% Catch rate for Pokemon
void    catch100(void) {
    u32 offset[] =
    {
        0x0803528C,
        0x080350BC,
    };
    u32 data = 0xEA000004;
    if (!checkAddress(offset[gameVer]))
        return;
    if (READU32(offset[gameVer]) == 0x0A000004)
        WRITEU32(offset[gameVer], data);
}


// Updates menu with current shiny rate
void    updateShiny(void) {
    char buf[7];
    xsprintf(buf, ": 1/%-4d", shinyChanceValue);
    replace_pattern(": ******", (shinyChanceValue == 4096) ? ": Normal" : buf, DECREASESHINYCHANCE);
}


// Cycles denominator for shiny chance
void    decreaseShinyChance(void) {
    if (shinyChanceValue == 4096)
        shinyChanceValue = 1;
    else
        shinyChanceValue *= 2;
    updateShiny();
}


// Make wild Pokemon shiny
void	shinyPokemon(void) {
    u32 offset[] =
    {
        0x003183EC,
        0x00318AF0,
    };
    u32 data = 0xEA00001C;
    u32 original = 0x0A00001C;
    if (shinyChanceValue == 4096) {
        WRITEU32(offset[gameVer], original);
        return;
    }
    int r = randomNum(1, shinyChanceValue);
    if (r == 1)
        WRITEU32(offset[gameVer], data);
    else
        WRITEU32(offset[gameVer], original);
}


// Use Z-Moves without the need of a Z-Crystal
void    zMoves(u32 state) {
    u32 offset[][2] =
    {
        {0x00595900, 0x00313DC0},
        {0x00597800, 0x00314300},
    };
    u32 data[][4] =
    {
        {0x0078BA28, 0xEB0A06CE, 0xEB0A06B5, 0xE3A00001},
        {0x0078BF60, 0xEB0A0D3E, 0xEB0A0D25, 0xE3A00001},
    };
    u32 original[] =
    {
        0xE1D510B4,
        0xE3A00000,
    };
    if (state) {
        static const u8    buffer[] =
        {
            0x05, 0x40, 0x2D, 0xE9, 0x06, 0x00, 0xA0, 0xE1,
            0x00, 0x00, 0x00, 0xEA, 0x05, 0x40, 0x2D, 0xE9,
            0x50, 0x20, 0x9D, 0xE5, 0x0C, 0x10, 0x9F, 0xE5,
            0x02, 0x00, 0x51, 0xE1, 0xB4, 0x10, 0xD5, 0x01,
            0x00, 0x10, 0xA0, 0x11, 0x05, 0x80, 0xBD, 0xE8
        };

        memcpy((void *)(offset[gameVer][0]), buffer, 0x28);
        WRITEU32(offset[gameVer][0] + 0x28, data[gameVer][0]);
        WRITEU32(offset[gameVer][1] + 0x00, data[gameVer][1]);
        WRITEU32(offset[gameVer][1] + 0x70, data[gameVer][2]);

        if (gameVer == 0)
            WRITEU32(offset[gameVer][1] + 0x5932C, data[gameVer][3]);
        if (gameVer == 1)
            WRITEU32(offset[gameVer][1] + 0x59CF4, data[gameVer][3]);

    } else {

        WRITEU32(offset[gameVer][1] + 0x00, original[0]);
        WRITEU32(offset[gameVer][1] + 0x70, original[0]);
        if (gameVer == 0)
            WRITEU32(offset[gameVer][1] + 0x5932C, original[1]);
        if (gameVer == 1)
            WRITEU32(offset[gameVer][1] + 0x59CF4, original[1]);
    }
}


// Inifinite Z-Moves
void    infZMoves(void) {
    u32 offset = 0x08031100;
    u32 data[] =
    {
        0xE3A00000,
        0xE5C30005,
        0xE1500000,
    };
    if (!checkAddress(offset + 0xDC))
        return;
    else {
        if (READU32(offset + 0xDC) == 0xE320F000) {
            WRITEU32(offset + 0xD4, data[0]);
            WRITEU32(offset + 0xD8, data[1]);
            WRITEU32(offset + 0xDC, data[2]);
        }
    }
}
