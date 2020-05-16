
set buildFolder=%1
set destinationSubFolder=%2

IF EXIST "%buildFolder%\..\Ressources\" (
    IF EXIST "%buildFolder%\%destinationSubFolder%\Ressources\" (
        RMDIR /S /Q "%buildFolder%\%destinationSubFolder%\Ressources\"
    )

    MKDIR "%buildFolder%\%destinationSubFolder%\Ressources\
    COPY "%buildFolder%\..\Ressources\" "%buildFolder%\%destinationSubFolder%\Ressources\"
)

IF EXIST "%buildFolder%\..\..\..\ServerRelease\" (
    IF EXIST "%buildFolder%\%destinationSubFolder%\Server\" (
        RMDIR /S /Q "%buildFolder%\%destinationSubFolder%\Server\"
    )

    MKDIR "%buildFolder%\%destinationSubFolder%\Server\
    COPY "%buildFolder%\..\..\..\ServerRelease\" "%buildFolder%\%destinationSubFolder%\Server\"
)

IF EXIST "%buildFolder%\..\..\..\Content\" (
    IF EXIST "%buildFolder%\%destinationSubFolder%\Content\" (
        RMDIR /S /Q "%buildFolder%\%destinationSubFolder%\Content\"
    )

    MKDIR "%buildFolder%\%destinationSubFolder%\Content\
    COPY "%buildFolder%\..\..\..\Content\" "%buildFolder%\%destinationSubFolder%\Content\"
)