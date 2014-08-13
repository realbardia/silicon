TEMPLATE = subdirs
SUBDIRS += SingleInnerDialog \
        CdRecord \
        AudioCdRecord \
        ReadCd \
        MkDiscFs \
        MkIsoFs \
        Eraser \
        SingleInnerDialogQML \
        mpg123 \
        SystemTray

unix:!macx {
SUBDIRS += HalDeviceNotifier \
        UDisks \
        HalUDisk \
        FUseIso \
        RootMount \
        mpg321
}

win32 {
SUBDIRS += Win32DeviceNotifier
}
