#pragma once

enum Teensy_Pins {
  PIN_ADC_CS = 5,
  PIN_ADC_DIN = 4,
  PIN_ADC_DOUT = 3,
  PIN_ADC_CLK = 2,

  PIN_SLD_DIN = 10,
  PIN_SLD_CLK = 11,
  PIN_SLD_CS = 12,

  PIN_POT_RED_1 = 23,
  PIN_POT_BLACK_1 = 22,
  PIN_POT_RED_2 = 21,
  PIN_POT_BLACK_2 = 20
};

enum MCP_1_Pins {
  PIN_PB_1 = 7,
  PIN_PB_2 = 6,
  PIN_PB_3 = 5,
  PIN_PB_4 = 4,
  PIN_PB_5 = 3,
  PIN_PB_6 = 2,
  PIN_PB_7 = 1,
  PIN_PB_8 = 0,

  PIN_PB_9 = 8,
  PIN_PB_10 = 9,
  PIN_PB_11 = 10,
  PIN_PB_12 = 11,
  PIN_PB_13 = 12,
  PIN_PB_14 = 13,
  PIN_PB_15 = 14,
  PIN_PB_16 = 15
};

enum MCP_2_Pins {
  PIN_PB_INST_1 = 8,
  PIN_PB_INST_2 = 9,
  PIN_PB_INST_3 = 10,
  PIN_PB_INST_4 = 11,
  PIN_PB_YELLOW = 12,
  PIN_PB_RED = 13,
  PIN_PB_BLUE = 14,
  PIN_PB_GREEN = 15,

  PIN_LED_INST_1 = 7,
  PIN_LED_INST_2 = 6,
  PIN_LED_INST_3 = 5,
  PIN_LED_INST_4 = 4,
  PIN_LED_YELLOW = 3,
  PIN_LED_RED = 2,
  PIN_LED_BLUE = 1,
  PIN_LED_GREEN = 0,
};

enum MCP_3_Pins {
  PIN_PB_SAVE = 0,
  PIN_PB_LOAD = 1,
};

enum MCP_3008_Pins {
  PIN_ADC_INST_1_TOP = 0,
  PIN_ADC_INST_1_BOT = 1,
  PIN_ADC_INST_2_TOP = 2,
  PIN_ADC_INST_2_BOT = 3,
  PIN_ADC_INST_3_TOP = 4,
  PIN_ADC_INST_3_BOT = 5,
  PIN_ADC_INST_4_TOP = 6,
  PIN_ADC_INST_4_BOT = 7
};