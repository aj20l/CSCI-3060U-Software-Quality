# Locate the executable
$EXECUTABLE = ""
if (Test-Path ".\build\main.exe") {
    $EXECUTABLE = ".\build\main.exe"
} elseif (Test-Path ".\build\Debug\main.exe") {
    $EXECUTABLE = ".\build\Debug\main.exe"
} else {
    Write-Host "Error: Could not find main.exe!" -ForegroundColor Red
    Write-Host "Please compile via CMake before running this script." -ForegroundColor Yellow
    exit 1
}

# Configuration & Counters
$TIMEOUT_MS = 200 # Max time (in milliseconds) allowed per test before assuming an infinite loop
$PASSED = 0
$FAILED = 0

Write-Host "========================================" -ForegroundColor Cyan
Write-Host " Starting Automated Regression Tests..." -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan

# Iterate dynamically through all test cases
$test_dirs = Get-ChildItem -Path ".\test-files\*\*" -Directory

foreach ($dir in $test_dirs) {
    $test_name = $dir.Name

    $input_file    = Join-Path $dir.FullName "${test_name}-IN.txt"
    $expected_file = Join-Path $dir.FullName "${test_name}-OUT.txt"
    $actual_file   = Join-Path $dir.FullName "${test_name}-ACTUAL.txt"

    if ((Test-Path $input_file) -and (Test-Path $expected_file)) {

        # Sanitize input file on the fly
        # Converts smart quotes to clean plain quotes using Unicode point values
        $input_raw = Get-Content $input_file -Raw
        $input_sanitized = $input_raw.Replace([char]0x201C, '"').Replace([char]0x201D, '"').Replace([char]0x2018, "'").Replace([char]0x2019, "'")

        # Test mode flag enabled
        #$input_sanitized | & $EXECUTABLE --test 2>&1 | Out-File -FilePath $actual_file -Encoding utf8

        Write-Host "Exit code: $LASTEXITCODE"

        # Configure the process to run with a strict timeout
        $psi = New-Object System.Diagnostics.ProcessStartInfo
        $psi.FileName = (Resolve-Path $EXECUTABLE).Path
        #$psi.Arguments = "--test"
        $psi.RedirectStandardInput = $true
        $psi.RedirectStandardOutput = $true
        $psi.RedirectStandardError = $true
        $psi.UseShellExecute = $false
        $psi.CreateNoWindow = $true

        $process = New-Object System.Diagnostics.Process
        $process.StartInfo = $psi

        # Start the C++ executable
        $process.Start() | Out-Null

        # Feed sanitized input into StandardInput and close the stream so the app knows input ended
        $process.StandardInput.Write($input_sanitized)
        $process.StandardInput.Close()

        # Read output asynchronously to prevent buffer deadlocks during infinite loops
        $stdoutTask = $process.StandardOutput.ReadToEndAsync()
        $stderrTask = $process.StandardError.ReadToEndAsync()

        # Wait up to $TIMEOUT_MS for the test to finish
        $completed = $process.WaitForExit($TIMEOUT_MS)

        if (-not $completed) {
            # Infinite loop or hang detected: kill the process and skip to the next test
            $process.Kill()
            $process.Dispose()
            Write-Host "Test $($test_name): FAILED (Timeout / Infinite Loop Detected after $($TIMEOUT_MS)ms)" -ForegroundColor Red
            $FAILED++
            continue
        }

        # Combine stdout and stderr and save to the actual file, matching original 2>&1 behavior
        $combinedOutput = $stdoutTask.Result + $stderrTask.Result
        $combinedOutput | Out-File -FilePath $actual_file -Encoding utf8
        $process.Dispose()

        # Filter out constructor debugging prints
        $raw_actual = Get-Content $actual_file
        $filtered_actual = @()
        $skipping = $true

        foreach ($line in $raw_actual) {
            if ($line -like "*===== STORE OPTIONS =====*") {
                $skipping = $false
            }
            if (-not $skipping) {
                $filtered_actual += $line
            }
        }

        # Encoding-Proof sanitization for evaluation
        # Converts lines to decimal byte codes to completely avoid string character rendering bugs.
        # This leaves ONLY plain letters, numbers, spaces, and safe punctuation symbols (like colons/dashes).
        $expected_content = Get-Content $expected_file | Where-Object { $_.Trim() -ne "" } | ForEach-Object {
            $bytes = [System.Text.Encoding]::UTF8.GetBytes($_.Trim())
            $cleanBytes = @()
            foreach ($b in $bytes) {
                # Keep alphanumeric space characters (ASCII 32, 45-58, 65-90, 95, 97-122)
                if (($b -eq 32) -or ($b -eq 45) -or (($b -ge 48) -and ($b -le 58)) -or (($b -ge 65) -and ($b -le 90)) -or ($b -eq 95) -or (($b -ge 97) -and ($b -le 122))) {
                    $cleanBytes += $b
                }
            }
            [System.Text.Encoding]::ASCII.GetString($cleanBytes).Trim()
        }

        # Read plain raw input lines to interleave and reconstruct what the console output should look like
        $input_lines = Get-Content $input_file | Where-Object { $_.Trim() -ne "" }
        $input_index = 0

        $actual_content = $filtered_actual | Where-Object { $_.Trim() -ne "" } | ForEach-Object {
            $line = $_

            # Fix "Enter choice: ---------------------------" split behavior
            if ($line -like "*Enter choice: ---------------------------*") {
                if ($input_index -lt $input_lines.Count) {
                    $line = $line -replace "Enter choice: ", "Enter choice: $($input_lines[$input_index])`n"
                    $input_index++
                }
            }

            # Fix bunched up user credentials prompt line
            if ($line -like "*Enter your username: Enter your password: *") {
                if (($input_index + 1) -lt $input_lines.Count) {
                    $user = $input_lines[$input_index]
                    $pass = $input_lines[$input_index+1]
                    $line = $line -replace "Enter your username: Enter your password: ", "Enter your username: ${user}`nEnter your password: ${pass}`n"
                    $input_index += 2
                }
            }

            # Fix bunched up game listing details line
            if ($line -like "*Enter name of game: Enter price of game: *") {
                if (($input_index + 1) -lt $input_lines.Count) {
                    $game = $input_lines[$input_index]
                    $price = $input_lines[$input_index+1]
                    $line = $line -replace "Enter name of game: Enter price of game: ", "Enter name of game: ${game}`nEnter price of game: ${price}`n"
                    $input_index += 2
                }
            }

            # Fix bunched up Admin credit details line
            if ($line -like "*Enter name of account credit is being added: Enter amount being added:*") {
                if (($input_index + 1) -lt $input_lines.Count) {
                    $targetUser = $input_lines[$input_index]
                    $amount = $input_lines[$input_index+1]
                    $line = $line -replace "Enter name of account credit is being added: Enter amount being added: ", "Enter name of account credit is being added: ${targetUser}`nEnter amount being added: ${amount}`n"
                    $input_index += 2
                }
            }

            # Fix bunched up Standard User credit details line, when target name is skipped
            if ($line -like "*Enter amount being added: Credit has been added to account*") {
                if ($input_index -lt $input_lines.Count) {
                    $amount = $input_lines[$input_index]
                    $line = "Enter amount being added: ${amount}`nCredit has been added to account"
                    $input_index++
                }
            }

            # Fix bunched up game purchase details line
            if ($line -like "*Enter name of game: Enter name of seller:*") {
                if (($input_index + 1) -lt $input_lines.Count) {
                    $game = $input_lines[$input_index]
                    $seller = $input_lines[$input_index+1]

                    $status = $line -replace "Enter name of game: Enter name of seller: ", ""

                    if ($status.Trim() -ne "") {
                        $line = "Enter name of game: ${game}`nEnter name of seller: ${seller}`n${status}"
                    } else {
                        $line = "Enter name of game: ${game}`nEnter name of seller: ${seller}"
                    }
                    $input_index += 2
                }
            }

            # Fix bunched up user creation prompts line
            if ($line -like "*Enter username of new user: Enter password of new user: Enter user privilege:*") {
                if (($input_index + 2) -lt $input_lines.Count) {
                    $new_user = $input_lines[$input_index]
                    $new_pass = $input_lines[$input_index+1]
                    $new_priv = $input_lines[$input_index+2]

                    $status = $line -replace "Enter username of new user: Enter password of new user: Enter user privilege: ", ""

                    if ($status.Trim() -ne "") {
                        $line = "Enter username of new user: ${new_user}`nEnter password of new user: ${new_pass}`nEnter user privilege: ${new_priv}`n${status}"
                    } else {
                        $line = "Enter username of new user: ${new_user}`nEnter password of new user: ${new_pass}`nEnter user privilege: ${new_priv}"
                    }
                    $input_index += 3
                }
            }

            # Split internal lines up again if we injected raw layout newlines (`n)
            $sub_lines = $line -split "`n"

            foreach ($sl in $sub_lines) {
                if ($sl.Trim() -ne "") {
                    $bytes = [System.Text.Encoding]::UTF8.GetBytes($sl.Trim())
                    $cleanBytes = @()
                    foreach ($b in $bytes) {
                        if (($b -eq 32) -or ($b -eq 45) -or (($b -ge 48) -and ($b -le 58)) -or (($b -ge 65) -and ($b -le 90)) -or ($b -eq 95) -or (($b -ge 97) -and ($b -le 122))) {
                            $cleanBytes += $b
                        }
                    }
                    [System.Text.Encoding]::ASCII.GetString($cleanBytes).Trim()
                }
            }
        }

        # Evaluate test assertions
        $diff = Compare-Object $expected_content $actual_content

        if ($null -eq $diff) {
            Write-Host "Test $($test_name): PASSED" -ForegroundColor Green
            $PASSED++
            Remove-Item $actual_file -Force
        } else {
            Write-Host "Test $($test_name): FAILED (Console output mismatch)" -ForegroundColor Red
            Write-Host "----------------------------------------------------"
            Write-Host "Differences found (Lines with <= are Expected, => are Actual):"
            $diff | ForEach-Object {
                if ($_.SideIndicator -eq "<=") {
                    Write-Host "Expected: $($_.InputObject)" -ForegroundColor Yellow
                } else {
                    Write-Host "Actual:   $($_.InputObject)" -ForegroundColor Red
                }
            }
            Write-Host "----------------------------------------------------"
            $FAILED++
        }
    }
}

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "Testing Complete: $PASSED Passed, $FAILED Failed." -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan

Write-Host "Executeable: $EXECUTABLE"
Write-Host "Arguments: --test"

if ($FAILED -gt 0) {
    exit 1
}
