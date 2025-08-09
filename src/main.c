#define NANO_GFX_IMPLEMENTATION
#include "nanogfx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

LRESULT CALLBACK WndProc(HWND h, UINT m, WPARAM w, LPARAM l) { return DefWindowProc(h,m,w,l); }

int WINAPI WinMain(HINSTANCE h, HINSTANCE p, LPSTR cmd, int show) {
    WNDCLASS wc = {0}; wc.lpfnWndProc=WndProc; wc.hInstance=h; wc.lpszClassName="n";
    RegisterClass(&wc);
    HWND hwnd = CreateWindow("n","nanogfx",WS_OVERLAPPEDWINDOW,0,0,800,600,0,0,h,0);
    ShowWindow(hwnd,SW_SHOW);

    NanoGfx g;
    ng_init(&g,hwnd);

    float t = 0.0f;
    unsigned int frame_count = 0;
    double last_time = (double)GetTickCount64() / 1000.0;
    double fps = 0.0;
    double prev_frame_time = last_time;
    const int GRID_SIZE = 32;
    const float BLOCK_SIZE = 0.06f;
    for(;;) {
        MSG msg; while(PeekMessage(&msg,0,0,0,PM_REMOVE)) { if(msg.message==WM_QUIT) return 0; DispatchMessage(&msg); }

        double now = (double)GetTickCount64() / 1000.0;
        double dt = now - prev_frame_time;
        prev_frame_time = now;
        t += (float)dt;

        
        float cam_angle = t * 0.5f;
        float cos_a = cosf(cam_angle);
        float sin_a = sinf(cam_angle);
        for (int x = 0; x < GRID_SIZE; ++x) {
            for (int z = 0; z < GRID_SIZE; ++z) {
                float bx = (x - GRID_SIZE/2) * BLOCK_SIZE;
                float bz = (z - GRID_SIZE/2) * BLOCK_SIZE;
                float by = 0.04f + 0.12f * sinf(t + x*0.3f + z*0.2f);

                float r = 0.3f + 0.7f * ((float)x / GRID_SIZE);
                float g_col = 0.6f + 0.4f * ((float)z / GRID_SIZE);
                float b = 0.2f + 0.8f * sinf(t + x*0.1f + z*0.1f);

                
                float iso[4][2];
                float vtx[4][3] = {
                    {bx,         by+BLOCK_SIZE, bz},           // 4
                    {bx+BLOCK_SIZE, by+BLOCK_SIZE, bz},           // 5
                    {bx+BLOCK_SIZE, by+BLOCK_SIZE, bz+BLOCK_SIZE},// 6
                    {bx,         by+BLOCK_SIZE, bz+BLOCK_SIZE} // 7
                };
                for (int i = 0; i < 4; ++i) {
                    float xw = vtx[i][0], yw = vtx[i][1], zw = vtx[i][2];
                    
                    float xr = xw * cos_a - zw * sin_a;
                    float zr = xw * sin_a + zw * cos_a;
                    
                    float xp = (xr - zr) * 0.7f;
                    float yp = (xr + zr) * 0.35f - yw * 0.8f;
                    iso[i][0] = xp;
                    iso[i][1] = yp;
                }
                
                ng_push_tri(&g, iso[0][0], iso[0][1], 0.0f, iso[1][0], iso[1][1], 0.0f, iso[2][0], iso[2][1], 0.0f, r, g_col, b, 1.0f);
                ng_push_tri(&g, iso[2][0], iso[2][1], 0.0f, iso[3][0], iso[3][1], 0.0f, iso[0][0], iso[0][1], 0.0f, r, g_col, b, 1.0f);
            }
        }

        ng_render(&g);
        frame_count++;
        if (now - last_time >= 1.0) {
            fps = frame_count / (now - last_time);
            char title[64];
            snprintf(title, sizeof(title), "nanogfx - FPS: %.1f | Blocks: %d | Tris/frame: %d", fps, GRID_SIZE*GRID_SIZE, GRID_SIZE*GRID_SIZE*2);
            SetWindowText(hwnd, title);
            frame_count = 0;
            last_time = now;
        }
    }
    ng_shutdown(&g);
}