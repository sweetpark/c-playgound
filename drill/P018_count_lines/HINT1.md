# HINT1
조각 단위로 읽으면서 `buf[strlen(buf)-1] == '\n'` 일 때만 `lines++`. 루프 뒤에 마지막 조각이 개행으로 끝나지 않았다면 한 줄 더. `ferror` 는 `fclose` **전에**.
