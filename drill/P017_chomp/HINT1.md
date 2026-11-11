# HINT1
먼저 `n = strlen(s)`. `n > 0 && s[n-1] == '\n'` 일 때만 줄이고, 그 앞이 `'\r'` 인지 한 번 더 본다. (또는 `strcspn(s, "\r\n")`)
