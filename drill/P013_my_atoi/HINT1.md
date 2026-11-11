# HINT1
`errno = 0; v = strtol(s, &end, 10);` 이후 순서대로: ① `end == s`(숫자 없음) ② `*end != '\0'`(뒤쪽 쓰레기) ③ `errno == ERANGE` ④ `v > INT_MAX || v < INT_MIN`.
`strtol` 은 앞 공백을 알아서 건너뛰므로, 거절하려면 첫 글자를 직접 검사.
