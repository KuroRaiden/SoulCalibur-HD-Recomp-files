# Renderer and application boundaries

ReXGlue owns PPC static recompilation, guest memory, threads, kernel/HLE and executable launch. Xenos/D3D12 is the temporary reference renderer. The final renderer will intercept Soulcalibur scene, fighter, stage, mesh/material, effects and UI submissions and translate them into a native scene consumed by SDL3 GPU. Do not build a generic Xenos-to-SDL GPU command translator.

Migration order: frame markers and captures; inspect stage/fighter submission; decode guest vertex/material/texture layouts with confidence ledger; capture native scene alongside reference output; replace one pass at a time; compare UI, effects, depth, blending, lighting and animation. D3DPresentViaVdSwap is an initial reference measurement point, not a high-level renderer replacement boundary. No high-level rendering hooks are installed yet.

SDL3 and Dear ImGui are already provided by the pinned SDK snapshot. Record and use their SDK versions rather than introducing a second SDL/ImGui copy. Controller database dependency will be pinned from gabomdq/SDL_GameControllerDB when integrated; SDL mappings should load before controller discovery. GekkoNet is deferred and must receive an explicit commit pin before integration.

Future Online mode: MenuApplicationHooks reserves a host callback and readiness gate. Wire it into the verified in-game main-menu event boundary after scene/callback layouts are understood. Keep the Online entry hidden/disabled until two-controller offline VS works, fixed-step input is isolated, complete simulation state can be saved/restored, and replay hashes match across repeated runs. Networking and rollback are not implemented. No new item is currently injected into the original menu.

Save/profile: CreateContentContainer and QueryUserSigninInfo identify platform wrappers only. Save payload, RNG, timing, heap ownership, asynchronous I/O and simulation globals remain to be mapped before deterministic state restoration. Versus strings are located but versus simulation boundaries remain unknown.
