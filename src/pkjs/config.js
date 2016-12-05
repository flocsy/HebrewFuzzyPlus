/**
  * Copyright (c) 2016 Gavriel Fleischer <flocsy@gmail.com>
  */

const RTLTR_CONFIG = require('pebble-rtltr/config');

module.exports = [
  {
    "type": "heading",
    "defaultValue": "App Configuration"
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Appearance"
      },
      {
        "type": "radiogroup",
        "messageKey": "FONT",
        "label": "Font",
        "defaultValue": "HANDWRITING",
        "options": [
          {
            "label": "Simple",
            "value": "SIMPLE"
          },
          {
            "label": "Handwriting",
            "value": "HANDWRITING"
          },
          {
            "label": "Tora Ashkenaz",
            "value": "TORA_ASHKENAZ"
          },
          {
            "label": "Maccabi",
            "value": "MACCABI"
          },
          {
            "label": "Rashi",
            "value": "RASHI"
          }
        ]
      }
    ]
  },
  RTLTR_CONFIG,
  {
    "type": "submit",
    "defaultValue": "Save Settings"
  }
];
