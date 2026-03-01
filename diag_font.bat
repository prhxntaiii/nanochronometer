@echo off
:: diag_font.bat
:: Ejecuta desde la misma carpeta que NanoChrono.exe
:: Muestra qué archivos encuentra en assets\font\

echo.
echo === Diagnóstico de fuentes NanoChrono ===
echo.
echo Directorio actual:
cd
echo.
echo Buscando assets\font\:
if not exist "assets\font\" (
    echo [ERROR] La carpeta assets\font\ NO EXISTE aqui.
    echo         Crea la carpeta y pon un .ttf o .otf dentro.
) else (
    echo [OK] Carpeta assets\font\ encontrada.
    echo.
    echo Contenido:
    dir "assets\font\" /B
    echo.
    for %%f in ("assets\font\*.ttf" "assets\font\*.otf") do (
        echo [OK] Fuente encontrada: %%f
        echo      Tamanio: %%~zf bytes
    )
)
echo.
pause
