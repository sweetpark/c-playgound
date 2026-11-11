#!/bin/sh
# 모든 문제에 대해: (1) 모범답안 PASS  (2) 백지 skeleton 은 컴파일되고 FAIL 이거나(또는 시험 중 새니타이저가 잡아 비정상 종료) 한다.
# d8 처럼 외부 라이브러리가 필요한 문제는 환경변수 CPATH / LIBRARY_PATH / LD_LIBRARY_PATH 로 위치를 알려 준다.
cd "$(dirname "$0")/.." || exit 1
bad=0
for d in drill/P*_*; do
	n=${d#drill/}; n=${n%%_*}
	out=$(make --no-print-directory "$n" ANS=1 2>&1); rc=$?
	if [ $rc -eq 0 ] && echo "$out" | grep -aq '^PASS'; then
		echo "ok   $n  answer -> $(echo "$out" | grep -a '^PASS' | head -n 1)"
	else
		echo "BAD  $n  answer did not PASS"; echo "$out" | tail -n 15; bad=1
	fi
	rm -f "build/$n"
	out=$(make --no-print-directory "$n" SKEL=1 2>&1); rc=$?
	if [ $rc -ne 0 ] && echo "$out" | grep -aq '^FAIL'; then
		echo "ok   $n  blank  -> $(echo "$out" | grep -a '^FAIL' | head -n 1)"
	elif [ $rc -ne 0 ] && [ -x "build/$n" ]; then
		echo "ok   $n  blank  -> (컴파일됨, 시험 중 비정상 종료: 새니타이저가 스텁의 NULL 등을 잡음)"
	else
		echo "BAD  $n  blank skeleton must compile and FAIL"; echo "$out" | tail -n 15; bad=1
	fi
done
[ $bad -eq 0 ] && echo "ALL OK" || { echo "SOME BAD"; exit 1; }
