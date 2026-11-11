# HINT1
`eq = strchr(line, '=')` 로 위치만. key 구간 `[line, eq)`, val 구간 `[eq+1, 끝)` 각각 앞에서 `while (b < e && blank(*b)) b++`, 뒤에서 `while (e > b && blank(e[-1])) e--`. 모두 검증한 뒤 마지막에 `memcpy`.
