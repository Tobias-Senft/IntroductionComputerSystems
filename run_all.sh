#!/usr/bin/env bash
# Run main.exe with both methods on every .bmp in the sample folders,
# and save everything the program prints into a CSV dataset.
#
# Usage:
#   ./run_all.sh                    # all levels: every folder inside samples/
#   ./run_all.sh samples/easy       # just one level
#
# Images:  OutputFolder/<level>/<method>/<name>_OUTPUT.bmp
# Dataset: OutputFolder/results.csv   (one row per run)
# Log:     OutputFolder/all_logs.txt   (full raw output of every run in one file)

set -u

PROGRAM="./main.exe"
SAMPLES_ROOT="samples"
OUTPUT_ROOT="OutputFolder"
METHODS=("basic" "dynamic")
CSV="$OUTPUT_ROOT/results.csv"
LOG_FILE="$OUTPUT_ROOT/all_logs.txt"

shopt -s nullglob

if [[ ! -x "$PROGRAM" ]]; then
    echo "Error: $PROGRAM not found or not executable (did you compile it?)" >&2
    exit 1
fi

if [[ $# -gt 0 ]]; then
    dirs=("$@")
else
    dirs=("$SAMPLES_ROOT"/*/)
fi

mkdir -p "$OUTPUT_ROOT"

# Write the CSV header once (keeps appending if you run levels separately)
if [[ ! -f "$CSV" ]]; then
    echo "level,method,image,input_path,output_path,exit_code,runtime_seconds,program_output" > "$CSV"
fi

# Quote a value for CSV: wrap in quotes, double any quotes inside,
# and turn newlines into " | " so each run stays on one row.
csv_field() {
    local s="${1//$'\r'/}"
    s="${s//$'\n'/ | }"
    s="${s//\"/\"\"}"
    printf '"%s"' "$s"
}

# Current time in seconds with decimals (works in bash 5 / Git Bash / Linux)
now() {
    if [[ -n "${EPOCHREALTIME:-}" ]]; then
        echo "${EPOCHREALTIME/,/.}"
    else
        date +%s.%N
    fi
}

ok=0
fail=0

for input_dir in "${dirs[@]}"; do
    input_dir="${input_dir%/}"
    level=$(basename "$input_dir")
    files=("$input_dir"/*.bmp)

    if [[ ${#files[@]} -eq 0 ]]; then
        echo "Skipping $input_dir (no .bmp files)"
        continue
    fi

    echo "=== $level (${#files[@]} images) ==="

    for method in "${METHODS[@]}"; do
        out_dir="$OUTPUT_ROOT/$level/$method"
        mkdir -p "$out_dir"

        for input in "${files[@]}"; do
            name=$(basename "$input" .bmp)
            output="$out_dir/${name}_OUTPUT.bmp"

            echo "[$level/$method] $input -> $output"

            start=$(now)
            prog_out=$("$PROGRAM" "$input" "$output" "$method" 2>&1)
            code=$?
            end=$(now)
            runtime=$(awk -v s="$start" -v e="$end" 'BEGIN { printf "%.4f", e - s }')

            # Show the program's output in the terminal as before
            [[ -n "$prog_out" ]] && echo "$prog_out"

            # Save the raw output, and a row in the dataset
            {
                echo "===== $level | $method | $name | exit $code | ${runtime}s ====="
                printf '%s\n\n' "$prog_out"
            } >> "$LOG_FILE"
            {
                csv_field "$level";   printf ','
                csv_field "$method";  printf ','
                csv_field "$name";    printf ','
                csv_field "$input";   printf ','
                csv_field "$output";  printf ','
                printf '%s,%s,' "$code" "$runtime"
                csv_field "$prog_out"
                printf '\n'
            } >> "$CSV"

            if [[ $code -eq 0 ]]; then
                ((ok++))
            else
                echo "  FAILED (exit code $code)" >&2
                ((fail++))
            fi
        done
    done
done

echo
echo "Done: $ok succeeded, $fail failed."
echo "Dataset: $CSV"
echo "Log:     $LOG_FILE"
[[ $fail -eq 0 ]]