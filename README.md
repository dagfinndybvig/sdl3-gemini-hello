# SDL3 Hello Gemini (WebAssembly)

A simple SDL3 application that renders "Hello Gemini" with a custom-drawn heart, compiled to WebAssembly using Emscripten.

## Features
- **SDL3 Core**: Uses the latest SDL3 features for rendering.
- **Custom Heart Rendering**: Draws a pixel-art heart using a bitmask array rather than an image file.
- **Web-Ready**: Configured with `emscripten_set_main_loop` for smooth performance in modern browsers.
- **Gemini Blue**: Custom background color matching the Gemini aesthetic.

## Files
- `main.c`: The C source code.
- `index.html / .js / .wasm`: The compiled WebAssembly output.
- `sdl3_hello`: (Excluded) Native Linux binary.

## How to Compile (Emscripten)
Ensure you have `emsdk` installed and activated, then run:
```bash
emcc main.c -o index.html -sUSE_SDL=3 -sALLOW_MEMORY_GROWTH=1
```

## How to Run Locally
You can serve the directory using a simple web server:
```bash
python3 -m http.server 8080
```
Then visit `http://localhost:8080` in your browser.

## Deployment
This project is designed to be hosted statically on platforms like GitHub Pages, Netlify, or Vercel.
