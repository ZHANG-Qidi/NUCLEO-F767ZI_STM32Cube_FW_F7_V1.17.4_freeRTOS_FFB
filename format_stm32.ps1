# format_stm32.ps1
# Description: Format STM32/Pico projects, supporting sub-projects and root-level sources

# ===== CONFIGURATION =====
# Subdirectories to treat as separate project roots (e.g., "CM7", "CM4")
$projectSubDirs = @("CM7", "CM4", "components")

# Target folders (recursively processed regardless of $processEntireRoot)
$targetFolders = @("Inc", "Src", "ArduinoCore-avr", "Arduino-FOC", "main", "ArduinoCore-avr-1.8.7", "Arduino-FOC-2.2", "HID-FFB", "HID-FFB-0.1")

# File extensions to format
$extensions = @("*.c", "*.h", "*.cpp", "*.hpp", "*.cc", "*.cxx", "*.hh")

# Set to $true to also process first-level files in the project root (non-recursive)
# If $true: processes root-level files + recursively processes $targetFolders
# If $false: only recursively processes $targetFolders
$processEntireRoot = $true

# ===== END CONFIGURATION =====

# Check clang-format availability
if (-not (Get-Command clang-format -ErrorAction SilentlyContinue)) {
    Write-Host "Error: clang-format not found. Please check PATH." -ForegroundColor Red
    exit 1
}

# Helper: filter files by extension using Where-Object (more reliable than -Include)
function Get-FilesByExtensions {
    param([string]$path, [switch]$recurse)
    $files = Get-ChildItem -Path $path -File -Recurse:$recurse
    $files | Where-Object { $extensions -contains ("*" + $_.Extension) }
}

# Process a project root
function Process-ProjectRoot {
    param([string]$rootPath, [string]$projectName)

    # 1. Recursively process target subfolders (always)
    $processedAny = $false
    foreach ($folder in $targetFolders) {
        $folderPath = Join-Path $rootPath $folder
        if (Test-Path $folderPath -PathType Container) {
            Write-Host "  Processing subfolder (recursive): $folderPath"
            $processedAny = $true
            $files = Get-FilesByExtensions -path $folderPath -recurse
            foreach ($file in $files) {
                Write-Host "    Would format: $($file.FullName)"
                clang-format -i -style=file $file.FullName   # Uncomment to actually format
            }
        }
    }
    if (-not $processedAny -and -not $processEntireRoot) {
        Write-Host "  No target subfolders found in $projectName" -ForegroundColor Gray
    }

    # 2. If enabled, process root-level files (non-recursive)
    if ($processEntireRoot) {
        Write-Host "  Processing root-level files (non-recursive): $rootPath"
        $rootFiles = Get-FilesByExtensions -path $rootPath -recurse:$false
        if ($rootFiles.Count -gt 0) {
            foreach ($file in $rootFiles) {
                Write-Host "    Would format: $($file.FullName)"
                clang-format -i -style=file $file.FullName   # Uncomment to actually format
            }
        } else {
            Write-Host "    No matching source files at root level." -ForegroundColor Gray
        }
    }
}

# Use the directory containing this script as the project root
$projectRoot = $PSScriptRoot
$projectName = Split-Path -Leaf $projectRoot

if (-not (Test-Path -LiteralPath $projectRoot -PathType Container)) {
    Write-Host "Project root not found: $projectRoot" -ForegroundColor Red
    exit 1
}

Write-Host "Checking project: $projectName"

# Process the main project root
Process-ProjectRoot -rootPath $projectRoot -projectName $projectName

# Process special sub-projects (e.g., CM7, CM4) if they exist
foreach ($sub in $projectSubDirs) {
    $subPath = Join-Path $projectRoot $sub
    if (Test-Path -LiteralPath $subPath -PathType Container) {
        Write-Host "  Found sub-project: $sub"
        Process-ProjectRoot -rootPath $subPath -projectName "$projectName\$sub"
    }
}

Write-Host "Project processed." -ForegroundColor Green