@echo off
for /R ".\in" %%f in (*.wav) do (
    .\ffmpeg -i ".\in\%%~nf.WAV" -f s16be -ar 44100 ".\out\%%~nf.PCM"
)