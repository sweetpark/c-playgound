# HINT1
`int c; while ((c = fgetc(fp)) != EOF && c != '\n')` — 담을 때는 `len + 1 < size` 일 때만 `buf[len++] = c`, 아니면 `truncated = 1` 로 표시하고 계속 읽어서 버린다.
루프가 끝나면 `buf[len] = '\0'` 은 항상 한 곳에서. "한 글자도 못 읽은 EOF" 는 읽은 개수 카운터로 구분.
