#! /bin/sh
# Set the home if not already set.
if [ "${SILICON_SRC_PATH}" = "" ]; then
    SILICON_SRC_PATH="`echo $0 | grep ^/`"
    if [ "$SILICON_SRC_PATH" = "" ]; then
	SILICON_SRC_PATH="$PWD"/"$0"
    fi
    cd `dirname $SILICON_SRC_PATH`
    SILICON_SRC_PATH=$PWD
    cd -
fi

cd $SILICON_SRC_PATH'/src'
OUTPUT="`find -name '*.h' -type f` `find -name '*.cpp' -type f` `find -name '*.ui' -type f` `find -name '*.qrc' -type f` `find -name '*.pro' -type f` "

wc -mwl $OUTPUT
echo "  in the "`echo $OUTPUT | wc -w`" Files"
