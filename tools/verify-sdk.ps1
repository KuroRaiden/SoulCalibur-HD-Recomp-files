$ErrorActionPreference = 'Stop'
$projectRoot = Split-Path $PSScriptRoot -Parent
$snapshotRoot = Join-Path $projectRoot 'thirdparty/rexglue-sdk'
$manifest = Get-Content (Join-Path $projectRoot 'docs/sdk-file-hashes.json') -Raw | ConvertFrom-Json
foreach ($entry in $manifest) {
  $actual = (Get-FileHash -LiteralPath (Join-Path $snapshotRoot $entry.path) -Algorithm SHA256).Hash
  if ($actual -ne $entry.sha256) { throw "SDK integrity mismatch: $($entry.path)" }
}
Write-Output "Verified $($manifest.Count) SDK files."
