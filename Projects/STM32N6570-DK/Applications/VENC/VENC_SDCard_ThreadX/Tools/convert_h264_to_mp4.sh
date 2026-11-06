#!/usr/bin/env bash
set -euo pipefail

# Log file (overwritable per run via LOG_FILE env var)
LOG_FILE=${LOG_FILE:-log.txt}
: >"$LOG_FILE"   # truncate log at start of run
exec >>"$LOG_FILE" 2>&1

echo "[INFO] $(date '+%Y-%m-%d %H:%M:%S') Starting conversion in: $(pwd)"
echo "[INFO] Using ffmpeg: ${FFMPEG:-ffmpeg} (FRAMERATE=${FRAMERATE:-30})"

# Optional: set a custom ffmpeg path via env var FFMPEG
FFMPEG=${FFMPEG:-ffmpeg}
FRAMERATE=${FRAMERATE:-30}

shopt -s nullglob
# Include both lowercase and uppercase extensions
files=( *.h264 *.H264 )

if [ ${#files[@]} -eq 0 ]; then
  echo "[INFO] No .h264 files found in the current directory."
  exit 0
fi

for f in "${files[@]}"; do
  base="${f%.*}"
  out="${base}.mp4"
  echo "[INFO] Converting: $f -> $out"
  "$FFMPEG" -f h264 -framerate "$FRAMERATE" -i "$f" -c copy "$out"
done

echo "[INFO] Done. Converted ${#files[@]} file(s)."