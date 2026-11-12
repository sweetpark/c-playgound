# HINT1
구조체: 임계 4개, `on`, `above`(연속 초과), `below`(연속 정상), `n_occ`, `n_clr`.

- 꺼짐: `above = (value >= set) ? above + 1 : 0;` → `above >= set_n` 이면 켠다.
- 켜짐: `below = (value < clear) ? below + 1 : 0;` → `below >= clear_n` 이면 끈다.
- 상태를 바꾸는 순간 `above`, `below` 를 0 으로.
