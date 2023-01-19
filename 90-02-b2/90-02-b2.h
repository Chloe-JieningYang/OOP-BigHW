/* 2050598 Ñî½àÄþ Í¨ÐÅ */
#pragma once

void CreateMatrix(int g2048[][10], const CONSOLE_GRAPHICS_INFO CGI);

void CombineSame(int g2048[][10], int mark[][10],const CONSOLE_GRAPHICS_INFO CGI, const int type, int& score);

void RemoveZero(int g2048[][10], const CONSOLE_GRAPHICS_INFO CGI, const int type);

bool JudgeSame(const int g2048_old[][10], const int g2048[][10]);

int SearchForMax(int g2048[][10]);

bool FindSameAhead(const int g2048[][10], const int i0, const int j0, const int row, const int col, const int type);

void AddNewNumber(const CONSOLE_GRAPHICS_INFO CGI, int g2048[][10], int& i0, int& j0);

void CompleteGame(CONSOLE_GRAPHICS_INFO CGI, int& score, int aim, int& highest, const int col, const int row, const int delaytime, bool& quit, bool& restart);

bool JudgeEnd(const int g2048[][10], const CONSOLE_GRAPHICS_INFO CGI);