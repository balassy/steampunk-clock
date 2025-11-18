## GitHub Copilot instructions for the "steampunk-clock" repository

Purpose
- Provide concise, actionable guidance for GitHub Copilot and other assistant agents working in this repository. The project is firmware for an ESP8266 (Wemos D1 Mini) controlling servos, an RTC, a PCF8574 I2C expander and a few peripherals — safety and non-blocking behavior are paramount.

Project summary (short)
- Platform: ESP8266 (Wemos D1 Mini profile is used).
- Main sketch: `steampunk-clock.ino`.
- Key hardware: RTC module (I2C), PCF8574 I2C expander at 0x20, two servos (hour/minute), status LEDs, active buzzer, selector switch, settings button.
- Important config: `config.h` defines pin mappings, WIFI AP defaults, NTP timezone/server and clock update interval.
- Libraries (versions in `sketch.yaml`): RTCLib, Adafruit BusIO, Adafruit PCF8574, WiFiManager, OneButton.

Read first
- `steampunk-clock.ino` — main program flow and non-blocking loop structure.
- `config.h` — authoritative pin mapping and network defaults. Any change to pins or network defaults requires explicit confirmation from the repo owner.
- `sketch.yaml` — board FQBN and library versions used to build.
- Modules: `rtc-manager.*`, `ntp-manager.*`, `speed-servo.*`, `beep-manager.*`, `selector-switch.*` — each encapsulates hardware responsibilities.

How to behave (high level)
- Prioritize safety: do not make changes that could cause continuous or extreme servo movement or otherwise stress the hardware.
- Preserve non-blocking behavior: avoid introducing `delay()` or other blocking calls in the main loop. If a longer operation is needed, break it into small steps or use asynchronous/event-based approach.
- Keep edits small and focused: one logical change per PR. Provide a "How I tested" section in the PR describing build results and manual hardware checks, when applicable.
- Use the repository's style: `inline constexpr` for constants and `F("...")` for constant Serial strings.

Operating constraints and safety
- This is firmware for an ESP8266 running servos and external hardware. Avoid changes that can cause permanent hardware damage (e.g., wrong PWM ranges, continuous servo movement beyond mechanical stops).
- The main loop intentionally avoids `delay()` to keep `OneButton` and other event handling responsive — preserve that approach.
- Serial output is used for debugging; keep helpful prints under `#ifdef` or guard them when making noisy changes.
- Changing pin assignments requires updating `config.h` and documenting the change in the PR.

Coding style & best practices (repo-specific)
- Use `F("...")` for constant strings sent to `Serial` (already used in the code).
- Prefer `inline constexpr` constants (pattern used in `config.h`).
- Keep heavy operations off the main loop or run them in small incremental steps (non-blocking pattern). If a long operation is required, document why and how interrupts and button handling are preserved.
- Avoid dynamic allocations in tight loops; prefer stack or static objects for resource-constrained ESP8266.

Allowed changes without explicit owner confirmation
- Documentation, README improvements, and tests that don't alter hardware wiring or runtime behavior.
- Small refactors that do not change external behavior (visible state, timing, servo ranges, pin assignments).

Ask before changing
- Any modification to `config.h` (pin mappings, `WIFI_AP_PASSWORD`, `CLOCK_UPDATE_INTERVAL_MSEC`, etc.).
- Changing servo motion ranges, PWM timings, or any code that alters physical movement.
- Upgrading or changing library versions in `sketch.yaml`.
- Changing behavior that affects power-up, reboot, or fallback network logic (e.g., removing WiFiManager AP fallback).

Tone & response format for Copilot
- Be concise and action-oriented. When proposing code changes, provide unified diffs or small patch-style modifications suitable for code review.
- For behavioural changes, list risks, required manual checks, and fallback plans (how to revert if hardware acts unexpectedly).

Common pitfalls to check
- Blocking delays in `loop()` and anywhere `OneButton` or other event-handling is used.
- Unchecked assumptions about I2C device presence — the code currently halts if PCF8574 isn't found; any change here must be conservative and documented.
- Network operations that can cause reboot loops if not guarded (e.g., `initNetwork` calling `ESP.restart()` on failure).

Build and CI
- When a build-related change is made (library versions, board profile), ensure the `compile.yml` workflow is updated accordingly.

---
Keep this file up-to-date when the board profile, pin wiring or library versions change.
