
set buildFolder=%1

IF EXIST "%buildFolder%\..\Ressources\" (
    IF EXIST "%buildFolder%\Ressources\" (
        RMDIR /S /Q "%buildFolder%\Ressources\"
    )

    MKDIR "%buildFolder%\Ressources\
    COPY "%buildFolder%\..\Ressources\" "%buildFolder%\Ressources\"
)

IF EXIST "%buildFolder%\..\..\..\ServerRelease\" (
    IF EXIST "%buildFolder%\Server\" (
        RMDIR /S /Q "%buildFolder%\Server\"
    )

    MKDIR "%buildFolder%\Server\
    COPY "%buildFolder%\..\..\..\ServerRelease\" "%buildFolder%\Server\"
)

IF EXIST "%buildFolder%\..\..\..\Content\" (
    IF EXIST "%buildFolder%\Content\" (
        RMDIR /S /Q "%buildFolder%\Content\"
    )

    MKDIR "%buildFolder%\Content\
    COPY "%buildFolder%\..\..\..\Content\" "%buildFolder%\Content\"
)