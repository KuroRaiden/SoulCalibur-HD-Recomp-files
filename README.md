# Soulcalibur HD ReXGlue research source archive

Development discontinued on 2026-10-07. This is an unfinished research archive, not a supported game release or a finished recompilation.

## Included

- `interop/symbols.csv`: discovered guest addresses, readable names, purpose, confidence, source, and notes.
- `interop/functions.toml`: ReXGlue naming/discovery table and guest setjmp/longjmp boundaries.
- `interop/structures.json`: partial guest structure layouts with evidence and confidence.
- Handwritten runtime hooks, launcher, controller support, diagnostics, tests, and native-renderer/menu interface scaffolding.
- Title identity metadata, SDK revision/provenance and integrity hashes, dependency notes, and build/codegen configuration.
- `docs/milestones.md`: summarized analysis, codegen, build and timing observations.

## Excluded

Game executables, game assets, generated PPC/decompiled/disassembled game code, IDA databases, SDK binaries/source copies, builds, saves, caches, logs, dumps, screenshots, local repository history, and machine-specific paths. The original private workspace is preserved separately.

## State at discontinuation

The local prototype reached offline battles using the reference Xenos renderer. The native SDL3 GPU renderer and Online mode were not implemented. Most functions remain unidentified. Character selection was slow, Help & Options had text corruption, and save/restore determinism was not established. True 16:9 scene rendering was not implemented; borders were retained. Audio had a tested DirectSound workaround, with startup-timing correction; broad reliability was not established.

## Reconstructing a local build

Supply your own game dump at `SoulCalibur/default.xex`; confirm its identity using `docs/title-metadata.json`. Install ReXGlue separately. The original SDK reported `0.9.0.1-dev.gcb58065`, source HEAD `cb58065c793429aa92895d778af58d12e9d26d8f`, with pre-existing local modifications. Those modifications and binaries are not distributed. A pristine upstream revision is not claimed to reproduce the local build.

From a Windows developer prompt with Clang, CMake and Ninja available, run `tools/codegen.ps1`, then `tools/build.cmd`. Set `CMAKE_PREFIX_PATH` or `rexglue_DIR` to your SDK installation. ReXGlue codegen must recreate both `generated/default` and the SDK CMake integration before compiling. Use `tools/verify-sdk.ps1` only if you have the exact historical private SDK snapshot; the hash manifest is provenance, not a download.

The original integration used Debug SDK binaries and Debug ABI. Optimized guest code retained that ABI. Do not mix incompatible runtime configurations. The portable packaging scripts have been syntax checked, but this archive has not been rebuilt from a clean upstream SDK.

## Licensing and attribution

Original project material is released under BSD-3-Clause; see LICENSE and NOTICE.md for scope and preserved third-party rights. Complete dependency license texts are provided for reference under licenses/dependencies. See docs/release-legal.md for the source-release record and its limits. No rights to the game or its assets are granted.
