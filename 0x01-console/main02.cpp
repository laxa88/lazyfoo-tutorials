// Tutorial
// https://www.youtube.com/watch?v=xW8skO7MFYw
#include <iostream>
#include <chrono>
using namespace std;

#include <Windows.h>
#include <math.h>
#include <vector>
#include <algorithm>

int nScreenWidth = 120;
int nScreenHeight = 40;

float fPlayerX = 8.0f;
float fPlayerY = 8.0f;
float fPlayerA = 0.0f;

int nMapWidth = 16;
int nMapHeight = 16;

float fPOV = 3.14159 / 4.0f;
float fDepth = 16.0f;

float fTurnSpeed = 1.2f;
float fMoveSpeed = 5.0f;

int main()
{
    wchar_t *screen = new wchar_t[nScreenWidth * nScreenHeight];
    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(hConsole);
    DWORD dwBytesWritten = 0;

    wstring map;
    map += L"################";
    map += L"#..........#...#";
    map += L"#..........#...#";
    map += L"#...#......#...#";
    map += L"#..........#...#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#"; // you are here
    map += L"#..............#";
    map += L"########.......#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#.......########";
    map += L"#..............#";
    map += L"#..............#";
    map += L"################";

    auto tp1 = chrono::system_clock::now();
    auto tp2 = chrono::system_clock::now();

    vector<float> fpses;
    float fTotalFps = 0.0;

    while (1)
    {
        tp2 = chrono::system_clock::now();
        chrono::duration<float> elapsedTime = tp2 - tp1;
        tp1 = tp2;
        float fElapsedTime = elapsedTime.count();

        // ==================================================
        // Turning
        // ==================================================

        if (GetAsyncKeyState(VK_LEFT))
        {
            fPlayerA -= fTurnSpeed * fElapsedTime;
        }

        if (GetAsyncKeyState(VK_RIGHT))
        {
            fPlayerA += fTurnSpeed * fElapsedTime;
        }

        // ==================================================
        // WSAD movement
        // ==================================================

        if (GetAsyncKeyState((unsigned short)'W') & 0x8000)
        {
            fPlayerX += sinf(fPlayerA) * fMoveSpeed * fElapsedTime;
            fPlayerY += cosf(fPlayerA) * fMoveSpeed * fElapsedTime;

            if (map[(int)fPlayerY * nMapWidth + (int)fPlayerX] == '#')
            {
                fPlayerX -= sinf(fPlayerA) * fMoveSpeed * fElapsedTime;
                fPlayerY -= cosf(fPlayerA) * fMoveSpeed * fElapsedTime;
            }
        }

        if (GetAsyncKeyState((unsigned short)'S') & 0x8000)
        {
            fPlayerX -= sinf(fPlayerA) * fMoveSpeed * fElapsedTime;
            fPlayerY -= cosf(fPlayerA) * fMoveSpeed * fElapsedTime;

            if (map[(int)fPlayerY * nMapWidth + (int)fPlayerX] == '#')
            {
                fPlayerX += sinf(fPlayerA) * fMoveSpeed * fElapsedTime;
                fPlayerY += cosf(fPlayerA) * fMoveSpeed * fElapsedTime;
            }
        }

        if (GetAsyncKeyState((unsigned short)'A') & 0x8000)
        {
            fPlayerX -= cosf(fPlayerA) * fMoveSpeed * fElapsedTime;
            fPlayerY += sinf(fPlayerA) * fMoveSpeed * fElapsedTime;

            if (map[(int)fPlayerY * nMapWidth + (int)fPlayerX] == '#')
            {
                fPlayerX += cosf(fPlayerA) * fMoveSpeed * fElapsedTime;
                fPlayerY -= sinf(fPlayerA) * fMoveSpeed * fElapsedTime;
            }
        }

        if (GetAsyncKeyState((unsigned short)'D') & 0x8000)
        {
            fPlayerX += cosf(fPlayerA) * fMoveSpeed * fElapsedTime;
            fPlayerY -= sinf(fPlayerA) * fMoveSpeed * fElapsedTime;

            if (map[(int)fPlayerY * nMapWidth + (int)fPlayerX] == '#')
            {
                fPlayerX -= cosf(fPlayerA) * fMoveSpeed * fElapsedTime;
                fPlayerY += sinf(fPlayerA) * fMoveSpeed * fElapsedTime;
            }
        }

        // ==================================================
        // Ray tracing render
        // ==================================================

        for (int x = 0; x < nScreenWidth; x++)
        {
            float fRayAngle = (fPlayerA - fPOV / 2.0f) + ((float)x / (float)nScreenWidth) * fPOV;

            float fDistanceToWall = 0;
            bool bHitWall = false;
            bool bBoundary = false;

            float fEyeX = sinf(fRayAngle);
            float fEyeY = cosf(fRayAngle);

            while (!bHitWall && fDistanceToWall < fDepth)
            {
                fDistanceToWall += 0.1f;

                int nTestX = (int)(fPlayerX + fEyeX * fDistanceToWall);
                int nTestY = (int)(fPlayerY + fEyeY * fDistanceToWall);

                if (nTestX < 0 || nTestX >= nMapWidth || nTestY < 0 || nTestY >= nMapHeight)
                {
                    bHitWall = true;
                    fDistanceToWall = fDepth;
                }
                else
                {
                    if (map[nTestY * nMapWidth + nTestX] == '#')
                    {
                        bHitWall = true;

                        vector<pair<float, float>> p; // distance, dot

                        for (int tx = 0; tx < 2; tx++)
                        {
                            for (int ty = 0; ty < 2; ty++)
                            {
                                float vy = (float)nTestY + ty - fPlayerY;
                                float vx = (float)nTestX + tx - fPlayerX;
                                float d = sqrt(vx * vx + vy * vy);
                                float dot = (fEyeX * vx / d) + (fEyeY * vy / d);
                                p.push_back(make_pair(d, dot));
                            }
                        }

                        sort(p.begin(), p.end(), [](const pair<float, float> &left, const pair<float, float> &right) { return left.first < right.first; });

                        float fBound = 0.01;
                        if (acos(p.at(0).second) < fBound)
                            bBoundary = true;
                        if (acos(p.at(1).second) < fBound)
                            bBoundary = true;
                        // if (acos(p.at(2).second) < fBound)
                        //     bBoundary = true;
                    }
                }
            }

            int nCeiling = (float)(nScreenHeight / 2.0) - nScreenHeight / ((float)fDistanceToWall);
            int nFloor = nScreenHeight - nCeiling;

            short nShade = ' ';
            if (fDistanceToWall <= fDepth / 4.0f)
                nShade = 0x2588;
            else if (fDistanceToWall < fDepth / 3.0f)
                nShade = 0x2593;
            else if (fDistanceToWall < fDepth / 2.0f)
                nShade = 0x2592;
            else if (fDistanceToWall < fDepth)
                nShade = 0x2591;
            else
                nShade = ' ';

            if (bBoundary)
                nShade = ' ';

            for (int y = 0; y < nScreenHeight; y++)
            {
                if (y <= nCeiling)
                {
                    screen[y * nScreenWidth + x] = ' ';
                }
                else if (y > nCeiling && y <= nFloor)
                {
                    screen[y * nScreenWidth + x] = nShade;
                }
                else
                {
                    float b = 1.0f - (((float)y - nScreenHeight / 2.0f) / ((float)nScreenHeight / 2.0f));
                    if (b < 0.25)
                        nShade = '#';
                    else if (b < 0.5)
                        nShade = 'x';
                    else if (b < 0.75)
                        nShade = '.';
                    else if (b < 0.9)
                        nShade = '-';
                    else
                        nShade = ' ';
                    screen[y * nScreenWidth + x] = nShade;
                }
            }
        }

        // ==================================================
        // Map and stats
        // ==================================================

        if (fElapsedTime < 0.01f)
        {
            fElapsedTime = 0.01f;
        }
        float fCurrFps = 1.0f / fElapsedTime;
        fTotalFps += fCurrFps;
        fpses.push_back(fCurrFps);
        if (fpses.size() > 60)
        {
            fTotalFps -= fpses.front();
            fpses.erase(fpses.begin());
        }
        float fFps = fTotalFps / (float)fpses.size();

        swprintf(screen, (const wchar_t *)60, L"LAKSAPEDIA.COM X=%3.1f, Y=%3.1f, A=%3.1f FPS=%3.1f", fPlayerX, fPlayerY, fPlayerA, fFps);

        for (int nx = 0; nx < nMapWidth; nx++)
        {
            for (int ny = 0; ny < nMapHeight; ny++)
            {
                screen[(ny + 1) * nScreenWidth + nx] = map[ny * nMapWidth + (nMapWidth - nx - 1)];
            }
        }

        screen[((int)fPlayerY + 1) * nScreenWidth + (int)(nMapWidth - fPlayerX)] = 'x';

        // ==================================================
        // Output to screen
        // ==================================================

        screen[nScreenWidth * nScreenHeight - 1] = '\0';
        WriteConsoleOutputCharacterW(hConsole, screen, nScreenWidth * nScreenHeight, {0, 0}, &dwBytesWritten);
    }

    return 0;
}