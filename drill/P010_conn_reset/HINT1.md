# HINT1
`memset(c, 0, sizeof(*c))` — `sizeof(c)` 는 포인터 크기(8), `sizeof(*c)` 는 구조체 크기. 포인터가 가리키는 타입에서 크기를 얻는 습관(`malloc(sizeof(*p))` 와 같은 이유).
호스트는 `snprintf(c->host, sizeof(c->host), "%s", host)` — 여기서 `c->host` 는 배열이라 `sizeof` 가 32 가 나온다.
