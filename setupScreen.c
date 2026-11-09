#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "raylib.h"
#include "generateMap.h"
#include "gameInformation.h"
#include "setupScreen.h"

extern Tile map[17][17];
extern kingdom c[4];
extern int x, y;
extern int kingnum;
extern int alivePlayers;

// ---------- state machine مراحل ----------
typedef enum {
    STAGE_MENU,
    STAGE_MAP_SIZE,
    STAGE_KINGDOM_COUNT,
    STAGE_KINGDOM_INFO,
    STAGE_VILLAGE_COUNT,
    STAGE_VILLAGE_INFO,
    STAGE_BLOCK_COUNT,
    STAGE_BLOCK_INFO,
    STAGE_DONE
} SetupStage;

#define MAX_FIELDS 4
#define FIELD_LEN 16

static SetupStage stage = STAGE_MENU;
static char fields[MAX_FIELDS][FIELD_LEN];
static int fieldCount = 0;
static int activeField = 0;
static char errorMsg[128] = "";

static int loadedPlayerTurn = -1;

// داده‌های موقت ورودی دستی
static int tmpX, tmpY;
static int tmpKingnum;
static int kingdomIndex = 0;

static int tmpVillageCount = 0;
static int villageIndex = 0;

static int tmpBlockCount = 0;
static int blockIndex = 0;

// ---------- کمک‌تابع‌ها ----------

static void ClearFields(int count) {
    fieldCount = count;
    activeField = 0;
    for (int i = 0; i < MAX_FIELDS; i++) fields[i][0] = '\0';
    errorMsg[0] = '\0';
}

static void HandleTextInput(void) {
    int key = GetCharPressed();
    while (key > 0) {
        if ((key >= '0' && key <= '9') || key == '-') {
            int len = (int)strlen(fields[activeField]);
            if (len < FIELD_LEN - 1) {
                fields[activeField][len] = (char)key;
                fields[activeField][len + 1] = '\0';
            }
        }
        key = GetCharPressed();
    }
    if (IsKeyPressed(KEY_BACKSPACE)) {
        int len = (int)strlen(fields[activeField]);
        if (len > 0) fields[activeField][len - 1] = '\0';
    }
    if (IsKeyPressed(KEY_TAB) || IsKeyPressed(KEY_DOWN)) {
        activeField = (activeField + 1) % fieldCount;
    }
    if (IsKeyPressed(KEY_UP)) {
        activeField = (activeField - 1 + fieldCount) % fieldCount;
    }
}

static bool FieldsFilled(void) {
    for (int i = 0; i < fieldCount; i++) {
        if (fields[i][0] == '\0') return false;
    }
    return true;
}

static int FieldInt(int index) {
    return atoi(fields[index]);
}

// ---------- رسم ----------

static void DrawFieldBox(const char *label, int index, int x0, int y0, int w) {
    Rectangle box = { (float)x0, (float)y0, (float)w, 36 };
    bool active = (index == activeField);
    DrawRectangleRec(box, active ? LIGHTGRAY : RAYWHITE);
    DrawRectangleLinesEx(box, active ? 2 : 1, active ? BLUE : GRAY);
    DrawText(label, x0, y0 - 22, 18, DARKGRAY);
    DrawText(fields[index], x0 + 8, y0 + 8, 20, BLACK);
    if (active && ((int)(GetTime() * 2) % 2 == 0)) {
        int tw = MeasureText(fields[index], 20);
        DrawText("|", x0 + 10 + tw, y0 + 6, 20, BLACK);
    }
}

static void DrawTitle(const char *title) {
    DrawText(title, 60, 40, 30, BLACK);
    DrawLine(60, 80, GetScreenWidth() - 60, 80, LIGHTGRAY);
}

static void DrawHint(const char *hint) {
    DrawText(hint, 60, GetScreenHeight() - 60, 18, GRAY);
    if (errorMsg[0] != '\0') {
        DrawText(errorMsg, 60, GetScreenHeight() - 90, 20, RED);
    }
}

// ---------- منطق هر مرحله ----------

static SetupResult UpdateMenu(void) {
    DrawTitle("Game Setup");
    DrawText("1 - Load map from file (map.txt)", 80, 140, 22, DARKGRAY);
    DrawText("2 - Enter map manually", 80, 180, 22, DARKGRAY);
    DrawText("3 - Load saved game", 80, 220, 22, DARKGRAY);
    DrawHint("Press 1, 2 or 3");

    if (IsKeyPressed(KEY_ONE)) {
        if (GetasFile() == 0) {
            return SETUP_RESULT_FILE;
        }
        strcpy(errorMsg, "Could not open map.txt. Enter map manually.");
        stage = STAGE_MAP_SIZE;
        ClearFields(2);
        return SETUP_RESULT_NONE;
    }
    if (IsKeyPressed(KEY_TWO)) {
        stage = STAGE_MAP_SIZE;
        ClearFields(2);
    }
    if (IsKeyPressed(KEY_THREE)) {
        loadedPlayerTurn = loadgame();
        if (loadedPlayerTurn != -1) {
            return SETUP_RESULT_LOAD;
        }
        // loadgame در صورت شکست خودش GetInformation ترمینالی را صدا می‌زد؛
        // این‌جا به‌جایش کاربر را به فرم دستی می‌فرستیم
        strcpy(errorMsg, "Could not load save. Enter map manually.");
        stage = STAGE_MAP_SIZE;
        ClearFields(2);
    }
    return SETUP_RESULT_NONE;
}

static void UpdateMapSize(void) {
    DrawTitle("Map size");
    DrawFieldBox("Width (x)", 0, 80, 140, 150);
    DrawFieldBox("Height (y)", 1, 260, 140, 150);
    DrawHint("Enter values between 1 and 17, then press ENTER");
    HandleTextInput();

    if (IsKeyPressed(KEY_ENTER)) {
        if (!FieldsFilled()) { strcpy(errorMsg, "Fill in both fields."); return; }
        int vx = FieldInt(0), vy = FieldInt(1);
        if (vx <= 0 || vx > MAP_SIZE || vy <= 0 || vy > MAP_SIZE) {
            strcpy(errorMsg, "x and y must be between 1 and 17.");
            return;
        }
        tmpX = vx; tmpY = vy;
        x = tmpX; y = tmpY;
        generate_array();
        stage = STAGE_KINGDOM_COUNT;
        ClearFields(1);
    }
}

static void UpdateKingdomCount(void) {
    DrawTitle("Number of kingdoms");
    DrawFieldBox("Kingdoms (1-4)", 0, 80, 140, 150);
    DrawHint("Press ENTER to continue");
    HandleTextInput();

    if (IsKeyPressed(KEY_ENTER)) {
        if (!FieldsFilled()) { strcpy(errorMsg, "Enter a value."); return; }
        int v = FieldInt(0);
        if (v <= 0 || v >= 5) {
            strcpy(errorMsg, "Number of kingdoms must be between 1 and 4.");
            return;
        }
        tmpKingnum = v;
        kingnum = v;
        alivePlayers = v;
        kingdomIndex = 0;
        stage = STAGE_KINGDOM_INFO;
        ClearFields(3);
    }
}

static void UpdateKingdomInfo(void) {
    char title[64];
    sprintf(title, "Kingdom %d of %d", kingdomIndex + 1, tmpKingnum);
    DrawTitle(title);
    DrawFieldBox("X", 0, 80, 140, 100);
    DrawFieldBox("Y", 1, 200, 140, 100);
    DrawFieldBox("Computer? (1=yes, 0=no)", 2, 320, 140, 200);
    DrawHint("Press ENTER to continue");
    HandleTextInput();

    if (IsKeyPressed(KEY_ENTER)) {
        if (!FieldsFilled()) { strcpy(errorMsg, "Fill in all fields."); return; }
        int kx = FieldInt(0), ky = FieldInt(1), isComp = FieldInt(2);
        if (kx <= 0 || kx > tmpX || ky <= 0 || ky > tmpY) {
            strcpy(errorMsg, "Coordinates must be within the map.");
            return;
        }
        int i = kingdomIndex;
        c[i].x = kx - 1; c[i].y = ky - 1;
        c[i].is_computer = isComp ? 1 : 0;
        c[i].GoldRate = 1; c[i].FoodRate = 0; c[i].gold = 5; c[i].food = 0; c[i].worker = 1;
        c[i].price_soldier = 1; c[i].soldier = 0;
        map[c[i].x][c[i].y].forkingdom = i;
        c[i].isAlive = 1;
        map[c[i].x][c[i].y].type = KINGDOM;
        c[i].price_worker = 3;

        kingdomIndex++;
        if (kingdomIndex >= tmpKingnum) {
            stage = STAGE_VILLAGE_COUNT;
            ClearFields(1);
        } else {
            ClearFields(3);
        }
    }
}

static void UpdateVillageCount(void) {
    DrawTitle("Number of villages");
    DrawFieldBox("Villages (0-10)", 0, 80, 140, 150);
    DrawHint("Press ENTER to continue");
    HandleTextInput();

    if (IsKeyPressed(KEY_ENTER)) {
        if (!FieldsFilled()) { strcpy(errorMsg, "Enter a value."); return; }
        int v = FieldInt(0);
        if (v < 0 || v > 10) {
            strcpy(errorMsg, "Number of villages must be 10 or fewer.");
            return;
        }
        tmpVillageCount = v;
        villageIndex = 0;
        if (tmpVillageCount == 0) {
            stage = STAGE_BLOCK_COUNT;
            ClearFields(1);
        } else {
            stage = STAGE_VILLAGE_INFO;
            ClearFields(4);
        }
    }
}

static void UpdateVillageInfo(void) {
    char title[64];
    sprintf(title, "Village %d of %d", villageIndex + 1, tmpVillageCount);
    DrawTitle(title);
    DrawFieldBox("X", 0, 80, 140, 90);
    DrawFieldBox("Y", 1, 190, 140, 90);
    DrawFieldBox("Food rate", 2, 300, 140, 120);
    DrawFieldBox("Gold rate", 3, 440, 140, 120);
    DrawHint("Press ENTER to continue");
    HandleTextInput();

    if (IsKeyPressed(KEY_ENTER)) {
        if (!FieldsFilled()) { strcpy(errorMsg, "Fill in all fields."); return; }
        int vx = FieldInt(0), vy = FieldInt(1);
        int foodrate = FieldInt(2), goldrate = FieldInt(3);
        if (vx <= 0 || vx > tmpX || vy <= 0 || vy > tmpY) {
            strcpy(errorMsg, "Coordinates must be within the map.");
            return;
        }
        map[vx - 1][vy - 1].type = VILLAGE;
        map[vx - 1][vy - 1].FoodRate = foodrate;
        map[vx - 1][vy - 1].GoldRate = goldrate;

        villageIndex++;
        if (villageIndex >= tmpVillageCount) {
            stage = STAGE_BLOCK_COUNT;
            ClearFields(1);
        } else {
            ClearFields(4);
        }
    }
}

static void UpdateBlockCount(void) {
    DrawTitle("Number of blocked houses");
    DrawFieldBox("Blocked houses", 0, 80, 140, 150);
    DrawHint("Press ENTER to continue");
    HandleTextInput();

    if (IsKeyPressed(KEY_ENTER)) {
        if (!FieldsFilled()) { strcpy(errorMsg, "Enter a value."); return; }
        int v = FieldInt(0);
        if (v < 0) { strcpy(errorMsg, "Value cannot be negative."); return; }
        tmpBlockCount = v;
        blockIndex = 0;
        if (tmpBlockCount == 0) {
            stage = STAGE_DONE;
        } else {
            stage = STAGE_BLOCK_INFO;
            ClearFields(2);
        }
    }
}

static void UpdateBlockInfo(void) {
    char title[64];
    sprintf(title, "Blocked house %d of %d", blockIndex + 1, tmpBlockCount);
    DrawTitle(title);
    DrawFieldBox("X", 0, 80, 140, 100);
    DrawFieldBox("Y", 1, 200, 140, 100);
    DrawHint("Press ENTER to continue");
    HandleTextInput();

    if (IsKeyPressed(KEY_ENTER)) {
        if (!FieldsFilled()) { strcpy(errorMsg, "Fill in all fields."); return; }
        int px = FieldInt(0), py = FieldInt(1);
        if (px <= 0 || px > tmpX || py <= 0 || py > tmpY) {
            strcpy(errorMsg, "Coordinates must be within the map.");
            return;
        }
        map[px - 1][py - 1].type = BLOCK_HOUSE;

        blockIndex++;
        if (blockIndex >= tmpBlockCount) {
            stage = STAGE_DONE;
        } else {
            ClearFields(2);
        }
    }
}

// ---------- تابع اصلی ----------

SetupResult UpdateSetupScreen(void) {
    switch (stage) {
        case STAGE_MENU:
            return UpdateMenu();
        case STAGE_MAP_SIZE:
            UpdateMapSize();
            return SETUP_RESULT_NONE;
        case STAGE_KINGDOM_COUNT:
            UpdateKingdomCount();
            return SETUP_RESULT_NONE;
        case STAGE_KINGDOM_INFO:
            UpdateKingdomInfo();
            return SETUP_RESULT_NONE;
        case STAGE_VILLAGE_COUNT:
            UpdateVillageCount();
            return SETUP_RESULT_NONE;
        case STAGE_VILLAGE_INFO:
            UpdateVillageInfo();
            return SETUP_RESULT_NONE;
        case STAGE_BLOCK_COUNT:
            UpdateBlockCount();
            return SETUP_RESULT_NONE;
        case STAGE_BLOCK_INFO:
            UpdateBlockInfo();
            return SETUP_RESULT_NONE;
        case STAGE_DONE:
            return SETUP_RESULT_MANUAL;
    }
    return SETUP_RESULT_NONE;
}

int GetLoadedPlayerTurn(void) {
    return loadedPlayerTurn;
}

void ResetSetupScreen(void) {
    stage = STAGE_MENU;
    fieldCount = 0;
    activeField = 0;
    errorMsg[0] = '\0';
    kingdomIndex = 0;
    villageIndex = 0;
    blockIndex = 0;
    loadedPlayerTurn = -1;
}
