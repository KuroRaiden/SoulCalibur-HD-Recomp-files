$ErrorActionPreference = 'Stop'
$projectRoot = Split-Path $PSScriptRoot -Parent
Push-Location $projectRoot
try {
  & rexglue codegen soulcalibur_hd_manifest.toml
  if ($LASTEXITCODE -ne 0) { throw "ReXGlue codegen failed: $LASTEXITCODE" }
} finally { Pop-Location }
