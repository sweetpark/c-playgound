# HINT1
`switch (lv) { case LV_DEBUG: return "DEBUG"; … default: return "UNKNOWN"; }`. 파싱은 `for (i = 0; i < (int)LV_COUNT; i++) if (strcasecmp(s, level_name(i)) == 0) return i;`.
