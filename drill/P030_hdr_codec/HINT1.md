# HINT1
`hdr_validate(ver, type, len)` 를 하나 만들어 encode/decode 가 같이 쓴다. encode 는 `buf[2] = len >> 8; buf[3] = len & 0xFF; buf[4] = id >> 24; …`, decode 는 반대로 시프트해서 합친다.
