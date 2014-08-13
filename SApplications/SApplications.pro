TEMPLATE = subdirs
SUBDIRS += sampleApp \
    DataDisc \
    Library \
    CopyDisc \
    DataBase \
    ImageBurner \
    DiscImaging \
    DiscEraser \
    DiscDetails \
    DiscScanner \
    ScriptRunner \
    AudioDisc \
    Converter

unix{
    SUBDIRS += Mounter
}
