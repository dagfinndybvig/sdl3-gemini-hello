#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

// Let's use a known good one
const uint16_t nice_heart[] = {
    0x0000,
    0x0660, // ....##..##....
    0x0FF0, // ...####..####...
    0x1FF8, // ..############..
    0x3FFC, // .##############.
    0x3FFC, // .##############.
    0x3FFC, // .##############.
    0x3FFC, // .##############.
    0x1FF8, // ..############..
    0x0FF0, // ...##########...
    0x07E0, // ....########....
    0x03C0, // .....######.....
    0x0180, // ......####......
    0x0000,
    0x0000,
    0x0000
};


void DrawHeart(SDL_Renderer* renderer, float x, float y, float scale) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red
    
    for (int row = 0; row < 16; row++) {
        for (int col = 0; col < 16; col++) {
            // Check if the bit at (15-col) is set (since 0x8000 is the first bit)
             if ((nice_heart[row] >> (15 - col)) & 1) {
                SDL_FRect pixel = { x + col * scale, y + row * scale, scale, scale };
                SDL_RenderFillRect(renderer, &pixel);
            }
        }
    }
}

typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool running;
} AppContext;

void MainLoop(void* arg) {
    AppContext* ctx = (AppContext*)arg;
    SDL_Event event;

    // Handle events
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            ctx->running = false;
#ifdef __EMSCRIPTEN__
            emscripten_cancel_main_loop();
#endif
        }
    }

    // --- Drawing ---
    
    // 1. Clear background to a "Gemini Blue"
    SDL_SetRenderDrawColor(ctx->renderer, 0, 31, 63, 255); 
    SDL_RenderClear(ctx->renderer);

    // 2. Draw the text (600% bigger using scale)
    SDL_SetRenderScale(ctx->renderer, 6.0f, 6.0f);
    
    // Draw "Hello Gemini " in white
    SDL_SetRenderDrawColor(ctx->renderer, 255, 255, 255, 255);
    SDL_RenderDebugText(ctx->renderer, 10, 35, "Hello Gemini ");
    
    // Draw the "proper" heart instead of "<3"
    // One character is 8px wide in debug font. "Hello Gemini " is 13 chars.
    // 13 * 8 = 104px. 
    SDL_SetRenderScale(ctx->renderer, 1.0f, 1.0f); // Reset scale to draw the heart precisely
    
    // Heart position (scaled 104 * 6 = 624, y: 35 * 6 = 210)
    // DrawHeart takes (renderer, x, y, scale)
    // 16x16 pixels * 4.0 scale = 64x64 pixel heart.
    DrawHeart(ctx->renderer, 630, 215, 4.0f);
    
    // Draw "!" in white (after the heart)
    SDL_SetRenderScale(ctx->renderer, 6.0f, 6.0f);
    SDL_SetRenderDrawColor(ctx->renderer, 255, 255, 255, 255);
    SDL_RenderDebugText(ctx->renderer, 114, 35, "!");
    
    SDL_SetRenderScale(ctx->renderer, 1.0f, 1.0f); // Reset scale for other things

    // 3. Present the frame
    SDL_RenderPresent(ctx->renderer);
}

int main(int argc, char* argv[]) {
    // Initialize SDL
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return 1;
    }

    // Create a window and a renderer (Width: 704 * 1.1 = ~774, let's go with 774 or 768)
    AppContext ctx;
    if (!SDL_CreateWindowAndRenderer("SDL3 Hello Gemini", 774, 480, 0, &ctx.window, &ctx.renderer)) {
        SDL_Log("SDL_CreateWindowAndRenderer failed: %s", SDL_GetError());
        return 1;
    }

    ctx.running = true;

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(MainLoop, &ctx, 0, 1);
#else
    while (ctx.running) {
        MainLoop(&ctx);
    }
#endif

    // Cleanup
    SDL_DestroyRenderer(ctx.renderer);
    SDL_DestroyWindow(ctx.window);
    SDL_Quit();

    return 0;
}
