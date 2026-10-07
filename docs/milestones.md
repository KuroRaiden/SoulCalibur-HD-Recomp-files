# Research milestones and timing observations

Status: discontinued, 2026-10-07.

Title: SOULCALIBUR XBLA, title ID 58410904, version 0.0.1.2. See title-metadata.json for hash and additional identity fields.

IDA analyzed 7,018 functions. Guest CRT entrypoint confirmed. Four missed tail targets, a JPEG callback, a UI delegate, and guest setjmp/longjmp boundaries were supplied to strict codegen. Initial analyze/codegen completed locally in approximately 11 seconds; generated game sources are excluded.

The Windows executable compiled and linked against the pinned modified SDK. Offline battles and controller-driven Play Game navigation were observed. The menu transition blocker was the unsigned local controller/profile gate; the integration aliases valid local controller users to the runtime's single local profile. Distinct user profiles are not implemented.

The optimized guest build retained Debug runtime ABI and improved battle speed in user testing. Frame diagnostics measured roughly 60 presents/second in menus and about 30 in one battle interval (300 presents over 10.02 seconds). These are present-call observations, not proof of simulation rate or guaranteed performance. Character selection remained slow.

Audio mixed nonzero PCM, then the default backend stalled on focus loss. A process-start DirectSound experiment kept PCM flowing for over 40 seconds after focus loss, and the user confirmed audible sound. A first persisted setting was too late because SDL caches its environment before the application factory. The final code moved selection before main and the launch script also sets the environment. The corrected normal-launch test continued submitting audio after focus loss; final long-term user confirmation was not obtained.

Existing host-math rounding and keyboard-controller-state tests passed. Save/reload, deterministic restore, rollback, Online, complete stage coverage, and a native renderer were not completed.
