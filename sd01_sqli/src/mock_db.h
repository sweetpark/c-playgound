#ifndef MOCK_DB_H
#define MOCK_DB_H
/* -1: 실패, 0 이상: 매칭된 회원 인덱스 */
int mock_db_login(const char *where_clause);
#endif
