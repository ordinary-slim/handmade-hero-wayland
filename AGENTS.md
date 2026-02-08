# Agent Guide: handmade-hero-wayland

This repository is a small Wayland client written in C/C++ using CMake.
Use this guide when making changes as an agentic coding assistant.

## Quick Facts
- Primary language: C++ with C headers and Wayland C APIs.
- Build system: CMake (out-of-tree builds).
- Executable: handmade-hero-wayland.
- Generated protocol sources: src/xdg-shell-protocol.c and
  src/xdg-shell-client-protocol.h.

## Cursor/Copilot Rules
- No Cursor rules found in .cursor/rules/ or .cursorrules.
- No Copilot rules found in .github/copilot-instructions.md.

## Build Commands
All commands assume repo root as the working directory.

### Configure (Release)
```
cmake -S . -B build
```

### Build (Release)
```
cmake --build build
```

### Configure (Debug)
```
cmake -S . -B debug_build -DCMAKE_BUILD_TYPE=Debug
```

### Build (Debug)
```
cmake --build debug_build
```

### Run
```
./build/handmade-hero-wayland
```

### Clean
```
cmake --build build --target clean
```

## Lint / Format
- No lint or formatter is configured in this repo.
- If you want one, prefer clang-format with a checked-in config.

## Tests
- No automated tests are currently configured.
- There is no single-test runner because no test framework exists yet.
- If tests are added later, document the target name and single-test flags
  in this file.

## Generated Files
- src/xdg-shell-protocol.c and src/xdg-shell-client-protocol.h are generated
  by wayland-scanner during the build.
- Do not hand-edit generated protocol files.

## Code Style Guidelines
Follow existing conventions from src/*.cpp and src/*.h.

### Formatting
- Indentation: 4 spaces.
- Braces: K&R style with opening brace on the next line for functions.
- Function return type on its own line, then function name on the next line.
- Keep line lengths reasonable; wrap long argument lists and strings.
- Align wrapped parameters by indentation, not by manual spacing columns.
- Use blank lines to separate logical blocks.

### Naming
- Types and structs: snake_case (e.g., struct client_state).
- Functions: snake_case (e.g., wl_pointer_frame, draw_frame).
- Variables: snake_case; short names OK for small scopes (x, y, fd).
- Constants: snake_case or lowercase with const (e.g., const int c_rad).
- Listener instances: wl_*_listener named after protocol object.

### Includes
- Prefer local headers before system headers when both are used.
- Group includes loosely by origin: local headers, then system headers.
- Use <...> for system headers and "..." for local headers.
- Keep includes minimal and specific to what the file uses.

### Types and Initialization
- Use C-style structs with explicit pointers to Wayland types.
- Prefer explicit casts when casting void* data to struct pointers.
- Initialize structs with `{ 0 }` for zeroed fields when needed.
- Use `bool` for boolean flags; initialize to sensible defaults.

### Error Handling
- Follow existing patterns: early returns on errors (NULL or -1).
- For fatal invariants, `assert(...)` is used in some paths.
- Check allocation/mmap results and close fds on failure.
- Avoid throwing exceptions; stick to C-style error returns.

### Memory and Ownership
- Respect Wayland object lifetimes; destroy or release when no longer needed.
- Shared memory buffers are created per-frame and destroyed by compositor
  via wl_buffer_listener.
- Always unmap shared memory after use.

### Wayland Protocol Usage
- Listener structs are static const and defined near their callbacks.
- Keep protocol callback signatures exactly as defined by Wayland headers.
- Acknowledge xdg_surface configure events before drawing.

### Logging and Diagnostics
- Current code uses fprintf(stderr, ...) for input events.
- Keep logging minimal and avoid spamming in hot paths unless needed.

### File Layout
- Headers provide forward declarations and extern listener declarations.
- Implementations keep listeners at the top and callbacks below.

## Agent Workflow Expectations
- Prefer small, incremental changes that fit existing style.
- Update this AGENTS.md when adding build tools, linting, or tests.
- Avoid introducing new dependencies without documenting them here.

## If You Add Tests Later
When a test framework is introduced, update:
- `## Tests` with full-suite and single-test commands.
- `## Build Commands` if tests require extra targets.
