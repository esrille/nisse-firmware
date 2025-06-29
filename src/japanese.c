/*
 * Copyright 2013-2025 Esrille Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "app.h"

//
// Stickney Next
//
static const uint8_t matrixStickney[MATRIX_ROWS][MATRIX_COLS] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, KANA_LCB, 0, 0, 0, 0, 0, 0, 0, 0, 0, KANA_TOUTEN},
    {0, KANA_RCB, 0, 0, 0, 0, 0, 0, 0, 0, KANA_KUTEN, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, KANA_HO, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, KANA_DAKUTEN, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, KANA_WO, 0, 0, 0, 0, 0, 0, KANA_CHOUON, 0},
};

static const uint8_t matrixStickneyShift[MATRIX_ROWS][MATRIX_COLS] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, KANA_LCB, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, KANA_RCB, 0, 0, 0, 0, 0, 0, 0, 0, KANA_KUTEN, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, KANA_SO, 0, 0, 0, 0, 0, KANA_HANDAKU, 0},
    {0, 0, 0, KANA_SE, KANA_HE, KANA_KE, 0, KANA_ME, KANA_NU, KANA_RO, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, KANA_MU, 0, 0, 0, 0},
};

//
// TRON
//
static const uint8_t matrixTron[MATRIX_ROWS][MATRIX_COLS] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, ROMA_LCB, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, ROMA_RCB, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, ROMA_RA, ROMA_RU, ROMA_KO, ROMA_HA, ROMA_XYO, ROMA_KI, ROMA_NO, ROMA_KU, ROMA_A, ROMA_RE, 0},
    {0, ROMA_TA, ROMA_TO, ROMA_KA, ROMA_TE, ROMA_MO, ROMA_WO, ROMA_I, ROMA_U, ROMA_SI, ROMA_NN, 0},
    {0, ROMA_MA, ROMA_RI, ROMA_NI, ROMA_SA, ROMA_NA, ROMA_SU, ROMA_TU, ROMA_TOUTEN, ROMA_KUTEN, ROMA_XTU, 0},
};

static const uint8_t matrixTronLeft[MATRIX_ROWS][MATRIX_COLS] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, ROMA_LCB, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, ROMA_RCB, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, ROMA_SANTEN, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, ROMA_HI, ROMA_SO, ROMA_NAKAGURO, ROMA_XYA, ROMA_HO, ROMA_GI, ROMA_GE, ROMA_GU, ROMA_QUESTION, ROMA_WYI, 0},
    {0, ROMA_NU, ROMA_NE, ROMA_XYU, ROMA_YO, ROMA_HU, ROMA_DAKUTEN, ROMA_DI, ROMA_VU, ROMA_ZI, ROMA_WYE, 0},
    {0, ROMA_XE, ROMA_XO, ROMA_SE, ROMA_YU, ROMA_HE, ROMA_ZU, ROMA_DU, ROMA_COMMA, ROMA_PERIOD, ROMA_XWA, 0},
};

static const uint8_t matrixTronRight[MATRIX_ROWS][MATRIX_COLS] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, ROMA_LWCB, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, ROMA_RWCB, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, ROMA_BI, ROMA_ZO, ROMA_GO, ROMA_BA, ROMA_BO, ROMA_E, ROMA_KE, ROMA_ME, ROMA_MU, ROMA_RO, 0},
    {0, ROMA_DA, ROMA_DO, ROMA_GA, ROMA_DE, ROMA_BU, ROMA_O, ROMA_TI, ROMA_CHOUON, ROMA_MI, ROMA_YA, 0},
    {0, ROMA_XKA, ROMA_XKE, ROMA_ZE, ROMA_ZA, ROMA_BE, ROMA_WA, ROMA_XI, ROMA_XA, ROMA_HANDAKU, ROMA_XU, 0},
};

//
// Nicola
//
static const uint8_t matrixNicola[MATRIX_ROWS][MATRIX_COLS] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, ROMA_LCB, 0, 0, 0, 0, 0, 0, 0, 0, ROMA_DAKUTEN, 0},
    {0, ROMA_RCB, 0, 0, 0, 0, 0, 0, 0, 0, ROMA_TOUTEN, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, ROMA_KUTEN, ROMA_KA, ROMA_TA, ROMA_KO, ROMA_SA, ROMA_RA, ROMA_TI, ROMA_KU, ROMA_TU, ROMA_TOUTEN, 0},
    {0, ROMA_U, ROMA_SI, ROMA_TE, ROMA_KE, ROMA_SE, ROMA_HA, ROMA_TO, ROMA_KI, ROMA_I, ROMA_NN, 0},
    {0, ROMA_KUTEN, ROMA_HI, ROMA_SU, ROMA_HU, ROMA_HE, ROMA_ME, ROMA_SO, ROMA_NE, ROMA_HO, ROMA_NAKAGURO, 0},
};

static const uint8_t matrixNicolaLeft[MATRIX_ROWS][MATRIX_COLS] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, ROMA_LCB, 0, 0, 0, 0, 0, 0, 0, 0, ROMA_DAKUTEN, 0},
    {0, ROMA_RCB, 0, 0, 0, 0, 0, 0, 0, 0, ROMA_TOUTEN, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, ROMA_QUESTION, ROMA_SLASH, ROMA_NAMI, ROMA_LCB, ROMA_RCB, ROMA_LSB, ROMA_RSB, 0, 0, 0, 0},
    {0, ROMA_XA, ROMA_E, ROMA_RI, ROMA_XYA, ROMA_RE, ROMA_PA, ROMA_DI, ROMA_GU, ROMA_DU, ROMA_PI, 0},
    {0, ROMA_WO, ROMA_A, ROMA_NA, ROMA_XYU, ROMA_MO, ROMA_BA, ROMA_DO, ROMA_GI, ROMA_PO, ROMA_NN, 0},
    {0, ROMA_XU, ROMA_CHOUON, ROMA_RO, ROMA_YA, ROMA_XI, ROMA_PU, ROMA_ZO, ROMA_PE, ROMA_BO, ROMA_NAKAGURO, 0},
};

static const uint8_t matrixNicolaRight[MATRIX_ROWS][MATRIX_COLS] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, ROMA_LWCB, 0, 0, 0, 0, 0, 0, 0, 0, ROMA_HANDAKU, 0},
    {0, ROMA_RWCB, 0, 0, 0, 0, 0, 0, 0, 0, ROMA_TOUTEN, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, ROMA_QUESTION, ROMA_SLASH, ROMA_NAMI, ROMA_LCB, ROMA_RCB, ROMA_LSB, ROMA_RSB, 0, 0, 0, 0},
    {0, ROMA_KUTEN, ROMA_GA, ROMA_DA, ROMA_GO, ROMA_ZA, ROMA_YO, ROMA_NI, ROMA_RU, ROMA_MA, ROMA_XE, 0},
    {0, ROMA_VU, ROMA_ZI, ROMA_DE, ROMA_GE, ROMA_ZE, ROMA_MI, ROMA_O, ROMA_NO, ROMA_XYO, ROMA_XTU, 0},
    {0, ROMA_KUTEN, ROMA_BI, ROMA_ZU, ROMA_BU, ROMA_BE, ROMA_NU, ROMA_YU, ROMA_MU, ROMA_WA, ROMA_XO, 0},
};

//
// JIS X 6004
//
static const uint8_t matrixX6004[MATRIX_ROWS][MATRIX_COLS] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, ROMA_LCB, 0, 0, 0, 0, 0, 0, 0, 0, 0, ROMA_NA},
    {0, ROMA_RCB, 0, 0, 0, 0, 0, 0, 0, 0, ROMA_TI, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, ROMA_SO, ROMA_KE, ROMA_SE, ROMA_TE, ROMA_XYO, ROMA_TU, ROMA_NN, ROMA_NO, ROMA_WO, ROMA_RI, 0},
    {0, ROMA_HA, ROMA_KA, ROMA_SI, ROMA_TO, ROMA_TA, ROMA_KU, ROMA_U, ROMA_I, ROMA_DAKUTEN, ROMA_KI, 0},
    {0, ROMA_SU, ROMA_KO, ROMA_NI, ROMA_SA, ROMA_A, ROMA_XTU, ROMA_RU, ROMA_TOUTEN, ROMA_KUTEN, ROMA_RE, 0},
};

static const uint8_t matrixX6004Shift[MATRIX_ROWS][MATRIX_COLS] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, ROMA_LWCB, 0, 0, 0, 0, 0, 0, 0, 0, 0, ROMA_RCB},
    {0, ROMA_RWCB, 0, 0, 0, 0, 0, 0, 0, 0, ROMA_LCB, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, ROMA_XA, ROMA_HANDAKU, ROMA_HO, ROMA_HU, ROMA_ME, ROMA_HI, ROMA_E, ROMA_MI, ROMA_YA, ROMA_NU, 0},
    {0, ROMA_XI, ROMA_HE, ROMA_RA, ROMA_XYU, ROMA_YO, ROMA_MA, ROMA_O, ROMA_MO, ROMA_WA, ROMA_YU, 0},
    {0, ROMA_XU, ROMA_XE, ROMA_XO, ROMA_NE, ROMA_XYA, ROMA_MU, ROMA_RO, ROMA_NAKAGURO, ROMA_CHOUON, ROMA_QUESTION, 0},
};

//
// M type
//
static const uint8_t matrixMtype[MATRIX_ROWS][MATRIX_COLS] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, ROMA_Q, ROMA_L, ROMA_J, ROMA_F, ROMA_C, ROMA_M, ROMA_Y, ROMA_R, ROMA_W, ROMA_P, 0},
    {0, ROMA_E, ROMA_U, ROMA_I, ROMA_A, ROMA_O, ROMA_K, ROMA_S, ROMA_T, ROMA_N, ROMA_H, 0},
    {0, ROMA_EI, ROMA_X, ROMA_V, ROMA_AI, ROMA_OU, ROMA_G, ROMA_Z, ROMA_D, ROMA_TOUTEN, ROMA_B, 0},
};

static const uint8_t matrixMtypeShift[MATRIX_ROWS][MATRIX_COLS] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, ROMA_EKI, ROMA_UKU, ROMA_IKU, ROMA_AKU, ROMA_OKU, ROMA_MY, ROMA_XTU, ROMA_RY, ROMA_NN, ROMA_PY, 0},
    {0, ROMA_ENN, ROMA_UNN, ROMA_INN, ROMA_ANN, ROMA_ONN, ROMA_KY, ROMA_SY, ROMA_TY, ROMA_NY, ROMA_HY, 0},
    {0, ROMA_ETU, ROMA_UTU, ROMA_ITU, ROMA_ATU, ROMA_OTU, ROMA_GY, ROMA_ZY, ROMA_DY, ROMA_KUTEN, ROMA_BY, 0},
};

//
// New Stickney
//
static const uint8_t matrixNewStickney[MATRIX_ROWS][MATRIX_COLS] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, ROMA_LCB, 0, 0, 0, 0, 0, 0, 0, 0, 0, ROMA_SPACE},
    {0, ROMA_RCB, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, ROMA_KE, ROMA_KU, ROMA_SU, ROMA_SA, ROMA_TU, ROMA_NU, ROMA_O, ROMA_NO, ROMA_NI, ROMA_NE, 0},
    {0, ROMA_HA, ROMA_KA, ROMA_SI, ROMA_TA, ROMA_TE, ROMA_RA, ROMA_U, ROMA_I, ROMA_DAKUTEN, ROMA_NA, 0},
    {0, ROMA_YO, ROMA_KI, ROMA_KO, ROMA_TO, ROMA_TI, ROMA_XTU, ROMA_NN, ROMA_TOUTEN, ROMA_KUTEN, ROMA_NAKAGURO, 0},
};

static const uint8_t matrixNewStickneyShift[MATRIX_ROWS][MATRIX_COLS] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, ROMA_LWCB, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, ROMA_RWCB, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, ROMA_HANDAKU, ROMA_HI, ROMA_HU, 0, ROMA_MU, ROMA_E, ROMA_MO, ROMA_MI, ROMA_ME, 0},
    {0, ROMA_YA, ROMA_SO, ROMA_SE, ROMA_HE, ROMA_HO, ROMA_RE, ROMA_RU, ROMA_RI, ROMA_A, ROMA_MA, 0},
    {0, ROMA_YU, ROMA_WYI, 0, ROMA_WYE, 0, ROMA_RO, ROMA_CHOUON, ROMA_WO, ROMA_WA, ROMA_QUESTION, 0},
};

static const uint8_t consonantSet[][2] =
{
    {0},
    {KEY_K},
    {KEY_S},
    {KEY_T},
    {KEY_N},
    {KEY_H},
    {KEY_M},
    {KEY_Y},
    {KEY_R},
    {KEY_W},
    {KEY_P},
    {KEY_G},
    {KEY_Z},
    {KEY_D},
    {KEY_B},
    {KEY_X},
    {KEY_X, KEY_K},
    {KEY_X, KEY_T},
    {KEY_X, KEY_Y},
    {KEY_X, KEY_W},
    {KEY_W, KEY_Y},
    {KEY_V},
    {KEY_L},
};

static const uint8_t vowelSet[] =
{
    0,
    KEY_A,
    KEY_I,
    KEY_U,
    KEY_E,
    KEY_O,
    KEY_Y
};

static const uint8_t mtypeSet[][3] =
{
    {KEY_A, KEY_N, KEY_N},
    {KEY_A, KEY_K, KEY_U},
    {KEY_A, KEY_T, KEY_U},
    {KEY_A, KEY_I},
    {KEY_I, KEY_N, KEY_N},
    {KEY_I, KEY_K, KEY_U},
    {KEY_I, KEY_T, KEY_U},
    {KEY_U, KEY_N, KEY_N},
    {KEY_U, KEY_K, KEY_U},
    {KEY_U, KEY_T, KEY_U},
    {KEY_E, KEY_N, KEY_N},
    {KEY_E, KEY_K, KEY_I},
    {KEY_E, KEY_T, KEY_U},
    {KEY_E, KEY_I},
    {KEY_O, KEY_N, KEY_N},
    {KEY_O, KEY_K, KEY_U},
    {KEY_O, KEY_T, KEY_U},
    {KEY_O, KEY_U},
    {KEY_C},
    {KEY_F},
    {KEY_J},
    {KEY_Q},
};

// ROMA_NN - ROMA_SPACE
static const uint8_t commonSet[][2] =
{
    {KEY_N, KEY_N},
    {KEY_MINUS},
    {KEY_DAKUTEN},
    {KEY_HANDAKU},
    {KEY_LEFT_SHIFT, KEY_SLASH},
    {KEY_COMMA},
    {KEY_PERIOD},
    {KEY_LEFT_SHIFT, KEY_COMMA},
    {KEY_LEFT_SHIFT, KEY_PERIOD},

    // Stickney Next
    {KEY_LEFT_BRACKET},
    {KEY_RIGHT_BRACKET},
    {KEY_LEFT_SHIFT, KEY_RIGHT_BRACKET},
    {KEY_LEFT_SHIFT, KEY_NON_US_HASH},
    {KEY_APOSTROPHE},
    {KEY_EQUAL},
    {KEY_MINUS},
    {KEY_1},
    {KEY_SLASH},
    {KEY_NON_US_HASH},
    {KEY_LEFT_SHIFT, KEY_0},
    {KEY_INTERNATIONAL1},
    {KEY_LEFT_SHIFT, KEY_COMMA},
    {KEY_LEFT_SHIFT, KEY_PERIOD},
    {KEY_LEFT_SHIFT, KEY_SLASH},
    {KEY_INTERNATIONAL3},

    // Extra
    {KEY_LEFT_SHIFT, KEY_1},
    {KEY_SPACE},
};

//
// ROMA_LCB - ROMA_NAMI
//
static const uint8_t msSet[][3] =
{
    {KEY_LEFT_BRACKET},
    {KEY_RIGHT_BRACKET},
    {KEY_LEFT_BRACKET},
    {KEY_RIGHT_BRACKET},
    {KEY_LEFT_BRACKET},
    {KEY_RIGHT_BRACKET},
    {KEY_SLASH},
    {KEY_SLASH},
    {KEY_SLASH, KEY_SLASH, KEY_SLASH},
    {KEY_COMMA},
    {KEY_PERIOD},
    {KEY_LEFT_SHIFT, KEY_GRAVE},
};

static const uint8_t googleSet[][3] =
{
    {KEY_LEFT_BRACKET},
    {KEY_RIGHT_BRACKET},
    {KEY_Z, KEY_LEFT_BRACKET},
    {KEY_Z, KEY_RIGHT_BRACKET},
    {KEY_LEFT_BRACKET},
    {KEY_RIGHT_BRACKET},
    {KEY_Z, KEY_SLASH},
    {KEY_SLASH},
    {KEY_Z, KEY_PERIOD},
    {KEY_COMMA},
    {KEY_PERIOD},
    {KEY_LEFT_SHIFT, KEY_GRAVE},
};

static const uint8_t atokSet[][3] =
{
    {KEY_LEFT_BRACKET},
    {KEY_RIGHT_BRACKET},
    {KEY_LEFT_BRACKET},
    {KEY_RIGHT_BRACKET},
    {KEY_LEFT_BRACKET},
    {KEY_RIGHT_BRACKET},
    {KEY_SLASH},
    {KEY_SLASH},
    {KEY_SLASH, KEY_SLASH, KEY_SLASH},
    {KEY_COMMA},
    {KEY_PERIOD},
    {KEY_LEFT_SHIFT, KEY_GRAVE},
};

static const uint8_t appleSet[][3] =
{
    {KEY_LEFT_BRACKET},
    {KEY_RIGHT_BRACKET},
    {KEY_LEFT_SHIFT, KEY_LEFT_BRACKET},
    {KEY_LEFT_SHIFT, KEY_RIGHT_BRACKET},
    {KEY_LEFT_ALT, KEY_LEFT_SHIFT, KEY_9},
    {KEY_LEFT_ALT, KEY_LEFT_SHIFT, KEY_0},
    {KEY_SLASH},
    {KEY_SLASH},
    {KEY_SLASH, KEY_SLASH, KEY_SLASH},
    {KEY_COMMA},
    {KEY_PERIOD},
    {KEY_LEFT_SHIFT, KEY_GRAVE},
};

static uint8_t ansiToJIS[5][2] = {
    {KEY_LEFT_BRACKET, KEY_RIGHT_BRACKET},
    {KEY_RIGHT_BRACKET, KEY_NON_US_HASH},
    {KEY_GRAVE, KEY_EQUAL},
    {KEY_9, KEY_8},
    {KEY_0, KEY_9}
};

static const uint8_t dakuonFrom[] = { KEY_K, KEY_S, KEY_T, KEY_H };
static const uint8_t dakuonTo[] = { KEY_G, KEY_Z, KEY_D, KEY_B };

static const uint8_t kogakiFrom[] = { KEY_A, KEY_I, KEY_U, KEY_E, KEY_O };
// static const uint8_t kogaki_to[] = { KEY_X, KEY_X, KEY_X, KEY_X, KEY_X };

static bool IsJP(void) {
    uint8_t baseLayer = PROFILE_Read(EEPROM_BASE);
    return baseLayer == BASE_JIS || baseLayer == BASE_NICOLA_F;
}

static void ProcessRomaji(uint8_t roma, uint8_t a[3]) {
    const uint8_t* c;
    uint8_t i;

    if (roma < ROMA_ANN) {
        c = consonantSet[roma / 7];
        for (i = 0; i < 2 && c[i]; ++i)
            a[i] = c[i];
        a[i++] = vowelSet[roma % 7];
        while (i < 3)
            a[i++] = 0;
        return;
    }
    if (ROMA_ANN <= roma && roma <= ROMA_Q) {
        memcpy(a, mtypeSet[roma - ROMA_ANN], 3);
        return;
    }
    if (ROMA_NN <= roma && roma <= ROMA_SPACE) {
        memcpy(a, commonSet[roma - ROMA_NN], 2);
        a[2] = 0;
        return;
    }
    if (ROMA_LCB <= roma && roma <= ROMA_NAMI) {
        i = roma - ROMA_LCB;
        switch (PROFILE_Read(EEPROM_IME)) {
        case IME_GOOGLE:
            c = googleSet[i];
            break;
        case IME_APPLE:
            c = appleSet[i];
            break;
        case IME_ATOK:
            c = atokSet[i];
            break;
        case IME_MS:
        default:
            c = msSet[i];
            break;
        }
        for (i = 0; i < 3; ++i) {
            uint8_t key = c[i];
            if (IsJP()) {
                for (int j = 0; i < (sizeof ansiToJIS / sizeof ansiToJIS[0]); ++i) {
                    if (ansiToJIS[j][0] == key) {
                        key = ansiToJIS[j][1];
                        break;
                    }
                }
            }
            a[i] = key;
        }
        return;
    }
    memset(a, 0, 3);
}

static uint8_t last[3];

// Assumptions: Control and Alt keys are not pressed.
// Minimalism: No auto repeat support.
static int8_t ProcessKana(const uint8_t mod,
                          const uint8_t normal[MATRIX_ROWS][MATRIX_COLS],
                          const uint8_t left[MATRIX_ROWS][MATRIX_COLS],
                          const uint8_t right[MATRIX_ROWS][MATRIX_COLS]) {
    uint8_t roma;
    uint8_t a[3];
    int8_t xmit = XMIT_NONE;

    for (int row = 0; row < MATRIX_ROWS; row++) {
        for (int column = 0; column < MATRIX_COLS; column++) {
            if (!KEYBOARD_IsMake(row, column)) {
                continue;
            }
            if (mod & MOD_SHIFT_LEFT)
                roma = left[row][column];
            else if (mod & MOD_SHIFT_RIGHT)
                roma = right[row][column];
            else
                roma = normal[row][column];
            if (roma) {
                ProcessRomaji(roma, a);
            } else {
                uint8_t key = KEYBOARD_GetNormalKey(row, column);
                key = KEYBOARD_ApplyModifiersMap(key);
                int i = 0;
                if (!KEYBOARD_IsModifier(key)) {
                    if (mod & MOD_SHIFT) {
                        a[i++] = KEY_LEFT_SHIFT;
                    }
                    a[i++] = key;
                }
                while (i < 3)
                    a[i++] = 0;
            }
            for (int i = 0; i < 3 && a[i]; ++i) {
                uint8_t key = a[i];
                switch (key) {
                case KEY_DAKUTEN:
                    if (last[0]) {
                        const uint8_t* dakuon;
                        if ((dakuon = memchr(dakuonFrom, last[0], sizeof dakuonFrom))) {
                            xmit = XMIT_MACRO;
                            MACRO_Put(KEY_BACKSPACE);
                            MACRO_Put(dakuonTo[dakuon - dakuonFrom]);
                            MACRO_Put(last[1]);
                        } else if ((dakuon = memchr(kogakiFrom, last[0], sizeof kogakiFrom))) {
                            xmit = XMIT_MACRO;
                            MACRO_Put(KEY_BACKSPACE);
                            MACRO_Put(KEY_X);
                            MACRO_Put(last[0]);
                        } else if (last[0] == KEY_Y && (dakuon = memchr(kogakiFrom, last[1], sizeof kogakiFrom))) {
                            xmit = XMIT_MACRO;
                            MACRO_Put(KEY_BACKSPACE);
                            MACRO_Put(KEY_X);
                            MACRO_Put(KEY_Y);
                            MACRO_Put(last[1]);
                        }
                    }
                    break;
                case KEY_HANDAKU:
                    if (last[0] == KEY_H) {
                        xmit = XMIT_MACRO;
                        MACRO_Put(KEY_BACKSPACE);
                        MACRO_Put(KEY_P);
                        MACRO_Put(last[1]);
                    }
                    break;
                default:
                    xmit = XMIT_MACRO;
                    MACRO_Put(key);
                    break;
                }
            }
            if (a[0])
                memcpy(last, a, 3);
        }
    }
    return xmit;
}

int8_t KEYBOARD_GetKanaReport(const uint8_t mod) {
    switch (PROFILE_Read(EEPROM_KANA)) {
    case KANA_NICOLA:
        return ProcessKana(mod, matrixNicola, matrixNicolaLeft, matrixNicolaRight);
    case KANA_TRON:
        return ProcessKana(mod, matrixTron, matrixTronLeft, matrixTronRight);
    case KANA_STICKNEY:
        return ProcessKana(mod, matrixStickney, matrixStickneyShift, matrixStickneyShift);
    case KANA_X6004:
        return ProcessKana(mod, matrixX6004, matrixX6004Shift, matrixX6004Shift);
    case KANA_MTYPE:
        return ProcessKana(mod, matrixMtype, matrixMtypeShift, matrixMtypeShift);
    case KANA_NEW_STICKNEY:
        return ProcessKana(mod, matrixNewStickney, matrixNewStickneyShift, matrixNewStickneyShift);
    default:
        return XMIT_NONE;
    }
}
