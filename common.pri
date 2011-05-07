#Includes common configuration for all subdirectory .pro files.
#INCLUDEPATH += . ..
#WARNINGS += -Wall

#TEMPLATE = lib

# The following keeps the generated files at least somewhat separate 
# from the source files.
#UI_DIR = uics
#MOC_DIR = mocs
#OBJECTS_DIR = objs

OUTPUT_DIR = $$PWD/output

CONFIG(debug, debug|release) {
    DESTDIR = $$OUTPUT_DIR/debug
    OBJECTS_DIR = $$OUTPUT_DIR/debug/.obj
    MOC_DIR = $$OUTPUT_DIR/debug/.moc
    RCC_DIR = $$OUTPUT_DIR/debug/.rcc
    UI_DIR = $$OUTPUT_DIR/debug/.ui

}
CONFIG(release, debug|release) {
    DESTDIR = $$OUTPUT_DIR/release
    OBJECTS_DIR = $$OUTPUT_DIR/release/.obj
    MOC_DIR = $$OUTPUT_DIR/release/.moc
    RCC_DIR = $$OUTPUT_DIR/release/.rcc
    UI_DIR = $$OUTPUT_DIR/release/.ui
}
