# HINT1
`fopen(path, "wx")` (C11) 는 파일이 있으면 실패하고 없으면 원자적으로 만든다. 실패했을 때 `errno == EEXIST` 인지 본다.
