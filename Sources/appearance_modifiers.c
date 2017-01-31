// Filename: appearance_modifiers.c

#include "cheats.h"
#include "hid.h"

/********************************
 *                              *
 *    Appearance Modifiers      *
 *                              *
 ********************************/

// Define variables to be used
u8 matchingHair,
   storedGender,
   storedSkintone;

u32 o_gender =          0x330D67D5,
    o_skintone =        0x330D6824;


// Appearance menu entry
void    appearanceMenu(void) {

    // Reads currently set Gender and Skin tone from memory to initialize menu with
    storedGender = READU8(o_gender);
    storedSkintone = READU8(o_skintone);

    // Creates spoiler and cheat entries
    new_spoiler("Appearance");
        new_unselectable_entry("WARNING: Gender change");
        new_unselectable_entry("resets clothes & hair");
        new_line();
        new_entry_managed_note("Current Gender: XXXXXX", "Open a menu to see change", setGender, SETGENDER, AUTO_DISABLE);
        new_entry_managed_note("Current Skin Tone: X", "Ride Pokemon to see change\nor save & reboot", setSkintone, SETSKINTONE, AUTO_DISABLE);
        new_separator();
        new_entry_managed("Apply Changes", switchLooks, SWITCHLOOKS, EXECUTE_ONCE);
        new_line();
    exit_spoiler();

    // Updates the menu with stored values the first time it's loaded
    updateGender();
    updateSkintone();
}


// Changes current gender and skin tone. Sets clothes and hair to default with gender change.
void    switchLooks(void) {
    u32 offset = 0x330D67D4;
    u32 currentGender = READU8(o_gender);
    // Runs a check to see if your selected gender is different than current
    // gender and resets clothes and hair to prevent crashing. Writes just skin
    // tone if gender was not changed, preserving appearance
    if (storedGender == 0x00 && storedGender != currentGender) {
        fixMakeupBag();
        WRITEU8(offset + 0x01, storedGender);
        WRITEU32(offset + 0x50, 0x00800000 + (matchingHair * 0x0100) + storedSkintone);
        WRITEU32(offset + 0x54, 0x00040000);
        WRITEU32(offset + 0x58, 0x0010040C);
        WRITEU16(offset + 0x5C, 0x1C01);
        WRITEU8(offset + 0x5E, 0x30);
    } else if (storedGender == 0x01 && storedGender != currentGender) {
        fixMakeupBag();
        WRITEU8(offset + 0x01, storedGender);
        WRITEU32(offset + 0x50, 0x00800000 + (matchingHair * 0x0100) + storedSkintone);
        WRITEU32(offset + 0x54, 0x00040000);
        WRITEU32(offset + 0x58, 0x00100405);
        WRITEU16(offset + 0x5C, 0x3001);
        WRITEU8(offset + 0x5E, 0x30);
    } else {
        WRITEU8(offset + 0x50, storedSkintone);
    }
    WRITEU32(offset + 0x64, 0x00000000);
}


// Cycles through gender choices
void    setGender(void) {

    // Checks if gender is male and sets it to female or vice versa. Also
    // increases or decreases skin tone value to match gender
    switch(storedGender) {
        case 0x00:
            storedGender++;
            storedSkintone += 0x04;
            break;
        case 0x01:
            storedGender--;
            storedSkintone -= 0x04;
            break;
    }
    updateGender();
}


// Updates menu with current gender
void    updateGender(void) {
    replace_pattern(": ******", (storedGender == 0x00) ? ": Male  " : ": Female", SETGENDER);
}


// Cycles through skintone choices
void    setSkintone(void) {
    switch(storedSkintone) {
        case 0x18:
            storedSkintone = 0x00;
            break;
        case 0x1C:
            storedSkintone = 0x04;
            break;
        default:
            storedSkintone += 0x08;
    }
    updateSkintone();
}


// Updates menu with current skintone and changes hair color to match
void    updateSkintone(void) {
    switch(storedSkintone) {
        case 0x00:
        case 0x04:
            matchingHair = 0x83;
            replace_pattern(": *", ": A", SETSKINTONE);
            break;
        case 0x08:
        case 0x0C:
            matchingHair = 0x04;
            replace_pattern(": *", ": B", SETSKINTONE);
            break;
        case 0x10:
        case 0x14:
            matchingHair = 0x48;
            replace_pattern(": *", ": C", SETSKINTONE);
            break;
        case 0x18:
        case 0x1C:
            matchingHair = 0x08;
            replace_pattern(": *", ": D", SETSKINTONE);
            break;
    }
}


// Adds or removes Makeup bag based on gender
void    fixMakeupBag(void) {
    bool moveUp = false;
    u32 offset = 0x330D5FEC;
    u32 data = READU16(offset);
    int i = 0;
    switch(storedGender) {
        case 0:
            while(data) {
                if (data != 0x06C2 && !moveUp) {
                    i += 4;
                    data = READU16(offset + i);
                } else {
                    moveUp = true;
                    data = READU16(offset + i + 4);
                    WRITEU32(offset + i, data);
                    i += 4;
                }
            }
            break;
        case 1:
            while(data) {
                if (data == 0x06C2)
                    break;
                i += 4;
                data = READU16(offset + i);
            }
            WRITEU32(offset + i, 0x06C2);
            break;
    }
}
