# HINT1
- 구조체: `max`, `last`, `has_last`.
- `value < last` 일 때만 WRAP/RESET 을 가른다. 조건: `last > max - max/4`.
- `wavg`: `sum(avg*cnt) / sum(cnt)`. 합이 0 이면 0.0.
