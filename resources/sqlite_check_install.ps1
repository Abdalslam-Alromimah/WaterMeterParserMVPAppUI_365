# Path to SQLite executable
$SQLitePath = "C:\\Program Files\\sqlite"


# Path to database
$DbPath = "C:\Qtbackup\16-01-2025\WaterMeterParserMVPAppUI_365\resources\db"
$DbFile = "$DbPath\ex2.db"

# Check if SQLite is installed
$SQLiteExecutable = "sqlite3.exe"
if (-not (Get-Command $SQLiteExecutable -ErrorAction SilentlyContinue)) {
    Write-Host "SQLite is not installed. Installing SQLite..."

    # Download SQLite (adjust the link as needed)
    $SQLiteZipUrl = "https://sqlite.org/2025/sqlite-tools-win32-x86-3390200.zip"
    $ZipFile = "sqlite.zip"
    Invoke-WebRequest -Uri $SQLiteZipUrl -OutFile $ZipFile

    # Unzip SQLite tools
    Expand-Archive -Path $ZipFile -DestinationPath $SQLitePath

    # Add SQLite to system PATH (for this session)
    $env:PATH = "$SQLitePath;$env:PATH"

    # Verify SQLite installation
    & "$SQLitePath\sqlite3.exe" --version
} else {
    Write-Host "SQLite is already installed."
}

# Set the path for the SQLite database in the 'Db' folder within the same folder as this script
$DbFolderPath = Join-Path $PSScriptRoot "Db"
$DbFile = Join-Path $DbFolderPath "database.db"

# Create the Db folder if it doesn't exist
if (-not (Test-Path $DbFolderPath)) {
    Write-Host "Creating Db folder at $DbFolderPath"
    New-Item -ItemType Directory -Path $DbFolderPath
}

# Create database if it doesn't exist
if (-not (Test-Path $DbFile)) {
    Write-Host "Creating SQLite database at $DbFile"
    & "$SQLitePath\sqlite3.exe" $DbFile "VACUUM;"
} else {
    Write-Host "Database $DbFile already exists."
}

