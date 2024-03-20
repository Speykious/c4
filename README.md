# C4

> A rhythm game from absolute scratch in C (if I stop procrastinating)

This is my attempt at making anything that resembles a game from absolute scratch, doing everything myself, from memory management to windowing to graphics to audio. Honestly knowing myself I don't think it's gonna go anywhere since I procrastinate a lot, but maybe this time I'll go somewhere.

I'll keep [notes](/notes.md) about my experience coding it as I go.

## Status

I barely have the start of a platform layer. This is gonna take a while.

- [ ] platform layer
  - [x] allocators
    - [x] arena
  - [x] allocating virtual memory
  - [ ] windowing (Linux, X11)
  - [ ] audio (Linux, ALSA)
    - how's the audio latency??
  - [ ] graphics (framebuffer)

- [ ] audio
  - [ ] audio mixer
  - [ ] FFT analyzer (optional)
  - [ ] audio spectrum (optional)

- [ ] graphics (2D)
  - [ ] text rendering
  - [ ] simple shapes
  - [ ] images

- [ ] game (bruh)
  - [ ] separate threads (rendering, input, update, audio)
  - [ ] rendering: simple sprites
  - [ ] game mechanics (basic 4k vsrg)
    - [ ] slap music on top of it
  - [ ] simple immediate-mode UI
  - [ ] chart file format

## License

This project is licensed under the [MIT license](/LICENSE).
