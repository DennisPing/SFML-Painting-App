# Simple Painting App

Dennis Ping

## Background

This program was originally designed in Foundations of Software Engineering (CS-5500) using C++ and SFML. All pixel locations and colors were computed with the CPU.

## Improvement

I improved the CPU and Memory usage significantly by utilizing the GPU to render shapes and display them onto the screen while only consuming about 40% of the GPU on a 2018 Macbook Pro 13.

## Features

- Draw with 8 different colors (black, white, red, green, blue, yellow, magenta, cyan)
- Variable cursor color to match the paintbrush color
- Variable paintbrush thickness (5 different sizes)
- Variable cursor size to match the paintbrush thickness
- Undo entire strokes of the paintbrush
- Redo entire strokes of the paintbrush
- Unlimited undo/redo (up to physical RAM capacity)
- Free memory in the redo stack after drawing something
- No memory leaks because all pointers are implemented with `smart_ptr`
- Smooth edge painting (no lag or stutter when using a large paintbrush on the edges)
- Antialiasing! No jagged edges because GPU is rendering the pixels.

## How to Build

cd into the bin directory
```txt
cmake ..
make
```

## How to Run

```txt
./App
```
